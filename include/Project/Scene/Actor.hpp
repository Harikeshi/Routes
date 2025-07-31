#pragma once

#include <QObject>

#include "Project/Models/Request.hpp"

namespace Scene {
class Actor : public QObject
{
    using Request = Models::Request;

public:
    ~Actor() = default;
    Actor(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    virtual void draw(QPainter& painter) = 0;
    virtual void reload(const Request& request) = 0;
    virtual void swapCoordinates() = 0;
};
} // namespace Scene
