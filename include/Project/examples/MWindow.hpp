#pragma once

#include "../Database/PostgreSQLRepository.hpp"
#include "ReportViewerWidget.hpp"

#include <QMainWindow>

class MWindow : public QMainWindow
{
    Q_OBJECT

public:
    MWindow(QWidget* parent = nullptr)
        : QMainWindow(parent)
    {
        // Initialize repository
        m_repository = std::make_shared<Database::PostgreSQLRepository>(
            "postgresql://viz_user:1@192.168.205.130/request_report");

        // Create and setup report viewer
        m_reportViewer = new ReportViewerWidget(m_repository, this);
        setCentralWidget(m_reportViewer);

        // Connect signals
        connect(m_reportViewer, &ReportViewerWidget::reportSelected, this, &MWindow::onReportSelected);

        setWindowTitle("Report Viewer");
        resize(800, 600);
    }

private slots:
    void onReportSelected(const Models::Report& report, const Models::Request& request)
    {
        // Handle selected report
        QMessageBox::information(this, "Report Selected", QString("Selected report ID: %1\nRequest: %2 (%3)").arg(report.id).arg(request.id).arg(request.id));

        // Здесь можно открыть детальное представление отчета
        // или выполнить другие действия с выбранным отчетом
    }

private:
    std::shared_ptr<Database::PostgreSQLRepository> m_repository;
    ReportViewerWidget* m_reportViewer;
};