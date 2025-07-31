#pragma once

#include <QWidget>

#include "../Objects/RouteObject.hpp"
#include "./Limits.hpp"
#include "Project/Models/Target.hpp"

namespace Scene::Entities {
using RouteObject = Objects::RouteObject;
class Targets final : public RouteObject
{
    using TargetParameters = Models::Target;
    using Objects = Objects::Objects;

    Q_OBJECT

    TargetParameters targetParameters;
    bool modelLoaded = false;

    // double currentTime = 0;
    // double startTime = 0;
    // double endTime = 0;
    //! 1. После нажатия принимаем старт тайм.
    //! Если пустая то Добавляем первую точку и старт = финишу,
    //! Далее всегда прибавляем пока не нажали Reset.
    //! При этом каждый раз рассчитываем время окончания.
    //! При отрисовки учитывать когда находимся в рамках времени.
    //!

public:
    explicit Targets(QWidget* parent = nullptr)
        : RouteObject(parent)
    {
    }

public:
    TargetParameters getParameters() const
    {
        return targetParameters;
    }

    double getFullTime() const
    {
        return RouteObject::getFullTime();
    }

    void setRoute(const QVector<QPointF>& positions)
    {
        RouteObject::reset();

        // TODO: clear старые значения route
        if (positions.size() == 1)
        {
            this->addSegment(positions[0], positions[0], targetParameters.currentVelocity); // Скорость

            return;
        }

        for (int i = 0; i < positions.size() - 1; ++i)
        {
            if (positions[i] == positions[i + 1])
                continue;

            this->addSegment(positions[i], positions[i + 1], targetParameters.currentVelocity); // Скорость
        }
    }

    void reset()
    {
        modelLoaded = false;
        RouteObject::reset();
    }

    void draw(QPainter& painter)
    {
        RouteObject::draw(painter);
    }

    void move(double speedMultiplier)
    {
        if (RouteObject::move(speedMultiplier))
        {
            //            emit sendCurrentPositionSpeed(getCurrentPosition(), getSpeed());
            // sendTargetInformation(); // TODO: Сделать одним сигналом
            // sendTargetInformation(pair<QPointF, double>) // точка и скорость
        }
        else
        {
        }
    }

    void setParameters(const TargetParameters& params)
    {
        targetParameters = params;
    }

    void setModel(Objects model, double size)
    {
        RouteObject::initHead(targetParameters);
        RouteObject::setModel(model, targetParameters.getMinNoiseReduced(), size);
    }

    void setSpeed(double speed)
    {
        targetParameters.currentVelocity = speed;
    }

    void swapCoordinates()
    {
        RouteObject::swapCoordinates();
    }
};
} // namespace Scene::Entities
