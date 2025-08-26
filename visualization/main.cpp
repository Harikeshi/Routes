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

void calculate(const std::vector<QPointF> points, const std::vector<double> velocities)
{
    if (points.size() < 2)
        throw std::runtime_error("Длина пути не может быть меньше двух точек.");
    if (points.size() == 2)
        return;

    Path path; // Общий путь
    QPointF A = points[0];
    QPointF B = points[1];
    for (size_t i = 2, j = 0; i < points.size() && j < velocities.size(); ++i, ++j)
    {
        // 1. вычислить угол поворота (угол между отрезками)
        auto turn = geometry::angle(A, B, points[i]);
        std::cout << "Угол поворота: " << turn << std::endl;

        // TODO: Точки A и B вывод из предыдущего решения
        // Точка A следующая будет C предыдущая, а B следующую высчитываем в фигуре.
        if (turn >= M_PI / 2)
        {
            // figure Тупой угол
            // Общий api
            //path.add(figure(&A, &B, C, speed1, speed2));
        }
        else if (turn < M_PI / 2)
        {
            // figure Острый угол
            path.add(figure1A(A, B, points[i], velocities[j], velocities[j + 1]));
        }
        else
        {
            // figure Разворот
            //            path.add(figure2L(A, B, points[i], velocities[j], velocities[j + 1]));
        }
    }
}

#include "figures/Segment.hpp"
#include <QPainter>
#include <QWidget>

class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    DrawWidget(QWidget* parent = nullptr) : QWidget{parent}
    {
    }

    void paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);

        painter.scale(0.5, 0.5);
        painter.translate(width() / 2.0, height() / 3.0);
        painter.setRenderHint(QPainter::Antialiasing); // Более плавная отрисовка, но наложение линий

        QPen pen(Qt::darkYellow);
        pen.setStyle(Qt::DotLine);
        painter.setPen(pen);

        painter.drawLine(segments.first.start(), segments.first.end());
        painter.drawLine(segments.second.start(), segments.second.end());

        // Path
        painter.setPen(QPen(Qt::blue, 2));
        _path.draw(painter);
    }

    void setSegments(const std::vector<std::pair<QPointF, double>> data)
    {
        segments.first = Segment(data[0].first, data[1].first, data[0].second);
        segments.second = Segment(data[1].first, data[2].first, data[1].second);
    }

    void setPath(Path path)
    {
        _path = path;
    }

private:
    Path _path;
    std::pair<Segment, Segment> segments;
};

int main(int argc, char* argv[])
{
    QPointF A{100, 100};
    QPointF B{100, 1000};
    QPointF C{-500, 100};
    double speed1 = 6;
    double speed2 = 6;

    //    Path path = figure2L(A, B, C, speed1, speed2);
    auto radius = 250;
    Path path = figure1A(A, B, C, speed1, speed2, radius);

    //    return 0;
    QApplication app(argc, argv);
    DrawWidget window;
    window.resize(2000, 2000);
    window.setSegments(std::vector<std::pair<QPointF, double>>{{A, speed1}, {B, speed1}, {C, speed1}});
    window.setPath(path);
    window.show();
    return app.exec();
}

#include "main.moc"
