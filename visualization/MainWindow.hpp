#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "FigureWidget.hpp"
#include <QMainWindow>
namespace DeepWidgets {
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:
    FigureWidget* figureWidget;

    void createTestFigure();
};
} // namespace DeepWidgets
#endif // MAINWINDOW_H