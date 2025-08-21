#pragma once

#include <QtCore/QPointF>
#include <QtCore/QVector>
#include <cmath>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

struct Segment {
    enum Type {
        Line, ArcChord
    } type;
    QPointF p0, p1;
    double speed = 0.0;
    double length = 0.0;
    double tStart = 0.0, tEnd = 0.0;
};

struct ShipPath {
    int shipId = -1;
    QVector<Segment> segments;
};

static constexpr double EPS = 1e-9;

static inline double vlen(const QPointF &v) { return std::hypot(v.x(), v.y()); }

static inline QPointF vnorm(const QPointF &v) {
    double L = vlen(v);
    return (L > EPS) ? QPointF(v.x() / L, v.y() / L) : QPointF(0, 0);
}

static inline double vdot(const QPointF &a, const QPointF &b) { return a.x() * b.x() + a.y() * b.y(); }

static inline double vcross(const QPointF &a, const QPointF &b) { return a.x() * b.y() - a.y() * b.x(); }

static inline QPointF vrot90(const QPointF &v) { return QPointF(-v.y(), v.x()); }

static inline double vang(const QPointF &v) { return std::atan2(v.y(), v.x()); }

struct TurnInfo {
    bool valid = false;
    int sign = 0;          // +1 left, -1 right
    double phi = 0.0;      // turn angle (0..pi)
    double R = 0.0;        // chosen centerline radius
    double tCenter = 0.0;  // trim on centerline
};

struct Input {
    int N = 0;
    double S = 100.0;
    double vLine = 0.0;
    bool hasBounds = false;
    double vMin = 0.0, vMax = 0.0;
    double eps = 0.1;
    QVector<QPointF> P; // polyline points
};

QVector<double> formationOffsets(int n, double spacing) {
    QVector<double> d;
    d.reserve(n);
    int mid = n - 1;
    for (int i = 0; i < n; ++i) {
        int k = 2 * i - mid; // -mid, -mid+2, ... +mid
        d.push_back(0.5 * k * spacing);
    }
    return d;
}

bool parseFirstLine(const std::string &line, Input &in) {
    std::istringstream ls(line);
    if (!(ls >> in.N >> in.S >> in.vLine)) return false;
    double vmin, vmax, eps;
    if (ls >> vmin >> vmax) {
        in.hasBounds = true;
        in.vMin = vmin;
        in.vMax = vmax;
        if (ls >> eps) in.eps = eps;
    }
    return true;
}

bool readInput(Input &in, std::istream &is) {
    std::string line;
    do {
        if (!std::getline(is, line)) return false;
        // trim
        auto notspace = [](unsigned char c) { return !std::isspace(c); };
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), notspace));
        line.erase(std::find_if(line.rbegin(), line.rend(), notspace).base(), line.end());
    } while (line.empty());
    if (!parseFirstLine(line, in)) return false;

    int M = 0;
    if (!(is >> M)) return false;
    if (M < 2) return false;
    in.P.resize(M);
    for (int i = 0; i < M; i++) {
        double x, y;
        if (!(is >> x >> y)) return false;
        in.P[i] = QPointF(x, y);
    }
    return true;
}

TurnInfo planTurnAt(const QPointF &A, const QPointF &B, const QPointF &C,
                    double Dmax, bool hasBounds, double vMin, double vMax) {
    TurnInfo tinfo{};
    QPointF uIn = vnorm(B - A);
    QPointF uOut = vnorm(C - B);
    double lenIn = vlen(B - A);
    double lenOut = vlen(C - B);
    if (vlen(uIn) < EPS || vlen(uOut) < EPS) return tinfo;
    double cosPhi = std::clamp(-vdot(uIn, uOut), -1.0, 1.0);
    double phi = std::acos(cosPhi);
    if (phi < 1e-6) return tinfo; // straight
    int sign = (vcross(uIn, uOut) > 0) ? +1 : -1;

    double tanHalf = std::tan(phi / 2.0);

    // Upper bound by available trimming for the outermost ship:
    // t_outer = (R + Dmax) * tan(phi/2) <= min(lenIn, lenOut)
    double RmaxLen = (std::min(lenIn, lenOut) / std::max(tanHalf, 1e-12)) - Dmax;

    // Lower bounds
    double RminPos = Dmax + 1e-6; // inner radius stays positive
    double RminSpeed = 0.0;
    if (hasBounds && vMax > vMin + 1e-9) {
        // to keep feasible common time interval
        RminSpeed = Dmax * (vMin + vMax) / (vMax - vMin);
    }
    double Rneed = std::max(RminPos, RminSpeed);

    // Choose R as large as allowed (smoother turn), but >= Rneed
    if (!(RmaxLen > RminPos + 1e-9)) {
        // Not enough length to insert valid fillet for all ships
        return tinfo;
    }
    double R = std::max(Rneed, std::min(RmaxLen, 1e12));

    tinfo.valid = true;
    tinfo.sign = sign;
    tinfo.phi = phi;
    tinfo.R = R;
    tinfo.tCenter = R * tanHalf;
    return tinfo;
}

