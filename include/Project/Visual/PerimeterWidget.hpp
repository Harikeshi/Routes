#pragma once

#include "SegmentWidget.hpp"

#include <QDebug>
#include <QPainter>
#include <QVector2D>
#include <QWidget>

#include "Models/Perimeter.hpp"

namespace Visual {
class PerimeterWidget : public QWidget
{
    Q_OBJECT
public:
    PerimeterWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
    }

public:
public:
    void show() const
    {
        perimeter.show();
    }

    QColor getColor() const
    {
        return color;
    }

    void setColor(const QColor& c)
    {
        color = c;
    }

    QVector<QPolygonF> getInners() const
    {
        return perimeter.getInners();
    }

    void setPerimeter(const Visual::Models::Perimeter& perimeter)
    {
        this->perimeter = perimeter;
    }

    Visual::Models::Perimeter getPerimeter() const
    {
        return perimeter;
    }

    void draw(QPainter& painter) const
    {
        QPen pen(color, 5); // Толщина
        pen.setCosmetic(true);

        painter.setPen(pen);

        for (const auto& inner : perimeter.getInners())
        {
            painter.drawPolygon(inner);
        }
    }

private:
    QColor color{Qt::blue};

    Visual::Models::Perimeter perimeter;
};
} // namespace Visual
