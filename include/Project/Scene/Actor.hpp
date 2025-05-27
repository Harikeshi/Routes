#pragma once

#include <QObject>
#include <QPainter>

#include "../Data/Request.hpp"

namespace Scene {
class Actor : public QObject
{
    using Request = Data::Request;

public:
    Actor(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    virtual void draw(QPainter& painter) = 0;
    virtual void reload(const Request& requst) = 0;
    virtual void swapCoordinates() = 0;
};
} // namespace Scene