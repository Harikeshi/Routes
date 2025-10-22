#ifndef DATABASE_REPOSITORY_HPP
#define DATABASE_REPOSITORY_HPP

#include "DataAccessObjects/ErrorLogDAO.hpp"
#include "DataAccessObjects/EventDAO.hpp"
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

    DataAccessObjects::EventDAO eventDao;
    DataAccessObjects::InfoDAO infoDao;
    DataAccessObjects::ErrorLogDAO errorDao;

public:
    InformationRepository()
        : connection(nullptr),
          eventDao(nullptr), infoDao(nullptr), errorDao(nullptr)
    {
    }

    explicit InformationRepository(const std::shared_ptr<pqxx::connection>& conn)
        : connection(conn),
          eventDao(connection),
          infoDao(connection),
          errorDao(connection)
    {
    }

    bool isNull()
    {
        if (connection == nullptr)
            return true;

        return false;
    }

    void updateErrorLog(const std::string_view& message,
                        const std::string_view& stack_trace,
                        const std::string_view& source,
                        const std::string_view& severity = "ERROR") const
    {
        try
        {
            errorDao.update(message, stack_trace, source, severity);
        }
        catch (...)
        {
            throw;
        }
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
