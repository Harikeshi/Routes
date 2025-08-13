#pragma once

//#include "core/*.h"
//#include "ui/*.h"

#include "Project/Database/ReportTableModel.hpp"
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>

namespace Widgets::Data {
class ReportListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReportListWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        auto* layout = new QVBoxLayout(this);
        tableView_ = new QTableView(this);
        model_ = new Database::ReportTableModel(this);
        tableView_->setModel(model_);
        tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView_->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);
        // tableView_->horizontalHeader()->setStretchLastSection(true);

        layout->addWidget(tableView_);
        setLayout(layout);

        connect(tableView_, &QTableView::doubleClicked, this, &ReportListWidget::onDoubleClicked);
    }

public slots:
    //! При загрузке и при обновлении по кнопке.
    void refresh(const QVector<Database::ReportRowModel>& rows)
    {
        try
        {
            model_->setRows(std::move(rows));
            update(); // TODO: ?
        }
        catch (const std::exception& ex)
        {
            // В реальном приложении — показать пользователю ошибку
            emit sendError(QString("Failed to load reports: %1").arg(ex.what()));
        }
    }

signals:
    void reportActivated(size_t report_id, size_t request_id);
    void sendError(const QString&);

private slots:
    void onDoubleClicked(const QModelIndex& idx)
    {
        const auto* row = model_->rowAt(idx.row());
        if (!row)
        {
            return;
        }

        emit reportActivated(row->report_id, row->request_id);
    }

private:
    QTableView* tableView_ = nullptr;
    Database::ReportTableModel* model_ = nullptr;
};
} // namespace Widgets::Data