#pragma once

#include <QtWidgets>
#include <QtConcurrent/qtconcurrentrun.h>

#include <Project/Widgets/Matrix3DView.hpp>
#include <Project/Operations/UtilityOperations.hpp>

namespace Widgets {

class MatrixViewWidget : public QWidget
{
    Q_OBJECT

public:
    void loadReport(const Models::Report& report)
    {
        // clear()
        route = Outputs::Route{};

        route = Operations::routeFromReport(report);
    }

    void loadRequest(const Models::Request& request)
    {
        polygon.clear();
        polygon = Operations::polygonFromRequest(request);

        radius = request.ship.detection_range;
    }

    MatrixViewWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QRectF bbox;

        auto layout = new QVBoxLayout(this);

        // --- Надпись загрузки и вьюшка ---
        loadingLabel_ = new QLabel("Loading…");
        loadingLabel_->setAlignment(Qt::AlignCenter);
        loadingLabel_->hide();
        layout->addWidget(loadingLabel_);

        view_ = new Matrix3DView();
        layout->addWidget(view_, 1);

        auto button = new QPushButton("calc");
        layout->addWidget(button);

        //TODO: future перевести на int
        futureWatcher_ = new QFutureWatcher<std::vector<std::vector<double>>>(this);
        connect(futureWatcher_, &QFutureWatcherBase::finished, this, &MatrixViewWidget::onMatrixReady);
        connect(button, &QPushButton::clicked, this, &MatrixViewWidget::startAsyncCalculationMatrix);

        futureWatcherTime_ = new QFutureWatcher<double>(this);
        connect(futureWatcherTime_, &QFutureWatcherBase::finished, this, &MatrixViewWidget::isTimeReady);
        connect(button, &QPushButton::clicked, this, &MatrixViewWidget::startAsyncCalculationTime);
    }
signals:
    void sendMessage(const QString& message, size_t type);
private slots:
    void onMatrixReady()
    {
        auto matrix_ = futureWatcher_->result();

        loadingLabel_->hide();

        if (matrix_.empty())
        {
            emit sendMessage("Матрица не построена. empty", 1);
            return;
        }

        view_->setMatrix(matrix_);
    }

    void isTimeReady()
    {
        auto time = futureWatcherTime_->result();

        view_->setAverageTime(time);
    }

    void startAsyncCalculationTime()
    {
        auto func = "Расчет времени::";

        if (!checkFunc(func))
            return;

        view_->setWait();

        view_->clear();
        QFuture<double> future = QtConcurrent::run([=]() {
            return Operations::buildAverageTime(polygon, route, radius);
        });
        futureWatcherTime_->setFuture(future);
    }

    void startAsyncCalculationMatrix()
    {
        auto func = "Расчет матрицы::";

        if (!checkFunc(func))
            return;

        loadingLabel_->show();

        view_->clear();

        QFuture<std::vector<std::vector<double>>> future = QtConcurrent::run([=]() {
            return Operations::buildMatrix(polygon, route, radius);
        });
        futureWatcher_->setFuture(future);
    }

    bool checkFunc(const char* func)
    {
        if (route.points.empty())
        {
            emit sendMessage(QString("%1%2").arg(func).arg("route.empty()"), 1);
            return false;
        }
        if (radius == 0)
        {
            emit sendMessage(QString("%1%2").arg(func).arg("radius == 0"), 1);
            return false;
        }
        if (polygon.outer().empty())
        {
            emit sendMessage(QString("%1%2").arg(func).arg("polygon.outer().empty()"), 1);
            return false;
        }

        return true;
    }

private:
    double radius{0};

    PrimaryEntities::Polygon<Point2D> polygon;
    Outputs::Route route; // -> polyline

    QFutureWatcher<std::vector<std::vector<double>>>* futureWatcher_;
    QFutureWatcher<double>* futureWatcherTime_;
    QLabel* loadingLabel_;

    Matrix3DView* view_;
};
} // namespace Widgets
