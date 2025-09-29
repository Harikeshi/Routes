#pragma once

#include <QVector>

#include <QtConcurrent/qtconcurrentrun.h>
#include <QtDataVisualization/Q3DBars>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QBar3DSeries>
#include <QtDataVisualization/QBarDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtWidgets>
#include <cmath>
#include <vector>

#include <Project/Models/Report.hpp>
#include <Project/Models/Request.hpp>

namespace Widgets {
using namespace QtDataVisualization;

inline QVector<QPointF> routeFromReport(const Models::Report& report)
{
    QVector<QPointF> route;

    for (auto const segment : report.routes()[0].getSegments())
        route.push_back(segment.getStart());

    route.push_back(report.routes()[0].getSegments().last().getEnd());

    return route;
}

inline double radiusFromRequest(const Models::Request& request)
{
    return request.getShip().detection_range;
}

inline QVector<QPointF> routeFromJson(const QString& path)
{
    auto json = Operations::jsonFromFile(path);

    Models::Report report;
    report.fromJson(json);

    return routeFromReport(report);
}

inline double radiusFromJson(const QString& path)
{
    auto json = Operations::jsonFromFile(path);

    // todo: json["ship"]["detection_range"].get<double>()
    Models::Request request;
    request.fromJson(json);

    return radiusFromRequest(request);
}

// ---------- Circle + SpatialGrid ----------
struct Circle
{
    QPointF center;
    qreal radius;
    QRectF bounds;

    Circle(const QPointF& c, qreal r)
        : center(c), radius(r), bounds(c.x() - r, c.y() - r, 2 * r, 2 * r)
    {
    }

    bool contains(const QPointF& pt) const
    {
        if (!bounds.contains(pt))
            return false;
        qreal dx = pt.x() - center.x();
        qreal dy = pt.y() - center.y();
        return dx * dx + dy * dy <= radius * radius;
    }
};

class SpatialGrid
{
public:
    SpatialGrid(const QRectF& bounds, int cols, int rows)
        : bounds_(bounds), cols_(cols), rows_(rows)
    {
        grid_.resize(rows_);
        for (auto& row : grid_)
            row.resize(cols_);
    }

    void insert(const Circle& c, int index)
    {
        QRectF b = c.bounds;
        int x0 = clamp((b.left() - bounds_.left()) / bounds_.width() * cols_, 0, cols_ - 1);
        int x1 = clamp((b.right() - bounds_.left()) / bounds_.width() * cols_, 0, cols_ - 1);
        int y0 = clamp((b.top() - bounds_.top()) / bounds_.height() * rows_, 0, rows_ - 1);
        int y1 = clamp((b.bottom() - bounds_.top()) / bounds_.height() * rows_, 0, rows_ - 1);
        for (int y = y0; y <= y1; ++y)
            for (int x = x0; x <= x1; ++x)
                grid_[y][x].push_back(index);
    }

    std::vector<int> query(const QPointF& pt) const
    {
        int x = clamp((pt.x() - bounds_.left()) / bounds_.width() * cols_, 0, cols_ - 1);
        int y = clamp((pt.y() - bounds_.top()) / bounds_.height() * rows_, 0, rows_ - 1);
        return grid_[y][x];
    }

private:
    QRectF bounds_;
    int cols_, rows_;
    std::vector<std::vector<std::vector<int>>> grid_;

