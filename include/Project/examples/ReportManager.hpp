#pragma once
#include "../Database/PostgreSQLRepository.hpp"
#include <QObject>

class ReportManager : public QObject
{
    Q_OBJECT
public:
    explicit ReportManager(std::shared_ptr<Database::IRepository> repo, QObject* parent = nullptr)
        : QObject(parent), m_repository(repo)
    {
    }

    QVector<Models::Report> loadAllReports() const
    {
        QVector<Models::Report> reports;
        try
        {
            size_t lastReportId = m_repository->getLastReportId();
            for (size_t id = 1; id <= lastReportId; ++id)
            {
                try
                {
                    // TODO: getAll()
                    reports.append(m_repository->findReportById(id));
                }
                catch (...)
                {
                    continue;
                }
            }
        }
        catch (const std::exception& e)
        {
            emit errorOccurred(QString("Failed to load reports: %1").arg(e.what()));
        }
        return reports;
    }

    Models::Report loadReport(size_t id) const
    {
        try
        {
            return m_repository->findReportById(id);
        }
        catch (const std::exception& e)
        {
            emit errorOccurred(QString("Failed to load report: %1").arg(e.what()));
            throw;
        }
    }

    Models::Request loadRequest(size_t id) const
    {
        try
        {
            return m_repository->findRequestById(id);
        }
        catch (const std::exception& e)
        {
            emit errorOccurred(QString("Failed to load request: %1").arg(e.what()));
            throw;
        }
    }

    void saveReport(const Models::Report& report)
    {
        try
        {
            m_repository->save(report);
            emit reportSaved(report.id);
        }
        catch (const std::exception& e)
        {
            emit errorOccurred(QString("Failed to save report: %1").arg(e.what()));
        }
    }

    void saveRequest(const Models::Request& request)
    {
        try
        {
            m_repository->save(request);
            emit requestSaved(request.id);
        }
        catch (const std::exception& e)
        {
            emit errorOccurred(QString("Failed to save request: %1").arg(e.what()));
        }
    }

signals:
    void reportSaved(size_t id);
    void requestSaved(size_t id);
    void errorOccurred(const QString& message) const;

private:
    std::shared_ptr<Database::IRepository> m_repository;
};