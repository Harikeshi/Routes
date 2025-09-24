#include <QApplication>

//#include <Project/MainWindow.hpp>

//int main(int argc, char* argv[])
//{
//    QApplication app(argc, argv);

//    //    Widgets::SceneWidget window;
//    MainWindow window;
//    window.resize(800, 800);

//    window.show();
//    return app.exec();
//}


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

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace Operations {

class MatrixSerializer
{
private:
    static std::vector<int> split(const std::string& line)
    {
        std::vector<int> result;
        std::string number;
        for (size_t i = 0; i < line.size(); ++i)
        {
            if (line[i] != ' ')
            {
                number.push_back(line[i]);
            }
            else
            {
                result.push_back(std::stoi(number));
                number.clear();
            }
        }
        return result;
    }

public:
    static void save(const std::vector<std::vector<int>>& m, const std::string& path)
    {
        std::ofstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error("");
        }
        for (const auto& row : m)
        {
            for (const auto& column : row)
            {
                file << column << " ";
            }
            file << std::endl;
        }
        file.close();
    }

    static std::vector<std::vector<int>> load(const std::string& path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error("");
        }
        std::vector<std::vector<int>> result;
        std::string line;

        while (std::getline(file, line))
        {
            std::vector<int> row = split(line);
            result.push_back(row);
        }
        file.close();
        return result;
    }
};

} // namespace Operations


 using namespace QtDataVisualization;

 static QSurfaceDataProxy* buildProxy(const QVector<QVector<float>>& M,
                                      float x0 = 0.f,
                                      float dx = 1.f,
                                      float y0 = 0.f,
                                      float dy = 1.f)
 {
     const int rows = M.size();
     const int cols = rows ? M[0].size() : 0;

     auto* proxy = new QSurfaceDataProxy();
     auto* dataArray = new QSurfaceDataArray;
     dataArray->reserve(rows);

     for (int r = 0; r < rows; ++r)
     {
         auto* row = new QSurfaceDataRow(cols);
         const float y = y0 + r * dy;
         for (int c = 0; c < cols; ++c)
         {
             const float x = x0 + c * dx;
             (*row)[c].setPosition(QVector3D(x, M[r][c], y));
         }
         dataArray->push_back(row);
     }
     proxy->resetArray(dataArray);
     return proxy;
 }

 class SurfaceWidget : public QWidget
 {
     Q_OBJECT
 public:
     explicit SurfaceWidget(const QVector<QVector<float>>& M,
                            QWidget* parent = nullptr)
         : QWidget(parent)
     {
         // Контейнер для 3D-графа
         auto* surface = new Q3DSurface();
         auto* container = QWidget::createWindowContainer(surface, this);

         auto* layout = new QHBoxLayout(this);
         layout->setContentsMargins(0, 0, 0, 0);
         layout->addWidget(container);

         // Данные
         auto* proxy = buildProxy(M);
         auto* series = new QSurface3DSeries(proxy);
         series->setDrawMode(QSurface3DSeries::DrawSurface);
         series->setFlatShadingEnabled(false); // плавные нормали/освещение

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
         for (const auto& row : M)
             for (float v : row)
             {
                 vmin = std::min(vmin, v);
                 vmax = std::max(vmax, v);
             }
         if (!std::isfinite(vmin) || !std::isfinite(vmax) || vmin == vmax)
         {
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

         // Визуальные настройки и камера
         surface->activeTheme()->setType(Q3DTheme::ThemeQt);
         surface->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftHigh);
         surface->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetIsometricRightHigh);

         // Подсказка: вращение/зум — мышью (ЛКМ — вращение, колесо — зум, ПКМ — панорама)
         setWindowTitle("3D Surface from matrix (Qt Data Visualization)");
     }
 };

 int main(int argc, char** argv)
 {
     QApplication app(argc, argv);

     // Пример: синтетическая матрица 2D (X, Y) -> Z
     const int W = 60;
     const int H = 40;
     QVector<QVector<float>> M(H, QVector<float>(W));
     for (int y = 0; y < H; ++y)
     {
         for (int x = 0; x < W; ++x)
         {
             float xf = float(x) * 0.15f;
             float yf = float(y) * 0.20f;
             M[y][x] = std::sin(xf) * std::cos(yf) * 5.0f + std::exp(-0.02f * ((x - 30) * (x - 30) + (y - 20) * (y - 20))) * 10.0f;
         }
     }

     SurfaceWidget w(M);
     w.resize(900, 600);
     w.show();

     return app.exec();
 }

 #include "main.moc"
