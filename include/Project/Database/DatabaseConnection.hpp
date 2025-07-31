#ifndef DATABASE_CONNECTION_HPP
#define DATABASE_CONNECTION_HPP

#include <pqxx/pqxx>

#include <memory>
#include <stdexcept>
#include <string>

namespace Database {
class DatabaseConnection
{
public:
    // Singleton статический адрес гарантирует:
    // Один способ создания подключений (единая конфигурация)
    // Контроль над жизненным циклом соединений
    // Предсказуемое поведение во всех частях программы
    static std::shared_ptr<pqxx::connection> createConnection(
        const std::string& host = "192.168.205.130",
        const std::string& dbname = "viz_log",
        const std::string& user = "viz_user",
        const std::string& password = "1")
    {
        std::string connectionString =
            "host=" + host + " "
                             "dbname=" +
            dbname + " "
                     "user=" +
            user + " "
                   "password=" +
            password + " connect_timeout=5";

        try
        {
            auto conn = std::make_shared<pqxx::connection>(connectionString);

            if (!conn->is_open())
            {
                throw std::runtime_error("Failed to open connection to database");
            }
            return conn;
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Failed to connect to database: " + std::string(e.what()));
        }
    }
};
} // namespace Database
#endif
