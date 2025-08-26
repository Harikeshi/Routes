#pragma once

#include "../config.hpp"
#include "Element.hpp"
#include "geometry.hpp"

#include <QDebug>

class Arc : public Element
{
    QPointF A;
    QPointF B;
    QPointF C;
    // TODO: radius можно рассчитать.

    double _speed;
    double radius;

    bool _clockwise; // Направление обхода. TODO: Определять при построении

public:
    Arc(const QPointF& s, const QPointF& e, const QPointF& c, double sp, bool cw)
        : A(s), B(e), C(c), _speed(sp), _clockwise(cw)
    {
        radius = hypot(s.x() - c.x(), s.y() - c.y());
    }

    // TODO: Точка должна быть на дуге
    void setEnd(const QPointF& pos)
    {
        B = pos;
    }

    void setStart(const QPointF& pos)
    {
        A = pos;
    }

    void setClockwise(bool value)
    {
        _clockwise = value;
    }

    static Arc
    calculateTangentiallyArc(const QPointF& A, const QPointF& B, const QPointF& C, double radius, double speed = 5)
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

    static Arc
    createFromAngle(const QPointF& start, const QPointF& center, double radians, bool clockwise, double speed)
    {
        double radius = std::hypot(start.x() - center.x(), start.y() - center.y());

        // TODO: sin и cos поменять местами
        QPointF end{center.x() + radius * std::cos(radians), center.y() + radius * std::sin(radians)};

        return Arc{start, end, center, speed, clockwise};
    }

    QPointF setEnd(double offset)
    {
        // TODO: необходимо проверить
        double radians = _clockwise ? (startAngle() - endAngle()) : (endAngle() - startAngle());
        double angle = radians + offset;

        // нормализация в диапазон 0–2π
        while (angle < 0)
            angle += 2.0 * M_PI;
        while (angle >= 2.0 * M_PI)
            angle -= 2.0 * M_PI;

        // TODO: sin и cos поменять местами
        QPointF end{C.x() + radius * std::sin(angle), C.y() + radius * std::cos(angle)};

        B = end;

        return end;
    }

    double calculateAngle(const QPointF& point) const
    {
        double dx = point.x() - C.x();
        double dy = point.y() - C.y();

        return std::atan2(dy, dx) * 180.0 / M_PI; // в градусах
    }

    QPointF calculatePointOnCircle(double angleDegrees) const
    {
        double angleRadians = angleDegrees * M_PI / 180.0;
        QPointF result{C.x() + radius * std::cos(angleRadians), C.y() + radius * std::sin(angleRadians)};

        return result;
    }

    void addDegreesToStart(double degrees)
    {
        double currentAngle = calculateAngle(A);
        double newAngle = currentAngle + (_clockwise ? -degrees : degrees);

        A = calculatePointOnCircle(newAngle);
    }

    // Метод для добавления градусов в конец дуги
    void addDegreesToEnd(double degrees)
    {
        double currentAngle = calculateAngle(B);
        double newAngle = currentAngle + (_clockwise ? -degrees : degrees);

        B = calculatePointOnCircle(newAngle);
    }

    QPointF setStart(double offset)
    {
        // TODO: необходимо проверить
        double radians = _clockwise ? (startAngle() - endAngle()) : (endAngle() - startAngle());
        double angle = radians + offset;

        // нормализация в диапазон 0–2π
        while (angle < 0)
            angle += 2.0 * M_PI;
        while (angle >= 2.0 * M_PI)
            angle -= 2.0 * M_PI;

        // TODO: sin и cos поменять местами
        QPointF end{C.x() + radius * std::cos(angle), C.y() + radius * std::sin(angle)};

        B = end;

        return end;
    }

    void show() const override
    {
        qDebug() << "Arc: A" << A << ", B" << B << ", C" << C << ", speed: " << _speed;
    }

    /*!
     * Разбить на сегменты.
     * @param segments количество сегментов.
     * @return
     */
    std::vector<Arc> split(int segments)
    {
        std::vector<Arc> result;

        double a1 = atan2(A.y() - C.y(), A.x() - C.x());
        double a2 = atan2(B.y() - C.y(), B.x() - C.x());

        if (_clockwise)
        {
            if (a2 < a1)
                a2 += 2 * M_PI;
        }
        else
        {
            if (a1 < a2)
                a1 += 2 * M_PI;
        }

        double angle_diff = _clockwise ? (a2 - a1) : (a1 - a2);
        double step = angle_diff / segments;

        auto start = A;

        for (int i = 0; i <= segments; ++i)
        {
            double angle = _clockwise ? (a1 + i * step) : (a1 - i * step);
            QPointF end{C.x() + radius * cos(angle), C.y() + radius * sin(angle)};

            result.emplace_back(start, end, C, _speed, _clockwise);
            start = end;
        }

        return result;
    }

