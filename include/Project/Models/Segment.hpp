#pragma once

#include <QDebug>
#include <QPointF>

#include <cmath>

namespace Models {
class Segment : public Input
{
    size_t id{0};
    QPointF start;
    QPointF end;

    double baseSpeed;

public:
    Segment(const QPointF& s = QPointF{}, const QPointF& e = QPointF{}, const double b = 0)
        : start(s), end(e), baseSpeed(b)
    {
    }

    size_t getId() const override
    {
        return id;
    }

    // TODO: Тут принимаем, что входные с x и y.
    void initializeProperties(const QJsonObject& json) override
    {
        if (json.contains("id"))
            id = json["id"].toInt();

        // start
        QJsonObject startObj = json["start"].toObject();
        start = QPointF(startObj["x"].toDouble(), startObj["y"].toDouble());

        // end
        QJsonObject endObj = json["end"].toObject();
        end = QPointF(endObj["x"].toDouble(), endObj["y"].toDouble());

        baseSpeed = json["baseSpeed"].toDouble();
    }

    QJsonObject toJson() const override
    {
        QJsonObject obj;
        obj["id"] = static_cast<qint64>(id);

        // start
        QJsonObject startObj;
        startObj["x"] = start.x();
        startObj["y"] = start.y();
        obj["start"] = startObj;

        // end
        QJsonObject endObj;
        endObj["x"] = end.x();
        endObj["y"] = end.y();
        obj["end"] = endObj;

        obj["baseSpeed"] = baseSpeed;

        return obj;
    }

    double getSpeed() const
    {
        return baseSpeed;
    }

    QPointF getEnd() const
    {
        return end;
    }

    QPointF getStart() const
    {
        return start;
    }

    void setSpeed(const double speed)
    {
        baseSpeed = speed;
    }

    void setEnd(const QPointF& point)
    {
        end = point;
    }

    void setStart(const QPointF& point)
    {
        start = point;
    }

    void swapCoordinates()
    {
        auto xStart = start.x();
        start.setX(start.y());
        start.setY(xStart);

        auto xEnd = end.x();
        end.setX(end.y());
        end.setY(xEnd);
    }

    double length() const
    {
        return std::hypot(end.x() - start.x(), end.y() - start.y());
    }

    double time() const
    {
        return length() / baseSpeed;
    }

    void show() const
    {
        qDebug() << "start: [" << start.x() << ", " << start.y() << "], end: [" << end.x() << ", " << end.y() << "], speed: " << baseSpeed;
    }
};
} // namespace Models
