#pragma once

#include <QPainter>
#include <QPolygonF>
#include <QWidget>

namespace Visual {
class ObjectWidget : public QWidget
{
    // TODO: Класс движушегося объекта
    // Точка берется с Route
    // ObjectWidget только отрисовывает
    QPolygonF model; // Это форма объекта

    double radiusHAS; // Радиус ГАС

    double currentVelocity;
    double maxVelocity;

public:
    ObjectWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
    }

    void draw(QPainter& painter)
    {
    }
    // TODO: Переместить в Object
    void drawArrow(QPainter& painter, const QColor& color, const double size) const
    {
        // // Отрисовка стрелки
        // double angle = std::atan2(end.y() - start.y(), end.x() - start.x());

        // QPolygonF arrow;

        // arrow << QPointF(0, 0)
        //       << QPointF(-size, size / 2)
        //       << QPointF(-size, -size / 2);

        // QTransform transform; // перенос в точку
        // transform.translate(current.x(), current.y());
        // transform.rotateRadians(angle); // повернуть на угол

        // arrow = transform.map(arrow);

        // // Заполнить полигон
        // painter.setBrush(color);
        // painter.drawPolygon(arrow);
    }
};
} // namespace Visual
