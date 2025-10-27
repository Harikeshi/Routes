#pragma once

#include <QVector>

#include <QtConcurrent/qtconcurrentrun.h>
#include <QtDataVisualization/Q3DBars>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QBar3DSeries>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtWidgets>
#include <vector>

#include <Project/Models/Report.hpp>
#include <Project/Models/Request.hpp>

#include <Task/Entities/Efficiency.hpp>
#include <Task/Operations/JsonOperations.hpp>

namespace Widgets {
using namespace QtDataVisualization;

inline void show_(const PrimaryEntities::Polygon<Point2D>& polygon)
{
    //! Полигон
    std::cout << "outer: [";
    for (const auto& point : polygon.outer())
        std::cout << "[" << point.getX() << ", " << point.getY() << "], ";
    std::cout << "]" << std::endl;
    std::cout << "inners: [";
    for (const auto& inner : polygon.inners())
    {
        for (const auto& point : inner)
            std::cout << "[" << point.getX() << ", " << point.getY() << "], ";
        std::cout << "]" << std::endl;
    }
    std::cout << "]" << std::endl;
}

/*!
 * \brief routeFromReport
 * \param report
 * \return
 */
inline Outputs::Route routeFromReport(const Models::Report& report)
{
    std::vector<Point2D> route;
    std::vector<double> vels;

    for (auto const segment : report.routes()[0].getSegments())
    {
        route.push_back(Point2D{segment.getStart().x(), segment.getStart().y()});
        vels.push_back(segment.baseSpeed);
    }

    route.push_back(Point2D{report.routes()[0].getSegments().last().getEnd().x(), report.routes()[0].getSegments().last().getEnd().y()});

    return Outputs::Route{route, vels};
}

/*!
 * \brief routeFromReport
 * \param report
 * \return
 */
inline PrimaryEntities::Polygon<Point2D> polygonFromRequest(const Models::Request& request)
{
    if (request.getPerimeter().getRings().isEmpty())
        throw std::runtime_error("Perimeter.Rings.isEmpty()");

    PrimaryEntities::Polygon<Point2D> result;
    Schemes::Search::InRegion::Input input;
    auto json = request.toNJson();

    try
    {
        input.fromJson(json);
        result = input.region.getPolygon();
    }
    catch (const std::exception& e)
    {
        throw;
    }

    return result;
}

/*!
 * \brief radiusFromRequest
 * \param request
 * \return
 */
inline double radiusFromRequest(const Models::Request& request)
{
    return request.ship.detection_range;
}

inline double velocityFromRequest(const Models::Request& request)
{
    return request.ship.search_velocity;
}

#include <Task/Entities/Efficiency.hpp>

/*!
 * \brief buildMatrix
 * \param polygon
 * \param route
 * \param detRange
 * \return
 */
inline std::vector<std::vector<double>> buildMatrix(PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange)
{
    // return std::vector<std::vector<double>>{{1, 1, 1}, {2, 2, 2}, {1, 1, 1}};
    Entities::EfficiencyIndicators indicators(100);
    // bbox
    indicators.calculateObservationDensity(polygon, route, detRange);

    return indicators.getObservationDensity();
}

/*!
 * \brief buildAverageTime
 * \param polygon
 * \param route
 * \param detRange
 * \param searchVelocity
 * \return
 */
inline double buildAverageTime(PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange, const double& searchVelocity)
{
    // return 10;
    double result;

    Entities::EfficiencyIndicators indicators(100);

    return indicators.averageTime(polygon, route, detRange);
}

/*!
 * \brief The Matrix3DView class
 */
class Matrix3DView : public QWidget
{
    Q_OBJECT

public:
    void clear()
    {
        if (!surface_->seriesList().isEmpty())
            surface_->removeSeries(surface_->seriesList().first());
    }
    //! Перезагружает отображение.
    void setMatrix(const std::vector<std::vector<double>>& matrix)
    {
        if (matrix.empty())
        {
            qDebug() << "Матрица говна";
            return;
        }

        clear();

        QSurfaceDataArray* dataArray = new QSurfaceDataArray;
        dataArray->reserve(matrix.size());
        for (int y = 0; y < (int)matrix.size(); ++y)
        {
            QSurfaceDataRow* row = new QSurfaceDataRow(matrix[y].size());
            for (int x = 0; x < (int)matrix[y].size(); ++x)
                (*row)[x].setPosition(QVector3D(x, matrix[y][x], y));
            *dataArray << row;
        }
        QSurfaceDataProxy* proxy = new QSurfaceDataProxy();
        proxy->resetArray(dataArray);
        QSurface3DSeries* series = new QSurface3DSeries(proxy);
        series->setDrawMode(QSurface3DSeries::DrawSurface);
        series->setFlatShadingEnabled(false);

        QLinearGradient grad;
        grad.setColorAt(0.00, QColor("#313695"));
        grad.setColorAt(0.10, QColor("#4575b4"));
        grad.setColorAt(0.25, QColor("#74add1"));
        grad.setColorAt(0.40, QColor("#abd9e9"));
        grad.setColorAt(0.50, QColor("#e0f3f8"));
        grad.setColorAt(0.60, QColor("#fee090"));
        grad.setColorAt(0.75, QColor("#fdae61"));
        grad.setColorAt(0.90, QColor("#f46d43"));
        grad.setColorAt(1.00, QColor("#a50026"));
        series->setBaseGradient(grad);
        series->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
        surface_->addSeries(series);

        update();
    }

