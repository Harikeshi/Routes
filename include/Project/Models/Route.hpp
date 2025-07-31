#pragma once

#include "Input.hpp"
#include "Segment.hpp"
#include "ValidateOperations.hpp"

namespace Models {
class Route : public Input
{
    size_t id{0};
    QVector<Segment> segments;

public:
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

    Route()
    {
        addValidator("points", [](const QJsonObject& json) {
        for (size_t i = 0; i != json["routes"].toArray().size(); ++i)
        {
            validatePointsArray(json["routes"].toArray()[i].toObject()["points"], "points");
            validateDoubleArray(json["routes"][i].toObject(), "velocities");
        } });
    }

    void initializeProperties(const QJsonObject& json) override
    {
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

            this->addSegment(Segment{begin, end, speed});
        }
    }
};
} // namespace Models