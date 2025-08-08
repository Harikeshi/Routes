#pragma once

#include <QPointF>
#include <QVector>

#include "Object.hpp"
#include "Perimeter.hpp"
#include "Target.hpp"

namespace Models {
struct Request : public Input
{
    size_t id{0};
    double time;
    Perimeter perimeter;
    QVector<QPointF> border{QPointF{0, 0}, QPointF{1, 1}, QPointF{2, 2}};
    Target target;
    Object ship;

    bool loaded = false;

    Request() = default;
    
    // TODO:
    size_t getId() const override
    {
        return id;
    }

    void initializeProperties(const QJsonObject& json) override
    {
        if (json.contains("id"))
            id = json["id"].toInt();

        time = json["time"].toDouble();
        target.fromJson(json["form_target"].toObject());
        perimeter.fromJson(json["search_region"].toObject());
        ship.fromJson(json["ships_parameters"].toObject());

        // init array
        if (json.contains("border_line"))
        {
            QJsonArray borderArray = json["border_line"].toArray();
            border.clear();

            for (const QJsonValue& pointValue : borderArray)
            {
                auto pointObj = pointValue.toArray();

                QPointF point(pointObj.at(0).toDouble(), pointObj.at(1).toDouble());
                border.append(point);
            }
        }

        loaded = true;
    }

    QJsonObject toJson() const override
    {
        QJsonObject obj;
        obj["id"] = static_cast<qint64>(id);
        obj["time"] = time;

        obj["perimeter"] = perimeter.toJson();

        QJsonArray borderArray;
        for (const QPointF& point : border)
        {
            QJsonObject pointObj;
            pointObj["x"] = point.x();
            pointObj["y"] = point.y();
            borderArray.append(pointObj);
        }
        obj["border"] = borderArray;

        obj["target"] = target.toJson();
        obj["ship"] = ship.toJson();

        return obj;
    }

    bool isLoaded() const
    {
        return loaded;
    }

    // TODO: getPerimeter()
    Perimeter getPerimeter() const
    {
        return perimeter;
    }

    Object getShip() const
    {
        return ship;
    }

    Target getTarget() const
    {
        return target;
    }

    void show() const
    {
        getPerimeter().show();
    }
};
} // namespace Models