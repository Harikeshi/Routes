// SceneWidget.cpp
#include "SceneWidget.hpp"
#include <QKeyEvent>
#include <QPainter>
#include <QWheelEvent>
#include <cmath>

namespace NewWidget {
SceneWidget::SceneWidget(QWidget* parent)
    : QWidget(parent), m_scale(1.0), m_translation(0, 0), m_sceneCenter(0, 0), m_isPanning(false), m_gridSpacing(50.0) // начальный шаг сетки
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus); // Важно для обработки событий клавиатуры

    // Добавим тестовый путь для демонстрации
    addPathSegment(QPointF(-100, -50), QPointF(100, -50));
    addPathSegment(QPointF(100, -50), QPointF(100, 50));
    addPathSegment(QPointF(100, 50), QPointF(-100, 50));
    addPathSegment(QPointF(-100, 50), QPointF(-100, -50));
}

void SceneWidget::addPathSegment(const QPointF& start, const QPointF& end)
{
    m_pathSegments.append(QLineF(start, end));
    update();
}

void SceneWidget::clearPath()
{
    m_pathSegments.clear();
    update();
}

void SceneWidget::resetView()
{
    m_scale = 1.0;
    m_translation = QPointF(0, 0);
    m_sceneCenter = QPointF(0, 0);
    update();
}

void SceneWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Сохраняем состояние painter'а
    painter.save();

    // Применяем трансформации
    painter.translate(width() / 2 + m_translation.x(), height() / 2 + m_translation.y());
    painter.scale(m_scale, m_scale);
    painter.translate(-m_sceneCenter.x(), -m_sceneCenter.y());

    // Рисуем сетку
    drawGrid(painter);

    // Рисуем путь
    drawPath(painter);

    // Восстанавливаем состояние
    painter.restore();

    // Отладочная информация (опционально)
    painter.drawText(10, 20, QString("Масштаб: %1x").arg(m_scale, 0, 'f', 2));
    painter.drawText(10, 40, QString("Сдвиг: (%1, %2)").arg(m_translation.x()).arg(m_translation.y()));
    painter.drawText(10, 60, "ESC - сброс вида");
}

void SceneWidget::drawGrid(QPainter& painter)
{
    painter.save();

    QPen gridPen(QColor(200, 200, 200, 150));
    gridPen.setWidthF(1.0 / m_scale); // Толщина линий не зависит от масштаба
    painter.setPen(gridPen);

    // Рассчитываем видимую область
    double scaledGridSpacing = m_gridSpacing;

    // Адаптивный шаг сетки в зависимости от масштаба
    while (scaledGridSpacing * m_scale < 20)
    {
        scaledGridSpacing *= 2;
    }
    while (scaledGridSpacing * m_scale > 100)
    {
        scaledGridSpacing /= 2;
    }

    // Определяем границы видимой области
    QRectF visibleRect = QRectF(-width() / 2 / m_scale, -height() / 2 / m_scale, width() / m_scale, height() / m_scale)
                             .translated(m_sceneCenter - m_translation / m_scale);

    // Рисуем вертикальные линии
    double startX = std::floor(visibleRect.left() / scaledGridSpacing) * scaledGridSpacing;
    double endX = std::ceil(visibleRect.right() / scaledGridSpacing) * scaledGridSpacing;

    for (double x = startX; x <= endX; x += scaledGridSpacing)
    {
        painter.drawLine(QPointF(x, visibleRect.top()),
                         QPointF(x, visibleRect.bottom()));
    }

    // Рисуем горизонтальные линии
    double startY = std::floor(visibleRect.top() / scaledGridSpacing) * scaledGridSpacing;
    double endY = std::ceil(visibleRect.bottom() / scaledGridSpacing) * scaledGridSpacing;

    for (double y = startY; y <= endY; y += scaledGridSpacing)
    {
        painter.drawLine(QPointF(visibleRect.left(), y),
                         QPointF(visibleRect.right(), y));
    }

    painter.restore();
}

void SceneWidget::drawPath(QPainter& painter)
{
    painter.save();

    QPen pathPen(Qt::blue);
    pathPen.setWidthF(2.0 / m_scale); // Толщина линии адаптируется к масштабу
    painter.setPen(pathPen);

    // Рисуем все отрезки пути
    for (const QLineF& segment : m_pathSegments)
    {
        painter.drawLine(segment);
    }

    painter.restore();
}

void SceneWidget::wheelEvent(QWheelEvent* event)
{
    double zoomFactor = 1.15; // Коэффициент масштабирования
    double oldScale = m_scale;

    if (event->angleDelta().y() > 0)
    {
        // Приближение
        m_scale *= zoomFactor;
    }
    else
    {
        // Удаление
        m_scale /= zoomFactor;
    }

    // Ограничиваем масштаб
    m_scale = qBound(0.1, m_scale, 10.0);

    if (m_scale != oldScale)
    {
        // Получаем позицию курсора в координатах сцены
        QPointF mousePos = event->position();
        QPointF scenePosBefore = widgetToScene(mousePos);

        // Корректируем смещение для масштабирования относительно курсора
        QPointF scenePosAfter = widgetToScene(mousePos);
        m_translation += (scenePosAfter - scenePosBefore) * m_scale;

        update();
    }

    event->accept();
}

void SceneWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isPanning = true;
        m_lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }

    event->accept();
}

void SceneWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isPanning)
    {
        QPoint delta = event->pos() - m_lastPanPoint;
        m_translation += delta;
        m_lastPanPoint = event->pos();
        update();
    }

    event->accept();
}

void SceneWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_isPanning)
    {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
    }

    event->accept();
}

void SceneWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        resetView();
        event->accept();
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}

QPointF SceneWidget::sceneToWidget(const QPointF& scenePoint) const
{
    QPointF result = scenePoint;
    result -= m_sceneCenter;
    result *= m_scale;
    result += QPointF(width() / 2 + m_translation.x(), height() / 2 + m_translation.y());
    return result;
}

QPointF SceneWidget::widgetToScene(const QPointF& widgetPoint) const
{
    QPointF result = widgetPoint;
    result -= QPointF(width() / 2 + m_translation.x(), height() / 2 + m_translation.y());
    result /= m_scale;
    result += m_sceneCenter;
    return result;
}
} // namespace main