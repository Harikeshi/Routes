#pragma once

#include <QWidget>

#include "Project/Models/Target.hpp"
#include "Project/Scene/Entities/Limits.hpp"
#include "Project/Scene/Objects/RouteState.hpp"
#include "RouteObject.hpp"

namespace Scene::Objects {
class TargetObject final : public RouteObject
{
    Q_OBJECT

    Models::Target targetParameters;
    bool modelLoaded = false;

    //! 1. После нажатия принимаем старт тайм.
    //! Если пустая то Добавляем первую точку и старт = финишу,
    //! Далее всегда прибавляем пока не нажали Reset.
    //! При этом каждый раз рассчитываем время окончания.
    //! При отрисовки учитывать когда находимся в рамках времени.
    //!

public:
    explicit TargetObject(QWidget* parent = nullptr)
        : RouteObject(parent)
    {
        //        this->setState(new Scene::Objects::WithOutDrawState());
    }

public:
    // TODO: Пересмотреть Инициализация только Цели начальной позицией.
    void initialize(const Models::Target& parameters, double pointSize, const QColor& color = Qt::black)
    {
        //! Установить Первую точку
        // pointsWidget.clear();
        // pointsWidget.push_back(new PointWidget(parameters.detectionPoint, pointSize, color));

        segments.clear();
        this->segments.push_back(new SegmentObject(parameters.detectionPoint, parameters.detectionPoint, parameters.currentVelocity, this));

        this->setColor(color);
        this->setRadius(parameters.avoidanceDistance);

        this->head->initialize(parameters);

        currentSegmentIndex = 0;
        // clear();
    }
    bool isLoaded() const
    {
        return modelLoaded;
    }

    auto getParameters() const
    {
        return targetParameters;
    }

    //    double getFullTime() const
    //    {
    //        return RouteObject::getFullTime();
    //    }

    QPair<double, double> getCourses() const
    {
        return qMakePair(targetParameters.courses.first, targetParameters.courses.second);
    }

    void setPosition()
    {
    }

    double getRootMeanSquareError() const
    {
        return targetParameters.rootMeanSquareError;
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

    void setParameters(const Models::Target& params)
    {
        targetParameters = params;
        modelLoaded = true;
    }

    void setModel(Objects model, double size)
    {
        RouteObject::initHead(targetParameters);
        RouteObject::setModel(model, targetParameters.minNoiseReduced, size);
        modelLoaded = true;
    }

    void show() const
    {
        RouteObject::show();
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
} // namespace Scene::Objects
