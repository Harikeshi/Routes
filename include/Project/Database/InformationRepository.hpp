#ifndef DATABASE_REPOSITORY_HPP
#define DATABASE_REPOSITORY_HPP

#include "DataAccessObjects/ErrorLogDAO.hpp"
#include "DataAccessObjects/EventDAO.hpp"
#include "DataAccessObjects/InfoDAO.hpp"
#include "DataAccessObjects/RequestDAO.hpp"
#include "DataAccessObjects/ShipDAO.hpp"

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

    DataAccessObjects::ShipDAO shipDao;
    DataAccessObjects::RequestDAO requestDao;
    DataAccessObjects::EventDAO eventDao;
    DataAccessObjects::InfoDAO infoDao;
    DataAccessObjects::ErrorLogDAO errorDao;

public:
    InformationRepository()
        : connection(nullptr), shipDao(nullptr), requestDao(nullptr),
          eventDao(nullptr), infoDao(nullptr), errorDao(nullptr)
    {
    }

    explicit InformationRepository(const std::shared_ptr<pqxx::connection>& conn)
        : connection(conn),
          shipDao(connection),
          requestDao(connection),
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

    int updateShip(const int count, const int detection) const
    {
        int id = -1;
        try
        {
            id = shipDao.update(count, detection);
        }
        catch (...)
        {
            throw;
        }

        return id;
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

    void updateRequest(int shipCount, int shipDetection, const DataAccessObjects::RequestDAO::Request& request, const DataAccessObjects::EventDAO::Event& event)
    {
        try
        {
            pqxx::work txn(*connection);

            // Создаем корабль
            auto shipId = shipDao.update(shipCount, shipDetection);

            // Создаем запрос
            const int requestId = requestDao.update(
                request.time, request.target, request.search_region, request.border_line, shipId);

            // Создаем событие
            eventDao.update(event.username, event.message, event.json_content, requestId);

            txn.commit();
        }
        catch (...)
        {
            throw;
        }
    }
    void updateReport()
    {
    }
};
} // namespace Database
#endif
