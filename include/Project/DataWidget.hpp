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
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <QLabel>
#include <QTabWidget>
#include <QVector>

#include "Data/RequestWidget.hpp"

class DataWidget : public QTabWidget
{
    Data::RequestWidget* requestWidget;

public:
    DataWidget(QWidget* parent = nullptr)
        : QTabWidget(parent)
    {
        requestWidget = new Data::RequestWidget();
        addTab(requestWidget, "Request.json");
    }

    void clear()
    {
        requestWidget->clear();
    }

    void initialize(const QJsonObject& jsonObject)
    {
        requestWidget->createTreeFromJson(jsonObject);
    }
};
