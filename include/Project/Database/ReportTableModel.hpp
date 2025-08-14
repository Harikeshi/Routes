#pragma once

#include <QAbstractTableModel>
#include <QVector>

#include "ReportRowModel.hpp"

namespace Database {
class ReportTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ReportTableModel(QObject* parent = nullptr)
        : QAbstractTableModel(parent)
    {
    }

    void setRows(const QVector<ReportRowModel>& _rows)
    {
        beginResetModel();
        rows = std::move(_rows);
        endResetModel();
    }

    const ReportRowModel* rowAt(int index) const
    {
        if (index < 0 || index >= static_cast<int>(rows.size()))
            return nullptr;

        return &rows[index];
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override
    {
        return parent.isValid() ? 0 : static_cast<int>(rows.size());
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return 4; // ID, Name, Date, RequestId
    }

    QVariant data(const QModelIndex& index, int role) const override
    {
        if (!index.isValid() || role != Qt::DisplayRole)
            return {};

        const auto& row = rows[static_cast<size_t>(index.row())];
        switch (index.column())
        {
        case 0:
            return row.report_id;
        case 1:
            return row.request_id;
        case 2:
            return row.scheme;
        case 3:
            return row.message;
            //return row.date.toUTC().toString(Qt::ISODate);
            //        case 4:
            //            return row.owner;
        }
        return {};
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return {};
        switch (section)
        {
        case 0:
            return "Report ID";
        case 1:
            return "Request ID";
        case 3:
            return "Scheme";
        case 2:
            return "Message";
            //        case 3:
            //            return "Date";
            //        case 4:
            //            return "Owner";
        }
        return {};
    }

private:
    QVector<ReportRowModel> rows;
};
} // namespace Database