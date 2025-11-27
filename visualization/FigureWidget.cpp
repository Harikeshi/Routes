#include "FigureWidget.hpp"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QWheelEvent>
#include <cmath>

namespace DeepWidgets {

FigureWidget::FigureWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(400, 400);
    setMouseTracking(true);
}

void FigureWidget::setSegments(const QVector<Segment>& segments)
{
    this->segments = segments;

    // Calculate fixed world bounds from segments
    if (!segments.isEmpty())
    {
        worldMinX = segments[0].a.x();
        worldMaxX = segments[0].a.x();
        worldMinY = segments[0].a.y();
        worldMaxY = segments[0].a.y();

        for (const Segment& segment : segments)
        {
            worldMinX = qMin(worldMinX, qMin(segment.a.x(), segment.b.x()));
            worldMaxX = qMax(worldMaxX, qMax(segment.a.x(), segment.b.x()));
            worldMinY = qMin(worldMinY, qMin(segment.a.y(), segment.b.y()));
            worldMaxY = qMax(worldMaxY, qMax(segment.a.y(), segment.b.y()));
        }

        // Add some padding
        double paddingX = (worldMaxX - worldMinX) * 0.1;
        double paddingY = (worldMaxY - worldMinY) * 0.1;
        worldMinX -= paddingX;
        worldMaxX += paddingX;
        worldMinY -= paddingY;
        worldMaxY += paddingY;

        // Ensure we have some reasonable bounds
        if (worldMaxX - worldMinX < 10)
        {
            double centerX = (worldMinX + worldMaxX) / 2;
            worldMinX = centerX - 5;
            worldMaxX = centerX + 5;
        }
        if (worldMaxY - worldMinY < 10)
        {
            double centerY = (worldMinY + worldMaxY) / 2;
            worldMinY = centerY - 5;
            worldMaxY = centerY + 5;
        }
    }

    // Initialize view to show entire figure
    viewMinX = worldMinX;
    viewMaxX = worldMaxX;
    viewMinY = worldMinY;
    viewMaxY = worldMaxY;
    viewScale = 1.0;
    viewOffset = QPointF(0, 0);

    updateTransformation();
    update();
}

void FigureWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fill background
    painter.fillRect(rect(), Qt::white);

    // Draw grid on entire widget first
    drawGrid(painter);

    // Apply transformation for world coordinates
    painter.setTransform(transform);

    // Draw segments
    drawSegments(painter);
}

void FigureWidget::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    updateTransformation();
}

