#pragma once

#include "Input.hpp"
#include "Segment.hpp"
#include "ValidateOperations.hpp"

namespace Models {
struct Route : public Input
{
    size_t id{0};
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
        obj["id"] = static_cast<qint64>(id);

        QJsonArray segmentsArray;
        for (const Segment& segment : segments)
        {
            segmentsArray.append(segment.toJson());
        }
        obj["segments"] = segmentsArray;

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
} // namespace Models