#pragma once

#include <QDebug>
#include <QPointF>
#include <QPolygonF>
#include <QVector>

#include "Input.hpp"
#include "ValidateOperations.hpp"

#include "Project/Operations/JsonOperations.hpp"

#include <algorithm>

namespace Models {
struct Perimeter : public Input
{
    size_t id{0};
    // TODO: надо ли

    QVector<QPolygonF> rings;

    QPointF entrance;
    QPointF exit;

    double minX{1e13}, minY{1e13}, maxX{-1e13}, maxY{-1e13};

    Perimeter()
    {
        addValidator("entry_point", [](const QJsonObject& json) { validatePointOrPair(json, "entry_point"); });
        addValidator("exit_point", [](const QJsonObject& json) { validatePointOrPair(json, "exit_point"); });

        addValidator("borders", [](const QJsonObject& json) { validateRegion(json, "borders"); });
    }

    Perimeter operator=(const Perimeter& value)
    {
        entrance = value.entrance;
        exit = value.exit;

        rings.clear();

        for (const auto& ring : value.rings)
        {
            QPolygonF r;
            for (const auto& point : ring)
            {
                r.append(point);
            }
            rings.append(r);
        }
        minX = value.minX, minY = value.minY, maxX = value.maxX, maxY = value.maxY;
        return *this;
    }

    size_t getId() const override
    {
        return id;
    }

    // "search_region"
    void initializeProperties(const QJsonObject& json) override
    {
        if (json.contains("id"))
            id = json["id"].toInt();

        Operations::setQPointF(entrance, json["entry_point"]);
        Operations::setQPointF(exit, json["exit_point"]);

        rings.clear();

        for (const auto& border : json["borders"].toArray())
        {
            QVector<QPointF> inner;
            // border [[],[]]
            for (const auto& point : border.toArray())
            {
                inner.push_back(Operations::toQPointF(point));
            }

            this->addInner(inner);
        }

        setLimits();
    }

    QJsonObject toJson() const override
    {
        QJsonObject obj;

        // TODO: Используются x и y для работы с базой данных.
        // entrance
        QJsonArray entranceArr;

        entranceArr.append(entrance.x());
        entranceArr.append(entrance.y());
        obj["entry_point"] = entranceArr;

        // exit
        QJsonArray exitArr;
        exitArr.append(exit.x());
        exitArr.append(exit.y());
        obj["exit_point"] = exitArr;

        // rings
        QJsonArray ringsArray;
        for (const QPolygonF& ring : rings)
        {
            QJsonArray ringArray;
            for (const QPointF& point : ring)
            {
                QJsonArray pointArr;
                pointArr.append(point.x());
                pointArr.append(point.y());

                ringArray.append(pointArr);
            }
            ringsArray.append(ringArray);
        }
        obj["borders"] = ringsArray;

        return obj;
    }

    nlohmann::json toNJson() const override
    {
        nlohmann::json obj;

        // TODO: Используются x и y для работы с базой данных.
        // entrance
        auto entranceArr = nlohmann::json::array();

        entranceArr.emplace_back(entrance.x());
        entranceArr.emplace_back(entrance.y());
        obj["entry_point"] = entranceArr;

        // exit
        auto exitArr = nlohmann::json::array();
        exitArr.emplace_back(exit.x());
        exitArr.emplace_back(exit.y());
        obj["exit_point"] = exitArr;

        // rings
        auto ringsArray = nlohmann::json::array();
        for (const QPolygonF& ring : rings)
        {
            auto ringArray = nlohmann::json::array();
            for (const QPointF& point : ring)
            {
                auto pointArr = nlohmann::json::array();
                pointArr.emplace_back(point.x());
                pointArr.emplace_back(point.y());

                ringArray.emplace_back(pointArr);
            }
            ringsArray.emplace_back(ringArray);
        }

        obj["borders"] = ringsArray;

        return obj;
    }

    void clear()
    {
        for (auto& inner : rings)
        {
            inner.clear();
        }
    }

    QVector<QPolygonF> getRings() const
    {
        return rings;
    }

    double getMinX() const
    {
        return minX;
    }

    double getMinY() const
    {
        return minY;
    }

    double getMaxX() const
    {
        return maxX;
    }

    double getMaxY() const
    {
        return maxY;
    }

    void setFrom(const Perimeter& perimeter)
    {
        rings = perimeter.rings;

        entrance = perimeter.entrance;
        exit = perimeter.exit;

        setLimits();
    }

    void swapCoordinates()
    {
        for (auto& ring : rings)
        {
            for (auto& point : ring)
            {
                auto x = point.x();
                point.setX(point.y());
                point.setY(x);
            }
        }

        std::swap(minX, minY);
        std::swap(maxX, maxY);
    }

    void show() const
    {
        for (const auto& ring : rings)
        {
            qDebug() << "[";
            for (const auto& point : ring)
            {
                qDebug() << "[" << point << "]";
            }
            qDebug() << "]";
        }

        qDebug() << "minX: " << minX << ", minY: " << minY << ", maxX: " << maxX << ", maxY: " << maxY;
    }

private:
    void setOuter(const QVector<QPointF>& points)
    {
        if (rings.empty())
            rings[0] = points;
        else
        {
            rings.push_back(points);
        }

        setLimits();
    }

    void setRings(const QVector<QPolygonF>& polygons)
    {
        // TODO: clear?
        for (const auto& polygon : polygons)
        {
            rings.push_back(polygon);
        }

        setLimits();
    }

    void addInner(const QVector<QPointF>& points)
    {
        rings.push_back(points);
    }

    void resetLimits()
    {
        minX = 1e13, minY = 1e13, maxX = -1e13, maxY = -1e13;
    }

    void setLimitsFromPoint(const QPointF& point)
    {
        minX = qMin(minX, point.x());
        minY = qMin(minY, point.y());
        maxX = qMax(maxX, point.x());
        maxY = qMax(maxY, point.y());
    }

    void setLimits()
    {
        resetLimits();

        for (const auto& ring : rings)
        {
            for (const auto& point : ring)
                setLimitsFromPoint(point);
        }

        setLimitsFromPoint(entrance);
        setLimitsFromPoint(exit);
    }

    void setLimits(const QVector<QPointF>& points)
    {
        for (const auto& point : points)
        {
            minX = qMin(minX, point.x());
            minY = qMin(minY, point.y());
            maxX = qMax(maxX, point.x());
            maxY = qMax(maxY, point.y());
        }
    }
};
} // namespace Models
