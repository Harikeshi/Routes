#include <QApplication>

#include <Project/MainWindow.hpp>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Widgets::SceneWidget window;
     MainWindow window;
    window.resize(800, 800);

    window.show();
    return app.exec();
}

// // #include <fstream>

// // class MatrixSerializer
// // {
// // private:
// //     static QVector<float> split(const std::string& line)
// //     {
// //         QVector<float> result;
// //         std::string number;
// //         for (size_t i = 0; i < line.size(); ++i)
// //         {
// //             if (line[i] != ' ')
// //             {
// //                 number.push_back(line[i]);
// //             }
// //             else
// //             {
// //                 result.push_back(std::stoi(number));
// //                 number.clear();
// //             }
// //         }
// //         return result;
// //     }

// // public:
// //     static void save(const QVector<QVector<float>>& m, const std::string& path)
// //     {
// //         std::ofstream file(path);
// //         if (!file.is_open())
// //         {
// //             throw std::runtime_error("");
// //         }
// //         for (const auto& row : m)
// //         {
// //             for (const auto& column : row)
// //             {
// //                 file << column << " ";
// //             }
// //             file << std::endl;
// //         }
// //         file.close();
// //     }

// //     static QVector<QVector<float>> load(const std::string& path)
// //     {
// //         std::ifstream file(path);
// //         if (!file.is_open())
// //         {
// //             throw std::runtime_error("");
// //         }
// //         QVector<QVector<float>> result;
// //         std::string line;

// //         while (std::getline(file, line))
// //         {
// //             QVector<float> row = split(line);
// //             result.push_back(row);
// //         }
// //         file.close();
// //         return result;
// //     }
// // };
// // #include "MatrixViewWidget.hpp"

// // int main(int argc, char** argv)
// // {
// //     // auto route = routeFromJson("d:/3d_examples/report_shift_rectangle.json");

// //     auto route = MatrixWidget::routeFromJson("e:/visualization/jsons/result.json");

// //     // auto radius = radiusFromJson("d:/3d_examples/request_rectangle.json");

// //     double step = 100;
// //     double gridW = 100;
// //     double gridH = 50;

// //     QApplication app(argc, argv);
// //     MatrixWidget::MatrixViewWidget w;
// //     w.resize(900, 600);
// //     w.show();
// //     w.setMatrix(route);
// //     return app.exec();
// // }

// // #include "main.moc"
