// #include <QApplication>
//
// #include <Project/MainWindow.hpp>
//
// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);
//
//     // Widgets::SceneWidget window;
//     MainWindow window;
//     window.resize(800, 800);
//
//     window.show();
//     return app.exec();
// }

#include <fstream>
#include <QVector>

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
#include <limits>

#include <QtMath>

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

        surface->axisX()->setTitle("X");
        surface->axisY()->setTitle("Z (value)");
        surface->axisZ()->setTitle("Y");
        surface->axisX()->setTitleVisible(true);
        surface->axisY()->setTitleVisible(true);
        surface->axisZ()->setTitleVisible(true);

        surface->axisX()->setLabelFormat("%.1f");
        surface->axisY()->setLabelFormat("%.2f");
        surface->axisZ()->setLabelFormat("%.1f");

        const int rows = M.size();
        const int cols = rows ? M[0].size() : 0;
        surface->axisX()->setRange(0.0, std::max(0, cols - 1));
        surface->axisZ()->setRange(0.0, std::max(0, rows - 1));

        surface->setReflection(false); //!

        surface->activeTheme()->setType(Q3DTheme::ThemeQt);
        surface->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftHigh); // todo: самый плавный
        surface->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetIsometricRightHigh);

        setWindowTitle("Visualization");
    }
};

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    auto M = MatrixSerializer::load("d:/3d_examples/density_rectangle_shift.txt");

    SurfaceWidget w(M);
    w.resize(900, 600);
    w.show();

    return app.exec();
}

#include "main.moc"
