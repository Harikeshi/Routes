#pragma once

#include "../Input.hpp"
#include "../ValidateOperations.hpp"
#include "Segment.hpp"

namespace Models::origin {
struct Route : public Input
{
    QVector<Segment> segments;

    Route()
    {
        addValidator("points", [](const QJsonObject& json) {
            for (size_t i = 0; i != json["routes"].toArray().size(); ++i)
            {
                validatePointsArray(json["routes"].toArray()[i].toObject()["points"], "points");
                validateDoubleArray(json["routes"][i].toObject(), "velocities");
            } });
    }

    void show() const
    {
        qDebug() << "Id: " << id;

        for (auto const& segment : segments)
        {
            segment.show();
        }
    }

    size_t getId() const override
    {
        return id;
    }

    // TODO: Инициализация без XY
    void initializeProperties(const QJsonObject& json) override
    {
        if (json.contains("id"))
            id = json["id"].toInt();

        segments.clear();

        QJsonArray points = json["points"].toArray();

        QJsonArray velocities = json["velocities"].toArray();

        for (size_t i = 0; i != points.size() - 1; ++i)
        {
            QJsonArray start = points[i].toArray();
            QJsonArray finish = points[i + 1].toArray();

            double speed = velocities[i].toDouble();

            QPointF begin = QPointF{start[0].toDouble(), start[1].toDouble()};
            QPointF end = QPointF{finish[0].toDouble(), finish[1].toDouble()};

            this->addSegment(Segment(0, begin, end, speed));
        }
    }

    QJsonObject toJson() const override
    {
        QJsonObject obj;

        if (segments.isEmpty())
            return obj;

        // obj["id"] = static_cast<qint64>(id);

        QJsonArray pointsArray;

        QJsonArray first_point{segments.at(0).getStart().x(), segments.at(0).getStart().y()};
        pointsArray.append(first_point);

        QJsonArray velocitiesArray;

        for (const Segment& segment : segments)
        {
            velocitiesArray.append(segment.baseSpeed);

            pointsArray.append(QJsonArray{segment.getEnd().x(), segment.getEnd().y()});
        }

        obj["points"] = pointsArray;
        obj["velocities"] = velocitiesArray;

        return obj;
    }

    nlohmann::json toNJson() const override
    {
        nlohmann::json obj;

        if (segments.isEmpty())
            return obj;

        // obj["id"] = static_cast<qint64>(id);

        nlohmann::json pointsArray = nlohmann::json::array();

        auto first_point = nlohmann::json::array();
        first_point.emplace_back(segments.at(0).getStart().x());
        first_point.emplace_back(segments.at(0).getStart().y());

        pointsArray.emplace_back(first_point);

        nlohmann::json velocitiesArray = nlohmann::json::array();

        for (const Segment& segment : segments)
        {
            velocitiesArray.emplace_back(segment.baseSpeed);

            auto first_point = nlohmann::json::array();
            first_point.emplace_back(segment.getEnd().x());
            first_point.emplace_back(segment.getEnd().y());

            pointsArray.emplace_back(nlohmann::json{segment.getEnd().x(), segment.getEnd().y()});
        }

        obj["points"] = pointsArray;
        obj["velocities"] = velocitiesArray;

        return obj;
    }

    void addSegment(const Segment& segment)
    {
        segments.push_back(segment);
    }

    QVector<Segment> getSegments() const
    {
        return segments;
    }

    void setSegments(const QVector<Segment>& segments)
    {
        this->segments = segments;
    }

    void swapCoordinates()
    {
        for (auto& segment : segments)
        {
            segment.swapCoordinates();
        }
    }
};
} // namespace Models::origin