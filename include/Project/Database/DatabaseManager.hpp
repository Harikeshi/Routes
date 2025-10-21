#pragma once

#include "JsonRepository.hpp"
#include "PostgreSQLRepository.hpp"
#include "ReportRowModel.hpp"

#include <QObject>

namespace Database {
class DatabaseManager : public QObject
{
    Q_OBJECT
private:
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
    explicit DatabaseManager(QObject* parent = nullptr)
        : QObject(parent)
    {
        //!
        //    QString connectionString = "host=127.0.0.1 dbname=requests_reports user=viz_user password=1 connect_timeout=3";
        QString connectionString = "host=192.168.50.52 dbname=requests_reports user=viz_user password=1 connect_timeout=1";
        //        QString connectionString = "host=192.168.205.130 dbname=requests_reports user=viz_user password=1 connect_timeout=3";
        try
        {
            // TODO: need Create Factory
            repository = std::make_unique<Database::PostgreSQLRepository>(connectionString);
            qDebug() << QString("Request/Report PostgreSQL connected! %1").arg(connectionString.split(' ')[0]);
        }
        catch (std::exception& ex)
        {
            // TODO: сообщение
            qDebug() << QString("Не удалось подключиться к PostgreSQL %1. %2").arg(connectionString.split(' ')[0]).arg(ex.what());

            repository = std::make_unique<Database::JsonRepository>(getHomePath());

            qDebug() << "Request/Report будут сохраняться в домашней директории.";
        }
    }

    QVector<ReportRowModel> allReportRowsModel() const
    {
        auto reports = repository->getAllReports();

        QVector<ReportRowModel> rows;

        rows.reserve(reports.size());

        for (const auto& report : reports)
        {
            ReportRowModel row;
            row.report_id = report.id;
            row.request_id = report.request_id;
            row.scheme = report.scheme;
            row.message = report._messages[0].text;
            row.date = report.created_at.toString("hh:mm:ss dd-MM-yyyy");
            row.owner = report.owner;

            rows.push_back(row);
        }

        return rows;
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

    Models::Report getReport(size_t id) const
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

    Models::Request getRequest(size_t id) const
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

    void deleteRequest(size_t id)
    {
        repository->deleteRequestById(id);
    }

    void deleteReport(size_t id)
    {
        repository->deleteReportById(id);
    }

    void saveReport(Models::Report report)
    {
        try
        {
            report.request_id = repository->getLastRequestId();
            report.scheme = scheme;

            emit reportSaved(repository->save(report));

            emit sendReportsModel(allReportRowsModel());
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
            emit requestSaved(repository->save(request));
        }
        catch (const std::exception& e)
        {
            emit sendError(QString("Failed to save request: %1").arg(e.what()));
        }
    }
public slots:
    void setScheme(const QString& name)
    {
        scheme = name;
    }

signals:
    void reportSaved(size_t id);
    void requestSaved(size_t id);

    void sendReportsModel(const QVector<ReportRowModel>&) const;
    void sendError(const QString& message) const;
    void sendMessage(const QString& message) const;

private:
    std::shared_ptr<Database::IRepository> repository;

    QString scheme;
};
} // namespace Database
