#include <QApplication>

#include <Project/MainWindow.hpp>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    //    Widgets::SceneWidget window;
    MainWindow window;
    window.resize(800, 800);

    window.show();
    return app.exec();
}
