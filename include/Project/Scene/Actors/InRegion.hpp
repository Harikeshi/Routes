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
    Entities::Limits getLimits() const override
    {
        Entities::Limits limits;

        limits.initFromPerimeter(perimeter->getPerimeter());

        return limits;
    }

    void show()
    {
        perimeter->show();
    }

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

        //        Entities::Limits limits;
        //        limits.initFromPerimeter(request.getPerimeter());
        //
        //        return limits;
    }

    void setCurrentTime(double time) override
    {
    }

    virtual void reset() override
    {
        perimeter = new PerimeterObject(this);
    }

private:
    PerimeterObject* perimeter;
};
} // namespace Scene::Actors