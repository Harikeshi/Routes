#ifndef SHIP_HPP
#define SHIP_HPP

#include "BaseDAO.hpp"
#include "Project/Models/Object.hpp"

namespace DataAccessObjects {
// final Запрет наследования от класса
class ShipDAO final : BaseDAO
{
public:
    using BaseDAO::BaseDAO;

    // TODO: Заменить на модели.
    struct Ship
    {
        int id;
        int count;
        int detection;
    };

    int update(int count, int detection) const
    {
        int id = -1;
        // exec_params Безопасный параметризированный запрос, exec - если запрос полностью статичный и нет подставляемых значений
        executeTransaction([&](pqxx::work& txn) {
            const auto result = txn.exec_params(
                "INSERT INTO ship (count, detection) VALUES ($1, $2) RETURNING id",
                count,
                detection);

            txn.commit();
            id = result[0][0].as<int>();
        });

        return id;
    }

    Ship get(int id) const
    {
        Ship ship;
        executeTransaction([&](pqxx::work& txn) {
            if (const auto result = txn.exec_params("SELECT * FROM ship WHERE id = %1", id); !result.empty())
            {
                ship.id = result[0]["id"].as<int>();
                ship.count = result[0]["count"].as<int>();
                ship.detection = result[0]["detection"].as<int>();
            }
        });

        return ship;
    }
};
} // namespace DataAccessObjects
#endif
