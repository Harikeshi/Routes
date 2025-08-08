#pragma once

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class JsonRepository
{
    QString path;

public:
    JsonRepository(const QString& base_path) : path{base_path}
    {
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

private:
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