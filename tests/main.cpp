#include <Project/MainWindow.hpp>
#include <Project/Visual/RouteWidget.hpp>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow widget;
    // RouteWidget widget;

    // TargetWidget widget;
    // SubWidget widget;
    // VisWidget widget;
    // InformationWidget widget;
    // RequestWidget widget;

    // auto obj = Json::jsonFromFile();

    // qDebug() << obj.size();

    // widget.createTreeFromJson(obj);
    // widget.setupP();
    // widget.setupR();

    widget.resize(800, 800);
    widget.show();

    return app.exec();
}
