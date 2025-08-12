#pragma once

#include "Models/Report.hpp"
#include "Models/Request.hpp"

#include "MessageType.hpp"
#include "Models/Perimeter.hpp"
#include "Scene/Entities/Limits.hpp"

#include <QObject>
#include <QString>

#include "Project/Database/JsonRepository.hpp"
#include "Project/Database/PostgreSQLRepository.hpp"

// TODO: Инициализатор должен только отправлять Request и Report и сообщение

/*!
 * Хранит состояние текущих Request и Repost.
 */
class Initializer : public QObject
{
    using Limits = Scene::Entities::Limits;
    Q_OBJECT
signals:
    void sendRequestJson(QJsonObject);

    void sendReportJson(QJsonObject);

    void sendMessage(QString);
    void sendError(QString);

    void changedRequest(const Models::Request&);
    void changedReport(const Models::Report&);

private:
    inline QString getHomePath()
    {
        QString result;
#ifdef _WIN32
        result = QString(std::getenv("USERPROFILE"));
#else
        result = QString(getenv("HOME"));
#endif

        return result;
    }

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
    bool isCorrect(const QVector<FieldInfo> fields, const QJsonObject& json)
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
                    emit sendError("Пропущено поле: " + field.path);
                    return false;
                }
            }

            if (current.isUndefined())
            {
                emit sendError("Пропущено поле: " + field.path);
                return false;
            }

            if (field.type != QJsonValue::Undefined && current.type() != field.type)
            {
                emit sendError(QString("Ошибка Поле: [%1], Except: %2, Receive: %3").arg(field.path).arg(typeToString(field.type)).arg(typeToString(current.type())));
                return false;
            }
        }

        return true;
    }

    bool isReport(const QJsonObject& json)
    {
        emit sendMessage("Проверка файла Report.");
        return isCorrect(reportFields, json);
    }

    bool isRequest(const QJsonObject& json)
    {
        emit sendMessage("Проверка файла Request.");
        return isCorrect(requestFields, json);
    }

public:
    void saveRequest(const Models::Request& request)
    {
        // TODO: Валидация
        QString message;
        try
        {
            this->request = request;
            this->request.id = repository->save(this->request);

            message += "Файл request загружен в базу!";

            emit changedRequest(request);
        }
        catch (std::runtime_error& ex)
        {
            emit sendError(QString("Файл request не загружен: %1").arg(ex.what()));
        }

        emit sendMessage(message);
    }

    void loadRequest(const QJsonObject& json)
    {
        QString message;
        try
        {
            // TODO: Формирование сообщения вынести выше по логике
            message += "Файл определен, как request. ";
            // TODO:
            request.fromJson(json);

            message += "Файл request загружен в базу!";

            emit changedRequest(request);
            // request.id = repository->save(request);
        }
        catch (std::runtime_error& ex)
        {
            emit sendMessage(QString(message + "Файл request.json не загружен: %1").arg(ex.what()));
        }

        emit sendMessage(message);
    }

    void loadReport(const QJsonObject& json)
    {
        QString message;

        try
        {
            message += "Файл определен, как report.";
            report.fromJson(json);
            message += "Файл report загружен в базу!";

            // TODO: Какое-то костыльное решение или нет
            // report.request_id = request.id;
            // repository->save(report);

            emit changedReport(report);
        }
        catch (std::runtime_error& ex)
        {
            message += "Файл report не загружен: ";
            emit sendError(QString(message + "%1").arg(ex.what()));

            return;
        }

        emit sendMessage(message);
    }

    void loadFromJson(const QJsonObject& json) // message
    {
        // QString message; // TODO: Отправить сообщением {text, type}
        // MessageType type = MessageType::Info;

        if (isRequest(json))
        {
            request_json = json;

            loadRequest(json);
        }
        else if (isReport(json))
        {
            report_json = json;

            loadReport(json);
        }
        else
        {
            emit sendError("Файл не соответствует требованиям");
        }
    }

    Models::Request getRequest() const
    {
        return request;
    }

    Models::Report getReport() const
    {
        return report;
    }

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
    //    QString connectionString = "host=127.0.0.1 dbname=request_report user=viz_user password=1 connect_timeout=3";
    // QString connectionString = "host=192.168.50.52 dbname=request_report user=viz_user password=1 connect_timeout=3";
    QString connectionString = "host=192.168.205.130 dbname=request_report user=viz_user password=1 connect_timeout=3";

public:
    static Initializer& instance()
    {
        static Initializer instance;
        return instance;
    }

    Initializer() = default;
    ~Initializer() = default;

    // Initializer(const std::string& str)
    // {
    //     // TODO: getHomePath()
    //     try
    //     {
    //         // TODO: need Create Factory
    //         repository = std::make_unique<Database::PostgreSQLRepository>(connectionString);
    //         qDebug() << "Request/Report PostgreSQL connected!" + connectionString.split(' ')[0];
    //     }
    //     catch (std::runtime_error& ex)
    //     {
    //         // TODO: сообщение
    //         qDebug() << "Не удалось подключиться к postgreSQL Request/Report." + connectionString.split(' ')[0] + ": " + QString(ex.what());
    //         repository = std::make_unique<Database::JsonRepository>(getHomePath());
    //     }
    // }

    void setRequest(const Models::Request& request)
    {
        this->request = request;
    }

private:
    std::unique_ptr<Database::IRepository> repository;

    QJsonObject request_json;
    QJsonObject report_json;

    Models::Request request;
    Models::Report report;
};
