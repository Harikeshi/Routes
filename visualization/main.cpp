#include <QApplication>

//#include <Project/MainWindow.hpp>
//
//int main(int argc, char* argv[])
//{
//    QApplication app(argc, argv);
//
//    MainWindow window;
//    window.resize(800, 800);
//
//    window.show();
//    return app.exec();
//}

/*! TODO:
начало разворота в крайней точке отрезка.
 1. расчет расстояния необходимого для сброса скорости ( это же относится и к циркуляции при поворотах в принципе);
 2. расчет дуги разворота, пока предположим, по константному радиусу циркуляции, по факту можно радиусов варьировать относительно скорости движения и угловой скорости;
 3. возврат в траекторию движения расчет требуемого расстояния для набора скорости;
 4. расчет дуги выхода на траекторию движения.
*/
/*!
    Расчет Радиуса циркуляции.
    L — длина судна (м)
    B — ширина судна (м)
    T — осадка (м)
    A_r — площадь руля (м²)
    delta — угол отклонения руля (°)
    V — скорость судна (м/с)
    w - угловая скорость (рад/с)
    O - угол, на который требуется повернуть (рад)
    R - радиус циркуляции (м)

    R = K * L / tan(delta);
    Через угловую скорость:
    w = V / R;
    t = O / w = O * R / V;

    Пример:
    L =100 м
    V = 5 м/c
    delta = 35°
    O = 90° = M_PI / 2
    1. Радиус Циркуляции
    R = 2 * 100 / tan(35) = 285.7 м
    w = V / R = 5/285.7 = 0.0175 рад/с
    t = O / w = (M_PI/2) / w = 89.7 с
 */
/*!

    a. Углы поворота до 90;
    b. Углы поворота до 120;
    c. Разворот;
    - Разделить по действиям;
    - Промоделировать каждое действие;
    - Соединить в единое, внедрить в схему.

   1. по длине участка определяем скорость, на которую можем сбросить скорость;
   2. Определяем радиус дуги, рассчитываем какую скорость можем под этот радиус
   3. Если все удовлетворяет

 */
#include "figures/Path.hpp"
#include "figures/figures.hpp"

Path calculate(const std::vector<QPointF> points, const std::vector<double> velocities) {
    if (points.size() < 2)
        throw std::runtime_error("Длина пути не может быть меньше двух точек.");

    Path path; // Общий путь

    if (points.size() == 2)
        return path;

    QPointF A = points[0];
    QPointF B = points[1];
    for (size_t i = 2, j = 0; i < points.size() && j < velocities.size(); ++i, ++j) {
        // 1. вычислить угол поворота (угол между отрезками)
        auto turn = geometry::angle(A, B, points[i]);
        std::cout << "Угол поворота: " << turn << std::endl;

        // TODO: Точки A и B вывод из предыдущего решения
        // Точка A следующая будет C предыдущая, а B следующую высчитываем в фигуре.

        if (std::fabs(turn) == 0) {
            // figure Разворот
            path.add(figure2L(A, B, points[i], velocities[j], velocities[j + 1]));
        } else if (std::fabs(turn) >= M_PI / 2) {
            // figure Тупой угол
            // Общий api
            path.add(figure0(A, B, points[i], velocities[j], velocities[j + 1]));
        } else if (std::fabs(turn) < M_PI / 2) {
            // figure Острый угол
            path.add(figure1A0(A, B, points[i], velocities[j], velocities[j + 1]));
        } else {
        }
    }
    path.add(Segment{A, B, velocities.at(points.size() - 1)});

    return path;
}

#include "figures/Segment.hpp"
#include <QPainter>
#include <QWidget>

class DrawWidget : public QWidget {
    Q_OBJECT

public:
    DrawWidget(QWidget *parent = nullptr) : QWidget{parent} {
    }

    void paintEvent(QPaintEvent *event) {
        QPainter painter(this);

        painter.scale(0.2, 0.2);
        painter.translate(width() / 2.0, height() / 3.0);
        painter.setRenderHint(QPainter::Antialiasing); // Более плавная отрисовка, но наложение линий

        QPen pen(Qt::red, 3);
        pen.setStyle(Qt::DotLine);
        painter.setPen(pen);

        for (size_t i = 0; i < points.size() - 1; ++i) {
            painter.drawLine(points[i], points[i + 1]);
        }

        // Path
        painter.setPen(QPen(Qt::blue, 2));
        _path.draw(painter);
    }

    void setSegments(const std::vector<QPointF> data) {
        points = data;
    }

    void setPath(Path path) {
        _path = path;
    }

private:
    Path _path;
    std::vector<QPointF> points;
};

int main(int argc, char *argv[]) {
    QPointF A{100, 100};
    QPointF B{100, 1000}; //! Разворот при маленьком галсе
    QPointF C{100, 100};

    auto _A = A;
    auto _B = B;
    double speed1 = 6;
    double speed2 = 6;

    std::vector<QPointF> points{
        {0, 0}, {1000, 500}, {2000, 200}, {3000, 1000}, {4500, 700}, {5000, 1400}
    }; //, {5500, 1000}};
    std::vector<double> velocities(20, 6);

    auto path = calculate(points, velocities);

    //    Path path = figure0(A, B, C, speed1, speed2);
    // Path path = figure2L(A, B, C, speed1, speed2);

    //    return 0;
    QApplication app(argc, argv);
    DrawWidget window;
    window.resize(2000, 2000);
    window.setSegments(points);
    window.setPath(path);
    window.show();
    return app.exec();
}

#include "main.moc"
