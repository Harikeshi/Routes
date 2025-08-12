#pragma once

#include "JsonRepository.hpp"
#include "PostgreSQLRepository.hpp"
#include <QObject>

namespace Database {
class DatabaseManager : public QObject
{
    Q_OBJECT
private:
    //    QString connectionString = "host=127.0.0.1 dbname=request_report user=viz_user password=1 connect_timeout=3";
    QString connectionString = "host=192.168.50.52 dbname=request_report user=viz_user password=1 connect_timeout=3";
    //    QString connectionString = "host=192.168.205.130 dbname=request_report user=viz_user password=1 connect_timeout=3";

    inline QString getHomePath()
    {
        QString result;
#ifdef _WIN32
        result = QString(std::getenv("USERPROFILE"));
#else
        result = QString(getenv("HOME"));
#endif
        return result;
    }

public:
    // TODO: DatabaseConnection
    explicit DatabaseManager(std::shared_ptr<Database::IRepository> repo, QObject* parent = nullptr)
        : QObject(parent), repository(repo)
    {
        //!
        try
        {
            // TODO: need Create Factory
            repository = std::make_unique<Database::PostgreSQLRepository>(connectionString);
            emit sendMessage(QString("Request/Report PostgreSQL connected! %1").arg(connectionString.split(' ')[0]));
        }
        catch (std::runtime_error& ex)
        {
            // TODO: сообщение
            emit sendError(QString("Не удалось подключиться к PostgreSQL %1. %2").arg(connectionString.split(' ')[0]).arg(ex.what()));

            repository = std::make_unique<Database::JsonRepository>(getHomePath());

            emit sendMessage("Request/Report будут сохраняться в домашней директории.");
        }
    }

    QVector<Models::Report> loadAllReports() const
    {
        QVector<Models::Report> reports;
        try
        {
            size_t lastReportId = repository->getLastReportId();
            for (size_t id = 1; id <= lastReportId; ++id)
            {
                try
                {
                    // TODO: getAll()
                    reports.append(repository->findReportById(id));
                }
                catch (...)
                {
                    continue;
                }
            }
        }
        catch (const std::exception& e)
        {
            emit sendError(QString("Failed to load reports: %1").arg(e.what()));
        }
        return reports;
    }

    Models::Report loadReport(size_t id) const
    {
        try
        {
            return repository->findReportById(id);
        }
        catch (const std::exception& e)
        {
            emit sendError(QString("Failed to load report: %1").arg(e.what()));
            throw;
        }
    }

    Models::Request loadRequest(size_t id) const
    {
        try
        {
            return repository->findRequestById(id);
        }
        catch (const std::exception& e)
        {
            emit sendError(QString("Failed to load request: %1").arg(e.what()));
            throw;
        }
    }

    void saveReport(const Models::Report& report)
    {
        try
        {
            repository->save(report);
            emit reportSaved(report.id);
        }
        catch (const std::exception& e)
        {
            emit sendError(QString("Failed to save report: %1").arg(e.what()));
        }
    }

    void saveRequest(const Models::Request& request)
    {
        try
        {
            repository->save(request);
            emit requestSaved(request.id);
        }
        catch (const std::exception& e)
        {
            emit sendError(QString("Failed to save request: %1").arg(e.what()));
        }
    }

signals:
    void reportSaved(size_t id);
    void requestSaved(size_t id);
    void sendError(const QString& message) const;
    void sendMessage(const QString& message) const;

private:
    std::shared_ptr<Database::IRepository> repository;
};
} // namespace Database