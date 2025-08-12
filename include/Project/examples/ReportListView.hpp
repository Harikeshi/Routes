// #pragma once
// #include "ReportListModel.hpp"
// #include <QHeaderView>
// #include <QTableView>
//
// class ReportListView : public QTableView
// {
//     Q_OBJECT
// public:
//     //! Вывод таблицы с Запросами, по двойному нажатию переслать request, report.
//     explicit ReportListView(ReportListModel* model, QWidget* parent = nullptr)
//         : QTableView(parent)
//     {
//         setModel(model);
//         setupView();
//         connectSignals();
//     }
//
// signals:
//     void reportDoubleClicked(const Models::Report& report, const Models::Request& request);
//
// private slots:
//     void onDoubleClicked(const QModelIndex& index)
//     {
//         try
//         {
//             auto model = qobject_cast<ReportListModel*>(this->model());
//             if (model)
//             {
//                 Models::Report report = model->getReport(index);
//                 Models::Request request = model->getRequest(index);
//                 emit reportDoubleClicked(report, request);
//             }
//         }
//         catch (const std::exception& e)
//         {
//             emit errorOccurred(QString("Failed to get report: %1").arg(e.what()));
//         }
//     }
//
// signals:
//     void errorOccurred(const QString& message);
//
// private:
//     void setupView()
//     {
//         setSelectionMode(QAbstractItemView::SingleSelection);
//         setSelectionBehavior(QAbstractItemView::SelectRows);
//         setEditTriggers(QAbstractItemView::NoEditTriggers);
//         verticalHeader()->hide();
//         horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//         setSortingEnabled(true);
//     }
//
//     void connectSignals()
//     {
//         connect(this, &QTableView::doubleClicked, this, &ReportListView::onDoubleClicked);
//     }
// };