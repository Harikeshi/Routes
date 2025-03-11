#include <Project/MainWindow.hpp>
#include <Project/Visual/RouteWidget.hpp>

#include <QImage>
#include <QPainter>
#include <QTransform>
#include <QWidget>

class MyWidget : public QWidget
{
public:
    MyWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        // Инициализация виджета
    }

    void paintEvent(QPaintEvent* event) override
    {
        // Создаем QImage, если он еще не создан или размер изменился
        if (m_image.isNull() || m_image.size() != size())
        {
            m_image = QImage(size(), QImage::Format_ARGB32);
            m_image.fill(Qt::white); // Заполняем изображение белым цветом

            QPainter imagePainter(&m_image);
            // Применяем трансформацию к QPainter
            imagePainter.setTransform(m_transform);
            // Отрисовываем содержимое виджета на QImage
            drawContents(&imagePainter);
        }

        // Отрисовываем QImage на виджете
        QPainter widgetPainter(this);
        widgetPainter.drawImage(0, 0, m_image);
    }

    void resizeEvent(QResizeEvent* event) override
    {
        // При изменении размера виджета сбрасываем буфер
        m_image = QImage();
        QWidget::resizeEvent(event);
    }

    void setTransform(const QTransform& transform)
    {
        // Устанавливаем новую трансформацию и обновляем буфер
        m_transform = transform;
        m_image = QImage();
        update();
    }

    void updateContents()
    {
        // Принудительно обновляем содержимое буфера
        m_image = QImage();
        update();
    }

private:
    void drawContents(QPainter* painter)
    {
        // Здесь вы можете отрисовать содержимое вашего виджета
        painter->setPen(Qt::black);
        painter->drawText(rect(), Qt::AlignCenter, "Hello, World!");
    }

    QImage m_image;
    QTransform m_transform; // Трансформация, которая будет применена
};

#include <iostream>
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // QTransform transform;

    // transform.rotateRadians(1.5);

    // QPainter painter;
    // painter.setTransform(transform);
    // painter.drawLine(QPointF{0, 0}, QPointF{10, 10});

    // MyWidget widget;
    // widget.setTransform(transform);
    MainWindow widget;
    // // RouteWidget widget;

    // // TargetWidget widget;
    // // SubWidget widget;
    // // VisWidget widget;
    // // InformationWidget widget;
    // // RequestWidget widget;

    // // auto obj = Json::jsonFromFile();

    // // qDebug() << obj.size();

    // // widget.createTreeFromJson(obj);
    // // widget.setupP();
    // // widget.setupR();

    widget.resize(800, 800);
    widget.show();

    return app.exec();
}
