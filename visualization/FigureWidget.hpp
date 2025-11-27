#ifndef FIGUREWIDGET_H
#define FIGUREWIDGET_H

#include "Segment.hpp"
#include <QMouseEvent>
#include <QPointF>
#include <QVector>
#include <QWheelEvent>
#include <QWidget>

namespace DeepWidgets {
class FigureWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FigureWidget(QWidget* parent = nullptr);
    void setSegments(const QVector<Segment>& segments);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void updateTransformation();
    void drawGrid(QPainter& painter);
    void drawSegments(QPainter& painter);
    void zoomAtPoint(const QPoint& widgetPos, double factor);

    QVector<Segment> segments;
    QTransform transform;
    QTransform inverseTransform;

    // View parameters
    double viewScale = 1.0;
    QPointF viewOffset = QPointF(0, 0);
    QPointF viewCenter = QPointF(0, 0);

    // Mouse interaction
    bool isPanning = false;
    QPoint lastMousePos;

    // Grid and display parameters
    double baseGridSpacing = 20.0;

    // World coordinates bounds (fixed for the figure)
    double worldMinX = 0, worldMaxX = 100;
    double worldMinY = 0, worldMaxY = 100;

    // View bounds (change with zoom/pan)
    double viewMinX = 0, viewMaxX = 100;
    double viewMinY = 0, viewMaxY = 100;
};

} // namespace DeepWidgets
#endif // FIGUREWIDGET_H