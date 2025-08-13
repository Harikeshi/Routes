#pragma once

#include <QTabWidget>
#include <QWidget>

#include "Data/JsonExplorerWidget.hpp"
#include "Data/PropertyTable.hpp"
#include "Data/ReportListWidget.hpp"
#include "Data/RequestWidget.hpp"

namespace Widgets {
/*!
 * Класс FileManager +
 */
class DataWidget : public QTabWidget
{
    Q_OBJECT

    JsonFileExplorer* jsonExplorer;
    Data::ReportListWidget* reportList;
    Data::PropertyEditor* propertyWidget;

signals:
    void sendPath(QString);
    void sendRequestFromWidget(Models::Request);
    void sendRequestToWidget(Models::Request);

    void sendReportRequestIds(size_t report_id, size_t request_id);

    void sendReportId(size_t id);
    void sendRequestId(size_t id);

    void refreshReports();

public slots:
    void setIdsFromReportList(size_t report_id, size_t request_id)
    {
        emit sendReportRequestIds(report_id, request_id);
    }

    void needRefreshReports()
    {
    }

    void updateReports(const QVector<Database::ReportRowModel>& rows)
    {
        reportList->refresh(rows);
        qDebug() << rows.size();
    }

    void pathtoFile(const QString string)
    {
        emit sendPath(string);
    }

    // void initializeRequest(const QJsonObject& jsonObject)
    // {
    //     requestWidget->setEnabled(true);
    //     requestWidget->clear();
    //     requestWidget->createTreeFromJson(jsonObject);
    // }

    void initializeReport(const QJsonObject& jsonObject)
    {
        // requestWidget->createTreeFromJson(jsonObject);
    }

public:
    void setRequest(Models::Request request)
    {
        emit sendRequestToWidget(request);
    }

    DataWidget(QWidget* parent = nullptr)
        : QTabWidget(parent)
    {
        jsonExplorer = new JsonFileExplorer(this);
        propertyWidget = new Data::PropertyEditor(this);
        reportList = new Data::ReportListWidget(this);

        addTab(jsonExplorer, "JsonExplorer");
        addTab(propertyWidget, "Request");
        addTab(reportList, "Reports");

        // Проброс пути выше
        connect(jsonExplorer, &JsonFileExplorer::sendPath, this, &DataWidget::pathtoFile);
        connect(propertyWidget, &Data::PropertyEditor::propertyChanged, this, &DataWidget::getRequestFromWidget);
        connect(reportList, &Data::ReportListWidget::reportActivated, this, &DataWidget::setIdsFromReportList);
        connect(this, &DataWidget::sendRequestToWidget, propertyWidget, &Data::PropertyEditor::updateFromRequest);
    }

public:
    void clear()
    {
        // requestWidget->clear();
    }

    /*!
     *
     * @param request
     */
    void getRequestFromWidget(const Models::Request& request)
    {
        emit sendRequestFromWidget(request);
    }
};
} // namespace Widgets