void FigureWidget::wheelEvent(QWheelEvent* event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    if (!numDegrees.isNull())
    {
        double zoomFactor = 1.0;
        if (numDegrees.y() > 0)
        {
            // Zoom in
            zoomFactor = 1.1;
        }
        else if (numDegrees.y() < 0)
        {
            // Zoom out
            zoomFactor = 1.0 / 1.1;
        }

        zoomAtPoint(event->position().toPoint(), zoomFactor);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void FigureWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        isPanning = true;
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void FigureWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (isPanning)
    {
        QPoint delta = event->pos() - lastMousePos;

        if (!delta.isNull())
        {
            // Convert mouse delta to world coordinates delta
            QPointF worldDelta = inverseTransform.map(QPointF(delta)) - inverseTransform.map(QPointF(0, 0));

            // Update view bounds
            viewMinX -= worldDelta.x();
            viewMaxX -= worldDelta.x();
            viewMinY += worldDelta.y(); // Y is inverted
            viewMaxY += worldDelta.y();

            updateTransformation();
            update();
        }

        lastMousePos = event->pos();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void FigureWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && isPanning)
    {
        isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void FigureWidget::updateTransformation()
{
    double widgetWidth = width();
    double widgetHeight = height();

    // Calculate scale factors based on current view bounds
    double scaleX = widgetWidth / (viewMaxX - viewMinX);
    double scaleY = widgetHeight / (viewMaxY - viewMinY);

    // Use the same scale for both axes to maintain aspect ratio
    double scale = qMin(scaleX, scaleY);

    // Calculate offset to center the current view
    double offsetX = (widgetWidth - (viewMaxX - viewMinX) * scale) / 2;
    double offsetY = (widgetHeight - (viewMaxY - viewMinY) * scale) / 2;

    // Create transformation matrix
    transform.reset();
    transform.translate(offsetX, widgetHeight - offsetY); // Move to bottom-left
    transform.scale(scale, -scale);                       // Flip Y axis
    transform.translate(-viewMinX, -viewMinY);            // Adjust for current view

    // Update inverse transformation
    inverseTransform = transform.inverted();

    // Update overall view scale (for grid calculation)
    viewScale = scale;
}

void FigureWidget::zoomAtPoint(const QPoint& widgetPos, double factor)
{
    // Convert widget position to world coordinates
    QPointF worldPos = inverseTransform.map(QPointF(widgetPos));

    // Calculate new view bounds
    double newWidth = (viewMaxX - viewMinX) / factor;
    double newHeight = (viewMaxY - viewMinY) / factor;

    // Calculate new bounds centered on the mouse position
    double centerX = (worldPos.x() - viewMinX) / (viewMaxX - viewMinX);
    double centerY = (worldPos.y() - viewMinY) / (viewMaxY - viewMinY);

    double newMinX = worldPos.x() - newWidth * centerX;
    double newMaxX = newMinX + newWidth;
    double newMinY = worldPos.y() - newHeight * centerY;
    double newMaxY = newMinY + newHeight;

    // Apply limits to prevent excessive zoom
    double minZoomWidth = (worldMaxX - worldMinX) * 0.01; // 1% of original size
    double maxZoomWidth = (worldMaxX - worldMinX) * 100;  // 100x of original size

    if (newWidth >= minZoomWidth && newWidth <= maxZoomWidth)
    {
        viewMinX = newMinX;
        viewMaxX = newMaxX;
        viewMinY = newMinY;
        viewMaxY = newMaxY;

        updateTransformation();
        update();
    }
}
void FigureWidget::drawGrid(QPainter& painter)
{
    painter.save();

    // Adjust grid spacing based on zoom level
    double gridSpacing = baseGridSpacing;

    // Find appropriate grid spacing
    double scaleFactors[] = {0.1, 0.2, 0.5, 1, 2, 5, 10, 20, 50, 100};
    for (double factor : scaleFactors)
    {
        if (viewScale * baseGridSpacing * factor > 40)
        {
            gridSpacing = baseGridSpacing * factor;
            break;
        }
    }

    // Set grid styles
    QPen mainGridPen(QColor(200, 200, 200), 1);
    QPen subGridPen(QColor(230, 230, 230), 1);
    QPen axisPen(Qt::black, 2);
    QPen textPen(Qt::black);

    // Calculate visible area in world coordinates
    QRectF visibleWorldRect = inverseTransform.mapRect(rect());

    // Calculate grid boundaries
    double startX = std::floor(visibleWorldRect.left() / gridSpacing) * gridSpacing;
    double endX = std::ceil(visibleWorldRect.right() / gridSpacing) * gridSpacing;
    double startY = std::floor(visibleWorldRect.bottom() / gridSpacing) * gridSpacing;
    double endY = std::ceil(visibleWorldRect.top() / gridSpacing) * gridSpacing;

    // Draw sub-grid lines (more frequent) - только если достаточно приближены
    if (viewScale > 0.5)
    {
        double subGridSpacing = gridSpacing / 5.0;
        painter.setPen(subGridPen);

        // Vertical sub-grid lines
        for (double x = std::floor(visibleWorldRect.left() / subGridSpacing) * subGridSpacing;
             x <= endX;
             x += subGridSpacing)
        {
            if (std::abs(std::fmod(x, gridSpacing)) < 1e-6)
                continue; // Skip main grid lines

            QPointF p1 = transform.map(QPointF(x, visibleWorldRect.bottom()));
            QPointF p2 = transform.map(QPointF(x, visibleWorldRect.top()));
            painter.drawLine(p1, p2);
        }

        // Horizontal sub-grid lines
        for (double y = std::floor(visibleWorldRect.bottom() / subGridSpacing) * subGridSpacing;
             y <= endY;
             y += subGridSpacing)
        {
            if (std::abs(std::fmod(y, gridSpacing)) < 1e-6)
                continue; // Skip main grid lines

            QPointF p1 = transform.map(QPointF(visibleWorldRect.left(), y));
            QPointF p2 = transform.map(QPointF(visibleWorldRect.right(), y));
            painter.drawLine(p1, p2);
        }
    }

    // Draw main grid lines
    painter.setPen(mainGridPen);

    // Vertical main grid lines
    for (double x = startX; x <= endX; x += gridSpacing)
    {
        QPointF p1 = transform.map(QPointF(x, visibleWorldRect.bottom()));
        QPointF p2 = transform.map(QPointF(x, visibleWorldRect.top()));
        painter.drawLine(p1, p2);
    }

    // Horizontal main grid lines
    for (double y = startY; y <= endY; y += gridSpacing)
    {
        QPointF p1 = transform.map(QPointF(visibleWorldRect.left(), y));
        QPointF p2 = transform.map(QPointF(visibleWorldRect.right(), y));
        painter.drawLine(p1, p2);
    }

    // Draw axes
    painter.setPen(axisPen);
    if (visibleWorldRect.bottom() <= 0 && visibleWorldRect.top() >= 0)
    {
        // X axis (horizontal)
        QPointF p1 = transform.map(QPointF(visibleWorldRect.left(), 0));
        QPointF p2 = transform.map(QPointF(visibleWorldRect.right(), 0));
        painter.drawLine(p1, p2);
    }
    if (visibleWorldRect.left() <= 0 && visibleWorldRect.right() >= 0)
    {
        // Y axis (vertical)
        QPointF p1 = transform.map(QPointF(0, visibleWorldRect.bottom()));
        QPointF p2 = transform.map(QPointF(0, visibleWorldRect.top()));
        painter.drawLine(p1, p2);
    }

    // Draw coordinate labels
    painter.setPen(textPen);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    // X axis labels (at bottom of widget)
    for (double x = startX; x <= endX; x += gridSpacing)
    {
        if (std::abs(x) < 1e-6)
            continue; // Skip zero for now

        QPointF labelPos = transform.map(QPointF(x, visibleWorldRect.bottom()));
        if (labelPos.x() >= 0 && labelPos.x() <= width())
        {
            QString label = QString::number(x, 'f', std::abs(x) < 1 ? 1 : 0);
            QRect textRect = painter.fontMetrics().boundingRect(label);
            painter.drawText(labelPos.x() - textRect.width() / 2,
                             height() - 5,
                             label);
        }
    }

    // Y axis labels (at left of widget)
    for (double y = startY; y <= endY; y += gridSpacing)
    {
        if (std::abs(y) < 1e-6)
            continue; // Skip zero for now

        QPointF labelPos = transform.map(QPointF(visibleWorldRect.left(), y));
        if (labelPos.y() >= 0 && labelPos.y() <= height())
        {
            QString label = QString::number(y, 'f', std::abs(y) < 1 ? 1 : 0);
            QRect textRect = painter.fontMetrics().boundingRect(label);
            painter.drawText(5,
                             labelPos.y() + textRect.height() / 3,
                             label);
        }
    }

    // Draw origin label if visible
    if (visibleWorldRect.contains(0, 0))
    {
        QPointF originWidgetPos = transform.map(QPointF(0, 0));
        painter.drawText(originWidgetPos.x() + 5, originWidgetPos.y() - 5, "0");
    }

    painter.restore();
}
void FigureWidget::drawSegments(QPainter& painter)
{
    painter.save();

    QPen segmentPen(Qt::blue, 3);
    painter.setPen(segmentPen);

    for (const Segment& segment : segments)
    {
        painter.drawLine(segment.a, segment.b);
    }

    // Draw points
    QBrush pointBrush(Qt::red);
    painter.setBrush(pointBrush);
    painter.setPen(Qt::NoPen);

    double pointRadius = 4.0 / viewScale; // Scale point size with zoom
    pointRadius = qMax(pointRadius, 2.0); // Minimum size

    for (const Segment& segment : segments)
    {
        painter.drawEllipse(segment.a, pointRadius, pointRadius);
        painter.drawEllipse(segment.b, pointRadius, pointRadius);
    }

    painter.restore();
}
} // namespace DeepWidgets