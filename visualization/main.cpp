// #include <QApplication>
//
// #include <Project/MainWindow.hpp>
//
// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);
//
//     //    Widgets::SceneWidget window;
//     MainWindow window;
//     window.resize(800, 800);
//
//     window.show();
//     return app.exec();
// }


#include <fstream>
#include <QVector>

#include "Project/Models/Request.hpp"

class MatrixSerializer {
private:
    static QVector<float> split(const std::string &line) {
        QVector<float> result;
        std::string number;
        for (size_t i = 0; i < line.size(); ++i) {
            if (line[i] != ' ') {
                number.push_back(line[i]);
            } else {
                result.push_back(std::stoi(number));
                number.clear();
            }
        }
        return result;
    }

public:
    static void save(const QVector<QVector<float> > &m, const std::string &path) {
        std::ofstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("");
        }
        for (const auto &row: m) {
            for (const auto &column: row) {
                file << column << " ";
            }
            file << std::endl;
        }
        file.close();
    }

    static QVector<QVector<float> > load(const std::string &path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("");
        }
        QVector<QVector<float> > result;
        std::string line;

        while (std::getline(file, line)) {
            QVector<float> row = split(line);
            result.push_back(row);
        }
        file.close();
        return result;
    }
};


#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>

#include <QtDataVisualization/Q3DCamera>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QSurfaceDataProxy>

#include <QLinearGradient>
#include <QVector>
#include <algorithm>
#include <cmath>
#include <limits>

using namespace QtDataVisualization;

static QSurfaceDataProxy *buildProxy(const QVector<QVector<float> > &M,
                                     float x0 = 0.f,
                                     float dx = 1.f,
                                     float y0 = 0.f,
                                     float dy = 1.f) {
    const int rows = M.size();
    const int cols = rows ? M[0].size() : 0;

    auto *proxy = new QSurfaceDataProxy();
    auto *dataArray = new QSurfaceDataArray;
    dataArray->reserve(rows);

    for (int r = 0; r < rows; ++r) {
        auto *row = new QSurfaceDataRow(cols);
        const float y = y0 + r * dy;
        for (int c = 0; c < cols; ++c) {
            const float x = x0 + c * dx;
            (*row)[c].setPosition(QVector3D(x, M[r][c], y));
        }
        dataArray->push_back(row);
    }
    proxy->resetArray(dataArray);
    return proxy;
}

class SurfaceWidget : public QWidget {
    Q_OBJECT

public:
    explicit SurfaceWidget(const QVector<QVector<float> > &M,
                           QWidget *parent = nullptr)
        : QWidget(parent) {
        // Контейнер для 3D-графа
        auto *surface = new Q3DSurface();
        auto *container = QWidget::createWindowContainer(surface, this);

        auto *layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(container);

        // Данные
        auto *proxy = buildProxy(M);
        auto *series = new QSurface3DSeries(proxy);
        series->setDrawMode(QSurface3DSeries::DrawSurface);

        series->setFlatShadingEnabled(false); // плавные нормали/освещение todo: false

        // Градиент по высоте Z (ось Y в Q3DSurface)
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

        surface->addSeries(series);

        // Авто-диапазон по данным для оси Z (в терминах Q3DSurface это axisY)
        float vmin = std::numeric_limits<float>::infinity();
        float vmax = -std::numeric_limits<float>::infinity();
        for (const auto &row: M)
            for (float v: row) {
                vmin = std::min(vmin, v);
                vmax = std::max(vmax, v);
            }
        if (!std::isfinite(vmin) || !std::isfinite(vmax) || vmin == vmax) {
            vmin = 0.f;
            vmax = 1.f;
        }
        surface->axisY()->setRange(vmin, vmax);

        // Оси (X/Z — индексы матрицы, Y — высота)
        surface->axisX()->setTitle("X");
        surface->axisY()->setTitle("Z (value)");
        surface->axisZ()->setTitle("Y");
        surface->axisX()->setTitleVisible(true);
        surface->axisY()->setTitleVisible(true);
        surface->axisZ()->setTitleVisible(true);

        surface->axisX()->setLabelFormat("%.1f");
        surface->axisY()->setLabelFormat("%.2f");
        surface->axisZ()->setLabelFormat("%.1f");

        // Диапазоны X/Z по размеру матрицы
        const int rows = M.size();
        const int cols = rows ? M[0].size() : 0;
        surface->axisX()->setRange(0.0, std::max(0, cols - 1));
        surface->axisZ()->setRange(0.0, std::max(0, rows - 1));

        surface->setReflection(false); //!

        // Визуальные настройки и камера
        surface->activeTheme()->setType(Q3DTheme::ThemeQt);
        surface->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftHigh); // todo: самый плавный
        surface->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetIsometricRightHigh);

        // Подсказка: вращение/зум — мышью (ЛКМ — вращение, колесо — зум, ПКМ — панорама)
        setWindowTitle("3D Surface from matrix (Qt Data Visualization)");
    }
};

#include <QVector>
#include <Project/Scene/Capsules.hpp>
#include <Project/Models/Report.hpp>
#include <Project/Models/Request.hpp>
#include <Project/Operations/JsonOperations.hpp>

