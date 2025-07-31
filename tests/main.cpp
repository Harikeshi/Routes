
// #include <../include/Project/GetStackTrace.hpp>
#include <Project/MainWindow.hpp>
#include <QApplication>

#include <Project/Scene/Objects/PathWidget.hpp>
#include <Project/Scene/Objects/PointWidget.hpp>

#include <cmath>
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // RuledProgressBar window;

    MainWindow window;

    // QWidget window;
    // QVBoxLayout* layout = new QVBoxLayout(&window);
    //
    // Scene::Objects::PointWidget* pb = new Scene::Objects::PointWidget(QPointF(100, 100), 10);
    // layout->addWidget(pb);

    window.resize(800, 800);

    window.show();
    return app.exec();

    // QApplication app(argc, argv);
    //
    // // JsonFileExplorer widget;
    // MainWindow widget;
    // //CustomTable widget;
    //
    // // Подготовка данных
    // QPointF plPosition(10.5, 20.3);
    // double plSpeed = 5.25;
    //
    // widget.resize(800, 800);
    // widget.show();
    //
    // return app.exec();
}

// #include "main.moc"
