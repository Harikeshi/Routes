#pragma once

#include <QHeaderView>
#include <QPointF>
#include <QTableWidget>

namespace Widgets {
class CustomTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit CustomTable(QWidget* parent = nullptr)
        : QTableWidget(0, 3, parent)
    {
        setupTable(2);
    }

signals:
    void dataUpdated();

public:
    void setupTable(size_t size)
    {
        setHorizontalHeaderLabels({"", "Position", "Speed"});
        horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        setColumnWidth(0, 50);

        insertRow(0);
        setItem(0, 0, new QTableWidgetItem(" ПЛ"));
        horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

        for (size_t i = 0; i != size; ++i)
        {
            insertRow(i + 1);
            setItem(i + 1, 0, new QTableWidgetItem(""));
            horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
            horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
        }

        verticalHeader()->setVisible(false);
        setEditTriggers(QAbstractItemView::NoEditTriggers);
        setSelectionBehavior(QAbstractItemView::SelectRows);
    }
public slots:
    void updateOrAddRow(size_t row, const QPointF& pos, double speed)
    {
        if (row >= rowCount())
        {
            insertRow(row);
        }
        // TODO: set
        if (row == 0)
            setItem(row, 0, new QTableWidgetItem(QString("ПЛ")));
        else
            setItem(row, 0, new QTableWidgetItem(QString("БЭНК %1").arg(row)));

        setItem(row, 1, new QTableWidgetItem(QString("(%1, %2)").arg(pos.x()).arg(pos.y())));
        setItem(row, 2, new QTableWidgetItem(QString::number(speed, 'f', 2)));

        emit dataUpdated();
    }

    void clearRow(size_t row)
    {
    }
};
} // namespace Widgets
