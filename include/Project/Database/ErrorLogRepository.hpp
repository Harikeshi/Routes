#ifndef ERROR_LOG_REPOSITORY_HPP
#define ERROR_LOG_REPOSITORY_HPP

#include "Project/GetCurrentUsername.hpp"
#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <string>
#include <string_view>

namespace Database {
class IDbConnection
{
public:
    virtual ~IDbConnection() = default;

    // Возвращает сырое подключение (для специфичных операций)
    virtual void* getRawConnection() = 0;

    // Проверяет активность подключения
    virtual bool isConnected() const = 0;

    // Выполняет запрос (опционально)
    virtual void execute(const std::string& query) = 0;
};

class PqxxConnection final : public IDbConnection
{
public:
    PqxxConnection(const std::string& connectionString);

    void* getRawConnection() override
    {
        return &connection_;
    }

    bool isConnected() const override
    {
        return connection_.is_open();
    }

    void execute(const std::string& query) override
    {
        pqxx::work txn(connection_);
        txn.exec(query);
        txn.commit();
    }

private:
    pqxx::connection connection_;
};

// Пример использования
std::unique_ptr<IDbConnection> createPostgresConnection(const std::string& connStr)
{
    return std::make_unique<PqxxConnection>(connStr);
}

class ErrorLogRepository
{
public:
    // Используем string_view для эффективной передачи строк
    void logError(const std::string_view message,
                  const std::string_view stack_trace,
                  const std::string_view source,
                  const std::string_view severity = "ERROR")
    {
        try
        {
            pqxx::work txn(*conn_);

            // Используем параметризованные запросы для безопасности
            txn.exec_params(
                "INSERT INTO error_logs (timestamp, message, stack_trace, source, severity) "
                "VALUES (NOW(), $1, $2, $3, $4)",
                std::string(message),
                std::string(stack_trace),
                std::string(source),
                std::string(severity));

            txn.commit();
        }
        catch (const pqxx::sql_error& e)
        {
            // Логирование ошибки в базе данных не должно ломать программу
            std::cerr << "Failed to log error to DB: " << e.what() << "\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << "Unexpected error: " << e.what() << "\n";
        }
    }

    void logInfo(const std::string_view message,
                 // const std::string_view stack_trace,
                 // const std::string_view source,
                 const std::string_view severity = "ERROR")
    {
        try
        {
            pqxx::work txn(*conn_);
            auto username = getCurrentUsername();

            // Используем параметризованные запросы для безопасности
            txn.exec_params(
                "INSERT INTO info (date, username, message, severity) "
                "VALUES (NOW(), $1, $2, $3)",
                username,
                std::string(message),
                // std::string(stack_trace),
                // std::string(source),
                std::string(severity));

            txn.commit();
        }
        catch (const pqxx::sql_error& e)
        {
            // Логирование ошибки в базе данных не должно ломать программу
            std::cerr << "Failed to log error to DB: " << e.what() << "\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << "Unexpected error: " << e.what() << "\n";
        }
    }

    explicit ErrorLogRepository(std::shared_ptr<pqxx::connection> conn)
        : conn_(std::move(conn))
    {
        if (!conn_ || !conn_->is_open())
        {
            throw std::runtime_error("Invalid database connection");
        }
    }

private:
    std::shared_ptr<pqxx::connection> conn_;
};
} // namespace Database
#endif
