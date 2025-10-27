#pragma once

//! TODO: Структура
//#include "core/*.hpp"
//#include "ui/*.hpp"

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
        //        tableView_->horizontalHeader()->setStretchLastSection(true);

        layout->addWidget(tableView_);
        setLayout(layout);

        connect(tableView_, &QTableView::doubleClicked, this, &ReportListWidget::onDoubleClicked);

        // Контекстное меню
        tableView_->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(tableView_, &QTableView::customContextMenuRequested, this, &ReportListWidget::onContextMenuRequested);
    }

public slots:

    void deleteReport(size_t id)
    {
        model_->removeReport(id);
    }

    //! При загрузке и при обновлении по кнопке.
    void refresh(const QVector<Database::ReportRowModel>& rows)
    {
        try
        {
            model_->setRows(rows);

            update(); // TODO: ?
        }
        catch (const std::exception& ex)
        {
            emit sendError(QString("Failed to load reports: %1").arg(ex.what()));
        }
    }

    void refresh(const Database::ReportRowModel& row)
    {
        try
        {
            model_->addRow(row);

            update(); // TODO: ?
        }
        catch (const std::exception& ex)
        {
            emit sendError(QString("Failed to load reports: %1").arg(ex.what()));
        }
    }

signals:
    void reportActivated(size_t report_id, size_t request_id);
    void sendError(const QString&);
    void saveReportToFile(qint64 report_id, qint64 request_id);
    void deleteReportFromDb(qint64 report_id, qint64 request_id);
    void addReportToDb();

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

    void onContextMenuRequested(const QPoint& pos)
    {
        QModelIndex idx = tableView_->indexAt(pos);
        if (!idx.isValid())
            return;

        const auto* row = model_->rowAt(idx.row());
        if (!row)
            return;

        QMenu menu(this);
        QAction* saveAction = menu.addAction(tr("Сохранить"));
        QAction* deleteAction = menu.addAction(tr("Удалить"));
        QAction* addAction = menu.addAction(tr("Добавить в базу"));

        QAction* chosen = menu.exec(tableView_->viewport()->mapToGlobal(pos));
        if (chosen == saveAction)
        {
            emit saveReportToFile(row->report_id, row->request_id);
        }
        else if (chosen == deleteAction)
        {
            emit deleteReportFromDb(row->report_id, row->request_id);
        }
        else if (chosen == addAction)
        {
            emit addReportToDb();
        }
    }

private:
    QTableView* tableView_ = nullptr;
    Database::ReportTableModel* model_ = nullptr;
};
} // namespace Widgets::Data
