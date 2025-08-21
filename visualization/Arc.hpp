//#pragma once
//
//#include <cmath>
//
//#include <QPointF>
//
//using namespace std;
//
//static inline double dot(const QPointF& a, const QPointF& b)
//{
//    return a.x() * b.x() + a.y() * b.y();
//}
//
//static inline double norm2(const QPointF& a)
//{
//    return dot(a, a);
//}
//
//static inline double norm(const QPointF& a)
//{
//    return sqrt(norm2(a));
//}
//
//static inline QPointF perp(const QPointF& a)
//{
//    return {-a.y(), a.x()};
//}
//
//static inline QPointF normalize(const QPointF& a)
//{
//    double n = norm(a);
//    if (n == 0)
//        return {0, 0};
//    return a / n;
//}
//
//constexpr double PI = 3.141592653589793238462643383279502884;
//constexpr double TAU = 2.0 * PI;
//
//static inline double wrap2pi(double a)
//{
//    a = fmod(a, TAU);
//    if (a < 0)
//        a += TAU;
//    return a;
//}
//
//static inline double angleAt(const QPointF& C, const QPointF& P)
//{
//    return atan2(P.y() - C.y(), P.x() - C.x());
//}
//
//struct Arc
//{
//    QPointF A, B;     // концы дуги
//    QPointF C;        // центр окружности
//    double R{0};      // радиус
//    int s{+1};        // направление: +1 CCW (против час.), -1 CW (по час.)
//    bool minor{true}; // true — малая дуга, false — большая
//};
//
//struct Centers
//{
//    QPointF C1, C2;
//    bool ok{false};
//};
//
//// вычислить два возможных центра окружности радиуса R через A и B
//Centers circleCentersFromChord(const QPointF& A, const QPointF& B, double R, double eps = 1e-12)
//{
//    Centers res;
//    QPointF AB = B - A;
//    double d = norm(AB);
//    if (d < eps)
//        return res; // вырождение
//    if (d > 2 * R + eps)
//        return res; // не существует
//    QPointF M = (A + B) * 0.5;
//    double half = 0.5 * d;
//    double h2 = R * R - half * half;
//    if (h2 < 0)
//        h2 = 0; // численно
//    double h = sqrt(h2);
//    QPointF nperp = normalize(perp(AB));
//    res.C1 = M + nperp * h;
//    res.C2 = M - nperp * h;
//    res.ok = true;
//    return res;
//}
//
//// ориентированный угловой ход дуги (в [0, 2π))
//static inline double arcDelta(const QPointF& C, const QPointF& A, const QPointF& B, int s)
//{
//    double tA = angleAt(C, A);
//    double tB = angleAt(C, B);
//    return wrap2pi(s * (tB - tA));
//}
//
//// выбрать центр по желаемой малой/большой дуге и направлению
//// если minor==true, выбираем центр с Δ ≤ π, иначе с Δ > π
//// если оба равнозначны из-за численных погрешностей — берём C1
//bool chooseCenter(const QPointF& A, const QPointF& B, double R, int s, bool minor, QPointF& Cchosen, double* outDelta = nullptr, double eps = 1e-12)
//{
//    Centers cs = circleCentersFromChord(A, B, R, eps);
//    if (!cs.ok)
//        return false;
//
//    double d1 = arcDelta(cs.C1, A, B, s);
//    double d2 = arcDelta(cs.C2, A, B, s);
//
//    bool c1minor = (d1 <= PI + 1e-12);
//    bool c2minor = (d2 <= PI + 1e-12);
//
//    if (minor)
//    {
//        if (c1minor && (!c2minor || d1 <= d2 + 1e-12))
//        {
//            Cchosen = cs.C1;
//            if (outDelta)
//                *outDelta = d1;
//            return true;
//        }
//        if (c2minor)
//        {
//            Cchosen = cs.C2;
//            if (outDelta)
//                *outDelta = d2;
//            return true;
//        }
//        // если оба "большие" численно — возьмём меньший
//        if (d1 <= d2)
//        {
//            Cchosen = cs.C1;
//            if (outDelta)
//                *outDelta = d1;
//            return true;
//        }
//        else
//        {
//            Cchosen = cs.C2;
//            if (outDelta)
//                *outDelta = d2;
//            return true;
//        }
//    }
//    else
//    {
//        if (!c1minor && (c2minor || d1 >= d2 - 1e-12))
//        {
//            Cchosen = cs.C1;
//            if (outDelta)
//                *outDelta = d1;
//            return true;
//        }
//        if (!c2minor)
//        {
//            Cchosen = cs.C2;
//            if (outDelta)
//                *outDelta = d2;
//            return true;
//        }
//        // если оба "малые" — возьмём больший
//        if (d1 >= d2)
//        {
//            Cchosen = cs.C1;
//            if (outDelta)
//                *outDelta = d1;
//            return true;
//        }
//        else
//        {
//            Cchosen = cs.C2;
//            if (outDelta)
//                *outDelta = d2;
//            return true;
//        }
//    }
//}
//
//// собрать определение дуги
//bool makeArc(const QPointF& A, const QPointF& B, double R, int s, bool minor, Arc& arc, double eps = 1e-12)
//{
//    QPointF C;
//    double Delta = 0;
//    if (!chooseCenter(A, B, R, s, minor, C, &Delta, eps))
//        return false;
//    arc = Arc{A, B, C, R, s, minor};
//    return true;
//}
//
//// проверка: точка X лежит на дуге (с допуском)
//bool pointOnArc(const Arc& arc, const QPointF& X, double angTol = 1e-12)
//{
//    // принадлежность окружности по радиусу проверять не здесь
//    double tA = angleAt(arc.C, arc.A);
//    double tB = angleAt(arc.C, arc.B);
//    double tX = angleAt(arc.C, X);
//    double Delta = wrap2pi(arc.s * (tB - tA));
//    double deltaX = wrap2pi(arc.s * (tX - tA));
//    return (deltaX >= -angTol && deltaX <= Delta + angTol);
//}
//#include <vector>
//// пересечение дуги с отрезком P-Q
//std::vector<QPointF> intersectArcWithSegment(const Arc& arc, const QPointF& P, const QPointF& Q, double eps = 1e-12, double mergeTol = 1e-9)
//{
//    std::vector<QPointF> out;
//    QPointF v = Q - P;
//    double a = dot(v, v);
//    if (a < eps)
//    {
//        // вырожденный отрезок — точка
//        QPointF w = P - arc.C;
//        double rr = norm2(w);
//        if (fabs(rr - arc.R * arc.R) <= max(1.0, arc.R) * 1e-9)
//        {
//            if (pointOnArc(arc, P))
//                out.push_back(P);
//        }
//        return out;
//    }
//    QPointF w = P - arc.C;
//    double b = 2.0 * dot(w, v);
//    double c = dot(w, w) - arc.R * arc.R;
//    double D = b * b - 4 * a * c;
//
//    if (D < -1e-14)
//        return out;
//    if (D < 0)
//        D = 0;
//
//    auto tryAdd = [&](double t) {
//        if (t < -1e-12 || t > 1 + 1e-12)
//            return;
//        t = min(1.0, max(0.0, t)); // clamp лёгкий
//        QPointF S = P + v * t;
//        // проверка принадлежности дуге
//        if (!pointOnArc(arc, S))
//            return;
//        // слить дубль (касание)
//        for (const auto& E : out)
//        {
//            if (norm(S - E) <= mergeTol)
//                return;
//        }
//        out.push_back(S);
//    };
//
//    double sqrtD = sqrt(D);
//    double t1 = (-b - sqrtD) / (2 * a);
//    double t2 = (-b + sqrtD) / (2 * a);
//    tryAdd(t1);
//    if (sqrtD > 0)
//        tryAdd(t2);
//    return out;
//}
//
//// разбиение дуги на N равных частей (возвращает точки i=0..N)
//vector<QPointF> splitArc(const Arc& arc, int N)
//{
//    vector<QPointF> Pts;
//    if (N <= 0)
//        return Pts;
//    double tA = angleAt(arc.C, arc.A);
//    double tB = angleAt(arc.C, arc.B);
//    double Delta = wrap2pi(arc.s * (tB - tA));
//    double alpha = Delta / N;
//    Pts.reserve(N + 1);
//    for (int i = 0; i <= N; ++i)
//    {
//        double ang = tA + arc.s * (alpha * i);
//        QPointF p{arc.C.x() + arc.R * cos(ang),
//                  arc.C.y() + arc.R * sin(ang)};
//        Pts.push_back(p);
//    }
//    return Pts;
//}