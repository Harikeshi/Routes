#include <QApplication>

#include "SceneWidget.hpp"
#include <Project/MainWindow.hpp>
#include <Project/Widgets/SceneWidget.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // NewWidget::SceneWidget window;
    // Widgets::SceneWidget window;
    MainWindow window;
    window.resize(800, 800);

    window.show();
    return app.exec();
}
