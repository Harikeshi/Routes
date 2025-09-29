#pragma once

#include <QFile>
#include <QJsonDocument>
#include <QPointF>
#include <QVector>

#include <nlohmann/detail/value_t.hpp>
#include <nlohmann/json.hpp>

#include <iostream>
#include <stdexcept>

namespace Operations {
    inline QJsonObject jsonFromFile(const QString &path) {
        QByteArray array;

        if (!path.isEmpty()) {
            QFile file(path);

            //
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                throw std::runtime_error("Не возможно открыть файл: " + path.toStdString());
            }

            array = file.readAll();

            file.close();
        }
        QJsonDocument doc = QJsonDocument::fromJson(array);

        return doc.object();
    }

    inline void setQPointF(QPointF &point, const QJsonValue &json) {
        point.setX(json[0].toDouble());
        point.setY(json[1].toDouble());
    }

    inline QPointF toQPointF(const QJsonValue &json) {
        return QPointF(json[0].toDouble(), json[1].toDouble());
    }

    inline void setDoublePair(std::pair<double, double> &pair, const QJsonValue &json) {
        pair.first = json[0].toDouble();
        pair.second = json[1].toDouble();
    }

    inline void setQVectorQPointF(QVector<QPointF> &result, const QJsonValue &json) {
        result.clear();

        for (const auto &point: json.toArray()) {
            result.push_back(toQPointF(point));
        }
    }

    inline QVector<QPointF> toQVectorQPointF(const QJsonValue &json) {
        QVector<QPointF> result;

        for (const auto &point: json.toArray()) {
            result.push_back(toQPointF(point));
        }

        return result;
    }

    inline nlohmann::json convertToNlohmann(const QJsonObject &qobj);

    inline nlohmann::json convertArray(const QJsonArray &qarray) {
        nlohmann::json jarray = nlohmann::json::array();

        for (const auto &v: qarray) {
            switch (v.type()) {
                case QJsonValue::Bool:
                    jarray.push_back(v.toBool());
                    break;
                case QJsonValue::Double:
                    jarray.push_back(v.toDouble());
                    break;
                case QJsonValue::String:
                    jarray.push_back(v.toString().toStdString());
                    break;
                case QJsonValue::Array:
                    jarray.push_back(convertArray(v.toArray()));
                    break;
                case QJsonValue::Object:
                    jarray.push_back(convertToNlohmann(v.toObject()));
                    break;
                case QJsonValue::Null:
                    jarray.push_back(nullptr);
                    break;
                default:
                    break;
            }
        }
        return jarray;
    }

    inline nlohmann::json convertToNlohmann(const QJsonObject &qobj) {
        nlohmann::json j;
        for (auto it = qobj.begin(); it != qobj.end(); ++it) {
            QString key = it.key();
            QJsonValue value = it.value();

            switch (value.type()) {
                case QJsonValue::Bool:
                    j[key.toStdString()] = value.toBool();
                    break;
                case QJsonValue::Double:
                    j[key.toStdString()] = value.toDouble();
                    break;
                case QJsonValue::String:
                    j[key.toStdString()] = value.toString().toStdString();
                    break;
                case QJsonValue::Array: {
                    QJsonArray array = value.toArray();
                    auto jarray = convertArray(array);

                    j[key.toStdString()] = jarray;

                    break;
                }
                case QJsonValue::Object:
                    j[key.toStdString()] = convertToNlohmann(value.toObject());
                    break;
                case QJsonValue::Null:
                    j[key.toStdString()] = nullptr;
                    break;
                default:
                    break;
            }
        }
        return j;
    }

    QJsonValue jsonToQJsonValue(const nlohmann::json &j);

    inline QJsonObject convertToQJsonObject(const nlohmann::json &j) {
        QJsonObject qobj;
        for (auto it = j.begin(); it != j.end(); ++it) {
            qobj[QString::fromStdString(it.key())] = jsonToQJsonValue(it.value());
        }
        return qobj;
    }

    inline QJsonArray convertToQJsonArray(const nlohmann::json &jarray) {
        QJsonArray qarr;
        for (const auto &item: jarray) {
            qarr.append(jsonToQJsonValue(item));
        }
        return qarr;
    }

    inline QJsonValue jsonToQJsonValue(const nlohmann::json &j) {
        if (j.is_boolean()) {
            return QJsonValue(j.get<bool>());
        } else if (j.is_number_integer()) {
            return QJsonValue(j.get<int>());
        } else if (j.is_number_float()) {
            return QJsonValue(j.get<double>());
        } else if (j.is_string()) {
            return QJsonValue(QString::fromStdString(j.get<std::string>()));
        } else if (j.is_array()) {
            return convertToQJsonArray(j);
        } else if (j.is_object()) {
            return convertToQJsonObject(j);
        } else {
            return QJsonValue(); // null
        }
    }

    inline void printJson(const QJsonObject &obj) {
        QJsonDocument doc(obj);
        QString jsonString = doc.toJson(QJsonDocument::Indented); // Compact для однострочного
        qDebug() << jsonString;
    }

    inline void printJson(const nlohmann::json &obj) {
        std::cout << obj.dump(4);
    }

    inline QString getHomePath() {
        QString result;
#ifdef _WIN32
        result = QString(std::getenv("USERPROFILE"));
#else
    result = QString(getenv("HOME"));
#endif

        return result;
    }
} // namespace Operations
