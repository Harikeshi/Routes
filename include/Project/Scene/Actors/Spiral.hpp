#pragma once

#include <QGraphicsPolygonItem>

#include "../Actor.hpp"
#include "Project/Operations/GeometryOperations.hpp"
#include "Project/Scene/Entities/Limits.hpp"
#include "Project/Scene/Entities/Targets.hpp"
#include "Project/Scene/Objects/ModelObject.hpp"

#include <iostream>

namespace Scene::Actors {
class Spiral final : public Actor
{
    using Request = Models::Request;

public:
    explicit Spiral(QObject* parent = nullptr)
        : Actor(parent)
    {
        target = new Entities::Targets();
        loaded = false;
    }

    ~Spiral()
    {
        delete target;
    }

public:
    Entities::Limits getLimits() const
    {
        Entities::Limits limits;

        qDebug() << target->getCurrentPosition();
        limits.fromPoint(target->getCurrentPosition());

        limits.show();

        return limits;
    }

    virtual void draw(QPainter& painter) override
    {
        if (!loaded)
            return;
        QPen pen(Qt::black, 5); // Толщина
        pen.setCosmetic(true);

        const auto& start = target->getCurrentPosition();

        painter.setPen(pen);
        painter.drawEllipse(start, 5, 5);
        painter.drawEllipse(start, 10, 10);
        // Линия 1
        QPointF end = Operations::getPointAtAngleDistance(start, target->getCourses().first, 1e10);
        painter.drawLine(start, end);

        // Линия 2
        end = Operations::getPointAtAngleDistance(start, target->getCourses().second, 1e10);
        painter.drawLine(start, end);

        //  Острый угол??
        auto temp = qAbs(target->getCourses().first - target->getCourses().second);

        auto angle = (temp > 180) ? target->getCourses().first + (360 - temp) / 2 : (target->getCourses().first) + temp / 2;

        end = Operations::getPointAtAngleDistance(start, angle, 1e10);

        // Средняя линия, линия движения
        pen.setStyle(Qt::DotLine);
        painter.setPen(pen);
        painter.drawLine(start, end);
    }

    virtual void swapCoordinates() override
    {
    }

    virtual void reload(const Request& request) override
    {
        // Инициализировать все сущности которые будут требоваться.
        target->setParameters(request.getTarget()); // Инициализация параметров.

        loaded = true;
    }

    virtual void reset() override
    {
    }

private:
    bool loaded = false;
    //! Модель Head

    Scene::Entities::Targets* target;

    //! Окружность пунктирная
    Scene::Objects::SegmentObject* line0;
    Scene::Objects::SegmentObject* line1;

    Scene::Objects::SegmentObject* midLine;
};
} // namespace Scene::Actors