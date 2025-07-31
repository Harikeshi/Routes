#ifndef TARGET_HPP
#define TARGET_HPP

#include "BaseDAO.hpp"
#include "Project/Models/Target.hpp"

namespace DataAccessObjects {
class TargetDAO final : public BaseDAO
{
public:
    using BaseDAO::BaseDAO;

    int update(const Models::Target& target) const
    {
        int id = -1;
        // exec_params Безопасный параметризированный запрос, exec - если запрос полностью статичный и нет подставляемых значений
        executeTransaction([&](pqxx::work& txn) {
            const auto result = txn.exec_params(
                "INSERT INTO targets (x, y, root_mean_square_error,left_course, right_course,current_velocity, max_velocity,min_noise_reduced, max_noise_reduced,obsolescence_time, avoidance_distance) VALUES ("
                "$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11) RETURNING id",
                target.detectionPoint.x,
                target.detectionPoint.y,
                target.rootMeanSquareError,
                target.courses.first,
                target.courses.second,
                target.currentVelocity,
                target.maxVelocity,
                target.minNoiseReduced,
                target.maxNoiseReduced,
                target.obsolescenceTime,
                target.avoidanceDistance);

            txn.commit();
            id = result[0][0].as<int>();
        });

        return id;
    }

    Models::Target get(int id) const
    {
        Models::Target target;
        executeTransaction([&](pqxx::work& txn) {
            if (const auto result = txn.exec_params("SELECT * FROM ship WHERE id = %1", id); !result.empty())
            {
                target.id = result[0]["id"].as<int>();

                target.detectionPoint.setX(result[0]["x"].as<double>());
                target.detectionPoint.setY(result[0]["y"].as<double>());
                target.rootMeanSquareError = result[0]["rootMeanSquareError"].as<double>();
                target.courses = std::make_pair(result[0]["left_course"].as<double>(), result[0]["right_course"].as<double>());
                target.currentVelocity = result[0]["current_velocity"].as<double>();
                target.maxVelocity = result[0]["max_velocity"].as<double>();
                target.minNoiseReduced = result[0]["min_noise_reduced"].as<double>();
                target.maxNoiseReduced = result[0]["max_noise_reduced"].as<double>();
                target.obsolescenceTime = result[0]["obsolescence_time"].as<double>();
                target.avoidanceDistance = result[0]["avoidance_distance"].as<double>();
            }
        });

        return target;
    }
};
} // namespace DataAccessObjects
#endif
