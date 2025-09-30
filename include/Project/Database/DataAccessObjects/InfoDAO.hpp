#ifndef INFO_HPP
#define INFO_HPP

#include "BaseDAO.hpp"

// #include <nlohmann/json.hpp>
#include <pqxx/transaction.hxx>

#include "../../GetCurrentUsername.hpp"

namespace DataAccessObjects {
class InfoDAO final : public BaseDAO
{
public:
    using BaseDAO::BaseDAO;

    // TODO: Заменить на модель для возврата по id из базы.
    struct Info
    {
        int id;
        std::string date;
        std::string username;
        std::string message;
        std::string severity;
    };

    void update(const std::string_view& message,
                const std::string_view severity = "ERROR") const
    {
        executeTransaction([&](pqxx::work& txn) {
            const auto result = txn.exec_params(
                "INSERT INTO info (date, username, message, severity)"
                "VALUES (NOW(), $1, $2, $3)",
                getCurrentUsername(),
                std::string(message),
                std::string(severity));

            txn.commit();
        });
    }
};
} // namespace DataAccessObjects
#endif
