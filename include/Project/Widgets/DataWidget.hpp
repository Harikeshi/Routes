#pragma once

#include <QTabWidget>
#include <QWidget>

#include "Data/JsonExplorerWidget.hpp"
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

signals:
    void sendPath(QString);

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
    DataWidget(QWidget* parent = nullptr)
        : QTabWidget(parent)
    {
        jsonExplorer = new JsonFileExplorer(this);
        requestWidget = new RequestWidget(this);

        requestWidget->setEnabled(false);

        addTab(jsonExplorer, "JsonExplorer");
        addTab(requestWidget, "Request");

        // Проброс пути выше
        connect(jsonExplorer, &JsonFileExplorer::sendPath, this, &DataWidget::pathtoFile);
    }

    void clear()
    {
        requestWidget->clear();
    }
};
} // namespace Widgets
