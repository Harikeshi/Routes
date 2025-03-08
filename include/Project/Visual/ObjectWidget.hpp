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
    QPolygonF model;  // Это форма объекта
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
};
} // namespace Visual
