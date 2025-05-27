#pragma once

#include <QVector>

// TODO: Перенести сюда
#include "../Scene/Objects/Models/Route.hpp"

// TODO: также можно распарсить сообщение и вывести
namespace Data {
struct Report
{
    using Route = Scene::Objects::Models::Route;
    using Routes = QVector<Route>;

    // TODO: Чтобы поддержать концепцию реализовать Route в валидате
    Routes _routes;

    bool loaded = false;

    bool isLoaded() const
    {
        return loaded;
    }

public:
    Routes routes() const
    {
        return _routes;
    }

    // Перенести в Route
    void fromJson(const QJsonObject& json)
    {
        _routes.clear();

        for (const QJsonValue& _route : json["routes"].toArray())
        {
            Route route;

            route.fromJson(_route.toObject());

            _routes.push_back(route);
        }

        loaded = true;
    }
};
} // namespace Data