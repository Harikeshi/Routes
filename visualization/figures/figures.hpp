#pragma once

#include "Path.hpp"
#include "geometry.hpp"
#include <QtMath>

namespace geometry {
enum turnType
{
    left,
    right,
    UTurn
};

/*!
 * Радиус циркуляции.
 * @return
 */
double radius1(double length = L, double _delta = delta)
{
    return K * length / std::tan(_delta);
}

double getDelta(double R, double length = L)
{
    return std::atan(K * length / R);
}

inline Arc calculateTangentiallyArc(const QPointF& A, const QPointF& B, const QPointF& C, double radius, double speed = 5)
{
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
}

/*!
 * Фигура 0. Для поворотов >= 90 градусов.
 * @param data
 * @return
 */
inline Path figure0(QPointF& A, QPointF& B, const QPointF& C, double speed1, double speed2, double radius = radius1())
{
    // Скорость дуги = скорость участка1;
    // TODO: На данный момент используется без участков торможения и разгона.

    // *реализовать* TODO: В случае нехватки длины галса для построения касательной удлинять Галс не меняя точку начала и точку конца.

    // 1. Рассчитаем Радиус циркуляции по максимальному углу руля.
    std::cout << "Радиус циркуляции: " << radius << std::endl;

    // 2. Находим направление.
    auto direction = geometry::angle(B - A);
    std::cout << "Направление первого участка: " << direction << std::endl;

    // 3. Находим дугу Радиуса циркуляции
    auto arc = calculateTangentiallyArc(A, B, C, radius, 0);

    // 4. Строим Выходной путь
    Path path;

    // move(time), start(), end(), speed(), draw(QPainter)
    path.add(Segment(A, arc.start(), speed1)); // unique
    // TODO: Изменить разгон и торможение
    path.add(Arc(arc.start(), arc.end(), arc.center(), speed1, arc.clockwise()));

    A = arc.end();
    B = C;

    //    path.add(Segment(arc.end(), C, speed2));

    return path;
}

enum Side
{
    _left,
    _right
};

/*!
 * Фигура 1P. Поворот на острый угол, построение по точкам отрезка.
 * @param data
 * @return
 */
Path figure1P(QPointF& A, QPointF& B, const QPointF& C, double speed1, double speed2,
              double radius = radius1()) // TODO: (A,B,C,speed0,speed1)
{
    Path path;
    // TODO: Первая часть расчета как и в figure 2

    std::cout << "Радиус циркуляции: " << radius << std::endl;
    //! 2. Расчет пути для сброса скорости к номинальной.
    // 2.1 Находим направление.
    auto direction = geometry::angle(B - A);
    auto range1 = geometry::distance(B - A);

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
                            k; //vector(AB_range * k, angle(B - A));
    auto BC_point = C + geometry::normalize(B - C) * BC_range *
                            k; //    vector(BC_range * k, angle(B - C));
    qDebug() << "Длина AB: " << AB_range * 3 / 5 << ", направление A-B: " << geometry::angle(B - A) << ", точка: "
             << AB_point; //!
    qDebug() << "Длина BC: " << BC_range * 3 / 5 << ", направление C-B: " << geometry::angle(B - C) << ", точка: "
             << BC_point; //!

    // Переносим параллельно правую и левую части
    // От то точек биссектрисы строим параллельные прямые слева и справа на расстоянии Rц

    auto l_offset = geometry::vector(radius, geometry::angle(bisector) - M_PI / 2);
    auto r_offset = geometry::vector(radius, geometry::angle(bisector) + M_PI / 2);
    auto l1 = b1 + l_offset;
    auto l2 = b2 + l_offset;
    auto r1 = b1 + r_offset;
    auto r2 = b2 + r_offset;

    qDebug() << "Точка биссектрисы: " << b1 << ", левая: " << l1 << ", правая: " << r1;
    qDebug() << "Точка биссектрисы: " << b2 << ", левая: " << l2 << ", правая: " << r2;

    auto first_intersections = geometry::intersection(l1, l2, AB_point, break_range);
    auto first_intersection = std::max_element(first_intersections.begin(), first_intersections.end(), [B](const QPointF& a, const QPointF& b) {
        return geometry::distance(B, a) > geometry::distance(B, b);
    });

    auto second_intersections = geometry::intersection(r1, r2, BC_point, accel_range);
    auto second_intersection = std::max_element(second_intersections.begin(), second_intersections.end(), [B](const QPointF& a, const QPointF& b) {
        return geometry::distance(B, a) > geometry::distance(B, b);
    });

    qDebug() << "Точка слева: " << *first_intersection << ", расстояние: "
             << geometry::distance(*first_intersection, AB_point); //! Проверка
    qDebug() << "Точка справа: " << *second_intersection << ", расстояние: "
             << geometry::distance(*second_intersection, BC_point); //! Проверка

    // TODO: такой подробный код для наглядности и дебага.
    // Определяем полуокружность по максимальному расстоянию

    Side side = Side::_left;

    if (break_range < accel_range)
    {
        side = Side::_right;
    }

    QPointF _begin, _end, _center;
    bool cw;
    // Скорость поисковая
    double speed = 5.;

    if (side == Side::_left)
    {
        _begin = *first_intersection;
        _center = geometry::projection(b1, b2, *first_intersection);
        _end = geometry::projection(r1, r2, *first_intersection);
        cw = true;
    }
    else
    {
        _begin = *second_intersection;
        _center = geometry::projection(b1, b2, *second_intersection);
        _end = geometry::projection(l1, l2, *second_intersection);
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

QVector<QPointF> moveFigureByAnchor(
    const QVector<QPointF>& points,
    const QPointF& oldAnchor,
    const QPointF& newAnchor)
{
    QVector<QPointF> moved;
    moved.reserve(points.size());

    QPointF offset = newAnchor - oldAnchor;

    for (const auto& pt : points)
    {
        moved.push_back(pt + offset);
    }
    return moved;
}

#include "figures/geometry.hpp"

// TODO: Фигура должна быть устойчива при изменении частей.
/*!
 * Фигура 2L. Разворот левый.
 * Сброс скорости на дуге поворота, набор после дуги до поисковой скорости с возвратом на галс.
 * @return
 */
Path figure2L(QPointF& A, QPointF& B, const QPointF& C, double speed1, double speed2, double radius = radius1())
{
    Path path;
    std::cout << "Радиус циркуляции: " << radius << std::endl;
    if (radius < 20)
        throw std::runtime_error("Недопустимый радиус циркуляции!");

    auto AB = geometry::normalize(B - A); //! Направление первого галса

    // TODO: Оставить зависимость только от точки A и направления AB
    // Проверка высоты дуг
    auto ab_direction = B - A;

    auto A1 = A + geometry::vector(100, geometry::angle(ab_direction));
    auto A1_shift = A1 + geometry::vector(1000, geometry::angle(ab_direction) - M_PI / 4);
    auto arc3 = calculateTangentiallyArc(A, A1, A1_shift, radius, 0);
    arc3.swap();

    // Прямая для малых дуг
    auto A_shift = A + geometry::vector(2 * radius, geometry::angle(ab_direction) - M_PI / 2);
    auto B_shift = B + geometry::vector(2 * radius, geometry::angle(ab_direction) - M_PI / 2);
    auto inter = geometry::intersection(A1, A1_shift, A_shift, B_shift);
    auto arc2 = calculateTangentiallyArc(B_shift, inter, A1, radius, 0);

    // Смещаем к {0,0}
    //    auto _shift = arc3.end();
    //    arc2.translate(-_shift.x(), -_shift.y());
    //    arc3.translate(-_shift.x(), -_shift.y());
    //    std::cout << "Координаты arc3: [" << arc3.end().x() << ", " << arc3.end().y() << "]" << std::endl;

    //! Отрезок 1
    auto seg1 = Segment{geometry::vector(arc2.start(), radius, geometry::angle(ab_direction)), arc2.start()};

    Segment seg2{arc2.end(), arc3.start(), 0};

    // 1. Строим Дугу разворота
    // принимаем центр дуги как 0,0
    QPointF arc1_B = seg1.start();

    auto offset = geometry::vector(radius, geometry::angle(ab_direction) + M_PI / 2);

    auto arc1_C = arc1_B + offset;
    auto arc1_A = arc1_C + offset;

    Arc arc1{arc1_A, arc1_B, arc1_C, 0, false};

    //! Сброс на первой дуге
    auto arcs1 = arc1.down(speed1, u_turn_velocity, split_factor);
    auto v = arcs1.back().speed();
    std::cout << "Скорость после дуги 1: " << v << std::endl;

    auto segs1 = seg1.upper(v, speed2, split_factor);
    v = segs1.back().speed();
    std::cout << "Скорость после отрезка 1: " << v << std::endl;

    auto arcs2 = arc2.down(v, turn_velocity, split_factor);
    v = arcs2.back().speed();
    std::cout << "Скорость после дуги 2: " << v << std::endl;

    auto segs2 = seg2.upper(v, speed2, split_factor);
    v = segs2.back().speed();
    std::cout << "Скорость после отрезка 2: " << v << std::endl;

    auto arcs3 = arc3.down(v, turn_velocity, split_factor);
    v = arcs3.back().speed();
    std::cout << "Скорость после дуги 3: " << v << std::endl;

    //! Длина заключительного галса.
    auto s = (speed2 * speed2 - v * v) / (2 * acceleration);
    auto seg3 = Segment{arc3.end(), arc3.end() + geometry::vector(s, geometry::angle(ab_direction) - M_PI)};

    std::cout << "Выходной сегмент: начало[" << seg3.start().x() << "," << seg3.start().y() << "], конец: ["
              << seg3.end().x() << "," << seg3.end().y() << "]" << std::endl;

    auto segs3 = seg3.upper(v, speed2, split_factor);
    v = segs3.back().speed();
    std::cout << "Скорость после отрезка 3: " << v << std::endl;

    std::cout << "Длина от начала дуги разворота до начала фигуры: "
              << geometry::distance(segs3.back().start(), arc1.start())
              << std::endl;
    std::cout << "Длина последнего галса: " << s << std::endl;

    //! Сдвигаем в место начала, определяем A и B
    // Находим точку начала фигуры
    auto BC = C - B;
    auto bc_range = geometry::distance(C, B);

    // TODO: сколько оставлять на следующий галс?
    auto figure_range = geometry::distance(segs3.back().start(), arc1.start());

    //! Сдвигаем в полученную точку A
    auto seg0 = Segment{A, arcs1.front().start(), speed1};

    // Определяем точку старта
    // TODO: не половину длины а два радиуса циркуляции например
    if (bc_range / 2 <= figure_range)
    {
        // Откладываем от середины галса
        A = geometry::vector(A, bc_range / 2, geometry::angle(ab_direction));
        //! Определяем начальный галс
    }
    else
    {
        // Проверка на длину первого галса
        A = geometry::vector(B, figure_range, geometry::angle(ab_direction) - M_PI);
    }
    B = C;

    // Используем смещение готовой фигуры?
    // Сместить в точку A

    seg0.show();
    arc1.show();
    seg1.show();
    arc2.show();
    seg2.show();
    arc3.show();
    seg3.show();

    //! Смещение от старого надо учитывать??
    QPointF from{seg0.start()}, to{A};
    seg0.transform(from, to);
    path.add(seg0);
    for (auto& arc : arcs1)
    {
        arc.transform(from, to);
    }
    path.add(arcs1);
    for (auto& seg : segs1)
    {
        seg.transform(from, to);
    }
    path.add(segs1);
    for (auto& arc : arcs2)
    {
        arc.transform(from, to);
    }
    path.add(arcs2);
    for (auto& seg : segs2)
    {
        seg.transform(from, to);
    }
    path.add(segs2);
    for (auto& arc : arcs3)
    {
        arc.transform(from, to);
    }
    path.add(arcs3);
    for (auto& seg : segs3)
    {
        seg.transform(from, to);
    }
    path.add(segs3);
    // последний отрезок

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
 * Фигура 1A. Поворот на острый угол, построение по наклону движения.
 * @param data
 * @return
 */
Path figure1A(QPointF& A, QPointF& B, const QPointF& C, double speed1, double speed2, double radius = radius1())
{
    std::cout << "Радиус циркуляции: " << radius << std::endl;
    if (radius < 20)
        throw std::runtime_error("Недопустимый радиус циркуляции!");

    Path path;
    auto ab_range = geometry::distance(A, B);
    auto bc_range = geometry::distance(B, C);

    auto turn_angle = geometry::angle(A, B, C);
    qDebug() << "Угол поворота: " << turn_angle * RAD_TO_DEG << "(град.)";

    auto bisect = geometry::bisector(A, B, C, false); //! направление

    // TODO: Обработать направление дуг
    QPointF b1 = B;

    // Увеличиваем отдаление от Точки B на расстояние 2/3 радиуса, если Длина одного из галсов не соответствует.
    //! Модно отодвинуть потом
    if (geometry::distance(A, B) < radius * 3 / 2 || geometry::distance(B, C) < radius * 3 / 2)
    {
        qDebug() << "Недостаточная длина галса";
        auto minimum = qMin(geometry::distance(A, B), geometry::distance(B, C));

        b1 += geometry::vector(radius * 3 / 2 - minimum, geometry::angle(bisect)); //! Увеличиваем отдаление
    }

    //! Построение Дуги поворота.
    auto l_offset = geometry::vector(radius, geometry::angle(bisect) - M_PI / 2);
    auto r_offset = geometry::vector(radius, geometry::angle(bisect) + M_PI / 2);

    Arc arc0{b1 + l_offset, b1 + r_offset, b1, 0, true};

    // Если поворот влево переворачиваем дугу
    if (turn_angle < 0)
        arc0.swap();

    // TODO: Двигать можно до определенного предела. Или определение радиуса кривизны автоматизировать.
    auto diff = 5;
    //! Угол на которой расширяем полосу справа и слева. Добавочный угол к прямым параллельным биссектрисе;

    if (turn_angle < 0)
        diff *= -1;

    //! Пересечения с отрезками.
    auto ab_intersection = geometry::intersection(A, B, arc0.start(), arc0.start() + geometry::vector(max_distance, geometry::angle(bisect) + M_PI + diff * DEG_TO_RAD));
    auto bc_intersection = geometry::intersection(B, C, arc0.end(), arc0.end() + geometry::vector(max_distance, geometry::angle(bisect) + M_PI - diff * DEG_TO_RAD));

    //! TODO: если мы сдвигаем то меняем diff, то есть разный с двух сторон
    const auto k = 5. / 6.; //! Коэффициент отступа фигуры от начала и конца.

    if (geometry::distance(A, B) * k < geometry::distance(B, ab_intersection))
    {
        ab_intersection = B + geometry::vector(geometry::distance(A, B) * k, geometry::angle(A - B));
    }

    if (geometry::distance(B, C) * k < geometry::distance(B, bc_intersection))
    {
        bc_intersection = B + geometry::vector(geometry::distance(B, C) * k, geometry::angle(C - B));
    }

    //! TODO: Важно разобраться с началом и концом
    const auto mult_radius = 1; //! Множитель Для уменьшения радиуса вспомогательных дуг касательных.
    // TODO: Проверить направление дуг
    Arc arc1 = calculateTangentiallyArc(A, ab_intersection, arc0.start(), radius / mult_radius, speed1);
    Arc arc2 = calculateTangentiallyArc(arc0.end(), bc_intersection, C, radius / mult_radius, speed2);

    // направление отрезков относительно нормали биссектрисы добавим с соответствующим знаком к дуге
    //! Определение добавочного угла к главной Дуге.
    //! Доворот дуги 0 со стороны дуг 1 и 2;
    auto v_seg01 = geometry::vector(arc1.end(), arc0.start());
    auto v_seg02 = geometry::vector(arc2.start(), arc0.end()); // TODO: все верно, так как в обратном направлении.

    qDebug() << "Направление биссектрисы: " << geometry::normalize(bisect) << "="
             << geometry::angle(bisect) * RAD_TO_DEG;
    qDebug() << "Направление 1: " << geometry::angle(v_seg01) * RAD_TO_DEG;
    qDebug() << "Направление 2: " << geometry::angle(v_seg02) * RAD_TO_DEG;

    // Углы доворота от биссектрисы
    auto bisect_seg01 = geometry::angle_between(bisect, v_seg01);
    auto bisect_seg02 = geometry::angle_between(bisect, v_seg02);

    qDebug() << "Угол от биссектрисы к отрезку 1: " << bisect_seg01 * RAD_TO_DEG;
    qDebug() << "Угол от биссектрисы к отрезку 2: " << bisect_seg02 * RAD_TO_DEG;

    // Левый угол добавляем к началу, правый к концу
    if (turn_angle < 0)
    {
        bisect_seg01 *= -1, bisect_seg02 *= -1;
    }

    arc0.addDegreesToStart(bisect_seg01 * RAD_TO_DEG);
    arc0.addDegreesToEnd(bisect_seg02 * RAD_TO_DEG);

    auto seg01 = Segment{arc1.end(), arc0.start(), speed1};
    auto seg02 = Segment{arc0.end(), arc2.start(), speed2};

    auto begin_seg = Segment{A, arc1.start(), speed1};

    // TODO: Второй отрезок передаем дальше в расчет
    auto end_seg = Segment{arc2.end(), C, speed2};

    // TODO: Можем передавать скорость дальше, проверять и делать разгон, если она не соответствует. Интересно на это посмотреть.

    // ! Распределить участки разгона
    path.add(begin_seg);

    auto arcs1 = arc1.down(begin_seg.speed(), turn_velocity, split_factor);
    std::cout << "Скорость после первой дуги: " << arcs1.back().speed() << std::endl;
    path.add(arcs1);

    auto segs1 = seg01.upper(arcs1.back().speed(), speed1, split_factor);
    std::cout << "Скорость после первого отрезка: " << segs1.back().speed() << std::endl;
    path.add(segs1);

    //! Разбить с торможением
    auto arcs0 = arc0.down(segs1.back().speed(), turn_velocity, split_factor);
    std::cout << "Скорость после дуги разворота: " << arcs0.back().speed() << std::endl;
    path.add(arcs0);

    auto segs2 = seg02.upper(arcs0.back().speed(), speed2, split_factor);
    std::cout << "Скорость после первого отрезка: " << segs2.back().speed() << std::endl;
    path.add(segs2);

    auto arcs2 = arc2.down(segs2.back().speed(), turn_velocity, split_factor);
    std::cout << "Скорость после дуги разворота: " << arcs2.back().speed() << std::endl;
    path.add(arcs2);

    //TODO: Последний не заносим а передаем точки для расчета дальше
    path.add(end_seg);

    return path;
}

double minimum_range(const QPointF& A, const QPointF& B, const QPointF& C)
{
    auto ab_range = geometry::distance(A, B);
    auto bc_range = geometry::distance(C, B);

    qDebug() << "AB range: " << ab_range;
    qDebug() << "BC range: " << bc_range;

    //! Оставляем 2 radius
    return qMin(geometry::distance(A, B), geometry::distance(C, B));
}

Path figure1A0(QPointF& A, QPointF& B, const QPointF& C, double speed1, double speed2, double radius = radius1())
{
    std::cout << "Радиус циркуляции: " << radius << std::endl;
    if (radius < 20)
        throw std::runtime_error("Недопустимый радиус циркуляции!");

    Path path;
    auto turn_angle = geometry::angle(A, B, C);
    qDebug() << "Угол поворота: " << turn_angle * RAD_TO_DEG << "(град.)";

    auto bisect = geometry::bisector(A, B, C, false); //! направление

    // AB проверяется на предыдущем шаге, оставляем минимум два радиуса
    // отступаем от C два радиуса
    auto bc_range = geometry::distance(A, B); //!

    auto length_offset = 2 * radius * cos(geometry::angle(bisect));

    qDebug() << length_offset;

    // AB отступаем от А 1 радиус строим

    // TODO: Обработать направление дуг
    QPointF b1 = B;
    // Отрезок 1: Отступаем радиус циркуляции radius - от края и проводим отрезок, находим касательную.
    //Если длина Галса меньше радиуса отодвигаем
    //! Модно отодвинуть потом
    // как решить сколько должны оставить на выходной галс
    if (geometry::distance(A, B) < radius * 2 || geometry::distance(A, B) < radius * 2)
    {
        qDebug() << "Недостаточная длина галса";
        auto minimum = qMin(geometry::distance(A, B), geometry::distance(B, C));

        b1 += geometry::vector(radius + minimum, geometry::angle(bisect)); //! Увеличиваем отдаление
    }
    //! Построение Дуги поворота.
    auto l_offset = geometry::vector(radius, geometry::angle(bisect) - M_PI / 2);
    auto r_offset = geometry::vector(radius, geometry::angle(bisect) + M_PI / 2);

    Arc arc0{b1 + l_offset, b1 + r_offset, b1, 0, true};

    // Если поворот влево переворачиваем дугу
    if (turn_angle < 0)
        arc0.swap();

    // TODO: Двигать можно до определенного предела. Или определение радиуса кривизны автоматизировать.
    auto diff = 5;
    //! Угол на которой расширяем полосу справа и слева. Добавочный угол к прямым параллельным биссектрисе;

    if (turn_angle < 0)
        diff *= -1;

    //! Пересечения с отрезками.
    auto ab_intersection = geometry::intersection(A, B, arc0.start(), arc0.start() + geometry::vector(max_distance, geometry::angle(bisect) + M_PI + diff * DEG_TO_RAD));
    auto bc_intersection = geometry::intersection(B, C, arc0.end(), arc0.end() + geometry::vector(max_distance, geometry::angle(bisect) + M_PI - diff * DEG_TO_RAD));

    //! TODO: если мы сдвигаем то меняем diff, то есть разный с двух сторон
    const auto k = 5. / 6.; //! Коэффициент отступа фигуры от начала и конца.

    if (geometry::distance(A, B) * 1 < geometry::distance(B, ab_intersection))
    {
        ab_intersection = B + geometry::vector(geometry::distance(A, B) - radius, geometry::angle(A - B));
    }

    if (geometry::distance(B, C) * k < geometry::distance(B, bc_intersection))
    {
        bc_intersection = B + geometry::vector(geometry::distance(B, C) - radius, geometry::angle(C - B));
    }

    //! TODO: Важно разобраться с началом и концом
    const auto mult_radius = 1; //! Множитель Для уменьшения радиуса вспомогательных дуг касательных.
    // TODO: Проверить направление дуг
    Arc arc1 = calculateTangentiallyArc(A, ab_intersection, arc0.start(), radius / mult_radius, speed1);
    Arc arc2 = calculateTangentiallyArc(arc0.end(), bc_intersection, C, radius / mult_radius, speed2);

    // направление отрезков относительно нормали биссектрисы добавим с соответствующим знаком к дуге
    //! Определение добавочного угла к главной Дуге.
    //! Доворот дуги 0 со стороны дуг 1 и 2;
    auto v_seg01 = geometry::vector(arc1.end(), arc0.start());
    auto v_seg02 = geometry::vector(arc2.start(), arc0.end()); // TODO: все верно, так как в обратном направлении.

    qDebug() << "Направление биссектрисы: " << geometry::normalize(bisect) << "="
             << geometry::angle(bisect) * RAD_TO_DEG;
    qDebug() << "Направление 1: " << geometry::angle(v_seg01) * RAD_TO_DEG;
    qDebug() << "Направление 2: " << geometry::angle(v_seg02) * RAD_TO_DEG;

    // Углы доворота от биссектрисы
    auto bisect_seg01 = geometry::angle_between(bisect, v_seg01);
    auto bisect_seg02 = geometry::angle_between(bisect, v_seg02);

    qDebug() << "Угол от биссектрисы к отрезку 1: " << bisect_seg01 * RAD_TO_DEG;
    qDebug() << "Угол от биссектрисы к отрезку 2: " << bisect_seg02 * RAD_TO_DEG;

    // Левый угол добавляем к началу, правый к концу
    if (turn_angle < 0)
    {
        bisect_seg01 *= -1, bisect_seg02 *= -1;
    }

    arc0.addDegreesToStart(bisect_seg01 * RAD_TO_DEG);
    arc0.addDegreesToEnd(bisect_seg02 * RAD_TO_DEG);

    auto seg01 = Segment{arc1.end(), arc0.start(), speed1};
    auto seg02 = Segment{arc0.end(), arc2.start(), speed2};

    auto begin_seg = Segment{A, arc1.start(), speed1};

    // TODO: Второй отрезок передаем дальше в расчет
    auto end_seg = Segment{arc2.end(), C, speed2};

    // TODO: Можем передавать скорость дальше, проверять и делать разгон, если она не соответствует. Интересно на это посмотреть.

    // ! Распределить участки разгона
    path.add(begin_seg);

    auto arcs1 = arc1.down(begin_seg.speed(), turn_velocity, split_factor);
    std::cout << "Скорость после первой дуги: " << arcs1.back().speed() << std::endl;
    path.add(arcs1);

    auto segs1 = seg01.upper(arcs1.back().speed(), speed1, split_factor);
    std::cout << "Скорость после первого отрезка: " << segs1.back().speed() << std::endl;
    path.add(segs1);

    //! Разбить с торможением
    auto arcs0 = arc0.down(segs1.back().speed(), turn_velocity, split_factor);
    std::cout << "Скорость после дуги разворота: " << arcs0.back().speed() << std::endl;
    path.add(arcs0);

    auto segs2 = seg02.upper(arcs0.back().speed(), speed2, split_factor);
    std::cout << "Скорость после первого отрезка: " << segs2.back().speed() << std::endl;
    path.add(segs2);

    auto arcs2 = arc2.down(segs2.back().speed(), turn_velocity, split_factor);
    std::cout << "Скорость после дуги разворота: " << arcs2.back().speed() << std::endl;
    path.add(arcs2);

    //TODO: Последний не заносим а передаем точки для расчета дальше
    A = arcs2.back().end();
    B = C;
    //    path.add(end_seg);

    return path;
}

Path figure1A2(QPointF& A, QPointF& B, const QPointF& C, double speed1, double speed2, double radius = radius1())
{
    std::cout << "Радиус циркуляции: " << radius << std::endl;
    if (radius < 20)
        throw std::runtime_error("Недопустимый радиус циркуляции!");

    Path path;
    auto turn_angle = geometry::angle(A, B, C);
    qDebug() << "Угол поворота: " << turn_angle * RAD_TO_DEG << "(град.)";

    auto bisect = geometry::bisector(A, B, C, false); //! направление

    // AB проверяется на предыдущем шаге, оставляем минимум два радиуса
    // отступаем от C два радиуса
    auto bc_range = geometry::distance(A, B); //!

    auto length_offset = 2 * radius * cos(geometry::angle(bisect));

    qDebug() << "Смещение от биссектрисы:" << length_offset;

    // AB отступаем от А 1 радиус строим

    // TODO: Обработать направление дуг
    QPointF b1 = B;
    // Отрезок 1: Отступаем радиус циркуляции radius - от края и проводим отрезок, находим касательную.
    //Если длина Галса меньше радиуса отодвигаем
    //! Модно отодвинуть потом
    // как решить сколько должны оставить на выходной галс
    if (geometry::distance(A, B) < radius * 2 || geometry::distance(A, B) < radius * 2)
    {
        qDebug() << "Недостаточная длина галса";
        auto minimum = qMin(geometry::distance(A, B), geometry::distance(B, C));

        b1 += geometry::vector(radius + minimum, geometry::angle(bisect)); //! Увеличиваем отдаление
    }
    //! Построение Дуги поворота.
    auto l_offset = geometry::vector(radius, geometry::angle(bisect) - M_PI / 2);
    auto r_offset = geometry::vector(radius, geometry::angle(bisect) + M_PI / 2);

    Arc arc0{b1 + l_offset, b1 + r_offset, b1, 0, true};

    // Если поворот влево переворачиваем дугу
    if (turn_angle < 0)
        arc0.swap();

    // TODO: Двигать можно до определенного предела. Или определение радиуса кривизны автоматизировать.
    auto diff = 5;
    //! Угол на которой расширяем полосу справа и слева. Добавочный угол к прямым параллельным биссектрисе;

    if (turn_angle < 0)
        diff *= -1;

    //! Пересечения с отрезками.
    auto ab_intersection = geometry::intersection(A, B, arc0.start(), arc0.start() + geometry::vector(max_distance, geometry::angle(bisect) + M_PI + diff * DEG_TO_RAD));
    auto bc_intersection = geometry::intersection(B, C, arc0.end(), arc0.end() + geometry::vector(max_distance, geometry::angle(bisect) + M_PI - diff * DEG_TO_RAD));

    //! TODO: если мы сдвигаем то меняем diff, то есть разный с двух сторон
    const auto k = 5. / 6.; //! Коэффициент отступа фигуры от начала и конца.

    if (geometry::distance(A, B) * 1 < geometry::distance(B, ab_intersection))
    {
        ab_intersection = B + geometry::vector(geometry::distance(A, B) - radius, geometry::angle(A - B));
    }

    if (geometry::distance(B, C) * k < geometry::distance(B, bc_intersection))
    {
        bc_intersection = B + geometry::vector(geometry::distance(B, C) - radius, geometry::angle(C - B));
    }

    //! TODO: Важно разобраться с началом и концом
    const auto mult_radius = 1; //! Множитель Для уменьшения радиуса вспомогательных дуг касательных.
    // TODO: Проверить направление дуг
    Arc arc1 = calculateTangentiallyArc(A, ab_intersection, arc0.start(), radius / mult_radius, speed1);
    Arc arc2 = calculateTangentiallyArc(arc0.end(), bc_intersection, C, radius / mult_radius, speed2);

    // направление отрезков относительно нормали биссектрисы добавим с соответствующим знаком к дуге
    //! Определение добавочного угла к главной Дуге.
    //! Доворот дуги 0 со стороны дуг 1 и 2;
    auto v_seg01 = geometry::vector(arc1.end(), arc0.start());
    auto v_seg02 = geometry::vector(arc2.start(), arc0.end()); // TODO: все верно, так как в обратном направлении.

    qDebug() << "Направление биссектрисы: " << geometry::normalize(bisect) << "="
             << geometry::angle(bisect) * RAD_TO_DEG;
    qDebug() << "Направление 1: " << geometry::angle(v_seg01) * RAD_TO_DEG;
    qDebug() << "Направление 2: " << geometry::angle(v_seg02) * RAD_TO_DEG;

    // Углы доворота от биссектрисы
    auto bisect_seg01 = geometry::angle_between(bisect, v_seg01);
    auto bisect_seg02 = geometry::angle_between(bisect, v_seg02);

    qDebug() << "Угол от биссектрисы к отрезку 1: " << bisect_seg01 * RAD_TO_DEG;
    qDebug() << "Угол от биссектрисы к отрезку 2: " << bisect_seg02 * RAD_TO_DEG;

    // Левый угол добавляем к началу, правый к концу
    if (turn_angle < 0)
    {
        bisect_seg01 *= -1, bisect_seg02 *= -1;
    }

    arc0.addDegreesToStart(bisect_seg01 * RAD_TO_DEG);
    arc0.addDegreesToEnd(bisect_seg02 * RAD_TO_DEG);

    auto seg01 = Segment{arc1.end(), arc0.start(), speed1};
    auto seg02 = Segment{arc0.end(), arc2.start(), speed2};

    auto begin_seg = Segment{A, arc1.start(), speed1};

    // TODO: Второй отрезок передаем дальше в расчет
    auto end_seg = Segment{arc2.end(), C, speed2};

    // TODO: Можем передавать скорость дальше, проверять и делать разгон, если она не соответствует. Интересно на это посмотреть.

    // ! Распределить участки разгона
    path.add(begin_seg);

    auto arcs1 = arc1.down(begin_seg.speed(), turn_velocity, split_factor);
    std::cout << "Скорость после первой дуги: " << arcs1.back().speed() << std::endl;
    path.add(arcs1);

    auto segs1 = seg01.upper(arcs1.back().speed(), speed1, split_factor);
    std::cout << "Скорость после первого отрезка: " << segs1.back().speed() << std::endl;
    path.add(segs1);

    //! Разбить с торможением
    auto arcs0 = arc0.down(segs1.back().speed(), turn_velocity, split_factor);
    std::cout << "Скорость после дуги разворота: " << arcs0.back().speed() << std::endl;
    path.add(arcs0);

    auto segs2 = seg02.upper(arcs0.back().speed(), speed2, split_factor);
    std::cout << "Скорость после первого отрезка: " << segs2.back().speed() << std::endl;
    path.add(segs2);

    auto arcs2 = arc2.down(segs2.back().speed(), turn_velocity, split_factor);
    std::cout << "Скорость после дуги разворота: " << arcs2.back().speed() << std::endl;
    path.add(arcs2);

    //TODO: Последний не заносим а передаем точки для расчета дальше
    A = arcs2.back().end();
    B = C;
    //    path.add(end_seg);

    return path;
}

Path figure1A1(QPointF& A, QPointF& B, const QPointF& C, double speed1, double speed2, double radius = radius1())
{
    std::cout << "Радиус циркуляции: " << radius << std::endl;
    if (radius < 20)
        throw std::runtime_error("Недопустимый радиус циркуляции!");

    Path path;
    auto turn_angle = geometry::angle(A, B, C);
    qDebug() << "Угол поворота: " << turn_angle * RAD_TO_DEG << "(град.)";

    auto bisect = geometry::bisector(A, B, C, false); //! направление

    QPointF b1 = B;
    // AB проверяется на предыдущем шаге, оставляем минимум два радиуса
    // отступаем от C два радиуса
    if (geometry::distance(A, B) < radius * 2)
        qDebug() << "Недостаточная длина галса";

    auto bc_range = geometry::distance(A, B); //!

    //! 2 радиуса передаем дальше, 1 радиус для этой дуги
    auto range_diff = geometry::distance(B, C) - radius * 4;

    qDebug() << "b1: " << b1;
    if (range_diff < radius)
    {
        //! Увеличиваем отдаление  Гипотенуза * cos(прилежащего угла);
        b1 += geometry::vector(std::fabs(range_diff + radius * 2) * cos(geometry::angle(bisect)),
                               geometry::angle(bisect));
        qDebug() << "b1: " << b1;
    }

    //! Построение Дуги поворота.
    auto l_offset = geometry::vector(radius, geometry::angle(bisect) - M_PI / 2);
    auto r_offset = geometry::vector(radius, geometry::angle(bisect) + M_PI / 2);

    qDebug() << "l_offset: " << l_offset;
    qDebug() << "r_offset: " << r_offset;

    Arc arc0{b1 + l_offset, b1 + r_offset, b1, 0, true};

    arc0.show();

    // Если поворот влево переворачиваем дугу
    if (turn_angle < 0)
        arc0.swap();

    //! Пересечения с отрезками.
    auto A1 = A + geometry::vector(1.5 * radius, geometry::angle(B - A));
    auto B1 = C + geometry::vector(3 * radius, geometry::angle(B - C));

    qDebug() << "A1: " << A1;
    qDebug() << "B1: " << B1;

    path.add(arc0);

    Arc arc1 = calculateTangentiallyArc(A, A1, arc0.start(), radius, speed1);
    Arc arc2 = calculateTangentiallyArc(arc0.end(), B1, C, radius, speed2);
    path.add(arc2);
    path.add(arc1);

    auto v_seg01 = geometry::vector(arc1.end(), arc0.start());
    auto v_seg02 = geometry::vector(arc2.start(), arc0.end()); // TODO: все верно, так как в обратном направлении.

    qDebug() << "Направление биссектрисы: " << geometry::normalize(bisect) << "="
             << geometry::angle(bisect) * RAD_TO_DEG;
    qDebug() << "Направление 1: " << geometry::angle(v_seg01) * RAD_TO_DEG;
    qDebug() << "Направление 2: " << geometry::angle(v_seg02) * RAD_TO_DEG;

    // Углы доворота от биссектрисы
    auto bisect_seg01 = geometry::angle_between(bisect, v_seg01);
    auto bisect_seg02 = geometry::angle_between(bisect, v_seg02);

    qDebug() << "Угол от биссектрисы к отрезку 1: " << bisect_seg01 * RAD_TO_DEG;
    qDebug() << "Угол от биссектрисы к отрезку 2: " << bisect_seg02 * RAD_TO_DEG;

    // Левый угол добавляем к началу, правый к концу
    if (turn_angle < 0)
    {
        bisect_seg01 *= -1, bisect_seg02 *= -1;
    }

    arc0.addDegreesToStart(bisect_seg01 * RAD_TO_DEG);
    arc0.addDegreesToEnd(bisect_seg02 * RAD_TO_DEG);

    auto seg01 = Segment{arc1.end(), arc0.start(), speed1};
    auto seg02 = Segment{arc0.end(), arc2.start(), speed2};

    auto begin_seg = Segment{A, arc1.start(), speed1};

    // TODO: Второй отрезок передаем дальше в расчет
    auto end_seg = Segment{arc2.end(), C, speed2};

    // TODO: Можем передавать скорость дальше, проверять и делать разгон, если она не соответствует. Интересно на это посмотреть.

    // ! Распределить участки разгона
    path.add(begin_seg);

    auto arcs1 = arc1.down(begin_seg.speed(), turn_velocity, split_factor);
    std::cout << "Скорость после первой дуги: " << arcs1.back().speed() << std::endl;
    path.add(arcs1);

    auto segs1 = seg01.upper(arcs1.back().speed(), speed1, split_factor);
    std::cout << "Скорость после первого отрезка: " << segs1.back().speed() << std::endl;
    path.add(segs1);

    //! Разбить с торможением
    auto arcs0 = arc0.down(segs1.back().speed(), turn_velocity, split_factor);
    std::cout << "Скорость после дуги разворота: " << arcs0.back().speed() << std::endl;
    path.add(arcs0);

    auto segs2 = seg02.upper(arcs0.back().speed(), speed2, split_factor);
    std::cout << "Скорость после первого отрезка: " << segs2.back().speed() << std::endl;
    path.add(segs2);

    auto arcs2 = arc2.down(segs2.back().speed(), turn_velocity, split_factor);
    std::cout << "Скорость после дуги разворота: " << arcs2.back().speed() << std::endl;
    path.add(arcs2);

    //TODO: Последний не заносим а передаем точки для расчета дальше
    A = arcs2.back().end();
    B = C;
    //    path.add(end_seg);

    return path;
}
} // namespace geometry