bool intersectLines(const QPointF &P1, const QPointF &d1, const QPointF &P2, const QPointF &d2, QPointF &X) {
    double D = vcross(d1, d2);
    if (std::abs(D) < 1e-12) return false;
    double s = vcross(P2 - P1, d2) / D;
    X = P1 + d1 * s;
    return true;
}

// Discretize arc into chords with arrow error <= eps; with safeguards
QVector<QPointF> discretizeArc(const QPointF &C, double R, double angStart, double angDelta, double eps) {
    const int Nmax = 2000;        // upper bound
    const double epsMin = 1e-3;   // avoid absurdly small arrow
    eps = std::max(eps, epsMin);

    double phi = std::abs(angDelta);
    int N = 1;
    if (R > EPS) {
        double term = std::max(0.0, 1.0 - eps / R);
        term = std::min(1.0, term);
        double denom = 2.0 * std::acos(term);
        if (denom > 1e-9) N = std::min(Nmax, std::max(1, (int) std::ceil(phi / denom)));
        else N = 1;
    }
    QVector<QPointF> pts;
    pts.reserve(N + 1);
    for (int i = 0; i <= N; i++) {
        double a = angStart + angDelta * (double(i) / double(N));
        pts.push_back(C + QPointF(std::cos(a), std::sin(a)) * R);
    }
    return pts;
}

#include <memory>

