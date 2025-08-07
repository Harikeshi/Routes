#pragma once

#include "Input.hpp"
#include "ValidateOperations.hpp"

namespace Models {
struct Message : public Input
{
    size_t id;
    double code;
    QString text;
    QString type;

public:
    Message()
        : id{0}, code{0.}, text{""}, type{""}
    {
        // code , text , type
        // json["messages"][0]
        addValidator("code", [](const QJsonObject& json) { validateDigitValue(json, "code"); });
        addValidator("text", [](const QJsonObject& json) { validateString(json, "text"); });
        addValidator("type", [](const QJsonObject& json) { validateString(json, "type"); });
    }

    size_t getId() const override
    {
        return id;
    }

    QJsonObject toJson() const override
    {
        QJsonObject obj;

        obj["id"] = static_cast<qint64>(id);
        obj["code"] = code;
        obj["text"] = text;
        obj["type"] = type;

        return obj;
    }

    void initializeProperties(const QJsonObject& json) override
    {
        code = json["code"].toDouble();
        text = json["text"].toString();
        type = json["type"].toString();
    }
};
} // namespace Models
