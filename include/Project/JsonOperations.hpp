#pragma once

#include "Visual/Models/Object.hpp"
#include "Visual/Models/Perimeter.hpp"
#include "Visual/Models/Route.hpp"
#include "Visual/Models/Target.hpp"

#include <QFile>

class JsonHelpers
{
    using Segment = Visual::Models::Segment;
    using Route = Visual::Models::Route;
    using Object = Visual::Models::Object;
    using Perimeter = Visual::Models::Perimeter;

public:
    static QJsonObject jsonFromFile(const QString& path)
    {
        QByteArray barray;

        if (!path.isEmpty())
        {
            QFile file(path);

            // Проверка
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                throw std::runtime_error("Не удалось открыть файл");
            }

            barray = file.readAll();

            file.close();
        }
        QJsonDocument doc = QJsonDocument::fromJson(barray);

        return doc.object();
    }

    QByteArray getByteArray(const QString& fileName)
    {
        QFile file(fileName);

        // Проверка
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            file.close();
            throw std::runtime_error("Не удалось открыть файл.");
        }

        QByteArray barray;
        barray = file.readAll();

        file.close();

        return barray;
    }

    // TODO: to JsonOper
    QJsonObject byteArrayToJson(const QByteArray& array)
    {
        QJsonDocument doc = QJsonDocument::fromJson(array);

        if (doc.isNull() || !doc.isObject())
        {
            throw std::runtime_error("Файл не удалось открыть.");
        }

        QJsonObject obj;
        try
        {
            obj = doc.object();
        }
        catch (std::runtime_error& ex)
        {
            throw ex;
        }

        return obj;
    }

    static QVector<QVector<Segment>> parseToSegments(const QJsonObject& obj)
    {
        // TODO: !Проверку сделать отдельно

        QVector<QVector<Segment>> result;

        if (!obj.contains("routes"))
        {
            throw std::runtime_error("Объект не содержит \"routes\".");
        }

        if (!obj["routes"].isArray())
        {
            throw std::runtime_error("\"routes\" не является массивом.");
        }

        auto routes = obj["routes"].toArray();

        for (const QJsonValue& jroute : routes)
        {
            auto route = jroute.toObject();

            if (!route.contains("points"))
            {
                throw std::runtime_error("Объект не содержит \"points\".");
            }

            if (!route["points"].isArray())
            {
                throw std::runtime_error("\"points\" не является массивом.");
            }
            if (!route.contains("velocities"))
            {
                throw std::runtime_error("Объект не содержит \"velocities\".");
            }

            if (!route["velocities"].isArray())
            {
                throw std::runtime_error("\"velocities\" не является массивом.");
            }

            // TODO: Проверка, что в точки содержатся числа
            QJsonArray points = route["points"].toArray();
            QJsonArray velocities = route["velocities"].toArray();

            //
            if (points.size() != velocities.size())
            {
                throw std::runtime_error("Длина \"points\" не совпадает с длиной \"velocities\".");
            }

            QVector<Segment> path;

            for (size_t i = 0; i != points.size() - 1; ++i)
            {
                if (!points[i].isArray() || !points[i + 1].isArray())
                {
                    throw std::runtime_error("Данные для точки не являются массивом.");
                }

                QJsonArray start = points[i].toArray();
                QJsonArray end = points[i + 1].toArray();

                if (!velocities[i].isDouble())
                {
                    throw std::runtime_error("Данные скорости не являются числом.");
                }
                double speed = velocities[i].toDouble();

                if (!(start.size() == 2) || !(end.size() == 2))
                {
                    throw std::runtime_error("Длина массива точки != 2.");
                }

                double x1 = start[0].toDouble();
                double x2 = end[0].toDouble();
                double y1 = start[1].toDouble();
                double y2 = end[1].toDouble();

                if (x1 >= -1e12 && x1 <= 1e12 && x2 >= -1e12 && x2 <= 1e12 && y1 >= -1e12 && y1 <= 1e12 && y2 >= -1e12 && y2 <= 1e12)
                {
                    Segment segment(QPointF(x1, y1), QPointF(x2, y2), speed);
                    path.push_back(segment);
                }
                else
                {
                    throw std::runtime_error("Точка пути не в интервале от -1e12 до +1e12.");
                }
            }

            result.push_back(path);
        }

        return result;
    }

    static QVector<Route> parseToRoutes(const QJsonObject& obj)
    {
        // TODO: !Проверку сделать отдельно

        QVector<Route> result;

        if (!obj.contains("routes"))
        {
            throw std::runtime_error("Объект не содержит \"routes\".");
        }

        if (!obj["routes"].isArray())
        {
            throw std::runtime_error("\"routes\" не является массивом.");
        }

        auto routes = obj["routes"].toArray();

        for (const QJsonValue& jroute : routes)
        {
            auto route = jroute.toObject();

            if (!route.contains("points"))
            {
                throw std::runtime_error("Объект не содержит \"points\".");
            }

            if (!route["points"].isArray())
            {
                throw std::runtime_error("\"points\" не является массивом.");
            }
            if (!route.contains("velocities"))
            {
                throw std::runtime_error("Объект не содержит \"velocities\".");
            }

            if (!route["velocities"].isArray())
            {
                throw std::runtime_error("\"velocities\" не является массивом.");
            }

            // TODO: Проверка, что в точки содержатся числа
            QJsonArray points = route["points"].toArray();
            QJsonArray velocities = route["velocities"].toArray();

            // if (points.size() != velocities.size())
            // {
            //     throw std::runtime_error("Длина \"points\" не совпадает с длиной \"velocities\".");
            // }

            Route path;

            for (size_t i = 0, j = 0; i != points.size() - 1; ++i)
            {
                if (!points[i].isArray() || !points[i + 1].isArray())
                {
                    throw std::runtime_error("Данные для точки не являются массивом.");
                }

                QJsonArray start = points[i].toArray();
                QJsonArray end = points[i + 1].toArray();

                if (!velocities[i].isDouble())
                {
                    throw std::runtime_error("Данные скорости не являются числом.");
                }
                double speed = velocities[i].toDouble();

                if (!(start.size() == 2) || !(end.size() == 2))
                {
                    throw std::runtime_error("Длина массива точки != 2.");
                }

                double x1 = start[0].toDouble();
                double x2 = end[0].toDouble();
                double y1 = start[1].toDouble();
                double y2 = end[1].toDouble();

                if (x1 >= -1e12 && x1 <= 1e12 && x2 >= -1e12 && x2 <= 1e12 && y1 >= -1e12 && y1 <= 1e12 && y2 >= -1e12 && y2 <= 1e12)
                {
                    Segment segment(QPointF(x1, y1), QPointF(x2, y2), speed);

                    path.addSegment(segment);
                }
                else
                {
                    throw std::runtime_error("Точка пути не в интервале от -1e12 до +1e12.");
                }
                // Для первого сегмента
                if (i == 0)
                {
                    continue;
                }

                ++j;
            }

            result.push_back(path);
        }

        return result;
    }

    static Object parseShip(const QJsonObject& obj)
    {
        // TODO: !Проверку сделать отдельно

        Object ship;

        if (!obj.contains("ships_parameters"))
        {
            throw std::runtime_error("Объект не содержит \"ships_parameters\".");
        }

        auto ships_parameters = obj["ships_parameters"].toObject();

        if (!ships_parameters.contains("detection_range"))
        {
            throw std::runtime_error("Объект не содержит \"detection_range\".");
        }

        if (!ships_parameters["detection_range"].isDouble())
        {
            throw std::runtime_error("\"detection_range\" не является числом.");
        }

        ship.setDetectionRange(ships_parameters["detection_range"].toDouble());

        return ship;
    }

    static Object parseTarget(const QJsonObject& obj)
    {
        // TODO: !Проверку сделать отдельно

        Object target;

        if (!obj.contains("form_target"))
        {
            throw std::runtime_error("Объект не содержит \"form_target\".");
        }

        auto form_target = obj["form_target"].toObject();

        if (!form_target.contains("velocities"))
        {
            throw std::runtime_error("Объект не содержит \"velocities\".");
        }

        auto velocities = form_target["velocities"].toObject();

        if (!velocities.contains("current"))
        {
            throw std::runtime_error("Объект не содержит \"current\".");
        }

        if (!velocities["current"].isDouble())
        {
            throw std::runtime_error("\"current\" не является числом.");
        }

        target.setCurrentVelocity(velocities["current"].toDouble());

        return target;
    }

    static Perimeter parseToPolygon(const QJsonObject& json)
    {
        Perimeter result;

        if (!json.contains("search_region"))
        {
            throw std::runtime_error("Объект не содержит \"search_region\".");
        }

        auto searchRegion = json["search_region"].toObject();

        if (!searchRegion.contains("borders"))
        {
            throw std::runtime_error("Объект \"search_region\" не содержит \"borders\".");
        }

        if (!searchRegion["borders"].isArray())
        {
            throw std::runtime_error("\"points\" не является массивом.");
        }

        auto borders = searchRegion["borders"].toArray();

        for (const QJsonValue& qborder : borders)
        {
            if (!qborder.isArray())
            {
                throw std::runtime_error("Объект внутри \"borders\" не является массивом.");
            }

            auto border = qborder.toArray();

            QVector<QPointF> inner;
            // border [[],[]]
            for (const QJsonValue& qpoint : border)
            {
                QJsonArray point = qpoint.toArray();

                if (!(point.size() == 2))
                {
                    throw std::runtime_error("Длина массива точки != 2.");
                }

                if (!point[0].isDouble() || !point[1].isDouble())
                {
                    throw std::runtime_error("Данные в массиве точки не являются числом.");
                }

                double x = point[0].toDouble();
                double y = point[1].toDouble();

                if (x >= -1e12 && x <= 1e12 && y >= -1e12 && y <= 1e12)
                {
                    inner.push_back(QPointF{x, y});
                }
                else
                {
                    throw std::runtime_error("Точка Периметра не в интервале от -1e12 до +1e12.");
                }
            }

            result.addInner(inner);
        }

        return result;
    }
};
