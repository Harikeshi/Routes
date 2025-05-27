#pragma once

#include <QTimer>
#include <QWidget>

#include "../Objects/Models/Target.hpp"
#include "../Objects/RouteObject.hpp"
#include "./Limits.hpp"

namespace Scene {
namespace Entities {
using RouteObject = Objects::RouteObject;
class Targets : public RouteObject
{
    using TargetParameters = Objects::Models::Target;
    using Objects = Objects::Objects;

    Q_OBJECT

    TargetParameters targetParameters;

    QVector<QPointF> targetPath;

signals:
    void sendCurrentPosition(const QPointF&);

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

    const QVector<QPointF>& getTargetPath() const
    {
        return targetPath;
    }

    void addToTargetPath(const QPointF& point)
    {
        targetPath.append(point);
    }

    void setRoute()
    {
        // TODO: clear старые значения route
        RouteObject::reset();

        if (targetPath.size() == 1)
        {
            this->addSegment(targetPath[0], targetPath[0], targetParameters.currentVelocity); // Cкорость
            return;
        }

        for (int i = 0; i < targetPath.size() - 1; ++i)
        {
            if (targetPath[i] == targetPath[i + 1])
                continue;

            this->addSegment(targetPath[i], targetPath[i + 1], targetParameters.currentVelocity); // Cкорость
        }
    }

    void reset()
    {
        targetPath.clear();

        RouteObject::reset();
    }

    void swapCoordinates()
    {
        RouteObject::swapCoordinates();

        for (size_t i = 0; i != targetPath.size(); ++i)
        {
            auto x = targetPath[i].x();
            targetPath[i].setX(targetPath[i].y());
            targetPath[i].setY(x);
        }
    }

    void draw(QPainter& painter)
    {
        QPen pen = QPen(Qt::darkYellow, 2);
        pen.setStyle(Qt::DotLine);
        pen.setCosmetic(true);

        painter.setPen(pen);

        for (int i = 0; i < targetPath.size() - 1; ++i)
        {
            painter.drawLine(targetPath[i], targetPath[i + 1]);
        }

        RouteObject::draw(painter);
    }

    void setTargetPath(const QVector<QPointF>& path)
    {
        targetPath = path;

        this->setRoute();
    }

    bool targetPathIsEmpty() const
    {
        return targetPath.isEmpty();
    }

    size_t targetPathSize() const
    {
        return targetPath.size();
    }

    void move(const double& speedMultiplier)
    {
        if (this->update(speedMultiplier))
        {
            emit sendCurrentPosition(getCurrentPosition());
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

    void setModel(const Limits& limits)
    {
        RouteObject::initHead(targetParameters);
        RouteObject::setModel(Objects::Arrow, targetParameters.getMinNoiseReduced(), 0.01 * limits.getMaxDifference());
    }
};
} // namespace Entities
} // namespace Scene