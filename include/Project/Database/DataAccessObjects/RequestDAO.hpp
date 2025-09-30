#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "BaseDAO.hpp"

namespace DataAccessObjects {
class RequestDAO final : public BaseDAO
{
public:
    using BaseDAO::BaseDAO;

    // TODO: Заменить на модель.
    struct Request
    {
        int id;
        long time;
        std::string target;
        std::string search_region;
        std::string border_line;
        int ship_id;
    };

    // TODO: Возможно после создания в Initializer
    int update(long time, const std::string& target, const std::string& search_region, const std::string& border_line, int ship_id) const
    {
        int id = -1;
        executeTransaction([&](pqxx::work& txn) {
            const auto result = txn.exec_params(
                "INSERT INTO requests (time, target, search_region, border_line, id_ship) "
                "VALUES ($1, $2, $3, $4, $5) RETURNING id",
                time,
                target,
                search_region,
                border_line,
                ship_id);

            txn.commit();

            id = result[0][0].as<int>();
        });
        return id;
    }
};
} // namespace DataAccessObjects
#endif
