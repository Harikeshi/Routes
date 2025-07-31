#pragma once

#include <functional>
#include <memory>
#include <pqxx/pqxx>

namespace DataAccessObjects {
class BaseDAO
{
protected:
    std::shared_ptr<pqxx::connection> connection;

public:
    explicit BaseDAO(const std::shared_ptr<pqxx::connection>& conn)
        : connection(conn)
    {
    }

    virtual ~BaseDAO()
    {
    }

protected:
    void executeTransaction(const std::function<void(pqxx::work&)>& transactionFunc) const
    {
        try
        {
            pqxx::work txn(*connection);
            transactionFunc(txn);
            txn.commit();
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error(std::string("Transaction Failed: ") + e.what());
        }
    }
};
} // namespace DataAccessObjects
