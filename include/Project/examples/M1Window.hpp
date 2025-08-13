// mainwindow.h
#pragma once

#include "ReportListView.hpp"
#include "ReportManager.hpp"
#include <QMainWindow>
#include <QMessageBox>

class M1Window : public QMainWindow
{
    Q_OBJECT
public:
    M1Window(QWidget* parent = nullptr)
        : QMainWindow(parent)
    {
        // Initialize dependencies
        auto repository = std::make_shared<Database::PostgreSQLRepository>(
            "postgresql://user:password@localhost/dbname");
        m_reportManager = new ReportManager(repository, this);
        m_reportModel = new ReportListModel(m_reportManager, this);
        m_reportView = new ReportListView(m_reportModel, this);
        // Setup UI
        setCentralWidget(m_reportView);
        setWindowTitle("Report Viewer");
        resize(800, 600);
        // Connect signals
        connect(m_reportView, &ReportListView::reportDoubleClicked, this, &M1Window::onReportSelected);
        //        connect(m_reportView, &ReportListView::errorOccurred, this, &M1Window::onError);
        connect(m_reportManager, &ReportManager::errorOccurred, this, &M1Window::onError);
    }

private slots:
    void onReportSelected(const Models::Report& report, const Models::Request& request)
    {
        QMessageBox::information(this, "Report Selected", QString("Report ID: %1\nRequest: %2").arg(report.id).arg(request.owner));
    }

    void onError(const QString& message)
    {
        QMessageBox::critical(this, "Error", message);
    }

private:
    ReportManager* m_reportManager;
    ReportListModel* m_reportModel;
    ReportListView* m_reportView;
};