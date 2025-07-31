#pragma once

#include <QVector>

#include "Message.hpp"
#include "Route.hpp"

//TODO: Проверка есть сообщения и Пути
namespace Models {
struct Report
{
    size_t id{0};
    // TODO: Чтобы поддержать концепцию реализовать Route в validate
    QVector<Route> _routes;
    QVector<Message> _messages;

    bool loaded = false;

    bool isLoaded() const
    {
        return loaded;
    }

public:
    QVector<Route> routes() const
    {
        return _routes;
    }

    // Перенести в Route
    void fromJson(const QJsonObject& json)
    {
        for (const auto& message : json["messages"].toArray())
        {
            Message mes{};
            mes.fromJson(message.toObject());
            _messages.push_back(mes);
        }

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
} // namespace Models
