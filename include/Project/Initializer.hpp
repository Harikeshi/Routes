#pragma once

#include "Models/Report.hpp"
#include "Models/Request.hpp"

#include <QObject>
#include <QString>

// TODO: валидация, хранение и передача.

/*!
 * Хранит текущее состояние Request и Repost.
 */
class Initializer : public QObject
{
    Q_OBJECT
signals:
    void sendMessage(const QString&);
    void sendError(const QString&);

    void changedRequest(const Models::Request&);
    void changedReport(const Models::Report&);

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
    void saveRequest(const Models::Request& _request)
    {
        // TODO: Валидация
        QString message;
        try
        {
            message += "Файл request загружен в базу!";

            // TODO: Валидация
            this->setRequest(_request);
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
            message += "Файл определен, как request. ";
            // TODO:
            request.fromJson(json);

            message += "Файл request загружен в базу!";

            emit changedRequest(request);
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

    /*!
     * При загрузке из Базы данных.
     * @param _request
     */
    void loadRequestFromDb(const Models::Request& _request)
    {
        request = _request;
    }

    /*!
     * При загрузке из Базы данных.
     * @param _report
     */
    void loadReportFromDb(const Models::Report& _report)
    {
        report = _report;
    }

    void setRequest(const Models::Request& _request)
    {
        //TODO: Валидация.
        request = _request;
        emit changedRequest(request);
    }

    void setReport(const Models::Report& _report)
    {
        //TODO: Валидация.
        report = _report;
        emit changedReport(report);
    }

    static Initializer& instance()
    {
        static Initializer instance;
        return instance;
    }

private:
    Initializer() = default;
    ~Initializer() = default;

    QJsonObject request_json;
    QJsonObject report_json;

    Models::Request request;
    Models::Report report;
};
