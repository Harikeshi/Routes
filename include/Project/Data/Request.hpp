#pragma once

#include <QFile>
#include <QPointF>
#include <QVector>

#include "../Scene/Objects/Models/Object.hpp"
#include "../Scene/Objects/Models/Perimeter.hpp"
#include "../Scene/Objects/Models/Target.hpp"

namespace Data {
struct Request
{
    using Perimeter = Scene::Objects::Models::Perimeter;
    using Target = Scene::Objects::Models::Target;
    using Object = Scene::Objects::Models::Object;

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
} // namespace Data