int main(int argc, char **argv) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Input: from file if provided, else stdin
    std::unique_ptr<std::istream> ifs;
    std::istream *inStr = &std::cin;
    if (argc >= 2) {
        ifs.reset(new std::ifstream(argv[1]));
        if (!ifs->good()) {
            std::cerr << "Cannot open input file: " << argv[1] << "\n";
            return 1;
        }
        inStr = ifs.get();
    }

    Input in;
    if (!readInput(in, *inStr)) {
        std::cerr << "Invalid or incomplete input.\n";
        std::cerr << "Expected: N S v_line [v_min v_max] [eps], then M and M points.\n";
        return 1;
    }

    int N = in.N;
    if (N < 2 || N > 5) {
        std::cerr << "N must be between 2 and 5.\n";
        return 1;
    }
    double S = in.S;
    double vLine = in.vLine;
    bool hasBounds = in.hasBounds;
    double vMin = in.vMin, vMax = in.vMax;
    double eps = in.eps;
    const auto &P = in.P;

    int M = P.size();
    int nSeg = M - 1;
    if (nSeg <= 0) {
        std::cerr << "Polyline must have at least 2 points.\n";
        return 1;
    }

    // Offsets for ships (centered around 0)
    QVector<double> d = formationOffsets(N, S);
    double Dmax = 0.0;
    for (double di: d) Dmax = std::max(Dmax, std::abs(di));

    // Precompute per-segment unit directions and left normals
    QVector<QPointF> u(nSeg), nLeft(nSeg);
    for (int k = 0; k < nSeg; k++) {
        u[k] = vnorm(P[k + 1] - P[k]);
        if (vlen(u[k]) < EPS) {
            std::cerr << "Zero-length segment at index " << k << "\n";
            return 1;
        }
        nLeft[k] = vrot90(u[k]);
    }

    // Plan turns at each vertex
    QVector<TurnInfo> turns(M, TurnInfo{});
    for (int j = 1; j <= M - 2; j++) {
        turns[j] = planTurnAt(P[j - 1], P[j], P[j + 1], Dmax, hasBounds, vMin, vMax);
    }

    // Per-turn per-ship arc speed and trim length
    struct ArcParam {
        bool valid = false;
        double Ri = 0, ti = 0, vArc = 0;
    };
    QVector<QVector<ArcParam>> arcParams(M, QVector<ArcParam>(N));

    for (int j = 1; j <= M - 2; j++) {
        const auto &tinfo = turns[j];
        if (!tinfo.valid) continue;

        double phi = tinfo.phi;
        // Radii and arc lengths per ship
        QVector<double> Ri(N), si(N);
        for (int i = 0; i < N; i++) {
            double Ri_i = tinfo.R + tinfo.sign * d[i];
            if (Ri_i <= 1e-6) Ri_i = 1e-6; // safeguard
            Ri[i] = std::abs(Ri_i);
            si[i] = Ri[i] * phi;
        }
        // Common turn time
        double sCenter = si[N / 2];
        double Tstar = sCenter / std::max(vLine, 1e-9);
        double Tturn = Tstar;
        if (hasBounds && vMax > vMin + 1e-9) {
            double sMax = *std::max_element(si.begin(), si.end());
            double sMin = *std::min_element(si.begin(), si.end());
            double Tlo = sMax / std::max(vMax, 1e-9);
            double Thi = sMin / std::max(vMin, 1e-9);
            if (Tlo <= Thi) Tturn = std::min(std::max(Tstar, Tlo), Thi);
        }
        for (int i = 0; i < N; i++) {
            ArcParam ap;
            ap.valid = true;
            ap.Ri = Ri[i];
            ap.ti = Ri[i] * std::tan(phi / 2.0);
            ap.vArc = si[i] / std::max(Tturn, 1e-9);
            arcParams[j][i] = ap;
        }
    }

    // Build paths
    QVector<ShipPath> ships(N);
    for (int i = 0; i < N; i++) {
        ships[i].shipId = i;
        double tAcc = 0.0;

        for (int k = 0; k < nSeg; k++) {
            // Offsets of endpoints for this segment
            QPointF Q0 = P[k] + nLeft[k] * d[i];
            QPointF Q1 = P[k + 1] + nLeft[k] * d[i];

            // Trims at start/end due to prev/next turns
            double tStartTrim = 0.0, tEndTrim = 0.0;
            if (k > 0 && turns[k].valid) {
                if (arcParams[k][i].valid) tStartTrim = arcParams[k][i].ti;
            }
            if (k < nSeg - 1 && turns[k + 1].valid) {
                if (arcParams[k + 1][i].valid) tEndTrim = arcParams[k + 1][i].ti;
            }

            QPointF L0 = Q0 + u[k] * tStartTrim;
            QPointF L1 = Q1 - u[k] * tEndTrim;

            // Straight segment (if any)
            double Llen = vlen(L1 - L0);
            if (Llen > 1e-6) {
                Segment s;
                s.type = Segment::Line;
                s.p0 = L0;
                s.p1 = L1;
                s.length = Llen;
                s.speed = vLine;
                s.tStart = tAcc;
                s.tEnd = tAcc + Llen / std::max(vLine, 1e-9);
                ships[i].segments.push_back(s);
                tAcc = s.tEnd;
            }

            // Arc after this segment?
            if (k < nSeg - 1 && turns[k + 1].valid) {
                int j = k + 1; // vertex index B = P[j]
                const auto &tinfo = turns[j];
                const auto &ap = arcParams[j][i];
                if (!ap.valid) continue;

                QPointF uIn = u[k];
                QPointF uOut = u[k + 1];
                int sign = tinfo.sign;

                // Tangency points for this ship
                QPointF T1i = P[j] + nLeft[k] * d[i] - uIn * ap.ti;
                QPointF T2i = P[j] + nLeft[k + 1] * d[i] + uOut * ap.ti;

                // Center via offset lines intersection
                QPointF nInToC = vrot90(uIn) * sign;   // points toward center
                QPointF nOutToC = vrot90(uOut) * sign;
                QPointF P1 = T1i + nInToC * ap.Ri;
                QPointF d1 = uIn;
                QPointF P2 = T2i + nOutToC * ap.Ri;
                QPointF d2 = uOut;
                QPointF Cc;
                if (!intersectLines(P1, d1, P2, d2, Cc)) {
                    // degenerate; skip arc
                    continue;
                }

                double angStart = vang(T1i - Cc);
                double angEnd = vang(T2i - Cc);
                double angDelta = std::remainder(angEnd - angStart, 2 * M_PI); // (-pi, pi]

                if (sign > 0 && angDelta <= 0) angDelta += 2 * M_PI;
                if (sign < 0 && angDelta >= 0) angDelta -= 2 * M_PI;

                double target = tinfo.phi;
                if (std::abs(angDelta) > target) angDelta = (sign > 0) ? +target : -target;

                // Discretize arc
                QVector<QPointF> pts = discretizeArc(Cc, ap.Ri, angStart, angDelta, eps);
                for (int m = 1; m < pts.size(); ++m) {
                    QPointF A = pts[m - 1], B = pts[m];
                    double len = vlen(B - A);
                    if (len <= 1e-9) continue;
                    Segment s;
                    s.type = Segment::ArcChord;
                    s.p0 = A;
                    s.p1 = B;
                    s.length = len;
                    s.speed = ap.vArc;
                    s.tStart = tAcc;
                    s.tEnd = tAcc + len / std::max(ap.vArc, 1e-9);
                    ships[i].segments.push_back(s);
                    tAcc = s.tEnd;
                }
            }
        }
    }

    // Output
    std::cout.setf(std::ios::fixed);
    std::cout.precision(6);
    for (int i = 0; i < N; i++) {
        std::cout << "Ship " << i << "\n";
        std::cout << ships[i].segments.size() << "\n";
        for (const auto &s: ships[i].segments) {
            std::cout << (s.type == Segment::Line ? "Line " : "ArcChord ")
                      << s.p0.x() << " " << s.p0.y() << " "
                      << s.p1.x() << " " << s.p1.y() << " "
                      << s.speed << " " << s.length << " "
                      << s.tStart << " " << s.tEnd << "\n";
        }
    }
    return 0;
}
