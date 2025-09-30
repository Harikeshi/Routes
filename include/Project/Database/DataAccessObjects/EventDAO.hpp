#ifndef EVENT_HPP
#define EVENT_HPP

#include "BaseDAO.hpp"

// #include <nlohmann/json.hpp>
#include <pqxx/transaction.hxx>

namespace DataAccessObjects {
class EventDAO final : public BaseDAO
{
public:
    using BaseDAO::BaseDAO;

    // TODO: Заменить на модель для возврата по id из базы.
    struct Event
    {
        int id;
        std::string date;
        std::string username;
        std::string message;
        std::string json_content;
        int request_id;
    };

    int update(const std::string_view& username,
               const std::string_view& message,
               const std::string_view& json_content,
               int request_id) const
    {
        int id = -1;
        executeTransaction([&](pqxx::work& txn) {
            const auto result = txn.exec_params(
                "INSERT INTO events (username, message, json_content, id_request) "
                "VALUES ($1, $2, $3, $4) RETURNING id",
                username,
                message,
                json_content,
                request_id);

            txn.commit();

            id = result[0][0].as<int>();
        });
        return id;
    }
};
} // namespace DataAccessObjects
#endif
