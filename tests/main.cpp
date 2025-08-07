
// #include <../include/Project/GetStackTrace.hpp>
#include <Project/MainWindow.hpp>
#include <QApplication>

#include <Project/Scene/Objects/PathWidget.hpp>
#include <Project/Scene/Objects/PointWidget.hpp>

#include <iostream>

#include <cmath>

#include "Project/Operations/JsonOperations.hpp"

int main(int argc, char* argv[])
{
    auto json = Operations::jsonFromFile("d:\\dev\\visualization\\jsons\\result.json");

    Operations::printJson(json);

    auto nloh = Operations::convertToNlohmann(json);

    json = Operations::convertToQJsonObject(nloh);

    Operations::printJson(json);

    return 0;

    QApplication app(argc, argv);

    MainWindow window;
    window.resize(800, 800);
    std::map<int, int> i{{1, 1}, {2, 2}};

    window.show();
    return app.exec();

    // QWidget window;
    // QVBoxLayout* layout = new QVBoxLayout(&window);
    //
    // Scene::Objects::PointWidget* pb = new Scene::Objects::PointWidget(QPointF(100, 100), 10);
    // layout->addWidget(pb);

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
