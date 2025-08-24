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

#include <QDebug>
#include <QPainter>
#include <cmath>

#include "figures/geometry.hpp"
#include "figures/Arc.hpp"

using namespace geometry;

enum turnType {
    left,
    right,
    UTurn
};


#include "config.hpp"
#include <QPointF>
#include <QtMath>
#include <iomanip>
#include <iostream>
#include <memory>

// Функция расчёта параметров циркуляции
void calculateTurningParameters(double speed_knots, double turn_angle_deg, double turn_radius_m) {
    double speed_mps = speed_knots * KNOT_TO_MPS;
    double angle_rad = turn_angle_deg * DEG_TO_RAD;

    double arc_length = turn_radius_m * angle_rad;
    double advance = turn_radius_m * sin(angle_rad);
    double transfer = turn_radius_m * (1 - cos(angle_rad));
    double time = arc_length / speed_mps;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Скорость: " << speed_knots << " узлов (" << speed_mps << " м/с)\n";
    std::cout << "Угол поворота: " << turn_angle_deg << "°\n";
    std::cout << "Радиус циркуляции: " << turn_radius_m << " м\n";
    std::cout << "Advance: " << advance << " м\n";
    std::cout << "Transfer: " << transfer << " м\n";
    std::cout << "Длина дуги: " << arc_length << " м\n";
    std::cout << "Время поворота: " << time << " сек\n\n";
}

#include <QPointF>

int getTurnDirection(const QPointF &A, const QPointF &B, const QPointF &C) {
    double cross = (B.x() - A.x()) * (C.y() - B.y()) - (A.y() - B.y()) * (C.x() - B.x());

    std::cout << cross << std::endl;

    if (cross < 0) {
        return -1;
    } else if (cross > 0) {
        return 1;
    } else {
        return 0;
    }
}

#include <QtMath>

Arc calculateTangentiallyArc(const QPointF &A, const QPointF &B, const QPointF &C, double radius, double speed = 5) {
    // 1. Определить сторону поворота
    auto a = geometry::normalize(B - A);
    auto b = geometry::normalize(C - B);

    auto turn = geometry::turnDirection(a, b);

    // 2. Находим Отрезки
    // Добавочный вектор 1
    auto a1 = geometry::vector(radius, geometry::angle(a) + turn * M_PI / 2);
    // Добавочный вектор 2
    auto b1 = geometry::vector(radius, geometry::angle(b) + turn * M_PI / 2);

    auto A1 = A + a1;
    auto B1 = B + a1;
    auto B2 = B + b1;
    auto C2 = C + b1;

    // 3. Находим пересечение.
    QPointF inter = geometry::intersection(A1, B1, B2, C2);

    // 4. Находим точки начала и конца дуги
    auto begin = geometry::projection(A, B, inter);
    auto end = geometry::projection(B, C, inter);

    return Arc(begin, end, inter, speed, turn > 0 ? true : false);
    //    arc.A = begin;
    //    arc.B = end;
    //    arc.C = inter;
    //    arc.R = radius;

    // Разбиение дуги

    // Находим точку пересечения Дуги и прямой из точек Центра окружности и точки поворота.
    // 1. Находим пересечение Дуги и прямой
    auto point = geometry::intersection(inter, radius, B);

    // Пока одну точку
    // 2. Получаем траекторию
    // A -> begin -> point -> end -> C

    // 3. Расчет скорости на участке
    // Время было
    auto len1 = std::hypot(a.x(), a.y());
    auto len2 = std::hypot(b.x(), b.y());
}

std::vector<QPointF> generateTrajectory(double radius, double angle_deg, int steps) {
    std::vector<QPointF> path;
    double angle_rad = angle_deg * DEG_TO_RAD;
    for (int i = 0; i <= steps; ++i) {
        double theta = angle_rad * i / steps;
        double x = radius * sin(theta);
        double y = radius * (1 - cos(theta));
        path.push_back({x, y});
    }
    return path;
}

/*!
 * Дистанция требуемая для изменения скорости.
 * @return
 */
double change_distance(double initialSpeed, double reachSpeed, double changeRate) {
    return std::fabs((initialSpeed * initialSpeed - reachSpeed * reachSpeed) / (2 * changeRate));
}

/*!
 * Время требуемое для изменения скорости.
 */
