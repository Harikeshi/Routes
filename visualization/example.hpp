#include "FigureWidget.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QWheelEvent>
#include <cmath>

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

    // Apply transformation for world coordinates
    painter.setTransform(transform);

    // Draw grid
    drawGrid(painter);

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

    double drawWidth = widgetWidth - 2 * margin;
    double drawHeight = widgetHeight - 2 * margin;

    // Calculate scale factors based on current view bounds
    double scaleX = drawWidth / (viewMaxX - viewMinX);
    double scaleY = drawHeight / (viewMaxY - viewMinY);

    // Use the same scale for both axes to maintain aspect ratio
    double scale = qMin(scaleX, scaleY);

    // Calculate offset to center the current view
    double offsetX = (widgetWidth - (viewMaxX - viewMinX) * scale) / 2;
    double offsetY = (widgetHeight - (viewMaxY - viewMinY) * scale) / 2;

    // Create transformation matrix
    transform.reset();
    transform.translate(offsetX, widgetHeight - offsetY); // Move to bottom-left with margin
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
    double logScale = std::log10(viewScale);

    // Find appropriate grid spacing
    double scaleFactors[] = {0.1, 0.2, 0.5, 1, 2, 5, 10, 20, 50, 100};
    for (double factor : scaleFactors)
    {
        if (viewScale * baseGridSpacing * factor > 30)
        {
            gridSpacing = baseGridSpacing * factor;
            break;
        }
    }

    // Set grid style
    QPen gridPen(QColor(200, 200, 200), 1, Qt::DotLine);
    QPen axisPen(Qt::black, 2);
    QPen textPen(Qt::black);

    // Draw grid lines
    gridPen.setColor(QColor(220, 220, 220));
    painter.setPen(gridPen);

    // viewMinX left
    // viewMaxX // right
    // viewMinY //bottom
    // viewMaxY //top
    // Calculate visible grid lines
    double startX = std::floor(viewMinX / gridSpacing) * gridSpacing;
    double endX = std::ceil(viewMaxX / gridSpacing) * gridSpacing;
    double startY = std::floor(viewMinY / gridSpacing) * gridSpacing;
    double endY = std::ceil(viewMaxY / gridSpacing) * gridSpacing;

    // Vertical grid lines
    for (double x = startX; x <= endX; x += gridSpacing)
    {
        if (std::abs(x) < 1e-6)
            continue; // Skip zero for now
        painter.drawLine(QPointF(x, viewMinY), QPointF(x, viewMaxY));
    }

    // Horizontal grid lines
    for (double y = startY; y <= endY; y += gridSpacing)
    {
        if (std::abs(y) < 1e-6)
            continue; // Skip zero for now
        painter.drawLine(QPointF(viewMinX, y), QPointF(viewMaxX, y));
    }

    // Draw axes
    painter.setPen(axisPen);
    if (viewMinY <= 0 && viewMaxY >= 0)
    {
        painter.drawLine(QPointF(viewMinX, 0), QPointF(viewMaxX, 0)); // X axis
    }
    if (viewMinX <= 0 && viewMaxX >= 0)
    {
        painter.drawLine(QPointF(0, viewMinY), QPointF(0, viewMaxY)); // Y axis
    }

    painter.restore();

    // Draw coordinate labels (in widget coordinates)
    painter.save();
    painter.setTransform(QTransform()); // Reset to widget coordinates
    painter.setPen(textPen);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    // X axis labels
    for (double x = startX; x <= endX; x += gridSpacing)
    {
        if (std::abs(x) < 1e-6)
            continue;
        QPointF widgetPos = transform.map(QPointF(x, 0));
        if (widgetPos.x() >= margin && widgetPos.x() <= width() - margin)
        {
            QString label = QString::number(x, 'f', std::abs(x) < 1 ? 1 : 0);
            QRect textRect = painter.fontMetrics().boundingRect(label);
            painter.drawText(widgetPos.x() - textRect.width() / 2,
                             height() - margin / 2,
                             label);
        }
    }

    // Y axis labels
    for (double y = startY; y <= endY; y += gridSpacing)
    {
        if (std::abs(y) < 1e-6)
            continue;
        QPointF widgetPos = transform.map(QPointF(0, y));
        if (widgetPos.y() >= margin && widgetPos.y() <= height() - margin)
        {
            QString label = QString::number(y, 'f', std::abs(y) < 1 ? 1 : 0);
            QRect textRect = painter.fontMetrics().boundingRect(label);
            painter.drawText(margin / 2 - textRect.width(),
                             widgetPos.y() + textRect.height() / 3,
                             label);
        }
    }

    // Draw origin label if visible
    if (viewMinX <= 0 && viewMaxX >= 0 && viewMinY <= 0 && viewMaxY >= 0)
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