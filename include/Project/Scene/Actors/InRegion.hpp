#pragma once

#include <QGraphicsPolygonItem>
#include <QPen>
#include <QPolygonF>

#include <cmath>

#include "../Objects/PerimeterObject.hpp"

#include "../Actor.hpp"

namespace Scene {
namespace Actors {
class InRegionScene : public Actor
{
    using Perimeter = Objects::Models::Perimeter;
    using PerimeterObject = Objects::PerimeterObject;
    using Request = Data::Request;

public:
    InRegionScene(QObject* parent = nullptr)
        : Actor(parent)
    {
        perimeter = new PerimeterObject(this);
    }

public:
    virtual void draw(QPainter& painter) override
    {
        perimeter->draw(painter);
    }

    virtual void swapCoordinates() override
    {
        // Смена периметра
        this->perimeter->swapCoordinates();
    }

    virtual void reload(const Request& request) override
    {
        perimeter->setPerimeter(request.getPerimeter());
    }

private:
    PerimeterObject* perimeter;
};
} // namespace Actors
} // namespace Scene