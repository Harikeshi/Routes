#pragma once

#include <QJsonArray>
#include <QJsonObject>

namespace Models {
// Содержит поле.
inline void checkContains(const QJsonObject& json, const QString& key)
{
    if (!json.contains(key))
    {
        throw std::runtime_error("[" + key.toStdString() + "] Поле отсутствует.");
    }
}

// double
inline void checkDouble(const QJsonValue& json, const QString& key)
{
    if (!json.isDouble())
    {
        throw std::runtime_error("[" + key.toStdString() + "] Поле не является цифрой.");
    }
}

// array
inline void checkArray(const QJsonValue& json, const QString& key)
{
    if (!json.isArray())
    {
        throw std::runtime_error("[" + key.toStdString() + "] Поле не является массивом.");
    }
}

// is empty
inline void checkEmpty(const QJsonArray& json, const QString& key)
{
    if (json.isEmpty())
        throw std::runtime_error("[" + key.toStdString() + "] Пустое поле.");
}

// is size
inline void checkSize(const QJsonArray& json, const QString& key, size_t size)
{
    if (json.size() != size)
    {
        throw std::runtime_error("[" + key.toStdString() + "] Не подходящий размер массива.");
    }
}

inline void checkInterval(const QJsonValue& json, const QString& key, double min, double max)
{
    // Проверка: лежит в интервале.
    if (json.toDouble() < min || json.toDouble() > max)
    {
        throw std::runtime_error("[" + key.toStdString() + "] Поле должно иметь значение от [min] до [max].");
    }
}

inline void validateString(const QJsonObject& json, const QString& key)
{
    checkContains(json, key);

    if (!json[key].isString())
    {
        throw std::runtime_error("[" + key.toStdString() + "] Поле не является строкой.");
    }
}

inline void validateDigit(const QJsonValue& json, const QString& key, const double min = -1e12, const double max = 1e12)
{
    /* QJsonDocument doc(json.toObject());
    qDebug() << doc.toJson(QJsonDocument::Compact);*/
    // Проверка: является числом
    checkDouble(json, key);

    checkInterval(json, key, min, max);
}

inline void validateDigitValue(const QJsonObject& json, const QString& key, double min = -1e12, double max = 1e12)
{
    checkContains(json, key);

    validateDigit(json[key], key);
}

// Неименованная точка или пара
inline void validatePointOrPair(const QJsonValue& point, const QString& key, const double min, const double max)
{
    checkArray(point, key);

    checkSize(point.toArray(), key, 2);

    checkDouble(point[0], key);
    checkDouble(point[1], key);

    checkInterval(point[0], key, min, max);
    checkInterval(point[1], key, min, max);
}

// Отдельно для точки именованной точки или пары
inline void validatePointOrPair(const QJsonObject& json, const QString& key, const double min = -1e12, const double max = 1e12)
{
    checkContains(json, key);

    validatePointOrPair(json[key], key, min, max);
}

// Не именованный Массив
inline void validatePointsArray(const QJsonValue& value, const QString& key, const double min = -1e12, const double max = 1e12)
{
    checkArray(value, key);

    checkEmpty(value.toArray(), key);

    for (const auto& point : value.toArray())
    {
        validatePointOrPair(point, key, min, max);
    }
}

inline void validatePointsArray(const QJsonObject& object, const QString& key, const double min = -1e12, const double max = 1e12)
{
    checkContains(object, key);

    checkArray(object[key], key);

    checkEmpty(object[key].toArray(), key);

    for (const auto& point : object[key].toArray())
    {
        validatePointOrPair(point, key, min, max);
    }
}

inline void validateDoubleArray(const QJsonObject& object, const QString& key)
{
    checkContains(object, key);

    checkArray(object[key], key);

    checkEmpty(object[key].toArray(), key);

    for (const auto& value : object[key].toArray())
    {
        validateDigit(value, key); // Любое значение
    }
}

// key = borders
inline void validateRegion(const QJsonObject& json, const QString& key)
{
    checkContains(json, key);

    // Массив "borders"
    checkArray(json[key], key);

    auto borders = json[key].toArray();

    checkEmpty(borders, key);

    for (const auto& border : borders)
    {
        validatePointsArray(border, key);
    }
}
} // namespace Models
