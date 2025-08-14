#pragma once

#include <QVector>

#include "Message.hpp"
#include "Route.hpp"

//TODO: Проверка есть сообщения и Пути
namespace Models {
// TODO: Модель для отображения достраивать от этой.
struct Report : public Input
{
    //! DAO
    size_t id{0};
    size_t request_id{0};
    QDateTime created_at;
    QString owner;
    QString scheme;

    // TODO: Чтобы поддержать концепцию реализовать Route в validate
    //! origin
    QVector<Route> _routes;
    QVector<Message> _messages;

    //! for scene
    bool loaded = false;

    void show() const
    {
        qDebug() << "Messages: ";
        for (const auto& message : _messages)
        {
            message.show();
        }

        qDebug() << "Routes:";
        for (const auto& route : _routes)
        {
            route.show();
        }
    }

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

        _messages.clear();
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

    nlohmann::json toNJson() const override
    {
        nlohmann::json obj;

        auto routesArray = nlohmann::json::array();

        for (const Route& route : _routes)
        {
            routesArray.emplace_back(route.toNJson());
        }

        obj["routes"] = routesArray;

        auto messagesArray = nlohmann::json::array();
        for (const Message& message : _messages)
        {
            messagesArray.emplace_back(message.toNJson());
        }

        obj["messages"] = messagesArray;

        return obj;
    }

    QVector<Route> routes() const
    {
        return _routes;
    }
};
} // namespace Models
