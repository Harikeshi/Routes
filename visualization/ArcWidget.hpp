#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include <QPoint>
#include <cmath>

// Вычисление угла между вектором и положительной осью X (в градусах)
qreal calculateAngle(const QPointF &center, const QPointF &point) {
    QPointF vector = point - center;
    qreal angle = std::atan2(vector.y(), vector.x()) * 180.0 / M_PI;
    return (angle < 0) ? angle + 360 : angle; // Нормализация в диапазон [0, 360)
}

// Вычисление длины дуги между двумя углами
qreal calculateArcLength(qreal startAngle, qreal endAngle, bool counterclockwise = true) {
    if (counterclockwise) {
        return (endAngle >= startAngle) ? (endAngle - startAngle) : (360 - startAngle + endAngle);
    } else {
        return (startAngle >= endAngle) ? (startAngle - endAngle) : (360 - endAngle + startAngle);
    }
}

class ArcWidget : public QWidget {
Q_OBJECT
public:
    ArcWidget(QWidget *parent = nullptr) : QWidget(parent) {
        // Пример начальных данных
        center = QPointF(200, 200);
        radius = 100;
        startPoint = QPointF(300, 200); // Право
        endPoint = QPointF(100, 200);   // Лево (дуга через верх)
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Очистка фона
        painter.fillRect(rect(), Qt::white);

        // Рисование дуги
        drawArc(painter, center, radius, startPoint, endPoint);

        // Вспомогательные элементы
        drawHelperElements(painter);
    }

private:
    QPointF center;
    qreal radius;
    QPointF startPoint;
    QPointF endPoint;

    void drawArc(QPainter &painter, const QPointF &center, qreal radius,
                 const QPointF &start, const QPointF &end) {

        // Вычисление углов
        qreal startAngle = calculateAngle(center, start);
        qreal endAngle = calculateAngle(center, end);
        qreal spanAngle = calculateArcLength(startAngle, endAngle);

        // Преобразование координат для drawArc
        QRectF boundingRect(center.x() - radius, center.y() - radius,
                            radius * 2, radius * 2);

        // Рисование дуги (углы в 1/16 градуса)
        painter.setPen(QPen(Qt::blue, 3));
        painter.drawArc(boundingRect, startAngle * 16, spanAngle * 16);
    }

    void drawHelperElements(QPainter &painter) {
        // Центр
        painter.setPen(QPen(Qt::red, 2));
        painter.setBrush(Qt::red);
        painter.drawEllipse(center, 5, 5);

        // Начальная точка
        painter.setPen(QPen(Qt::green, 2));
        painter.setBrush(Qt::green);
        painter.drawEllipse(startPoint, 5, 5);

        // Конечная точка
        painter.setPen(QPen(Qt::magenta, 2));
        painter.setBrush(Qt::magenta);
        painter.drawEllipse(endPoint, 5, 5);

        // Радиус
        painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter.drawLine(center, startPoint);
        painter.drawLine(center, endPoint);
    }
};

class AdvancedArcWidget : public QWidget {
public:
    enum Direction {
        Clockwise, CounterClockwise
    };

    void drawArcWithPoints(QPainter &painter, const QPointF &center, qreal radius,
                           const QPointF &start, const QPointF &end,
                           Direction direction = CounterClockwise) {

        qreal startAngle = calculateAngle(center, start);
        qreal endAngle = calculateAngle(center, end);
        qreal spanAngle = calculateArcLength(startAngle, endAngle, direction == CounterClockwise);

        // Для clockwise направления нужно отрицательное значение
        if (direction == Clockwise) {
            spanAngle = -spanAngle;
        }

        QRectF boundingRect(center.x() - radius, center.y() - radius,
                            radius * 2, radius * 2);

        painter.setPen(QPen(Qt::blue, 3));
        painter.drawArc(boundingRect, startAngle * 16, spanAngle * 16);
    }
};

//#include <QApplication>
//#include <QMainWindow>
//
//int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);
//
//    QMainWindow window;
//    ArcWidget *widget = new ArcWidget();
//    window.setCentralWidget(widget);
//    window.resize(400, 400);
//    window.show();
//
//    return app.exec();
//}