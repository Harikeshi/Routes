#include "MainWindow.hpp"
#include <QVBoxLayout>
#include <QWidget>

namespace DeepWidgets {
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Figure Visualizer with Zoom and Pan");
    setGeometry(100, 100, 800, 600);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    figureWidget = new DeepWidgets::FigureWidget(this);
    layout->addWidget(figureWidget);

    createTestFigure();
}

void MainWindow::createTestFigure()
{
    QVector<Segment> segments;

    // Create a more complex test figure (house-like shape)
    segments.append(Segment(QPointF(0, 0), QPointF(40, 0)));
    segments.append(Segment(QPointF(40, 0), QPointF(40, 30)));
    segments.append(Segment(QPointF(40, 30), QPointF(20, 50)));
    segments.append(Segment(QPointF(20, 50), QPointF(0, 30)));
    segments.append(Segment(QPointF(0, 30), QPointF(0, 0)));

    // Add some internal segments
    segments.append(Segment(QPointF(10, 0), QPointF(10, 20)));
    segments.append(Segment(QPointF(10, 20), QPointF(30, 20)));
    segments.append(Segment(QPointF(30, 20), QPointF(30, 0)));
    segments.append(Segment(QPointF(15, 25), QPointF(25, 35)));
    segments.append(Segment(QPointF(25, 35), QPointF(15, 35)));
    segments.append(Segment(QPointF(15, 35), QPointF(15, 25)));

    figureWidget->setSegments(segments);
}
} // namespace DeepWidgets