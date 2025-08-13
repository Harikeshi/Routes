#pragma once
#include "ReportManager.hpp"
#include <QAbstractTableModel>
#include <QDateTime>

#include "Project/Initializer.hpp"

class ReportListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Column
    {
        ID,
        CreatedAt,
        RequestId,
        RequestName,
        COLUMN_COUNT
    };

    explicit ReportListModel(ReportManager* manager, QObject* parent = nullptr)
        : QAbstractTableModel(parent), m_manager(manager)
    {
        connect(m_manager, &ReportManager::reportSaved, this, &ReportListModel::refresh);
        refresh();
    }

    int rowCount(const QModelIndex& = QModelIndex()) const override
    {
        return m_reports.size();
    }

    int columnCount(const QModelIndex& = QModelIndex()) const override
    {
        return COLUMN_COUNT;
    }

    QVariant data(const QModelIndex& index, int role) const override
    {
        if (!index.isValid() || index.row() >= m_reports.size())
            return QVariant();

        const Models::Report& report = m_reports[index.row()];
        const Models::Request& request = m_requests[index.row()];

        if (role == Qt::DisplayRole || role == Qt::EditRole)
        {
            switch (index.column())
            {
            case ID:
                return report.id;
            case CreatedAt:
                // TODO: regular
                return report.created_at;
            case RequestId:
                return report.request_id;
            case RequestName:
                return request.owner;
            }
        }
        else if (role == Qt::UserRole)
        {
            return report.id; // Return report ID for internal use
        }

        return QVariant();
    }

    /*!
     * Header
     * @param section
     * @param orientation
     * @param role
     * @return
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        {
            switch (section)
            {
            case ID:
                return tr("ID");
            case CreatedAt:
                return tr("Created At");
            case RequestId:
                return tr("Request ID");
            case RequestName:
                return tr("Request Name");
            }
        }
        return QVariant();
    }

    void refresh()
    {
        beginResetModel();
        m_reports = m_manager->loadAllReports();
        m_requests.clear();

        for (const auto& report : m_reports)
        {
            try
            {
                m_requests.append(m_manager->loadRequest(report.request_id));
            }
            catch (...)
            {
                m_requests.append(Models::Request{}); // Add empty request if loading failed
            }
        }
        endResetModel();
    }

    Models::Report getReport(const QModelIndex& index) const
    {
        if (index.isValid() && index.row() < m_reports.size())
            return m_reports[index.row()];
        throw std::out_of_range("Invalid index");
    }

    Models::Request getRequest(const QModelIndex& index) const
    {
        if (index.isValid() && index.row() < m_requests.size())
            return m_requests[index.row()];
        throw std::out_of_range("Invalid index");
    }

    size_t getRequestId(const QModelIndex& index) const
    {
        if (index.isValid() && index.row() < m_reports.size())
        {
            auto report = m_reports[index.row()];

            return report.request_id;
        }

        throw std::out_of_range("Invalid index");
    }

    size_t getReportId(const QModelIndex& index) const
    {
        if (index.isValid() && index.row() < m_reports.size())
        {
            return m_reports[index.row()].getId();
        }

        throw std::out_of_range("Invalid index");
    }

    void setReports(const QVector<Models::Report>& reports)
    {
        m_reports = reports;
    }

    void setRequest(const QVector<Models::Request>& requests)
    {
        m_requests = requests;
    }

    void setCurrentFromIndex(const QModelIndex& index)
    {
        if (index.isValid() && index.row() < m_reports.size())
        {
            Initializer::instance().setReport(m_reports[index.row()]);
            Initializer::instance().setRequest(m_requests[index.row()]);
        }
    }

private:
    ReportManager* m_manager;
    QVector<Models::Report> m_reports;
    QVector<Models::Request> m_requests;
};