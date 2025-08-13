#pragma once

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "IRepository.hpp"
#include "Project/GetCurrentUsername.hpp"
#include <QDateTime>

namespace Database {
class JsonRepository : public Database::IRepository
{
    QString path;

    // TODO:
    QString requestPath;
    QString reportPath;

public:
    JsonRepository(const QString& base_path)
        : Database::IRepository{base_path}
    {
#ifdef _WIN32
        path = base_path + "\\json_db\\";
#else
        path = base_path + "/json_db/";
#endif
        QDir dir(path);

        if (!dir.exists())
        {
            if (dir.mkdir(path))
            {
            }
            else
            {
                qDebug() << "Папка не создана.";
            }
        }

        requestPath = path + "requests.json";
        reportPath = path + "reports.json";

        if (!QFile::exists(requestPath))
            createEmptyJsonArrayFile(requestPath);

        if (!QFile::exists(reportPath))
            createEmptyJsonArrayFile(reportPath);

        // QDir::homePath();
        // QFileInfo file{requestPath};
    }

    size_t save(const Models::Request& request) override
    {
        appendRequestJsonObjectToArrayFile(request.toJson());

        return 0;
    }

    size_t save(const Models::Report& report) override
    {
        appendReportJsonObjectToArrayFile(report.toJson());

        return 0;
    }

    // TODO: template<typename T>
    Models::Request findRequestById(size_t id) override
    {
        QJsonArray array = readJsonArrayFromFile(requestPath);

        auto it = std::find_if(array.begin(), array.end(), [&id](const QJsonValue& obj) { return obj["id"].toInt() == id; });

        // TODO: if(it)
        return initRequest(it->toObject());
    }

    Models::Report findReportById(size_t id) override
    {
        QJsonArray array = readJsonArrayFromFile(reportPath);

        auto it = std::find_if(array.begin(), array.end(), [&id](const QJsonValue& obj) { return obj["id"].toInt() == id; });

        auto value = it->toObject();

        return initReport(value);
    }

    Models::Report initReport(const QJsonObject& obj)
    {
        Models::Report report;

        report.fromJson(obj["data"].toObject());

        report.id = obj["id"].toInt();
        report.request_id = obj["request_id"].toInt();
        report.owner = obj["owner"].toString();
        report.created_at = QDateTime::fromString(obj["created_at"].toString(), "yyyy-MM-dd hh:mm:ss");

        return report;
    }

    size_t getLastReportId() override
    {
        auto reports = readJsonArrayFromFile(reportPath);

        return reports.last()["id"].toInt();
    }

    Models::Request initRequest(const QJsonObject& obj)
    {
        Models::Request request;

        request.fromJson(obj["data"].toObject());

        request.owner = obj["owner"].toString();
        request.created_at = QDateTime::fromString(obj["created_at"].toString(), "yyyy-MM-dd hh:mm:ss");
        request.id = obj["id"].toInt();

        return request;
    }

    QVector<Models::Request> getAllRequests() override
    {
        auto requests = readJsonArrayFromFile(requestPath);

        QVector<Models::Request> result;

        for (const auto& request : requests)
        {
            result.push_back(initRequest(request.toObject()));
        }

        return result;
    }

    QVector<Models::Report> getAllReports() override
    {
        auto reports = readJsonArrayFromFile(reportPath);

        QVector<Models::Report> result;

        for (auto report : reports)
        {
            result.push_back(initReport(report.toObject()));
        }

        return result;
    }

