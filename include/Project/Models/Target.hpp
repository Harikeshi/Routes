#pragma once

#include "Input.hpp"
#include "Project/Operations/JsonOperations.hpp"
#include "ValidateOperations.hpp"

namespace Models {
struct Target : public Input
{
    size_t id{0};
    QPointF detectionPoint{0, 0};
    std::pair<double, double> courses{360, 360};

    double rootMeanSquareError = 99999;
    // Velocities
    double currentVelocity = 99.99;
    double maxVelocity = 99.99;
    double minNoiseReduced = 99.99;
    double maxNoiseReduced = 99.99;

    double obsolescenceTime = 999999;
    double avoidanceDistance = 999999;

public:
    Target(
        size_t _id,
        QPointF _detectionPoint,
        std::pair<double, double> _courses,
        double _rootMeanSquareError,
        double _currentVelocity,
        double _maxVelocity,
        double _minNoiseReduced,
        double _maxNoiseReduced,
        double _obsolescenceTime,
        double _avoidanceDistance) : id(_id), detectionPoint(_detectionPoint), courses(_courses), rootMeanSquareError(_rootMeanSquareError), currentVelocity(_currentVelocity),
                                     maxVelocity(_maxVelocity), minNoiseReduced(_minNoiseReduced), maxNoiseReduced(_maxNoiseReduced), obsolescenceTime(_obsolescenceTime),
                                     avoidanceDistance(_avoidanceDistance)
    {
    }
    Target()
    {
        addValidator("detection_point", [](const QJsonObject& json) { validatePointOrPair(json, "detection_point"); });

        addValidator("courses", [](const QJsonObject& json) { validatePointOrPair(json, "courses"); });

        addValidator("root-mean-square_error", [](const QJsonObject& j) {
            validateDigit(j["root-mean-square_error"], "root-mean-square_error", 0., 99999);
        });

        // velocities
        addValidator("current", [](const QJsonObject& j) {
            validateDigit(j["velocities"]["current"], "current", 0, 99.99);
        });
        addValidator("max", [](const QJsonObject& j) {
            validateDigit(j["velocities"]["max"], "max", 0, 99.99);
        });
        addValidator("min_noise-reduced", [](const QJsonObject& j) {
            validateDigit(j["velocities"]["min_noise-reduced"], "min_noise-reduced", 0, 99.99);
        });
        addValidator("max_noise-reduced", [](const QJsonObject& j) {
            validateDigit(j["velocities"]["max_noise-reduced"], "max_noise-reduced", 0, 99.99);
        });

        // other
        addValidator("obsolescence_time", [](const QJsonObject& j) {
            validateDigit(j["obsolescence_time"], "obsolescence_time", 0, 999999);
        });
        addValidator("avoidance_distance", [](const QJsonObject& j) {
            validateDigit(j["avoidance_distance"], "avoidance_distance", 0, 999999);
        });
    }

    size_t getId() const override
    {
        return id;
    }

    void initializeProperties(const QJsonObject& json) override
    {
        if (json.contains("id"))
            id = json["id"].toInt();

        Operations::setQPointF(detectionPoint, json["detection_point"]);

        Operations::setDoublePair(courses, json["courses"]);

        rootMeanSquareError = json["root-mean-square_error"].toDouble();
        currentVelocity = json["velocities"]["current"].toDouble();
        maxVelocity = json["velocities"]["max"].toDouble();
        minNoiseReduced = json["velocities"]["min_noise-reduced"].toDouble();
        maxNoiseReduced = json["velocities"]["max_noise-reduced"].toDouble();

        obsolescenceTime = json["obsolescence_time"].toDouble();
        avoidanceDistance = json["avoidance_distance"].toDouble();
    }

    QJsonObject toJson() const override
    {
        QJsonObject obj;
        //        obj["id"] = static_cast<qint64>(id);

        // detection point
        QJsonArray detectionPointArr{detectionPoint.x(), detectionPoint.y()};

        obj["detectionPoint"] = detectionPointArr;

        // courses
        QJsonArray coursesArray;
        coursesArray.append(courses.first);
        coursesArray.append(courses.second);
        obj["courses"] = coursesArray;

        obj["rootMeanSquareError"] = rootMeanSquareError;
        obj["obsolescenceTime"] = obsolescenceTime;
        obj["avoidanceDistance"] = avoidanceDistance;

        QJsonObject velocitiesObj;

        velocitiesObj["currentVelocity"] = currentVelocity;
        velocitiesObj["maxVelocity"] = maxVelocity;
        velocitiesObj["minNoiseReduced"] = minNoiseReduced;
        velocitiesObj["maxNoiseReduced"] = maxNoiseReduced;

        obj["velocities"] = velocitiesObj;

        return obj;
    }

    double getMinNoiseReduced() const
    {
        return minNoiseReduced;
    }

    void setCurrentVelocity(const double velocity)
    {
        currentVelocity = velocity;
    }

    double getCurrentVelocity() const
    {
        return currentVelocity;
    }
};
} // namespace Models
