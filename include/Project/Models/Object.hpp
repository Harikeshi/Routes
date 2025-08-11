#pragma once

#include "Input.hpp"
#include "ValidateOperations.hpp"

namespace Models {
// Ship
struct Object : public Input
{
    size_t id;
    double detectionRange;
    double maxVelocity;
    double currentVelocity;

    double detection_range;
    double max_velocity;
    double search_velocity;
    double turning_radius;
    double min_lenght_section;

public:
    // TODO: Значения излишни.
    Object(
        size_t _id = 0,
        double _detectionRange = 99,
        double _maxVelocity = 99.99,
        double _currentVelocity = 99.99,
        double _detection_range = 3,
        double _max_velocity = 18,
        double _search_velocity = 12,
        double _turning_radius = 3,
        double _min_lenght_section = 2) : id{_id}, detectionRange{_detectionRange}, maxVelocity{_maxVelocity}, currentVelocity{_currentVelocity}, detection_range{_detection_range}, max_velocity{_max_velocity}, search_velocity{_search_velocity}, turning_radius{_turning_radius},
                                          min_lenght_section{_min_lenght_section}
    {
        addValidator("detection_range", [](const QJsonObject& json) { validateDigit(json["detection_range"], "detection_range"); });
        addValidator("max_velocity", [](const QJsonObject& json) { validateDigit(json["max_velocity"], "max_velocity"); });
        addValidator("search_velocity", [](const QJsonObject& json) { validateDigit(json["search_velocity"], "search_velocity"); });
        addValidator("turning_radius", [](const QJsonObject& json) { validateDigit(json["turning_radius"], "turning_radius"); });
        addValidator("min_lenght_section", [](const QJsonObject& json) { validateDigit(json["min_lenght_section"], "min_lenght_section"); });
    }

    void reset()
    {
        detectionRange = 0;
        maxVelocity = 0;
        currentVelocity = 0;

        detection_range = 0;
        max_velocity = 0;
        search_velocity = 0;
        turning_radius = 0;
        min_lenght_section = 0;
    }

    //    Object()
    //    {
    //        addValidator("detection_range", [](const QJsonObject& json) { validateDigit(json["detection_range"], "detection_range"); });
    //        addValidator("max_velocity", [](const QJsonObject& json) { validateDigit(json["max_velocity"], "max_velocity"); });
    //        addValidator("search_velocity", [](const QJsonObject& json) { validateDigit(json["search_velocity"], "search_velocity"); });
    //        addValidator("turning_radius", [](const QJsonObject& json) { validateDigit(json["turning_radius"], "turning_radius"); });
    //        addValidator("min_lenght_section", [](const QJsonObject& json) { validateDigit(json["min_lenght_section"], "min_lenght_section"); });
    //    }

    size_t getId() const override
    {
        return id;
    }

    void initializeProperties(const QJsonObject& json) override
    {
        if (json.contains("id"))
            id = json["id"].toInt();

        detection_range = json["detection_range"].toDouble();
        detectionRange = detection_range;
        max_velocity = json["max_velocity"].toDouble();
        maxVelocity = max_velocity;
        search_velocity = json["search_velocity"].toDouble();
        currentVelocity = search_velocity;
        turning_radius = json["turning_radius"].toDouble();
        min_lenght_section = json["min_lenght_section"].toDouble();
    }

    QJsonObject toJson() const override
    {
        QJsonObject obj;

        // obj["id"] = static_cast<qint64>(id);
        obj["count"] = 1;
        obj["detection_range"] = detection_range;
        obj["max_velocity"] = max_velocity;
        obj["search_velocity"] = search_velocity;
        obj["turning_radius"] = turning_radius;
        obj["min_lenght_section"] = min_lenght_section;

        return obj;
    }

    nlohmann::json toNJson() const override
    {
        nlohmann::json obj;

        // obj["id"] = static_cast<qint64>(id);
        obj["count"] = 1;
        obj["detection_range"] = detection_range;
        obj["max_velocity"] = max_velocity;
        obj["search_velocity"] = search_velocity;
        obj["turning_radius"] = turning_radius;
        obj["min_lenght_section"] = min_lenght_section;

        return obj;
    }
};
} // namespace Models
