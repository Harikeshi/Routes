#pragma once

#include "../../../Operations/JsonOperations.hpp"
#include "./Input.hpp"
#include "./ValidateOperations.hpp"

namespace Scene {
namespace Objects {
namespace Models {
struct Target : public Input
{
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

    void initializeProperties(const QJsonObject& json)
    {
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
} // namespace Objects
} // namespace Scene