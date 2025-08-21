#pragma once

#include <QPointF>
#include <QVector>

#include <cmath>

struct Segment {
    enum Type {
        Line, ArcChord
    } type;
    QPointF p0, p1; // for Line or chord endpoints
    double speed;   // constant on this element
    double length;
    double tStart, tEnd;
};

struct ShipPath {
    int shipId;
    QVector<Segment> segments;
};

struct PolylineSpeed {
    QVector<QPointF> points;      // P0..Pn
    QVector<double> speeds;       // per segment i: Pi->Pi+1
};

struct Formation {
    QVector<double> offsets;      // d_i, i=0..M-1
    double filletRadius;          // R for center/reference
    double arcChordEps;           // epsilon for chord error
};

static inline QPointF normalize(const QPointF &v) {
    double L = std::hypot(v.x(), v.y());
    return (L > 0) ? QPointF(v.x() / L, v.y() / L) : QPointF(0, 0);
}

static inline double dot(const QPointF &a, const QPointF &b) { return a.x() * b.x() + a.y() * b.y(); }

static inline double cross(const QPointF &a, const QPointF &b) { return a.x() * b.y() - a.y() * b.x(); }

static inline QPointF rot90(const QPointF &v) { return QPointF(-v.y(), v.x()); }

struct Fillet {
    QPointF T1, T2;  // tangency points on centerline
    QPointF C;       // arc center for centerline
    double phi;      // turn angle (0..pi)
    int turnSign;    // +1 left, -1 right
    double t;        // trim length on centerline
};

Fillet computeFillet(const QPointF &A, const QPointF &B, const QPointF &C, double R) {
    QPointF uIn = normalize(B - A);
    QPointF uOut = normalize(C - B);
    double cosPhi = std::clamp(-dot(uIn, uOut), -1.0, 1.0); // interior turn angle
    double phi = std::acos(cosPhi);
    Fillet f{};
    f.phi = phi;
    if (phi < 1e-6) {
        f.turnSign = 0;
        return f;
    } // no turn
    f.turnSign = (cross(uIn, uOut) > 0) ? +1 : -1;
    double t = R * std::tan(phi / 2.0);
    f.t = t;
    QPointF T1 = B - uIn * t;
    QPointF T2 = B + uOut * t;
    // Center on bisector:
    QPointF nIn = rot90(uIn) * f.turnSign;   // points to center
    QPointF nOut = rot90(uOut) * f.turnSign; // points to center
    // Lines offset by R from the segments intersect at arc center
    // Compute intersection of lines: (through T1 along uIn shifted by nIn*R) and (through T2 along uOut shifted by nOut*R)
    QPointF P1 = T1 + nIn * R;
    QPointF d1 = uIn;
    QPointF P2 = T2 + nOut * R;
    QPointF d2 = uOut;
    // Solve P1 + d1*s = P2 + d2*t
    double D = cross(d1, d2);
    double s = cross(P2 - P1, d2) / (D == 0 ? 1e-12 : D);
    QPointF center = P1 + d1 * s;

    f.T1 = T1;
    f.T2 = T2;
    f.C = center;
    return f;
}

QVector<QPointF> offsetSegment(const QPointF &P0, const QPointF &P1, double d) {
    QPointF dir = normalize(P1 - P0);
    QPointF n = rot90(dir); // left normal
    return {P0 + n * d, P1 + n * d};
}

// Discretize arc for ship with radius Ri, angle phi, turnSign, from T1i to T2i
QVector<Segment> arcToChords(const QPointF &center, double Ri, double startAngle, double endAngle,
                             double eps, double v, double tStartAcc) {
    double phi = std::fabs(endAngle - startAngle);
    int N = std::max(1, (int) std::ceil(phi / (2 * std::acos(std::max(0.0, 1.0 - eps / std::max(1e-6, Ri))))));
    QVector<Segment> segs;
    double dt = (endAngle - startAngle) / N;
    double tStart = tStartAcc;
    QPointF prev = center + QPointF(std::cos(startAngle), std::sin(startAngle)) * Ri;
    for (int i = 1; i <= N; i++) {
        double ang = startAngle + dt * i;
        QPointF curr = center + QPointF(std::cos(ang), std::sin(ang)) * Ri;
        double len = std::hypot(curr.x() - prev.x(), curr.y() - prev.y());
        Segment s;
        s.type = Segment::Line;
        s.p0 = prev;
        s.p1 = curr;
        s.length = len;
        s.speed = v;
        s.tStart = tStart;
        s.tEnd = tStart + len / v;
        segs.push_back(s);
        tStart = s.tEnd;
        prev = curr;
    }
    return segs;
}
