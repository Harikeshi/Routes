#pragma once

#include <QGraphicsPolygonItem>

#include "../Objects/PerimeterObject.hpp"

#include "../Actor.hpp"

namespace Scene::Actors {
class InRegionScene final : public Actor
{
    using Perimeter = Models::Perimeter;
    using PerimeterObject = Objects::PerimeterObject;
    using Request = Models::Request;

public:
    explicit InRegionScene(QObject* parent = nullptr)
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
} // namespace Scene::Actors