    bool contains(const QPointF& point, double eps = 1e-3) const
    {
        // Расстояние до центра
        double point_center_range = qSqrt(qPow(point.x() - C.x(), 2) + qPow(point.y() - C.y(), 2));

        if (qAbs(radius - point_center_range) > eps)
            return false;

        //! Угол должен быть между start->end
        double angStart = geometry::normalize(qAtan2(A.y() - C.y(), A.x() - C.x()));
        double angEnd = geometry::normalize(qAtan2(B.y() - C.y(), B.x() - C.x()));
        double angPoint = geometry::normalize(qAtan2(point.y() - C.y(), point.x() - C.x()));

        // Проверяем, лежит ли угол в диапазоне дуги
        if (_clockwise)
        {
            if (angStart < angEnd)
                angStart += 2 * M_PI;
            if (angPoint < angEnd)
                angPoint += 2 * M_PI;

            return angPoint <= angStart && angPoint >= angEnd;
        }
        else
        {
            if (angEnd < angStart)
                angEnd += 2 * M_PI;
            if (angPoint < angStart)
                angPoint += 2 * M_PI;

            return angPoint >= angStart && angPoint <= angEnd;
        }
    }

    QPointF shift(const QPointF& from, double range)
    {
        if (range > length())
        {
            throw std::runtime_error("Расстояние больше длины дуги!");
        }

        if (!contains(from))
            throw std::runtime_error("Точка не лежит на дуге!");

        double angleFrom = qAtan2(from.y() - C.y(), from.x() - C.x());
        double deltaAngle = range / radius;

        if (_clockwise)
            angleFrom -= deltaAngle;
        else
            angleFrom += deltaAngle;

        // TODO: x*sin y*cos
        QPointF result(
            C.x() + radius * qCos(angleFrom),
            C.y() + radius * qSin(angleFrom));

        //std::cout << result.x() << ", " << result.y() << std::endl;
        //
        //        if (!contains(result))
        //        {
        //            this->show();
        //            qDebug() << result;
        //            throw std::runtime_error("Полученная точка выходит за границы дуги!");
        //        }

        return result;
    }

    /*!
     * Смещение на dx, dy
     * @param dx
     * @param dy
     */
    void translate(double dx, double dy)
    {
        A.setX(A.x() + dx);
        A.setY(A.y() + dy);

        B.setX(B.x() + dx);
        B.setY(B.y() + dy);

        C.setX(C.x() + dx);
        C.setY(C.y() + dy);
    }

    /*!
 * Метод разбиения дуги на дуги, с понижением скорости до заданной с понижением.
 * @param v0 начальная скорость.
 * @param v1 конечная скорость.
 * @param number количество частей.
 * @return
 */
    std::vector<Arc> down(double from, double to, size_t number)
    {
        std::vector<Arc> result;
        //std::cout << "Начальная скорость: " << from << ", конечная скорость: " << to << std::endl;
        // 1. найдем требуемое расстояние
        auto range = (from * from - to * to) / (2 * deceleration);
        auto speed = std::sqrt(std::fabs(from * from - range * 2 * deceleration)); // TODO: требуется только для тестов
        //std::cout << "Длина дуги: " << length_() << ", требуемое расстояние: " << range << std::endl;

        if (range >= length_())
        {
            //std::cout << "range >= length" << std::endl;
            range = length_();
        }
        // Разбить на части и вернуть.
        auto step = range / (number);
        std::cout << step << std::endl;
        auto sh = shift(A, range);
        //std::cout << "Расчетная точка: [" << sh.x() << ", " << sh.y() << "], расчетная скорость: " << speed
        //        << std::endl;
        //        if (range >= length()) {
        //            number -= 1;
        //        }

        QPointF start = A;
        for (size_t i = 0; i != number; ++i)
        {
            auto end = shift(start, step);
            auto to = std::sqrt(std::fabs(from * from - step * 2 * deceleration));
            //std::cout << "Начало: [" << start.x() << "," << start.y() << "], конец: " << end.x() << ","
            //        << end.y() << "], скорость: " << to << ", смещение: " << step + i * step << std::endl;
            result.emplace_back(start, end, C, to, _clockwise);
            from = to;
            start = end;
        }

        // TODO: Последний участок
        if (range < length_())
            result.emplace_back(result.back().B, B, C, to, _clockwise);

        return result;
    }

    // TODO: Проверить
    QPointF move(double time) const override
    {
        // Углы
        double ang0 = std::atan2(A.y() - C.y(), A.x() - C.x());
        double ang1 = std::atan2(B.y() - C.y(), B.x() - C.x());

        ang0 = geometry::normalize(ang0);
        ang1 = geometry::normalize(ang1);

        // TODO: тоже часто считаем
        double angle_full = ang1 - ang0;
        if (_clockwise && angle_full > 0)
            angle_full -= 2 * M_PI;
        if (!_clockwise && angle_full < 0)
            angle_full += 2 * M_PI;

        double length = std::abs(radius * angle_full);
        double sign = _clockwise ? -1.0 : 1.0;

        // Пройденное расстояние по дуге
        double distance = _speed * time;
        // TODO: Должны проверять выше.
        if (distance > length)
            distance = length; // ограничение по длине дуги

        double angle_offset = (distance / radius) * sign;
        double theta = ang0 + angle_offset;

        return {
            C.x() + radius * std::cos(theta),
            C.y() + radius * std::sin(theta)};
    }

