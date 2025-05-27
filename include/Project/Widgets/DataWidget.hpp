#pragma once

// Устанавливаем ширину колонки "Key" равной максимальному ключу
// QFontMetrics metrics(treeWidget->font());
// int maxWidth = getMaxKeyWidth(doc.object(), metrics);
//  treeWidget->setColumnWidth(0,

#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QTabWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "./JsonExplorerWidget.hpp"
#include "./RequestWidget.hpp"

namespace Widgets {
class DataWidget : public QTabWidget
{
    Q_OBJECT

    RequestWidget* requestWidget;
    JsonFileExplorer* jsonExplorer;

signals:
    void sendPath(QString);

public slots:
    void pathtoFile(const QString string)
    {
        emit sendPath(string);
    }

    void initializeRequest(const QJsonObject& jsonObject)
    {
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
        requestWidget = new RequestWidget(this);
        jsonExplorer = new JsonFileExplorer(this);
        addTab(requestWidget, "Request");
        addTab(jsonExplorer, "JsonExplorer");

        // Проброс пути выше
        connect(jsonExplorer, &JsonFileExplorer::sendPath, this, &DataWidget::pathtoFile);
    }

    void clear()
    {
        requestWidget->clear();
    }
};
} // namespace Widgets