    static int clamp(double v, int min, int max)
    {
        return std::max(min, std::min(static_cast<int>(v), max));
    }
};

inline std::vector<std::vector<double>> buildMatrix(const QVector<QPointF>& polyline,
                                                    double step,
                                                    double R,
                                                    int gridW,
                                                    int gridH,
                                                    int samplesPerCell)
{
    // bbox
    QRectF bbox(polyline[0], QSizeF(1, 1));
    for (auto& pt : polyline)
        bbox = bbox.united(QRectF(pt.x() - R, pt.y() - R, 2 * R, 2 * R));
    // bboxOut = bbox;

    // spatial grid
    int gridCols = std::max(1, static_cast<int>(bbox.width() / (2.5 * R)));
    int gridRows = std::max(1, static_cast<int>(bbox.height() / (2.5 * R)));
    SpatialGrid grid(bbox, gridCols, gridRows);

    std::vector<QPointF> centers;
    for (size_t i = 0; i + 1 < polyline.size(); ++i)
    {
        QPointF A = polyline[i];
        QPointF B = polyline[i + 1];
        QPointF v = B - A;
        qreal L = std::hypot(v.x(), v.y());
        if (L < 1e-6)
        {
            centers.push_back(A);
            continue;
        }
        QPointF dir(v.x() / L, v.y() / L);
        int nSteps = static_cast<int>(std::floor(L / step));
        centers.push_back(A);
        for (int s = 1; s <= nSteps; ++s)
            centers.push_back(A + dir * (s * step));
        centers.push_back(B);
    }

    for (size_t i = 0; i < centers.size(); ++i)
        grid.insert(Circle(centers[i], R), static_cast<int>(i));

    std::vector<std::vector<double>> matrix(gridH, std::vector<double>(gridW, 0.0));
    double cellW = bbox.width() / gridW;
    double cellH = bbox.height() / gridH;
    double stepX = cellW / samplesPerCell;
    double stepY = cellH / samplesPerCell;

    for (int y = 0; y < gridH; ++y)
    {
        for (int x = 0; x < gridW; ++x)
        {
            double sum = 0.0;
            for (int sy = 0; sy < samplesPerCell; ++sy)
            {
                for (int sx = 0; sx < samplesPerCell; ++sx)
                {
                    QPointF pt(bbox.left() + x * cellW + (sx + 0.5) * stepX,
                               bbox.top() + y * cellH + (sy + 0.5) * stepY);
                    int count = 0;
                    for (int idx : grid.query(pt))
                    {
                        QPointF c = centers[idx];
                        qreal dx = pt.x() - c.x();
                        qreal dy = pt.y() - c.y();
                        if (dx * dx + dy * dy <= R * R)
                            ++count;
                    }
                    sum += count;
                }
            }
            // TODO: Убрать деление
            matrix[y][x] = sum / (samplesPerCell * samplesPerCell);
        }
    }
    return matrix;
}

class Matrix3DView : public QWidget
{
    Q_OBJECT

public:
    //! Перезагружает отображение.
    void setMatrix(const std::vector<std::vector<double>>& matrix)
    {
        if (!surface_->seriesList().isEmpty())
            surface_->removeSeries(surface_->seriesList().first());

        if (!bars_->seriesList().isEmpty())
            bars_->removeSeries(bars_->seriesList().first());

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

        QBarDataArray* barArray = new QBarDataArray;
        barArray->reserve(matrix.size());
        for (int y = 0; y < (int)matrix.size(); ++y)
        {
            QBarDataRow* row = new QBarDataRow(matrix[y].size());
            for (int x = 0; x < (int)matrix[y].size(); ++x)
                (*row)[x].setValue(matrix[y][x]);
            *barArray << row;
        }
        QBarDataProxy* barProxy = new QBarDataProxy();
        barProxy->resetArray(barArray);
        QBar3DSeries* barSeries = new QBar3DSeries(barProxy);

        bars_->addSeries(barSeries);
    }

    void setSurface(const std::vector<std::vector<double>>& matrix)
    {
        QSurfaceDataArray* dataArray = new QSurfaceDataArray;
        dataArray->reserve(matrix.size());
        for (size_t y = 0; y < matrix.size(); ++y)
        {
            QSurfaceDataRow* row = new QSurfaceDataRow(matrix[y].size());
            for (size_t x = 0; x < matrix[y].size(); ++x)
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
    }

    void setBars(const std::vector<std::vector<double>>& matrix)
    {
        QBarDataArray* barArray = new QBarDataArray;
        barArray->reserve(matrix.size());
        for (size_t y = 0; y < matrix.size(); ++y)
        {
            QBarDataRow* row = new QBarDataRow(matrix[y].size());
            for (size_t x = 0; x < matrix[y].size(); ++x)
                (*row)[x].setValue(matrix[y][x]);
            *barArray << row;
        }
        QBarDataProxy* barProxy = new QBarDataProxy();
        barProxy->resetArray(barArray);
        QBar3DSeries* barSeries = new QBar3DSeries(barProxy);

        bars_->addSeries(barSeries);
    }

    Matrix3DView(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        stacked_ = new QStackedLayout(this);

        // --- Surface ---
        surface_ = new Q3DSurface();
        QWidget* surfContainer = QWidget::createWindowContainer(surface_);
        stacked_->addWidget(surfContainer);

        // --- Bars ---
        bars_ = new Q3DBars();
        QWidget* barsContainer = QWidget::createWindowContainer(bars_);
        stacked_->addWidget(barsContainer);

        bars_->activeTheme()->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
        bars_->activeTheme()->setBaseColors({Qt::blue, Qt::green, Qt::red});
    }

public slots:
    void showSurface()
    {
        stacked_->setCurrentIndex(0);
    }
    void showBars()
    {
        stacked_->setCurrentIndex(1);
    }

private:
    QStackedLayout* stacked_;
    Q3DSurface* surface_;
    Q3DBars* bars_;
};

class MatrixViewWidget : public QWidget
{
    Q_OBJECT

public:
    void loadReport(const Models::Report& report)
    {
        setMatrix(routeFromReport(report));
    }

