#pragma once

#include <QtConcurrent/qtconcurrentrun.h>
#include <QtWidgets>

#include <Project/Operations/UtilityOperations.hpp>
#include <Project/Widgets/Matrix3DView.hpp>

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
        futureWatcher_ = new QFutureWatcher<void>(this);
        connect(futureWatcher_, &QFutureWatcherBase::finished, this, &MatrixViewWidget::onMatrixReady);
        connect(button, &QPushButton::clicked, this, &MatrixViewWidget::startAsyncCalculationMatrix);
    }
signals:
    void sendMessage(const QString& message, size_t type);
private slots:
    void onMatrixReady()
    {
        loadingLabel_->hide();

        auto matrix_ = indicators.getObservationDensity(); //futureWatcher_->result();

        if (matrix_.empty())
        {
            emit sendMessage("Матрица не построена. empty", 1);
            return;
        }
        auto stats = indicators.timeStatistics(polygon, route, radius);

        view_->setTimeStatistics(stats);
        view_->setMatrix(matrix_);
    }

    void startAsyncCalculationMatrix()
    {
        auto func = "Расчет матрицы::";
        view_->setWait();

        if (!checkFunc(func))
            return;

        loadingLabel_->show();

        view_->clear();

        QFuture<void> future = QtConcurrent::run([=]() {
            indicators = Entities::EfficiencyIndicators{100};
            indicators.calculateObservationDensity(polygon, route, radius);
        });

        futureWatcher_->setFuture(future);
    }

    bool checkFunc(const char* func)
    {
        if (route.velocities.empty())
        {
            emit sendMessage(QString("%1%2").arg(func).arg("velocities.empty()"), 1);
            return false;
        }
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

    QFutureWatcher<void>* futureWatcher_;

    QLabel* loadingLabel_;

    Entities::EfficiencyIndicators indicators{100};

    Matrix3DView* view_;
};
} // namespace Widgets
