#pragma once
#include "ReportListModel.hpp"
#include <QHeaderView>
#include <QTableView>

class ReportListView : public QTableView
{
    Q_OBJECT
public:
    //! Вывод таблицы с Запросами, по двойному нажатию переслать request, report.
    explicit ReportListView(ReportListModel* model, QWidget* parent = nullptr)
        : QTableView(parent)
    {
        setModel(model);
        setupView();
        connectSignals();
    }

signals:
    void reportDoubleClicked(const Models::Report& report, const Models::Request& request);

    void sendError(const QString& message);
    void sendMessage(const QString&);
private slots:
    void onDoubleClicked(const QModelIndex& index)
    {
        //        try
        //        {
        //            auto model = qobject_cast<ReportListModel*>(this->model());
        //            if (model)
        //            {
        //                model->se
        //                    Models::Report report = model->getReport(index);
        //                Models::Request request = model->getRequest(index);
        //                emit reportDoubleClicked(report, request);
        //            }
        //        }
        //        catch (const std::exception& e)
        //        {
        //            emit sendError(QString("Failed to get report: %1").arg(e.what()));
        //        }
    }

    void loadReports()
    {
        //        try
        //        {
        //            // Clear table
        //            this->setRowCount(0);
        //
        //            // Get all reports from database
        //            QVector<Models::Report> reports;
        //            size_t lastReportId = 10; //m_repository->getLastReportId();
        //            for (size_t id = 1; id <= lastReportId; ++id)
        //            {
        //                try
        //                {
        //                    reports.append(m_repository->findReportById(id));
        //                }
        //                catch (...)
        //                {
        //                    // Skip missing reports
        //                    continue;
        //                }
        //            }
        //
        //            // Populate table
        //            m_table->setRowCount(reports.size());
        //            for (int i = 0; i < reports.size(); ++i)
        //            {
        //                const Models::Report& report = reports[i];
        //                Models::Request request = m_repository->findRequestById(report.request_id);
        //
        //                // ID column (hidden UserRole contains actual ID)
        //                QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(report.id));
        //                idItem->setData(Qt::UserRole, QVariant::fromValue(report.id));
        //                m_table->setItem(i, 0, idItem);
        //
        //                // Created At column (assuming reports table has created_at field)
        //                QTableWidgetItem* dateItem = new QTableWidgetItem(
        //                    // QDateTime::fromSecsSinceEpoch(report.created_at).toString("yyyy-MM-dd HH:mm:ss")
        //                );
        //                m_table->setItem(i, 1, dateItem);
        //
        //                // Request ID column
        //                m_table->setItem(i, 2, new QTableWidgetItem(QString::number(report.request_id)));
        //
        //                // Request Name column (example - you might need to add name to Request structure)
        //                QTableWidgetItem* nameItem = new QTableWidgetItem(request.id); // name // Assuming Request has name field
        //                m_table->setItem(i, 3, nameItem);
        //            }
        //        }
        //        catch (const std::exception& e)
        //        {
        //            QMessageBox::critical(this, "Error", QString("Failed to load reports: %1").arg(e.what()));
        //        }
    }

private:
    void setupView()
    {
        setSelectionMode(QAbstractItemView::SingleSelection);
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setEditTriggers(QAbstractItemView::NoEditTriggers);
        verticalHeader()->hide();
        horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        setSortingEnabled(true);
    }

    void connectSignals()
    {
        connect(this, &QTableView::doubleClicked, this, &ReportListView::onDoubleClicked);
    }
};