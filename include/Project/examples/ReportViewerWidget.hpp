#pragma once

#include "../Database/PostgreSQLRepository.hpp"
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

class ReportViewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReportViewerWidget(std::shared_ptr<Database::PostgreSQLRepository> repo, QWidget* parent = nullptr)
        : QWidget(parent), m_repository(repo)
    {
        setupUI();
        loadReports();
    }

signals:
    void reportSelected(const Models::Report& report, const Models::Request& request);

private slots:
    void onItemDoubleClicked(QTableWidgetItem* item)
    {
        if (!item)
            return;

        try
        {
            size_t reportId = m_table->item(item->row(), 0)->data(Qt::UserRole).toULongLong();
            Models::Report report = m_repository->findReportById(reportId);
            Models::Request request = m_repository->findRequestById(report.request_id);

            emit reportSelected(report, request);
        }
        catch (const std::exception& e)
        {
            QMessageBox::critical(this, "Error", QString("Failed to load report: %1").arg(e.what()));
        }
    }

private:
    std::shared_ptr<Database::PostgreSQLRepository> m_repository;
    QTableWidget* m_table;

    void setupUI()
    {
        QVBoxLayout* layout = new QVBoxLayout(this);

        // Create table
        m_table = new QTableWidget(this);
        m_table->setColumnCount(4);
        m_table->setHorizontalHeaderLabels({"ID", "Created At", "Request ID", "Request Name"});
        m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        m_table->setSelectionMode(QAbstractItemView::SingleSelection);
        m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

        // Connect double click signal
        connect(m_table, &QTableWidget::itemDoubleClicked, this, &ReportViewerWidget::onItemDoubleClicked);

        layout->addWidget(m_table);
        setLayout(layout);
    }

    void loadReports()
    {
        try
        {
            // Clear table
            m_table->setRowCount(0);

            // Get all reports from database
            QVector<Models::Report> reports;
            size_t lastReportId = 10; //m_repository->getLastReportId();
            for (size_t id = 1; id <= lastReportId; ++id)
            {
                try
                {
                    reports.append(m_repository->findReportById(id));
                }
                catch (...)
                {
                    // Skip missing reports
                    continue;
                }
            }

            // Populate table
            m_table->setRowCount(reports.size());
            for (int i = 0; i < reports.size(); ++i)
            {
                const Models::Report& report = reports[i];
                Models::Request request = m_repository->findRequestById(report.request_id);

                // ID column (hidden UserRole contains actual ID)
                QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(report.id));
                idItem->setData(Qt::UserRole, QVariant::fromValue(report.id));
                m_table->setItem(i, 0, idItem);

                // Created At column (assuming reports table has created_at field)
                QTableWidgetItem* dateItem = new QTableWidgetItem(
                    // QDateTime::fromSecsSinceEpoch(report.created_at).toString("yyyy-MM-dd HH:mm:ss")
                );
                m_table->setItem(i, 1, dateItem);

                // Request ID column
                m_table->setItem(i, 2, new QTableWidgetItem(QString::number(report.request_id)));

                // Request Name column (example - you might need to add name to Request structure)
                QTableWidgetItem* nameItem = new QTableWidgetItem(request.id); // name // Assuming Request has name field
                m_table->setItem(i, 3, nameItem);
            }
        }
        catch (const std::exception& e)
        {
            QMessageBox::critical(this, "Error", QString("Failed to load reports: %1").arg(e.what()));
        }
    }
};