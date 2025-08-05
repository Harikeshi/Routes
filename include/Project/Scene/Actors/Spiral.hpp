#pragma once

#include <QGraphicsPolygonItem>

#include "../Actor.hpp"
#include "Project/Operations/GeometryOperations.hpp"
#include "Project/Scene/Entities/Limits.hpp"
#include "Project/Scene/Objects/ModelObject.hpp"
#include "Project/Scene/Objects/TargetObject.hpp"

#include <iostream>

namespace Scene::Actors {
class Spiral final : public Actor
{
    using Request = Models::Request;

public:
    explicit Spiral(QObject* parent = nullptr)
        : Actor(parent)
    {
        target = new Objects::TargetObject();
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
        if (!loaded || !target->isLoaded())
            return;

        const auto& start = target->getCurrentPosition();

        double maxLength = 500;

        QPen pen(Qt::black, 1); // Толщина
        pen.setCosmetic(true);
        painter.setPen(pen);

        // Отрисовать ПЛ
        painter.drawEllipse(start, target->getRootMeanSquareError(), target->getRootMeanSquareError());

        // Линия 1
        QPointF end = Operations::getPointAtAngleDistance(start, target->getCourses().first, maxLength);
        painter.drawLine(start, end);

        // Линия 2
        end = Operations::getPointAtAngleDistance(start, target->getCourses().second, maxLength);
        painter.drawLine(start, end);

        //  Острый угол??
        auto temp = qAbs(target->getCourses().first - target->getCourses().second);

        auto angle = (temp > 180) ? target->getCourses().first + (360 - temp) / 2 : (target->getCourses().first) + temp / 2;

        end = Operations::getPointAtAngleDistance(start, angle, maxLength);

        //! Увеличение происходит по времени.
        if (currentTime != 0)
        {
            pen = QPen(Qt::darkGreen, 4);
            pen.setStyle(Qt::DotLine);
            pen.setCosmetic(true);

            painter.setPen(pen);
            painter.drawEllipse(start, currentTime * target->getParameters().currentVelocity, currentTime * target->getParameters().currentVelocity);
        }

        // Средняя линия, линия движения
        pen = QPen(Qt::darkYellow, 4);
        pen.setStyle(Qt::DotLine);
        pen.setCosmetic(true);

        painter.setPen(pen);
        painter.drawLine(start, end);
    }

    void setCurrentTime(double time) override
    {
        currentTime = time;
    }

    virtual void swapCoordinates() override
    {
        target->swapCoordinates();
    }

    virtual void reload(const Request& request) override
    {
        // Инициализировать все сущности которые будут требоваться.
        target->setParameters(request.getTarget()); // Инициализация параметров.

        loaded = true;
    }

    virtual void reset() override
    {
        loaded = false;
        currentTime = 0;
    }

private:
    bool loaded = false;

    //! Модель Head
    Scene::Objects::TargetObject* target;

    double currentTime{0};
};
} // namespace Scene::Actors