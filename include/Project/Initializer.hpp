#pragma once

#include "Models/Report.hpp"
#include "Models/Request.hpp"

#include "MessageType.hpp"
#include "Models/Perimeter.hpp"
#include "Scene/Entities/Limits.hpp"

#include <QObject>
#include <QString>

// TODO: Инициализатор должен только отправлять Request и Report и сообщение
class Initializer : public QObject
{
    using Limits = Scene::Entities::Limits;
    Q_OBJECT
signals:
    //    void sendRequest(Models::Request);
    //    void sendReport(Models::Report);
    void sendMessage(QString, MessageType);

    void reportLoaded();
    void requestLoaded();

    //    void sendLimits(Limits);

    void sendRequestJson(QJsonObject);
    void sendReportJson(QJsonObject);

private:
    struct FieldInfo
    {
        QString path;
        QJsonValue::Type type;
    };

    // Простая валидация на наличие полей
    QString typeToString(QJsonValue::Type type)
    {
        switch (type)
        {
        case QJsonValue::Null:
            return "Null";
        case QJsonValue::Bool:
            return "Bool";
        case QJsonValue::Double:
            return "Double";
        case QJsonValue::String:
            return "String";
        case QJsonValue::Array:
            return "Array";
        case QJsonValue::Object:
            return "Object";
        case QJsonValue::Undefined:
            return "Undefined";
        default:
            return "Unknown";
        }
    }

    // TODO: Поля лучше хранить или генерить
    QVector<FieldInfo> requestFields{
        //
        {"time", QJsonValue::Double},
        {"form_target", QJsonValue::Object},
        {"search_region", QJsonValue::Object},
        {"border_line", QJsonValue::Array},
        {"ships_parameters", QJsonValue::Object},

        // form_target
        {"form_target.detection_point", QJsonValue::Array},
        {"form_target.root-mean-square_error", QJsonValue::Double},
        {"form_target.courses", QJsonValue::Array},
        {"form_target.velocities", QJsonValue::Object},
        {"form_target.obsolescence_time", QJsonValue::Double},
        {"form_target.avoidance_distance", QJsonValue::Double},

        // velocities
        {"form_target.velocities.current", QJsonValue::Double},
        {"form_target.velocities.max", QJsonValue::Double},
        {"form_target.velocities.min_noise-reduced", QJsonValue::Double},
        {"form_target.velocities.max_noise-reduced", QJsonValue::Double},

        // search_region
        {"search_region.borders", QJsonValue::Array},
        {"search_region.entry_point", QJsonValue::Array},
        {"search_region.exit_point", QJsonValue::Array},

        // ships_parameters
        /*{"ships_parameters.count", QJsonValue::Double},*/
        {"ships_parameters.detection_range", QJsonValue::Double},
        {"ships_parameters.max_velocity", QJsonValue::Double},
        {"ships_parameters.search_velocity", QJsonValue::Double},
        {"ships_parameters.turning_radius", QJsonValue::Double},
        {"ships_parameters.min_lenght_section", QJsonValue::Double}};

    QVector<FieldInfo> reportFields{{"routes", QJsonValue::Array}};

    // Высокоуровневая проверка, только наличие полей
    bool isCorrect(const QVector<FieldInfo> fields, const QJsonObject& json, QString& message)
    {
        for (const auto& field : fields)
        {
            QStringList path = field.path.split('.');
            QJsonValue current = json;

            for (const QString& part : path)
            {
                if (current.isObject())
                {
                    current = current.toObject()[part];
                }
                else
                {
                    //emit sendMessage("Пропущено поле:" + field.path, MessageType::Warning);
                    return false;
                }
            }

            if (current.isUndefined())
            {
                //emit sendMessage("Пропущено поле:" + field.path, MessageType::Warning);
                return false;
            }

            if (field.type != QJsonValue::Undefined && current.type() != field.type)
            {
                //emit sendMessage(QString("Ошибка Поле:[%1], Except: %2, Recieve: %3").arg(field.path).arg(typeToString(field.type)).arg(typeToString(current.type())), MessageType::Warning);
                return false;
            }
        }

        return true;
    }

    bool isReport(const QJsonObject& json, QString& message)
    {
        //emit sendMessage("Проверка файла Report.", MessageType::Info);
        return isCorrect(reportFields, json, message);
    }

    bool isRequest(const QJsonObject& json, QString& message)
    {
        //emit sendMessage("Проверка файла Request.", MessageType::Info);
        return isCorrect(requestFields, json, message);
    }

public:
    void loadRequest(const QJsonObject& json, QString& message, MessageType& type)
    {
        try
        {
            // TODO: Формирование сообщения вынести выше по логике
            message += "Файл определен, как request. ";
            request.fromJson(json);

            message += "Файл request загружен в базу!";

            // emit sendRequest(request); // Отослать Request
            emit requestLoaded();
            emit sendRequestJson(json); // Для инициализации dataWidget

            //            setLimits(); // Рассчитать лимиты и отправить
        }
        catch (std::runtime_error& ex)
        {
            message += "Файл request не загружен:";
            message += ex.what();
            type = MessageType::Error;
        }
    }

    void loadReport(const QJsonObject& json, QString& message, MessageType& type)
    {
        try
        {
            message += "Файл определен, как report.";
            report.fromJson(json);

            message += "Файл report загружен в базу!";
            // TODO: Какое-то костыльное решение

            emit reportLoaded();
            // emit sendReport(report);
            emit sendReportJson(json); // Для инициализации dataWidget

            //            setLimits();
        }
        catch (std::runtime_error& ex)
        {
            message += "Файл report не загружен:";
            message += ex.what();

            type = MessageType::Error;
        }
    }

    void loadFromJson(const QJsonObject& json) // message
    {
        QString message; // TODO: Отправить сообщением {text, type}
        MessageType type = MessageType::Info;

        if (isRequest(json, message))
        {
            request_json = json;
            loadRequest(json, message, type);
        }
        else if (isReport(json, message))
        {
            report_json = json;
            loadReport(json, message, type);
        }
        else
        {
            // Файл не соответствует требованиям
            message = "Файл не подходит!";
            type = MessageType::Error;
        }

        emit sendMessage(message, type);
    }

    //    void setLimits()
    //    {
    //        limits.reset();
    //
    //        if (report.isLoaded())
    //        {
    //            limits.initFromRoutes(report.routes());
    //        }
    //
    //        if (request.isLoaded())
    //        {
    //            limits.initFromPerimeter(request.getPerimeter());
    //        }
    //
    //        emit sendLimits(limits);
    //    }

    Models::Request getRequest() const
    {
        return request;
    }

    Models::Report getReport() const
    {
        return report;
    }

    //    Limits getLimits() const
    //    {
    //        return limits;
    //    }

    QVector<Models::Message> getMessages() const
    {
        return report._messages;
    }

    QJsonObject getRequestJson() const
    {
        return request_json;
    }

    QJsonObject getReportJson() const
    {
        return report_json;
    }

private:
    //    Limits limits;

    QJsonObject request_json;
    QJsonObject report_json;

    Models::Request request;
    Models::Report report;
};
