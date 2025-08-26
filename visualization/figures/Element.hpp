#pragma once

#include <QPainter>
#include <QPointF>

class Element
{
    // draw
    // ...
public:
    virtual QPointF start() const = 0;

    virtual QPointF end() const = 0;

    virtual double speed() const = 0;

    virtual void draw(QPainter& painter) = 0;

    virtual double length() const = 0;

    virtual void show() const = 0;

    virtual QPointF move(double time) const = 0;

    Element()
    {
    }

    ~Element() = default;
};