    size_t getLastRequestId() override
    {
        auto requests = readJsonArrayFromFile(requestPath);
        return requests.last()["id"].toInt();
    }

private:
    QJsonArray readJsonArrayFromFile(const QString& fileName)
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning("Couldn't open file for reading. Returning empty array.");
            return QJsonArray();
        }

        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isArray())
        {
            qWarning("File does not contain a JSON array. Returning empty array.");
            return QJsonArray();
        }

        return doc.array();
    }

    bool writeJsonArrayToFile(const QJsonArray& array, const QString& fileName)
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            qWarning("Couldn't open file for writing.");
            return false;
        }

        QJsonDocument doc(array);
        if (file.write(doc.toJson()) == -1)
        {
            file.close();
            return false;
        }

        file.close();
        return true;
    }

    bool appendRequestJsonObjectToArrayFile(const QJsonObject& obj)
    {
        QJsonArray requests = readJsonArrayFromFile(requestPath);

        QJsonObject newEntry;

        newEntry["id"] = requests.last()["id"].toInt() + 1;
        newEntry["date"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        newEntry["user"] = QString::fromStdString(getCurrentUsername());
        newEntry["data"] = obj;

        requests.append(newEntry);

        return writeJsonArrayToFile(requests, requestPath);
    }

    bool appendReportJsonObjectToArrayFile(const QJsonObject& obj)
    {
        QJsonArray requests = readJsonArrayFromFile(requestPath);
        QJsonArray reports = readJsonArrayFromFile(reportPath);

        QJsonObject newEntry;

        newEntry["id"] = reports.last()["id"].toInt() + 1;
        newEntry["request_id"] = requests.last()["id"].toInt();
        newEntry["date"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        newEntry["user"] = QString::fromStdString(getCurrentUsername());
        newEntry["data"] = obj;

        reports.append(newEntry);

        return writeJsonArrayToFile(reports, reportPath);
    }

    bool createEmptyJsonArrayFile(const QString& fileName)
    {
        return writeJsonArrayToFile(QJsonArray{}, fileName);
    }

    bool removeObjectById(const QString& fileName, const QString& id)
    {
        QJsonArray jsonArray = readJsonArrayFromFile(fileName);
        QJsonArray newArray;
        bool found = false;

        for (const QJsonValue& value : jsonArray)
        {
            QJsonObject obj = value.toObject();
            if (obj["id"].toString() != id)
            {
                newArray.append(obj);
            }
            else
            {
                found = true;
            }
        }

        if (!found)
        {
            qWarning() << "ID" << id << "not found";
            return false;
        }

        return writeJsonArrayToFile(newArray, fileName);
    }

    bool isIdExists(const QJsonArray& array, const QString& id)
    {
        for (const QJsonValue& value : array)
        {
            if (value.toObject()["id"].toString() == id)
            {
                return true;
            }
        }
        return false;
    }

    // Модифицированная версия appendDataToJsonFile с проверкой ID
    bool appendDataToJsonFileWithIdCheck(const QString& fileName,
                                         const QString& id,
                                         const QString& name,
                                         const QJsonObject& dataObject)
    {
        QJsonArray jsonArray = readJsonArrayFromFile(fileName);

        if (isIdExists(jsonArray, id))
        {
            qWarning() << "ID" << id << "already exists";
            return false;
        }

        QJsonObject newEntry;
        newEntry["id"] = id;
        newEntry["date"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        newEntry["name"] = name;
        newEntry["data"] = dataObject;

        jsonArray.append(newEntry);
        return writeJsonArrayToFile(jsonArray, fileName);
    }

    // Repository methods
    template<typename T>
    void save(const QString& filename, const T& item)
    {
        QJsonObject json = item.toJson();
        saveJsonToFile(filename, json);
    }

    template<typename T>
    T load(const QString& filename, size_t id)
    {
        QJsonObject json = loadJsonFromFile(filename);
        T item;
        item.fromJson(json);
        if (item.getId() != id)
        {
            throw std::runtime_error("ID mismatch in loaded object");
        }
        return item;
    }

    template<typename T>
    std::map<size_t, T> loadAll(const QString& filename)
    {
        QJsonArray array = loadJsonArrayFromFile(filename);
        std::map<size_t, T> result;

        for (const QJsonValue& val : array)
        {
            T item;
            item.fromJson(val.toObject());
            result[item.getId()] = item;
        }

        return result;
    }

    template<typename T>
    void saveAll(const QString& filename, const std::map<size_t, T>& items)
    {
        QJsonArray array;

        for (const auto& pair : items)
        {
            array.append(pair.second.toJson());
        }

        saveJsonArrayToFile(filename, array);
    }

    void saveJsonToFile(const QString& filename, const QJsonObject& json)
    {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly))
        {
            throw std::runtime_error("Could not open file for writing");
        }

        QJsonDocument doc(json);
        file.write(doc.toJson());
        file.close();
    }

    QJsonObject loadJsonFromFile(const QString& filename)
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
        {
            throw std::runtime_error("Could not open file for reading");
        }

        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        return doc.object();
    }

    void saveJsonArrayToFile(const QString& filename, const QJsonArray& array)
    {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly))
        {
            throw std::runtime_error("Could not open file for writing");
        }

        QJsonDocument doc(array);
        file.write(doc.toJson());
        file.close();
    }

    QJsonArray loadJsonArrayFromFile(const QString& filename)
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
        {
            throw std::runtime_error("Could not open file for reading");
        }

        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        return doc.array();
    }
};
} // namespace Database