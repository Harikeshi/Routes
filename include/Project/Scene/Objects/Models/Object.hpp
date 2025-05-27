#pragma once

#include <QPainter>
#include <QPointF>

#include "./Input.hpp"
#include "./ValidateOperations.hpp"

namespace Scene {
namespace Objects {
namespace Models {
// Ship
struct Object : public Input
{
    double detectionRange{99};
    double maxVelocity{99.99};
    double currentVelocity{99.99};

    double detection_range = 3;
    double max_velocity = 18;
    double search_velocity = 12;
    double turning_radius = 3;
    double min_lenght_section = 2;

public:
    Object()
    {
        addValidator("detection_range", [](const QJsonObject& json) { validateDigit(json["detection_range"], "detection_range"); });
        addValidator("max_velocity", [](const QJsonObject& json) { validateDigit(json["max_velocity"], "max_velocity"); });
        addValidator("search_velocity", [](const QJsonObject& json) { validateDigit(json["search_velocity"], "search_velocity"); });
        addValidator("turning_radius", [](const QJsonObject& json) { validateDigit(json["turning_radius"], "turning_radius"); });
        addValidator("min_lenght_section", [](const QJsonObject& json) { validateDigit(json["min_lenght_section"], "min_lenght_section"); });
    }

    void initializeProperties(const QJsonObject& json)
    {
        // QJsonDocument doc(json);
        // qDebug() << doc.toJson(QJsonDocument::Compact);
        detection_range = json["detection_range"].toDouble();
        detectionRange = detection_range;
        max_velocity = json["max_velocity"].toDouble();
        maxVelocity = max_velocity;
        search_velocity = json["search_velocity"].toDouble();
        currentVelocity = search_velocity;
        turning_radius = json["turning_radius"].toDouble();
        min_lenght_section = json["min_lenght_section"].toDouble();
    }
};
} // namespace Models
} // namespace Objects
} // namespace Scene