    // TODO: категорически проверять
    std::vector<Arc> split(double v0, double v1, int n)
    {
        std::vector<Arc> result;

        // TODO: Выделить в отдельную функцию?
        double ang0 = std::atan2(A.y() - C.y(), A.x() - C.x());
        double ang1 = std::atan2(B.y() - C.y(), B.x() - C.x());

        ang0 = geometry::normalize(ang0);
        ang1 = geometry::normalize(ang1);

        double angle_full = ang1 - ang0;
        if (_clockwise && angle_full > 0)
            angle_full -= 2 * M_PI;
        if (!_clockwise && angle_full < 0)
            angle_full += 2 * M_PI;

        double length = std::abs(radius * angle_full);
        double a = (v1 > v0) ? acceleration : -deceleration;

        double segLength = length / n;
        double angle_step = angle_full / n;
        double currentV = v0;
        double currentAngle = ang0;

        QPointF currentStart = A;

        for (int i = 0; i < n; ++i)
        {
            double nextV = std::sqrt(std::max(0.0, currentV * currentV + 2 * a * segLength));
            if ((a > 0 && nextV > v1) || (a < 0 && nextV < v1))
                nextV = v1;

            double nextAngle = currentAngle + angle_step;
            QPointF nextEnd{
                C.x() + radius * std::cos(nextAngle),
                C.y() + radius * std::sin(nextAngle)};

            result.emplace_back(currentStart, nextEnd, C, _clockwise, currentV);

            currentStart = nextEnd;
            currentAngle = nextAngle;
            currentV = nextV;
        }

        return result;
    }

    double length() const
    {
        double angle = _clockwise ? (startAngle() - endAngle()) : (endAngle() - startAngle());

        if (angle < 0)
            angle += 2 * M_PI;

        return radius * angle;
    }

    double length_()
    {
        double ang1 = qAtan2(A.y() - C.y(), A.x() - C.x());
        double ang2 = qAtan2(B.y() - C.y(), B.x() - C.x());

        // нормализация углов в диапазон [0, 2π)
        auto norm = [](double a) {
            while (a < 0)
                a += 2 * M_PI;
            while (a >= 2 * M_PI)
                a -= 2 * M_PI;
            return a;
        };

        ang1 = norm(ang1);
        ang2 = norm(ang2);

        double delta;
        if (_clockwise)
        {
            if (ang1 < ang2)
                ang1 += 2 * M_PI;
            delta = ang1 - ang2;
        }
        else
        {
            if (ang2 < ang1)
                ang2 += 2 * M_PI;
            delta = ang2 - ang1;
        }

        return radius * delta; // длина дуги
    }

    void draw(QPainter& painter) override
    {
        // Вычисляем углы (в градусах от оси X по часовой стрелке в координатах Qt)
        double startAngleDeg = -qRadiansToDegrees(qAtan2(A.y() - C.y(),
                                                         A.x() - C.x()));
        double endAngleDeg = -qRadiansToDegrees(qAtan2(B.y() - C.y(),
                                                       B.x() - C.x()));

        // Вычисляем дугу (в шестнадцатых долях градуса для drawArc)
        double spanDeg;
        if (_clockwise)
        {
            spanDeg = fmod((endAngleDeg - startAngleDeg + 360), 360);
        }
        else
        {
            spanDeg = -(fmod((startAngleDeg - endAngleDeg + 360), 360));
        }

        QRectF rect(C.x() - radius, C.y() - radius, 2 * radius, 2 * radius);

        painter.drawArc(rect, startAngleDeg * 16, spanDeg * 16);
    }

    bool clockwise() const
    {
        return _clockwise;
    }

    QPointF center() const
    {
        return C;
    }

    QPointF start() const override
    {
        return A;
    }

    QPointF end() const override
    {
        return B;
    }

    double speed() const override
    {
        return _speed;
    }

    void swap()
    {
        std::swap(A, B);
        _clockwise = !_clockwise;
    }

    Arc(Arc&&) = default;

    double startAngle() const
    {
        return atan2(C.x() - A.x(), C.y() - A.y());
        //        return atan2(A.x() - C.x(), A.y() - C.y());
    }

    double endAngle() const
    {
        return atan2(C.x() - B.x(), C.y() - B.y());
        //        return atan2(B.x() - C.x(), B.y() - C.y());
    }
};
