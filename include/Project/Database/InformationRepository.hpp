#ifndef DATABASE_REPOSITORY_HPP
#define DATABASE_REPOSITORY_HPP

#include "DataAccessObjects/InfoDAO.hpp"

#include <memory>
#include <pqxx/connection.hxx>

namespace Database {
class InformationRepository
{
    /*
     * Пример использования
     *
    DatabaseRepository data(DatabaseConnection::createConnection());

    try
    {
        data.updateInfo("Test ",
                        "CRITICAL");
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    */

    std::shared_ptr<pqxx::connection> connection;

    DataAccessObjects::InfoDAO infoDao;

public:
    InformationRepository()
        : connection(nullptr),
          infoDao(nullptr)
    {
    }

    explicit InformationRepository(const std::shared_ptr<pqxx::connection>& conn)
        : connection(conn),
          infoDao(connection)
    {
    }

    bool isNull()
    {
        if (connection == nullptr)
            return true;

        return false;
    }

    void updateInfo(const std::string_view& message,
                    const std::string_view& severity = "ERROR") const
    {
        try
        {
            infoDao.update(message, severity);
        }
        catch (...)
        {
            throw;
        }
    }
};
} // namespace Database
#endif
