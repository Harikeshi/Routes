#include <Project/Widgets/JsonExplorerWidget.hpp>
#include <Project/MainWindow.hpp>

#include <Project/Data/testCases.hpp>
#include <Project/Data/Initializer.hpp>
#include <iostream>

#include <Project/CustomTable.hpp>

int main(int argc, char* argv[])
{
    // читать из json
    //QString message;

    //auto json = JsonHelpers::jsonFromFile("d:\\dev\\vproject\\jsons\\request.json");

    //DatabaseInitializer db;

    //db.loadFromJson(json);

    //db.loadFromJson(setupRequest());
    //db.loadFromJson(setupReport());

    //qDebug() << message;
    //return 0;
  
    QApplication app(argc, argv);

    // JsonFileExplorer widget;
    MainWindow widget;
    //CustomTable widget;

    // Подготовка данных
    QPointF plPosition(10.5, 20.3);
    double plSpeed = 5.25;

    widget.resize(800, 800);
    widget.show();

    return app.exec();
}