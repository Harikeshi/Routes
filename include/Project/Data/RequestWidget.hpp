#pragma once

#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <QLabel>
#include <QTabWidget>
#include <QVector>

namespace Data {
class RequestWidget : public QWidget
{
private:
    void parseJsonObject(const QJsonObject& jsonObject, QTreeWidgetItem* parentItem)
    {
        for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
            item->setText(0, it.key());
            parseJsonValue(it.value(), item);
        }
    }

    void parseJsonArray(const QJsonArray& jsonArray, QTreeWidgetItem* parentItem)
    {
        for (const QJsonValue& value : jsonArray)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
            parseJsonValue(value, item);
        }
    }

    void parseJsonValue(const QJsonValue& value, QTreeWidgetItem* parentItem)
    {
        if (value.isObject())
        {
            parseJsonObject(value.toObject(), parentItem);
        }
        else if (value.isArray())
        {
            parseJsonArray(value.toArray(), parentItem);
        }
        else if (value.isString())
        {
            parentItem->setText(1, value.toString());
        }
        else if (value.isDouble())
        {
            parentItem->setText(1, QString::number(value.toDouble()));
        }
        else if (value.isBool())
        {
            parentItem->setText(1, value.toBool() ? "true" : "false");
        }
        else if (value.isNull())
        {
            parentItem->setText(1, "null");
        }
    }

public:
    RequestWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QVBoxLayout* layout = new QVBoxLayout(this);

        treeWidget = new QTreeWidget(this);

        if (treeWidget)
        {
            layout->addWidget(treeWidget);
        }

        setLayout(layout);
    }

    void createTreeFromJson(const QJsonObject& jsonObject)
    {
        treeWidget->setColumnCount(2);
        treeWidget->setHeaderLabels(QStringList() << "Key"
                                                  << "Value");
        QTreeWidgetItem* rootItem = new QTreeWidgetItem(treeWidget);
        rootItem->setText(0, "root");

        parseJsonObject(jsonObject, rootItem);
        // treeWidget->expandAll(); // Развернуть все элементы дерева
        treeWidget->expandToDepth(0); // Закрыть все вкладки
    }

    void clear()
    {
        treeWidget->clear();
    }

private:
    QTreeWidget* treeWidget;
};
} // namespace Data
