#pragma once

#include <QVector>

#include "Message.hpp"
#include "Route.hpp"

//TODO: Проверка есть сообщения и Пути
namespace Models {
struct Report : public Input
{
    size_t id{0};
    size_t request_id{0};
    // TODO: Чтобы поддержать концепцию реализовать Route в validate
    QVector<Route> _routes;
    QVector<Message> _messages;

    bool loaded = false;

public:
    size_t getId() const override
    {
        return id;
    }

    // Перенести в Route
    void initializeProperties(const QJsonObject& json) override
    {
        if (json.contains("id"))
            id = json["id"].toInt();

        for (const auto& message : json["messages"].toArray())
        {
            Message mes{0, 0, "", ""};
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

    QJsonObject toJson() const override
    {
        QJsonObject obj;
        obj["id"] = static_cast<qint64>(id);

        QJsonArray routesArray;
        for (const Route& route : _routes)
        {
            routesArray.append(route.toJson());
        }
        obj["routes"] = routesArray;

        QJsonArray messagesArray;
        for (const Message& message : _messages)
        {
            messagesArray.append(message.toJson());
        }
        obj["messages"] = messagesArray;

        return obj;
    }

    bool isLoaded() const
    {
        return loaded;
    }

    QVector<Route> routes() const
    {
        return _routes;
    }
};
} // namespace Models
