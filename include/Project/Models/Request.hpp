#pragma once

#include <QPointF>
#include <QVector>

#include "Object.hpp"
#include "Perimeter.hpp"
#include "Target.hpp"

namespace Models {
struct Request
{
    size_t id{0};
    double time;
    Perimeter perimeter;
    QVector<QPointF> border{QPointF{0, 0}, QPointF{1, 1}, QPointF{2, 2}};
    Target target;
    Object ship;

    bool loaded = false;

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

    // TODO:
    void fromJson(const QJsonObject& json)
    {
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
};
} // namespace Models