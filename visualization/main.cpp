#include <Project/MainWindow.hpp>
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.resize(800, 800);

    window.show();
    return app.exec();
}
