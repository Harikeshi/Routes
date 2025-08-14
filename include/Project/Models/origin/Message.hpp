#pragma once

#include "../Input.hpp"
#include "../ValidateOperations.hpp"

namespace Models::origin {
struct Message : public Input
{
    int code;
    QString text;
    QString type;

    Message(int _code = 0, const QString& _text = QString{}, const QString& _type = QString{})
        : code{_code}, text{_text}, type{_type}
    {
        addValidator("code", [](const QJsonObject& json) { validateDigitValue(json, "code"); });
        addValidator("text", [](const QJsonObject& json) { validateString(json, "text"); });
        addValidator("type", [](const QJsonObject& json) { validateString(json, "type"); });
    }

    QJsonObject toJson() const override
    {
        QJsonObject obj;

        obj["code"] = code;
        obj["text"] = text;
        obj["type"] = type;

        return obj;
    }

    nlohmann::json toNJson() const override
    {
        nlohmann::json obj;

        obj["code"] = code;
        obj["text"] = text.toStdString();
        obj["type"] = type.toStdString();

        return obj;
    }

    void initializeProperties(const QJsonObject& json) override
    {
        code = json["code"].toInt();
        text = json["text"].toString();
        type = json["type"].toString();
    }

    void show() const
    {
        qDebug() << "Код: " << code << ", текст: " << text << ", тип: " << type;
    }
};
} // namespace Models::origin
