#pragma once

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "IRepository.hpp"
#include "Project/GetCurrentUsername.hpp"

namespace Database {
class JsonRepository : public Database::IRepository
{
    QString path;

    // TODO:
    QString requestPath;
    QString reportPath;

public:
    JsonRepository(const QString& base_path) : Database::IRepository{base_path}
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
        appendJsonObjectToArrayFile(request.toJson(), requestPath);

        return 0;
    }

    size_t save(const Models::Report& report) override
    {
        appendJsonObjectToArrayFile(report.toJson(), reportPath);

        return 0;
    }

    Models::Request findRequestById(size_t id) override
    {
        return Models::Request{};
    }
    
    Models::Report findReportById(size_t id) override
    {
        return Models::Report{};
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

    bool appendJsonObjectToArrayFile(const QJsonObject& obj, const QString& fileName)
    {
        QJsonArray array = readJsonArrayFromFile(fileName);

        QJsonObject newEntry;

        newEntry["id"] = array.size();
        newEntry["date"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        newEntry["user"] = QString::fromStdString(getCurrentUsername());
        newEntry["data"] = obj;

        array.append(newEntry);

        return writeJsonArrayToFile(array, fileName);
    }

    bool createEmptyJsonArrayFile(const QString& fileName)
    {
        return writeJsonArrayToFile(QJsonArray{}, fileName);
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