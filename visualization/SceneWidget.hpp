#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPointF>
#include <QTransform>
#include <QVector>
#include <QWheelEvent>
#include <QWidget>

namespace NewWidget {
class SceneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SceneWidget(QWidget* parent = nullptr);

    // Добавление отрезка пути
    void addPathSegment(const QPointF& start, const QPointF& end);
    void clearPath();

    // Сброс трансформации к исходному состоянию
    void resetView();

protected:
    void paintEvent(QPaintEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void drawGrid(QPainter& painter);
    void drawPath(QPainter& painter);
    QPointF sceneToWidget(const QPointF& scenePoint) const;
    QPointF widgetToScene(const QPointF& widgetPoint) const;

    // Данные сцены
    QVector<QLineF> m_pathSegments;

    // Параметры трансформации
    double m_scale;
    QPointF m_translation;
    QPointF m_sceneCenter;

    // Для панорамирования
    bool m_isPanning;
    QPoint m_lastPanPoint;

    // Параметры сетки
    double m_gridSpacing;
};
} // namespace main
#endif // SCENEWIDGET_H