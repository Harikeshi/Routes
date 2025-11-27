//#include <QApplication>
//
//#include "SceneWidget.hpp"
//#include <Project/MainWindow.hpp>
//
//int main(int argc, char *argv[])
//{
//    QApplication app(argc, argv);
//
//    // NewWidget::SceneWidget window;
//    MainWindow window;
//    window.resize(800, 800);
//
//    window.show();
//    return app.exec();
//}

#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    DeepWidgets::MainWindow window;
    window.show();

    return app.exec();
}