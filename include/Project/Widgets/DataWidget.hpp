#pragma once

#include <QTabWidget>
#include <QWidget>

#include "Data/JsonExplorerWidget.hpp"
#include "Data/PropertyTable.hpp"
#include "Data/RequestWidget.hpp"

namespace Widgets {
/*!
 * Класс FileManager +
 */
class DataWidget : public QTabWidget
{
    Q_OBJECT

    JsonFileExplorer* jsonExplorer;
    RequestWidget* requestWidget;
    Data::PropertyEditor* propertyWidget;

signals:
    void sendPath(QString);
    void sendRequestFromWidget(Models::Request);
    void sendRequestToWidget(Models::Request);

public slots:
    void pathtoFile(const QString string)
    {
        emit sendPath(string);
    }

    void initializeRequest(const QJsonObject& jsonObject)
    {
        requestWidget->setEnabled(true);
        requestWidget->clear();
        requestWidget->createTreeFromJson(jsonObject);
    }

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
        requestWidget = new RequestWidget(this);
        propertyWidget = new Data::PropertyEditor(this);

        requestWidget->setEnabled(false);

        addTab(jsonExplorer, "JsonExplorer");
        addTab(propertyWidget, "PropertyWidget");
        addTab(requestWidget, "Request");

        // Проброс пути выше
        connect(jsonExplorer, &JsonFileExplorer::sendPath, this, &DataWidget::pathtoFile);
        connect(propertyWidget, &Data::PropertyEditor::propertyChanged, this, &DataWidget::getRequestFromWidget);
        connect(this, &DataWidget::sendRequestToWidget, propertyWidget, &Data::PropertyEditor::updateFromRequest);
    }

public:
    void clear()
    {
        requestWidget->clear();
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