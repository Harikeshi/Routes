#ifndef ERROR_LOG_HPP
#define ERROR_LOG_HPP

#include "BaseDAO.hpp"

// #include <nlohmann/json.hpp>
#include <pqxx/transaction.hxx>

namespace DataAccessObjects {
class ErrorLogDAO final : public BaseDAO
{
public:
    using BaseDAO::BaseDAO;

    // TODO: Заменить на модель для возврата по id из базы.
    struct ErrorLog
    {
        int id;
        std::string date;
        std::string message;
        std::string stack_trace;
        std::string source;
        std::string severity;
    };

    void update(const std::string_view& message,
                const std::string_view& stack_trace,
                const std::string_view& source,
                const std::string_view& severity) const
    {
        executeTransaction([&](pqxx::work& txn) {
            const auto result = txn.exec_params(
                "INSERT INTO error_logs (timestamp, message, stack_trace, source, severity) "
                "VALUES (NOW(), $1, $2, $3, $4)",
                std::string(message),
                std::string(stack_trace),
                std::string(source),
                std::string(severity));

            txn.commit();
        });
    }
};
} // namespace DataAccessObjects
#endif