QVector<QPointF> routeFromJson(const QString &path) {
    QVector<QPointF> route;

    // 1. Прочитать из файла.
    auto json = Operations::jsonFromFile(path);

    // 2. Создать вектор
    Models::Report report;
    report.fromJson(json);
    for (auto const segment: report.routes()[0].getSegments())
        route.push_back(segment.getStart());

    route.push_back(report.routes()[0].getSegments().last().getEnd());

    return route;
}

double radiusFromJson(const QString &path) {
    auto json = Operations::jsonFromFile(path);

    Models::Request request;
    request.fromJson(json);

    return request.getShip().detection_range;
}

QPainterPath makeCapsule(const QPointF &a, const QPointF &b, qreal R) {
    QPainterPath path;
    if (a == b) {
        path.addEllipse(a, R, R);
        return path;
    }
    QPointF v = b - a;
    qreal L = std::hypot(v.x(), v.y());
    QPointF dir(v.x() / L, v.y() / L);
    QPointF n(-dir.y(), dir.x());

    QPointF aL = a + n * R;
    QPointF aR = a - n * R;
    QPointF bL = b + n * R;
    QPointF bR = b - n * R;

    path.moveTo(aL);
    path.lineTo(bL);
    path.lineTo(bR);
    path.lineTo(aR);
    path.closeSubpath();

    QRectF arcRectA(a.x() - R, a.y() - R, 2 * R, 2 * R);
    QRectF arcRectB(b.x() - R, b.y() - R, 2 * R, 2 * R);

    QPainterPath capA;
    capA.moveTo(aR);
    capA.arcTo(arcRectA, std::atan2(-(aR.y() - a.y()), aR.x() - a.x()) * 180 / M_PI, 180);
    QPainterPath capB;
    capB.moveTo(bL);
    capB.arcTo(arcRectB, std::atan2(-(bL.y() - b.y()), bL.x() - b.x()) * 180 / M_PI, 180);

    path = path.united(capA);
    path = path.united(capB);
    return path;
}

QVector<QPainterPath> buildCapsules(const QVector<QPointF> pts_, double R_) {
    QVector<QPainterPath> capsules_;
    // for (auto x: pts_) {
    //     qDebug() << x;
    // }
    for (size_t i = 0; i + 1 < pts_.size(); ++i) {
        // QPainterPath cap = Capsule::make(pts_[i], pts_[i + 1], R_);
        QPainterPath cap = makeCapsule(pts_[i], pts_[i + 1], R_);
        capsules_.push_back(cap);
    }

    return capsules_;
}

QVector<QVector<float> > buildMatrix(QVector<QPainterPath> capsules_, int gridW_, int gridH_, double width,
                                     double height, std::pair<double, double> mininumXY) {
    QVector<QVector<float> > matrix_(gridH_, QVector<float>(gridW_, 0));

    // Разница максимального и минимального x и y

    // [0,0] 800x600
    // double width = 80000;
    // double height = 60000;
    double left = mininumXY.first;
    double top = mininumXY.second;

    double cellW = width / gridW_;
    double cellH = height / gridH_;

    for (int y = 0; y < gridH_; ++y) {
        for (int x = 0; x < gridW_; ++x) {
            QPointF center(left + (x + 0.5) * cellW,
                           top + (y + 0.5) * cellH);
            int count = 0;
            for (auto &cap: capsules_) {
                if (cap.contains(center))
                    ++count;
            }
            matrix_[y][x] = count;
        }
    }

    return matrix_;
}

QVector<QVector<float> > testMatrix() {
    // Пример: синтетическая матрица (X, Y) -> Z
    const int W = 60;
    const int H = 40;
    QVector<QVector<float> > M(H, QVector<float>(W));
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            float xf = float(x) * 0.15f;
            float yf = float(y) * 0.20f;
            M[y][x] = std::sin(xf) * std::cos(yf) * 5.0f + std::exp(
                          -0.02f * ((x - 30) * (x - 30) + (y - 20) * (y - 20))) * 10.0f;
        }
    }

    return M;
}


#include <iostream>

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    //
    auto route = routeFromJson("d:/3d_examples/report_zigzag_rectangle.json");
    auto radius = radiusFromJson("d:/3d_examples/request_rectangle.json");

    double minX, minY, maxX, maxY;

    for (auto point: route) {
        if (point.x() < minX) {
            minX = point.x();
        }
        if (point.x() > maxX) {
            maxX = point.x();
        }
        if (point.y() < minY) {
            minY = point.y();
        }
        if (point.y() > maxY) {
            maxY = point.y();
        }
    }

    qDebug() << "minX: " << minX << ", maxX: " << maxX;
    qDebug() << "minY: " << minY << ", maxY: " << maxY;

    qDebug() << maxX - minX << ", " << maxY - minY;

    //
    qDebug() << radius;
    auto capsules = buildCapsules(route, radius);
    auto M = buildMatrix(capsules, 500, 250, (maxX - minX + 2 * radius), (maxY - minY + 2 * radius),
                         std::make_pair(minX - radius, minY - radius));
    std::cout << "OK" << std::endl;

    // auto M = MatrixSerializer::load("d:/3d_examples/density_rec_inners_shift.txt");

    SurfaceWidget w(M);
    w.resize(900, 600);
    w.show();

    return app.exec();
}

#include "main.moc"
