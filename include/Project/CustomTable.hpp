#pragma once

#include <QHeaderView>
#include <QPointF>
#include <QTableWidget>

class CustomTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit CustomTable(QWidget* parent = nullptr)
        : QTableWidget(0, 3, parent)
    {
        // Настройка таблицы
        setupTable();
    }

public slots:
    void updateSubmarine(const QPointF& position, double speed)
    {
        updateOrAddRow(0, "ПЛ", position, speed);
    }

    void updateObjects(const QVector<std::pair<QPointF, double>>& data)
    {
        for (int i = 0; i != data.size() - 1; ++i)
        {
            updateOrAddRow(i + 1, QString::number(i + 1), data[i].first, data[i].second);
        }
    }

signals:
    void dataUpdated();

private:
    void setupTable()
    {
        setHorizontalHeaderLabels({"", "Position", "Speed"});
        horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        setColumnWidth(0, 50);
        horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
        //setColumnWidth(1, 10);
        //setColumnWidth(2, 80);
        //horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        //horizontalHeader()->setMinimumSectionSize(50);
        //horizontalHeader()->setMaximumSectionSize(300);
        //horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        verticalHeader()->setVisible(false);
        setEditTriggers(QAbstractItemView::NoEditTriggers);
        setSelectionBehavior(QAbstractItemView::SelectRows);
    }

    void updateOrAddRow(int row, const QString& name, const QPointF& pos, double speed)
    {
        if (row >= rowCount())
        {
            insertRow(row);
        }
        // TODO: set только двух полей, но при этом требуется загрузка
        setItem(row, 0, new QTableWidgetItem(name));
        setItem(row, 1, new QTableWidgetItem(QString("(%1, %2)").arg(pos.x()).arg(pos.y())));
        setItem(row, 2, new QTableWidgetItem(QString::number(speed, 'f', 2)));

        emit dataUpdated();
    }
};