double change_time(double initialSpeed, double reachSpeed, double changeRate) {
    return std::fabs((initialSpeed - reachSpeed) / changeRate);
}

struct WayPoint {
    QPointF position;
    double speed;
};

double distance(const std::vector<WayPoint> &vec) {
    auto full{0};

    for (auto &v: vec) {
        full += std::hypot(v.position.x(), v.position.y());
    }

    return full;
}

double speed(double initialSpeed, double time) {
    return std::sqrt(1);
}

/*!
 * Путь требуемый для изменения скорости.
 */
std::vector<WayPoint> upper_path(const double initialSpeed, double reachSpeed, double radians) {
    std::cout << "*** Набор скорости! ***" << std::endl;
    auto time = (reachSpeed - initialSpeed) / acceleration;

    auto time_step = time / sampling_factor;

    auto full_distance = change_distance(initialSpeed, reachSpeed, acceleration);
    std::cout << "Время набора скорости: " << time << ", шаг по времени:" << time_step << ", Общая длина пути: "
              << full_distance << std::endl;

    // Время при ускорении t = (v2 - v1) / a; v2 = t*a + v1;
    std::vector<WayPoint> result;

    for (size_t t = 1; t < sampling_factor; ++t) {
        auto v1 = initialSpeed + (t - 1) * time_step * acceleration;
        auto v2 = v1 + time_step * acceleration;

        auto s = (v2 * v2 - v1 * v1) / (2 * acceleration);


        result.push_back({geometry::vector(s, radians), v2});
    }

    auto full{0};

    for (auto &vec: result) {
        std::cout << "X: " << vec.position.x() << ", Y: " << vec.position.y() << ", speed: " << vec.speed << std::endl;

        full += std::hypot(vec.position.x(), vec.position.y());
    }

    std::cout << "Общая длина пути полученная: " << full << std::endl;

    std::cout << "***********************" << std::endl;

    return result;
}

/*!
 * Путь замедления.
 */
std::vector<WayPoint> down_path(const double initialSpeed, double reachSpeed, double radians) {
    std::cout << "*** Сброс скорости! ***" << std::endl;
    auto full_distance = change_distance(initialSpeed, reachSpeed, deceleration);

    // TODO: Пока оставим
    std::vector<WayPoint> result;

    if (full_distance == 0)
        return result;

    std::cout << "Общая длина пути расчетная: " << full_distance << std::endl;

    auto time = std::fabs((initialSpeed - reachSpeed) / deceleration);
    auto time_step = time / sampling_factor;

    //    std::vector<segment> segments;

    // TODO: Пропадает 5 метров.
    for (size_t t = 1; t < sampling_factor; ++t) {
        auto v1 = initialSpeed - (t - 1) * time_step * deceleration;
        auto v2 = v1 - time_step * deceleration;

        auto s = change_distance(v2, v1, deceleration);

        result.push_back({geometry::vector(s, radians), v2});
        //        segments.push_back();
    }

    auto full{0};

    for (auto &vec: result) {
        std::cout << "X: " << vec.position.x() << ", Y: " << vec.position.y() << ", speed: " << vec.speed << std::endl;

        full += std::hypot(vec.position.x(), vec.position.y());
    }

    std::cout << "Общая длина пути полученная: " << full << std::endl;

    std::cout << "***********************" << std::endl;
    return result;
}

/*!
 * Радиус циркуляции.
 * @return
 */
double radius1(double length = L, double _delta = delta) {
    return K * length / std::tan(_delta);
}

double getDelta(double R, double length = L) {
    return std::atan(K * length / R);
}

/*!
 * Радиус циркуляции.
 * @param V - Скорость судна
 * @param w - Угловая скорость судна
 * @return
 */
double radius2(double V, double w) {
    return V / w;
}

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

/*!
 * Расчет по двум отрезкам.
 * @param data
 */
void calculate(std::vector<std::pair<QPointF, double>> data = {{{0,    0},    6},
                                                               {{0,    1000}, 6},
                                                               {{1000, 2000}, 6}}) {
    // Условные входные данные

    // 1. вычислить угол поворота (угол между отрезками)
    auto turn = geometry::angle(data[0].first, data[1].first, data[2].first);
    std::cout << "Угол поворота: " << turn << std::endl;
    if (turn >= M_PI / 2) {
    } else if (turn < M_PI / 2) {
        // Поворот на острый угол
    } else {
        // Разворот
    }
}