    Matrix3DView(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QWidget* stackContainer = new QWidget(this);

        stacked_ = new QStackedLayout(stackContainer);

        //! Надписи
        averageTime = new QLabel("<T> =");
        averageTime->setAlignment(Qt::AlignLeft);
        averageTime->setAttribute(Qt::WA_TransparentForMouseEvents);

        // --- Surface ---
        surface_ = new Q3DSurface();
        QWidget* surfContainer = QWidget::createWindowContainer(surface_);
        stacked_->addWidget(surfContainer);

        auto* layout = new QVBoxLayout(this);

        layout->addWidget(averageTime);
        layout->addWidget(stackContainer);
    }

public slots:
    void showSurface()
    {
        stacked_->setCurrentIndex(0);
    }

    void setAverageTime(double value)
    {
        averageTime->setText(QString("<T> = %1").arg(value));
    }

    void setWait()
    {
        averageTime->setText("-.-");
    }

private:
    QStackedLayout* stacked_;
    Q3DSurface* surface_;

    QLabel* averageTime;
};

/*!
 * \brief The MatrixViewWidget class
 */
class MatrixViewWidget : public QWidget
{
    Q_OBJECT

public:
    void loadReport(const Models::Report& report)
    {
        // clear()
        route = Outputs::Route{};

        route = routeFromReport(report);
    }

    void loadRequest(const Models::Request& request)
    {
        polygon.clear();
        polygon = polygonFromRequest(request);

        radius = radiusFromRequest(request);
        velocity = velocityFromRequest(request);
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
            return buildAverageTime(polygon, route, radius, velocity);
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
            return buildMatrix(polygon, route, radius);
        });
        futureWatcher_->setFuture(future);
    }
    bool checkFunc(const char* func)
    {
        if (route.points.empty())
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

        if (velocity == 0)
        {
            emit sendMessage(QString("%1%2").arg(func).arg("velocity == 0"), 1);
            return false;
        }

        return true;
    }

private:
    double radius{0};
    double velocity{0};

    PrimaryEntities::Polygon<Point2D> polygon;
    Outputs::Route route; // -> polyline

    QFutureWatcher<std::vector<std::vector<double>>>* futureWatcher_;
    QFutureWatcher<double>* futureWatcherTime_;
    QLabel* loadingLabel_;

    Matrix3DView* view_;
};
} // namespace Widgets