    void loadRequest(const Models::Request& request)
    {
        radiusSpin_->setValue(radiusFromRequest(request));
    }

    void setMatrix(const std::vector<std::vector<double>>& matrix)
    {
        view_->setMatrix(matrix);
    }

    void setMatrix(const QVector<QPointF>& polyline)
    {
        polyline_ = polyline;
        auto matrix = buildMatrix(polyline_, stepSpin_->value(), radiusSpin_->value(), gridWSpin_->value(), gridHSpin_->value(), qSpin_->value());

        //view_->setMatrix(matrix);
    }

    MatrixViewWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QRectF bbox;

        auto layout = new QVBoxLayout(this);

        // --- Верхняя панель управления ---
        auto topControls = new QHBoxLayout;
        radiusSpin_ = new QSpinBox;
        radiusSpin_->setRange(1, 10000);
        radiusSpin_->setValue(static_cast<int>(5000));
        topControls->addWidget(new QLabel("R:"));
        topControls->addWidget(radiusSpin_);

        gridWSpin_ = new QSpinBox;
        gridWSpin_->setRange(10, 500);
        gridWSpin_->setValue(200);
        topControls->addWidget(new QLabel("W:"));
        topControls->addWidget(gridWSpin_);

        gridHSpin_ = new QSpinBox;
        gridHSpin_->setRange(10, 500);
        gridHSpin_->setValue(100);
        topControls->addWidget(new QLabel("H:"));
        topControls->addWidget(gridHSpin_);

        stepSpin_ = new QSpinBox;
        stepSpin_->setRange(10, 1000);
        stepSpin_->setValue(100);
        topControls->addWidget(new QLabel("Step:"));
        topControls->addWidget(stepSpin_);

        qSpin_ = new QSpinBox;
        qSpin_->setRange(1, 4);
        qSpin_->setValue(1);
        topControls->addWidget(new QLabel("Q:"));
        topControls->addWidget(qSpin_);

        layout->addLayout(topControls);

        // --- Надпись загрузки и вьюшка ---
        loadingLabel_ = new QLabel("Loading…");
        loadingLabel_->setAlignment(Qt::AlignCenter);
        loadingLabel_->hide();
        layout->addWidget(loadingLabel_);

        view_ = new Matrix3DView();
        layout->addWidget(view_, 1);

        // --- Кнопки управления ---
        auto buttonsLayout = new QHBoxLayout;
        auto recalcBtn = new QPushButton("calc");
        auto btnSurf = new QPushButton("sur");
        auto btnBars = new QPushButton("bar");

        buttonsLayout->addWidget(recalcBtn);
        buttonsLayout->addWidget(btnSurf);
        buttonsLayout->addWidget(btnBars);

        layout->addLayout(buttonsLayout);

        connect(btnSurf, &QPushButton::clicked, view_, &Matrix3DView::showSurface);
        connect(btnBars, &QPushButton::clicked, view_, &Matrix3DView::showBars);

        //TODO: future перевести на int
        futureWatcher_ = new QFutureWatcher<std::vector<std::vector<double>>>(this);
        connect(futureWatcher_, &QFutureWatcherBase::finished, this, &MatrixViewWidget::onMatrixReady);
        connect(recalcBtn, &QPushButton::clicked, this, &MatrixViewWidget::startAsyncCalculation);
    }

private slots:
    void onMatrixReady()
    {
        auto matrix_ = futureWatcher_->result();
        loadingLabel_->hide();

        view_->setMatrix(matrix_);
    }

    void startAsyncCalculation()
    {
        if (polyline_.isEmpty())
        {
            qDebug() << "polyline is empty";
            return;
        }
        int step = stepSpin_->value();
        double radius = radiusSpin_->value();
        double gridW = gridWSpin_->value();
        double gridH = gridHSpin_->value();
        int q = qSpin_->value();
        loadingLabel_->show();
        // matrix_.clear();
        QFuture<std::vector<std::vector<double>>> future = QtConcurrent::run([=]() {
            return buildMatrix(polyline_, step, radius, gridW, gridH, q);
        });
        futureWatcher_->setFuture(future);
    }

private:
    QSpinBox *radiusSpin_, *gridWSpin_, *gridHSpin_, *stepSpin_, *qSpin_;

    QFutureWatcher<std::vector<std::vector<double>>>* futureWatcher_;
    QLabel* loadingLabel_;

    QVector<QPointF> polyline_;
    Matrix3DView* view_;
};
} // namespace MatrixWidget