#include <QPainter>
#include <QWidget>
#include "figures/Segment.hpp"
#include "figures/Path.hpp"

class DrawWidget : public QWidget {
Q_OBJECT
public:
    DrawWidget(QWidget *parent = nullptr) : QWidget{parent} {
    }

    void paintEvent(QPaintEvent *event) {
        QPainter painter(this);
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

    void setSegments(const std::vector<std::pair<QPointF, double>> data) {
        segments.first = Segment(data[0].first, data[1].first, data[0].second);
        segments.second = Segment(data[1].first, data[2].first, data[1].second);
    }

    void setPath(Path path) {
        _path = path;
    }

private:
    Path _path;
    std::pair<Segment, Segment> segments;
};

/*!
 * Фигура 6. Для поворотов >= 90 градусов.
 * @param data
 * @return
 */
Path figure6(const std::vector<std::pair<QPointF, double>> data) {
    // Скорость дуги = скорость участка1;
    // TODO: На данный момент используется без участков торможения и разгона.
    // 1. Рассчитаем Радиус циркуляции по максимальному углу руля.
    auto radius = radius1();
    std::cout << "Радиус циркуляции: " << radius << std::endl;
    std::cout << "При этом угловая скорость при поисковой: " << data[0].second << ", равна: " << data[0].second / radius
              << std::endl;

    // 2. Находим направление.
    auto direction = geometry::angle(data[1].first - data[0].first);
    std::cout << "Направление первого участка: " << direction << std::endl;

    // 3. Находим дугу Радиуса циркуляции
    auto arc = calculateTangentiallyArc(data[0].first, data[1].first, data[2].first, radius, data[1].second);

    // 4. Строим Выходной путь
    Path path;

    // move(time), start(), end(), speed(), draw(QPainter)
    path.add(Segment(data[0].first, arc.start(), data[1].second)); // unique
    path.add(Arc(arc.start(), arc.end(), arc.center(), arc.speed(), arc.clockwise()));
    path.add(Segment(arc.end(), data[2].first, data[2].second));

    return path;
}

/*!
 * TODO: Использовать нельзя, слишком большая погрешность когда одна точка.
 * @param P0
 * @param P1
 * @param center
 * @param radius
 * @return
 */
std::vector<QPointF> intersection(const QPointF &P0, const QPointF &P1, const QPointF &center, double radius) {
    std::vector<QPointF> result;

    // Направление линии
    double dx = P1.x() - P0.x();
    double dy = P1.y() - P0.y();

    // Сдвиг относительно центра окружности
    double fx = P0.x() - center.x();
    double fy = P0.y() - center.y();

    // уравнение прямой
    double A = dx * dx + dy * dy;
    double B = 2 * (fx * dx + fy * dy);
    double C = fx * fx + fy * fy - radius * radius;

    double discriminant = B * B - 4 * A * C;

    if (discriminant < 0)
        return result;
    else if (std::abs(discriminant) < 1e-9) {
        // == 0
        double t = -B / (2 * A);

        result.emplace_back(P0.x() + t + dx, P0.y() + t * dy);
    } else {
        double sqrtD = std::sqrt(discriminant);
        double t1 = (-B + sqrtD) / (2 * A);
        double t2 = (-B - sqrtD) / (2 * A);

        result.emplace_back(P0.x() + t1 * dx, P0.y() + t1 * dy);
        result.emplace_back(P0.x() + t2 * dx, P0.y() + t2 * dy);
    }

    return result;
}

enum Side {
    _left,
    _right
};

/*!
 * Фигура 2. Поворот на острый угол, построение по точкам отрезка.
 * @param data
 * @return
 */
Path figure1(const std::vector<std::pair<QPointF, double>> data) // TODO: (A,B,C,speed0,speed1)
{
    Path path;
    auto A = data[0].first;
    auto B = data[1].first;
    auto C = data[2].first;

    // TODO: Первая часть расчета как и в figure 2
    //! 1. Рассчитаем Радиус циркуляции по максимальному углу руля.
    // На какой скорости будем входить? 3 м/c?
    auto radius = 100; //radius2(turn_velocity, omega); // Можем оставить радиус циркуляции на 35
    //    auto radius = radius1(); // TODO: Берем для
    std::cout << "Радиус циркуляции: " << radius << std::endl;
    //! 2. Расчет пути для сброса скорости к номинальной.
    // 2.1 Находим направление.
    auto direction = geometry::angle(data[1].first - data[0].first);

    // Направление внешней биссектрисы
    auto AB = B - A;
    auto CB = B - C;

    auto nAB = geometry::normalize(AB);
    auto nCB = geometry::normalize(CB);

    auto bisector = geometry::normalize(nCB + nAB); //! Направление биссектрисы

    // Точки биссектрисы
    auto b1 = B;
    auto b2 = B + 100 * bisector;

    // Вычисляем пути торможения и ускорения.
    auto v0 = 6.;
    auto v1 = 4.;

    auto break_range = (v0 * v0 - v1 * v1) / (2 * deceleration);
    auto accel_range = (v1 * v1 - v0 * v0) / (2 * -acceleration);
    qDebug() << "Путь торможения: " << break_range;
    qDebug() << "Путь разгона: " << accel_range;
    // TODO: тут начинаются отличия

    return path;

    // Решение по известным точкам на отрезках допустим 3/5 от начала
    auto k = 3. / 5.;
    //! Найти точки на отрезках 1. отрезок торможение, 2. отрезок разгона
    auto AB_range = geometry::distance(B - A);
    auto BC_range = geometry::distance(C - B);

    auto AB_point = A + geometry::normalize(B - A) * AB_range *
                        k;                                                              //vector(AB_range * k, angle(B - A));
    auto BC_point = C + geometry::normalize(B - C) * BC_range *
                        k;                                                              //    vector(BC_range * k, angle(B - C));
    qDebug() << "Длина AB: " << AB_range * 3 / 5 << ", направление A-B: " << geometry::angle(B - A) << ", точка: "
             << AB_point; //!
    qDebug() << "Длина BC: " << BC_range * 3 / 5 << ", направление C-B: " << geometry::angle(B - C) << ", точка: "
             << BC_point; //!

    // Переносим параллельно правую и левую части
    // От то точек биссектрисы строим параллельные прямые слева и справа на расстоянии Rц

    auto l_offset = vector(radius, angle(bisector) - M_PI / 2);
    auto r_offset = vector(radius, angle(bisector) + M_PI / 2);
    auto l1 = b1 + l_offset;
    auto l2 = b2 + l_offset;
    auto r1 = b1 + r_offset;
    auto r2 = b2 + r_offset;

    qDebug() << "Точка биссектрисы: " << b1 << ", левая: " << l1 << ", правая: " << r1;
    qDebug() << "Точка биссектрисы: " << b2 << ", левая: " << l2 << ", правая: " << r2;

    auto l_inters = intersection(l1, l2, AB_point, break_range);
    auto l_inter = std::max_element(l_inters.begin(), l_inters.end(), [B](const QPointF &a, const QPointF &b) {
        return distance(B, a) > distance(B, b);
    });

    auto r_inters = intersection(r1, r2, BC_point, accel_range);
    auto r_inter = std::max_element(r_inters.begin(), r_inters.end(), [B](const QPointF &a, const QPointF &b) {
        return distance(B, a) > distance(B, b);
    });

    qDebug() << "Точка слева: " << *l_inter << ", расстояние: " << distance(*l_inter, AB_point);  //! Проверка
    qDebug() << "Точка справа: " << *r_inter << ", расстояние: " << distance(*r_inter, BC_point); //! Проверка

    // TODO: такой подробный код для наглядности и дебага.
    // Определяем полуокружность по максимальному расстоянию

    Side side = Side::_left;

    if (break_range < accel_range) {
        side = Side::_right;
    }

    QPointF _begin, _end, _center;
    bool cw;
    // Скорость поисковая
    double speed = 5.;

    if (side == Side::_left) {
        _begin = *l_inter;
        _center = projection(b1, b2, *l_inter);
        _end = projection(r1, r2, *l_inter);
        cw = true;
    } else {
        _begin = *r_inter;
        _center = projection(b1, b2, *r_inter);
        _end = projection(l1, l2, *l_inter);
        cw = false;
    }

    Arc _arc2{_begin, _end, _center, speed, cw};

    // вторую сторону свести с точкой.

    //! Сводим всю схему
    auto _arc1 = calculateTangentiallyArc(A, AB_point, _arc2.start(), radius, speed);
    auto _seg1 = Segment{_arc1.end(), _arc2.start(), speed};

    auto _begin_seg = Segment{A, _arc1.start(), speed};

    auto _arc3 = calculateTangentiallyArc(_arc2.end(), BC_point, C, radius, speed);

    auto _seg2 = Segment{_arc2.end(), _arc3.start(), speed};

    auto _end_seg = Segment{_arc3.end(), C, speed};

    //! Разбить схему на скоростные участки.

    path.add(_begin_seg);
    path.add(_arc1);
    path.add(_seg1);
    path.add(_arc2);
    path.add(_seg2);
    path.add(_arc3);
    path.add(_end_seg);

    return path;
}

#include "figures/geometry.hpp"

// TODO: Фигура должна быть устойчива при изменении частей.
/*!
 * Фигура 2. Разворот левый.
 * Сброс скорости на дуге поворота, набор после дуги до поисковой скорости с возвратом на галс.
 * @return
 */
Path figure3(const std::vector<std::pair<QPointF, double>> data) {
    Path path;
    const auto search_speed = 5.;
    // TODO: Первая часть расчета как и в figure 1.
    auto radius = radius1(); // TODO: Берем для
    radius = 150;
    std::cout << "Радиус циркуляции: " << radius << std::endl;
    if (radius < 20) throw std::runtime_error("Недопустимый радиус циркуляции!");

    auto A = data[0].first;
    auto B = data[1].first;
    auto C = data[2].first;
    auto search_velocity1 = data[0].second;
    auto search_velocity2 = data[1].second;

    auto AB = normalize(B - A); //! Направление первого галса

    // Проверка высоты дуг
    auto ab_direction = B - A;

    auto A1 = A + vector(100, angle(ab_direction));
    auto A1_shift = A1 + vector(1000, angle(ab_direction) - M_PI / 4);
    auto arc3 = calculateTangentiallyArc(A, A1, A1_shift, radius, search_speed);
    arc3.swap();

    // Прямая для малых дуг
    auto A_shift = A + vector(2 * radius, angle(ab_direction) - M_PI / 2);
    auto B_shift = B + vector(2 * radius, angle(ab_direction) - M_PI / 2);
    auto inter = intersection(A1, A1_shift, A_shift, B_shift);
    auto arc2 = calculateTangentiallyArc(B_shift, inter, A1, radius, search_speed);

    // Смещаем к {0,0}
    auto _shift = arc3.end();
    arc2.translate(-_shift.x(), -_shift.y());
    arc3.translate(-_shift.x(), -_shift.y());

    //! Отрезок 1
    auto seg1 = Segment{geometry::vector(arc2.start(), radius, angle(ab_direction)), arc2.start()};

    Segment seg2{arc2.end(), arc3.start(), search_speed};

    // 1. Строим Дугу разворота
    // принимаем центр дуги как 0,0
    QPointF arc1_B = seg1.start();

    auto offset = vector(radius, angle(ab_direction) + M_PI / 2);

    auto arc1_C = arc1_B + offset;
    auto arc1_A = arc1_C + offset;

    Arc arc1{arc1_A, arc1_B, arc1_C, search_speed, false};

    //! Сброс на первой дуге
    auto arcs1 = arc1.down(search_velocity1, u_turn_velocity, split_factor);
    auto v = arcs1.back().speed();
    std::cout << "Скорость после дуги 1: " << v << std::endl;

    auto segs1 = seg1.upper(v, search_velocity2, split_factor);
    v = segs1.back().speed();
    std::cout << "Скорость после отрезка 1: " << v << std::endl;

    auto arcs2 = arc2.down(v, turn_velocity, split_factor);
    v = arcs2.back().speed();
    std::cout << "Скорость после дуги 2: " << v << std::endl;

    auto segs2 = seg2.upper(v, search_velocity2, split_factor);
    v = segs2.back().speed();
    std::cout << "Скорость после отрезка 2: " << v << std::endl;

    auto arcs3 = arc3.down(v, turn_velocity, split_factor);
    v = arcs3.back().speed();
    std::cout << "Скорость после дуги 3: " << v << std::endl;

    auto s = (search_velocity2 * search_velocity2 - v * v) / (2 * acceleration);
    auto seg3 = Segment{vector(s, geometry::angle(ab_direction) - M_PI), arc3.end()};

    auto segs3 = seg3.upper(v, search_velocity2, split_factor);
    v = segs3.back().speed();
    std::cout << "Скорость после отрезка 3: " << v << std::endl;

    path.add(arcs1);
    path.add(segs1);
    path.add(arcs2);
    path.add(segs2);
    path.add(arcs3);
    path.add(segs3);

    std::cout << "Длина от начала дуги разворота до начала фигуры: " << distance(segs3.back().start(), arc1.start())
              << std::endl;

    // если больше половины длины второго галса, начинаем от половины,
    // Совмещаем конечную точку первого галса и линию центра разворота.







    // Разбиваем дугу 1 От поисковой скорости первого участка
    //auto split_arc1 = arc1.down(data[0].second, u_turn_velocity, split_factor);

    //! Пока нет расчета торможения и разгона
    // Фиксированные значения
    //! 1. Посчитать сброс на дуге1 , будем считать фиксированное замедление или до скорости которая поддердивается на дуге например 4.5-5 м/с
    // 2. какой сброс на дуге2 и дуге 3.
    // 3. какой набор на отрезке 3.
    // Переменные значения Длина отрезка 1, длина отрезка 2, длина отрезка 4.
    // Длина отрезка 2 минимальна. Он влияет на длину отрезка 1 непосредственно
    // Основной набор осуществляется на отрезке 4. его начало не должно быть ближе половины сегмента
    // и точка 0,0 не должна быть дальше точки разворота


    // Надо посчитать сколько скинет на дуге 2, наберет на отрезке 3, скинет на дуге 3



    return path;
}

// TODO: Фигура должна быть устойчива при изменении частей.
/*!
 * Фигура 2. Поворот на острый угол, построение по наклону движения.
 * @param data
 * @return
 */
Path figure2(const std::vector<std::pair<QPointF, double>> data) {
    Path path;
    const auto search_speed = 5.;
    // TODO: Первая часть расчета как и в figure 1.
    auto radius = radius1(); // TODO: Берем для
    // radius = 150;
    std::cout << "Радиус циркуляции: " << radius << std::endl;

    if (radius < 20) throw std::runtime_error("Недопустимый радиус циркуляции!");

    auto A = data[0].first;
    auto B = data[1].first;
    auto C = data[2].first;

    auto turn_angle = angle(A, B, C);
    qDebug() << "Угол поворота: " << turn_angle * RAD_TO_DEG << "(град.)";

    auto bisect = bisector(A, B, C, false); //!

    // TODO: Обработать направление дуг
    QPointF b1 = B;

    if (distance(A, B) < radius * 3 / 2 || distance(B, C) < radius * 3 / 2) {
        qDebug() << "Недостаточная длина галса";
        auto minimum = std::min(distance(A, B), distance(B, C));

        b1 += vector(radius * 3 / 2 - minimum, angle(bisect)); //! Увеличиваем отдаление
    }

//! Построение Дуги поворота.
    auto l_offset = vector(radius, angle(bisect) - M_PI / 2);
    auto r_offset = vector(radius, angle(bisect) + M_PI / 2);
    Arc arc0{b1 + l_offset, b1 + r_offset, b1, search_speed, true};
    if (turn_angle < 0) arc0.swap();

// TODO: Двигать можно до определенного предела. Или определение радиуса кривизны атоматизировать.
    auto diff = 20; //! Угол на которой расширяем полосу справа и слева.

    if (turn_angle < 0) diff *= -1;

    //! Пересечения с отрезками.
    const auto max_distance = 5000; //! Максимальная длина для пересечения
    auto ab_intersection = intersection(A, B, arc0.start(),
                                        arc0.start() + vector(max_distance, angle(bisect) + M_PI + diff * DEG_TO_RAD));
    auto bc_intersection = intersection(B, C, arc0.end(),
                                        arc0.end() + vector(max_distance, angle(bisect) + M_PI - diff * DEG_TO_RAD));

    //! TODO: если мы сдвигаем то меняем diff, то есть разный с двух сторон
    const auto k = 5. / 6.;//! Коэффициент отступа фигуры от начала и конца.

    if (distance(A, B) * k < distance(B, ab_intersection)) {
        ab_intersection = B + vector(distance(A, B) * k, angle(A - B));
    }

    if (distance(B, C) * k < distance(B, bc_intersection)) {
        bc_intersection = B + vector(distance(B, C) * k, angle(C - B));
    }

    //! TODO: Важно разобраться с началом и концом
    const auto mult_radius = 2; //! Множитель Для уменьшения радиуса вспомогательных дуг касательных.
    Arc acr1_l = calculateTangentiallyArc(A, ab_intersection, arc0.start(), radius / mult_radius, search_speed);
    Arc acr2_r = calculateTangentiallyArc(arc0.end(), bc_intersection, C, radius / mult_radius, search_speed);

    // направление отрезков относительно нормали биссектрисы добавим с соответствующим знаком к дуге
    //! Определение добавочного угла к главной Дуге.
    auto v_seg01 = vector(acr1_l.end(), arc0.start());
    auto v_seg02 = vector(acr2_r.start(), arc0.end()); // TODO: все верно, так как в обратном направлении.

    qDebug() << "Направление биссектрисы: " << normalize(bisect) << "=" << angle(bisect) * RAD_TO_DEG;
    qDebug() << "Направление 1: " << angle(v_seg01) * RAD_TO_DEG;
    qDebug() << "Направление 2: " << angle(v_seg02) * RAD_TO_DEG;

    // Углы доворота от биссектрисы
    auto bisect_seg01 = angle_between(bisect, v_seg01);
    auto bisect_seg02 = angle_between(bisect, v_seg02);

    qDebug() << "Угол от биссектрисы к отрезку 1: " << bisect_seg01 * RAD_TO_DEG;
    qDebug() << "Угол от биссектрисы к отрезку 2: " << bisect_seg02 * RAD_TO_DEG;

    // Левый угол добавляем к началу, правый к концу
    if (turn_angle < 0) {
        bisect_seg01 *= -1, bisect_seg02 *= -1;
    }

    arc0.addDegreesToStart(bisect_seg01 * RAD_TO_DEG);
    arc0.addDegreesToEnd(bisect_seg02 * RAD_TO_DEG);

    auto seg01 = Segment{acr1_l.end(), arc0.start(), search_speed};
    auto seg02 = Segment{arc0.end(), acr2_r.start(), search_speed};

    auto begin_seg = Segment{A, acr1_l.start(), search_speed};
    auto end_seg = Segment{acr2_r.end(), C, search_speed};

// ! Распределить участки разгона
    path.add(begin_seg);
    path.add(acr1_l);
    path.add(seg01);

    //! Разбить с торможением
    path.add(arc0);

    path.add(seg02);
    path.add(acr2_r);
    path.add(end_seg);

    return path;
}

int main(int argc, char *argv[]) {
//    auto a = QPointF{-1.0, 1.0};
//    auto b = QPointF{0, 1.0};
//
//    double angle_rad = angle_between(a, b);
//    double angle_rad1 = angle_between(b, a);
//
//    double angle_deg = angle_rad * 180.0 / M_PI;
//    double angle_deg1 = angle_rad1 * 180.0 / M_PI;
//
//    std::cout << "a->b: " << angle_deg << " degrees" << std::endl;
//    std::cout << "b->a: " << angle_deg1 << " degrees" << std::endl;
//    return 0;



    //
    std::vector<std::pair<QPointF, double>> data = {{{1000, 0},   6},
                                                    {{1000, 500}, 6},
                                                    {{500,  500}, 6}};

    data = {{{1000, 100},  6},
            {{1000, 1000}, 6},
            {{1000, 100},  6}};

    //auto path = figure6(data);
//    Path path;
//   Arc arc{{100, 100}, {900, 100}, {500, 100}, 5, true};
//
//   arc.down(6, 3.3, 10);
//
//    Segment seg{{0, 0}, {1000, 0}, 5};
//
//    seg.upper(3.5, 6, 10);

//    path.add(arc);
    //
    // Path path = figure2(data);
    Path path = figure3(data);
//    return 0;
    QApplication app(argc, argv);
    DrawWidget window;
    window.resize(2000, 2000);
    window.setSegments(data);
    window.setPath(path);
    window.show();
    return app.exec();
}

#include "main.moc"
