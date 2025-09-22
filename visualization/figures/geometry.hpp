#pragma once

#include "../config.hpp"

#include <cmath>
#include <iostream>

#include <QPointF>
#include <QtMath>

namespace geometry {

/*!
 * Пересечение прямой и окружности.
 * TODO: Использовать нельзя, слишком большая погрешность когда одна точка.
 * @param P0
 * @param P1
 * @param center
 * @param radius
 * @return
 */
inline std::vector<QPointF> intersection(const QPointF& P0, const QPointF& P1, const QPointF& center, double radius)
{
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
    else if (std::abs(discriminant) < 1e-9)
    {
        // == 0
        double t = -B / (2 * A);

        result.emplace_back(P0.x() + t + dx, P0.y() + t * dy);
    }
    else
    {
        double sqrtD = std::sqrt(discriminant);
        double t1 = (-B + sqrtD) / (2 * A);
        double t2 = (-B - sqrtD) / (2 * A);

        result.emplace_back(P0.x() + t1 * dx, P0.y() + t1 * dy);
        result.emplace_back(P0.x() + t2 * dx, P0.y() + t2 * dy);
    }

    return result;
}

/*!
 * Длина вектора.
 * @param point
 * @return
 */
inline double distance(const QPointF& point)
{
    return std::hypot(point.x(), point.y());
}

/*!
 * Угол между векторами.
 * @param A
 * @param B
 * @return
 */
inline double angle(const QPointF& A, const QPointF& B)
{
    return std::atan2(B.y() - A.y(), B.x() - A.x());
}

inline double dot(const QPointF& A, const QPointF& B)
{
    return A.x() * B.x() + A.y() * B.y();
}

inline QPointF vector(const QPointF& from, const QPointF& to)
{
    return QPointF{to.x() - from.x(), to.y() - from.y()};
}

inline double cross(const QPointF& a, const QPointF& b)
{
    return a.x() * b.y() - a.y() * b.x();
}

inline double normalize(double radians)
{
    while (radians < 0)
        radians += 2 * M_PI;
    while (radians >= 2 * M_PI)
        radians -= 2 * M_PI;

    return radians;
}

inline QPointF normalize(const QPointF& v)
{
    double len = std::hypot(v.x(), v.y());

    if (len == 0)
        return {0, 0};

    return {v.x() / len, v.y() / len};
}

inline double angle_between(const QPointF& a, const QPointF& b)
{
    normalize(a);
    normalize(b);

    // TODO: Перестановить a<->b
    return std::atan2(cross(b, a), dot(b, a));
}

inline double angle(const QPointF& A, const QPointF& B, const QPointF& C)
{
    auto a = vector(B, A);
    auto b = vector(B, C);

    return std::atan2(cross(a, b), dot(a, b));
}

inline double angle(const QPointF& A)
{
    return std::atan2(A.x(), A.y());
}

inline double distance(const QPointF& A, const QPointF& B)
{
    return std::hypot(B.x() - A.x(), B.y() - A.y());
}

inline QPointF vector(const QPointF& p, double distance, double radians)
{
    double x = p.x() + distance * sin(radians);
    double y = p.y() + distance * cos(radians);

    return {std::fabs(x) < EPS ? 0 : x, std::fabs(y) < EPS ? 0 : y};
}

inline QPointF vector(double distance, double radians)
{
    return {std::fabs(distance * sin(radians)) < EPS ? 0 : (distance * sin(radians)),
            std::fabs(distance * cos(radians)) < EPS ? 0 : distance * cos(radians)};
}

inline bool intersect(const QPointF& A, const QPointF& B, const QPointF& C, const QPointF& D)
{
    QPointF AB = vector(A, B);
    QPointF AC = vector(A, C);
    QPointF AD = vector(A, D);

    QPointF CD = vector(C, D);
    QPointF CA = vector(C, A);
    QPointF CB = vector(C, B);

    // Проверка с помощью векторных произведений
    double cross1 = cross(AB, AC);
    double cross2 = cross(AB, AD);
    double cross3 = cross(CD, CA);
    double cross4 = cross(CD, CB);

    // Отрезки пересекаются, если знаки произведений разные
    if (((cross1 * cross2) < 0) && ((cross3 * cross4) < 0))
    {
        return true;
    }

    // Дополнительная проверка, если отрезки коллинеарны или имеют общие вершины
    if (cross1 == 0 && cross2 == 0 && cross3 == 0 && cross4 == 0)
    {
        // Проверка наложения проекций
        if (qMax(A.x(), B.x()) < qMin(C.x(), D.x()) ||
            qMax(C.x(), D.x()) < qMin(A.x(), B.x()) ||
            qMax(A.y(), B.y()) < qMin(C.y(), D.y()) ||
            qMax(C.y(), D.y()) < qMin(A.y(), B.y()))
        {
            return false;
        }
        return true;
    }

    return false;
}

// Найти точку пересечения (если есть)
inline bool intersection(const QPointF& A, const QPointF& B, const QPointF& C, const QPointF& D, QPointF& intersection)
{
    if (!intersect(A, B, C, D))
    {
        return false;
    }

    // Параметрическое уравнение пересечения
    double denom = (A.x() - B.x()) * (C.y() - D.y()) - (A.y() - B.y()) * (C.x() - D.x());

    if (denom == 0)
    { // Отрезки параллельны или коллинеарны
        return false;
    }

    double t = ((A.x() - C.x()) * (C.y() - D.y()) - (A.y() - C.y()) * (C.x() - D.x())) / denom;
    intersection.setX(A.x() + t * (B.x() - A.x()));
    intersection.setY(A.y() + t * (B.y() - A.y()));

    return true;
}

inline QPointF project(const QPointF& A, const QPointF& B, const QPointF& P)
{
    QPointF AB = B - A; // Вектор AB
    QPointF AP = P - A; // Вектор AP

    double dot = AP.x() * AB.x() + AP.y() * AB.y();

    double lengthAB = AB.x() * AB.x() + AB.y() * AB.y();

    // A == B
    if (lengthAB == 0)
    {
        return A;
    }

    // Параметр t (если t < 0 → проекция вне отрезка ближе к A, если t > 1 → ближе к B)

    double t = dot / lengthAB;

    // Ограничиваем t, чтобы проекция была строго на отрезке (если нужно)
    // t = std::max(0.0, std::min(1.0, t));  // ← раскомментировать, если нужна проекция только на отрезке

    // Вычисляем проекцию P'
    QPointF projection = A + t * AB;
    return projection;
}

/*!
 * Проекция используя уравнение линии.
 * @param A
 * @param B
 * @param P
 * @return
 */
inline QPointF projection(const QPointF& A, const QPointF& B, const QPointF& P)
{
    auto AB = vector(A, B);
    auto AP = vector(A, P);

    double dot = AP.x() * AB.x() + AP.y() * AB.y();

    auto sq_len = AB.x() * AB.x() + AB.y() * AB.y();

    auto t = dot / sq_len;

    if (t < 0)
        std::cout << "Отрицательн." << std::endl; // Точка не лежит на отрезке.

    return QPointF{A.x() + t * AB.x(), A.y() + t * AB.y()};
}

inline int turnDirection(const QPointF& a, const QPointF& b)
{
    // Вправо
    if (cross(a, b) < 0)
    {
        //TODO: turnType::Right
        return 1;
    }
    // Влево
    else if (cross(a, b) > 0)
    {
        //TODO: turnType::Left
        return -1;
    }
    else
    {
        //TODO: turnType::UTurn
        return 0;
    }
}

// Прямые
inline QPointF intersection(const QPointF& A, const QPointF& B, const QPointF& C, const QPointF& D)
{
    auto a1 = B.y() - A.y();
    auto b1 = A.x() - B.x();
    auto c1 = a1 * A.x() + b1 * A.y();

    auto a2 = D.y() - C.y();
    auto b2 = C.x() - D.x();
    auto c2 = a2 * C.x() + b2 * C.y();

    double det = a1 * b2 - a2 * b1;

    if (det == 0)
    {
        throw std::runtime_error("Прямые параллельны.");
    }

    return {(c1 * b2 - c2 * b1) / det, (a1 * c2 - a2 * c1) / det};
}

inline QPointF intersection(const QPointF& center, double radius, const QPointF& dir)
{
    // Направление от центра к точке dir
    double dx = dir.x() - center.x();
    double dy = dir.y() - center.y();

    auto vec = normalize(vector(center, dir));

    return vector(center, radius, angle(vec));
}
// Отражение точки P относительно прямой (O + t*v), v — единичный
// TODO: Проверить
/*!
 * Отражение нормали от прямой.
 * @param P
 * @param O
 * @param v
 * @return
 */
inline QPointF reflectAcrossLine(const QPointF& P, const QPointF& O, const QPointF& v)
{
    QPointF n = {-v.y(), v.x()}; // единичная нормаль, т.к. v — unit
    QPointF r = P - O;
    double pv = geometry::dot(r, v);
    double pn = geometry::dot(r, n);
    return O + v * pv - n * pn;
}

/*!
 * Сдвиг вектора на угол.
 * @param a
 * @param ang
 * @return
 */
inline QPointF shift(const QPointF& a, double ang)
{
    return {a.x() * cos(ang) - a.y() * sin(ang), a.x() * sin(ang) + a.y() * cos(ang)};
}

// Находим точку A на линии l: A(t) = A0 + t*v (v — unit), такую что |A - O| = L.
// Возвращает флаг успеха и выбранную A. Берём «переднее» решение t = -proj + sqrt(...)
inline bool placeAlongLineAtDistanceFromO(const QPointF& O, const QPointF& A0, const QPointF& v, double L, QPointF& Aout)
{
    QPointF w0 = A0 - O;
    double proj = geometry::dot(w0, v);
    double d2 = fmax(0.0, geometry::dot(w0, w0) - proj * proj); // перпендикуляр до O

    if (L * L + 1e-12 < d2)
        return false; // нет решения

    double s = sqrt(fmax(0.0, L * L - d2));
    double t = -proj + s; // выбираем «вперёд» по v
    Aout = A0 + v * t;

    return true;
}

/*!
 * Проверка точки на луче.
 * @param O
 * @param u_unit
 * @param A
 * @param angleDeg
 * @param Q
 * @return
 */
inline bool pointOnRayByAngle(const QPointF& O, const QPointF& u_unit, const QPointF& A, double radians, QPointF& Q)
{
    // Направления d1,d2 от Q к A (то есть Q = A - t*d, t >= 0), образующие нужный угол с u
    QPointF d1 = shift(u_unit, +radians);
    QPointF d2 = shift(u_unit, -radians);

    auto tryDir = [&](const QPointF& d, QPointF& Qcand) -> bool {
        double det = cross(d, u_unit);
        if (fabs(det) < 1e-12)
            return false; // параллельны — решения нет
        QPointF r = A - O;
        // Решаем r = t*d + s*u => t = cross(r,u)/cross(d,u), s = cross(d,r)/cross(d,u)
        double t = cross(r, u_unit) / det;
        double s = cross(d, r) / det;
        if (t >= -1e-12 && s >= -1e-12)
        { // допускаем маленькие погрешности
            if (t < 0)
                t = 0;
            if (s < 0)
                s = 0;
            Qcand = O + u_unit * s;
            return true;
        }
        return false;
    };

    QPointF Q1, Q2;
    bool ok1 = tryDir(d1, Q1);
    bool ok2 = tryDir(d2, Q2);

    if (!ok1 && !ok2)
        return false;
    if (ok1 && !ok2)
    {
        Q = Q1;
        return true;
    }
    if (!ok1 && ok2)
    {
        Q = Q2;
        return true;
    }

    // Если обе годятся — берём ближнюю к A
    double d1len = distance(A - Q1);
    double d2len = distance(A - Q2);
    Q = (d1len <= d2len ? Q1 : Q2);

    return true;
}

inline QPointF bisector(const QPointF& A, const QPointF& B, const QPointF& C, bool inner = true)
{
    QPointF a, b;
    if (inner)
    {
        a = A - B;
        b = C - B;
    }
    else
    {
        a = B - A;
        b = B - C;
    }
    auto norm_a = normalize(a);
    auto norm_b = normalize(b);

    auto bis = normalize(norm_a + norm_b); //! Направление биссектрисы

    return bis;
}
} // namespace geometry