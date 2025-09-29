#pragma once

#include <QVector>

double pointToSegmentDistance(const QPointF &p, const QPointF &a, const QPointF &b) {
    double dx = b.x() - a.x();
    double dy = b.y() - a.y();
    if (dx == 0 && dy == 0)
        return std::hypot(p.x() - a.x(), p.y() - a.y());

    double t = ((p.x() - a.x()) * dx + (p.y() - a.y()) * dy) / (dx * dx + dy * dy);
    t = std::max(0.0, std::min(1.0, t));
    double projx = a.x() + t * dx;
    double projy = a.y() + t * dy;
    return std::hypot(p.x() - projx, p.y() - projy);
}

struct Circle {
    QPointF center;
    qreal radius;
    QRectF bounds;

    Circle(const QPointF &c, qreal r)
        : center(c), radius(r), bounds(c.x() - r, c.y() - r, 2 * r, 2 * r) {
    }

    bool contains(const QPointF &pt) const {
        if (!bounds.contains(pt)) return false;
        qreal dx = pt.x() - center.x();
        qreal dy = pt.y() - center.y();
        return dx * dx + dy * dy <= radius * radius;
    }
};

class SpatialGrid {
public:
    SpatialGrid(const QRectF &bounds, int cols, int rows)
        : bounds_(bounds), cols_(cols), rows_(rows) {
        grid_.resize(rows_);
        for (auto &row: grid_) row.resize(cols_);
    }

    void insert(const Circle &c, int index) {
        QRectF b = c.bounds;
        int x0 = clamp((b.left() - bounds_.left()) / bounds_.width() * cols_, 0, cols_ - 1);
        int x1 = clamp((b.right() - bounds_.left()) / bounds_.width() * cols_, 0, cols_ - 1);
        int y0 = clamp((b.top() - bounds_.top()) / bounds_.height() * rows_, 0, rows_ - 1);
        int y1 = clamp((b.bottom() - bounds_.top()) / bounds_.height() * rows_, 0, rows_ - 1);
        for (int y = y0; y <= y1; ++y)
            for (int x = x0; x <= x1; ++x)
                grid_[y][x].push_back(index);
    }

    std::vector<int> query(const QPointF &pt) const {
        int x = clamp((pt.x() - bounds_.left()) / bounds_.width() * cols_, 0, cols_ - 1);
        int y = clamp((pt.y() - bounds_.top()) / bounds_.height() * rows_, 0, rows_ - 1);
        return grid_[y][x];
    }

private:
    QRectF bounds_;
    int cols_, rows_;
    std::vector<std::vector<std::vector<int> > > grid_;

    static int clamp(double v, int min, int max) {
        return std::max(min, std::min(static_cast<int>(v), max));
    }
};

/*!
 * Матрица наложения галсов
 */
QVector<QVector<int> > buildMatrix(const QVector<QPointF> &polyline,
                                   double R, int gridW, int gridH,
                                   QRectF &bboxOut) {
    // bbox
    QRectF bbox(polyline[0], QSizeF(1, 1));
    for (auto &pt: polyline)
        bbox = bbox.united(QRectF(pt.x() - R, pt.y() - R, 2 * R, 2 * R));
    bboxOut = bbox;

    QVector<QVector<int> > matrix(gridH, QVector<int>(gridW, 0));
    double cellW = bbox.width() / gridW;
    double cellH = bbox.height() / gridH;

    for (int y = 0; y < gridH; ++y) {
        for (int x = 0; x < gridW; ++x) {
            QPointF pt(bbox.left() + (x + 0.5) * cellW,
                       bbox.top() + (y + 0.5) * cellH);

            int count = 0;
            for (size_t i = 0; i + 1 < polyline.size(); ++i) {
                double dist = pointToSegmentDistance(pt, polyline[i], polyline[i + 1]);
                if (dist <= R) ++count;
            }
            matrix[y][x] = count;
        }
    }
    return matrix;
}

struct Circle {
    QPointF center;
    qreal radius;
    QRectF bounds;

    Circle(const QPointF &c, qreal r)
        : center(c), radius(r), bounds(c.x() - r, c.y() - r, 2 * r, 2 * r) {
    }

    bool contains(const QPointF &pt) const {
        if (!bounds.contains(pt)) return false;
        qreal dx = pt.x() - center.x();
        qreal dy = pt.y() - center.y();
        return dx * dx + dy * dy <= radius * radius;
    }
};

class SpatialGrid {
public:
    SpatialGrid(const QRectF &bounds, int cols, int rows)
        : bounds_(bounds), cols_(cols), rows_(rows) {
        grid_.resize(rows_);
        for (auto &row: grid_) row.resize(cols_);
    }

    void insert(const Circle &c, int index) {
        QRectF b = c.bounds;
        int x0 = clamp((b.left() - bounds_.left()) / bounds_.width() * cols_, 0, cols_ - 1);
        int x1 = clamp((b.right() - bounds_.left()) / bounds_.width() * cols_, 0, cols_ - 1);
        int y0 = clamp((b.top() - bounds_.top()) / bounds_.height() * rows_, 0, rows_ - 1);
        int y1 = clamp((b.bottom() - bounds_.top()) / bounds_.height() * rows_, 0, rows_ - 1);
        for (int y = y0; y <= y1; ++y)
            for (int x = x0; x <= x1; ++x)
                grid_[y][x].push_back(index);
    }

    std::vector<int> query(const QPointF &pt) const {
        int x = clamp((pt.x() - bounds_.left()) / bounds_.width() * cols_, 0, cols_ - 1);
        int y = clamp((pt.y() - bounds_.top()) / bounds_.height() * rows_, 0, rows_ - 1);
        return grid_[y][x];
    }

private:
    QRectF bounds_;
    int cols_, rows_;
    std::vector<std::vector<std::vector<int> > > grid_;

    static int clamp(double v, int min, int max) {
        return std::max(min, std::min(static_cast<int>(v), max));
    }
};

/*!
 * Расчет матрицы по окружностям ГАС с заданным шагом.
 */
QVector<QVector<int> > buildMatrix(const QVector<QPointF> &polyline,
                                   double step, double R,
                                   int gridW, int gridH,
                                   int samplesPerCell,
                                   QRectF &bboxOut) {
    // bbox
    QRectF bbox(polyline[0], QSizeF(1, 1));
    for (auto &pt: polyline)
        bbox = bbox.united(QRectF(pt.x() - R, pt.y() - R, 2 * R, 2 * R));
    bboxOut = bbox;

    // spatial grid
    int gridCols = std::max(1, static_cast<int>(bbox.width() / (2.5 * R)));
    int gridRows = std::max(1, static_cast<int>(bbox.height() / (2.5 * R)));
    SpatialGrid grid(bbox, gridCols, gridRows);

    std::vector<QPointF> centers;
    for (size_t i = 0; i + 1 < polyline.size(); ++i) {
        QPointF A = polyline[i];
        QPointF B = polyline[i + 1];
        QPointF v = B - A;
        qreal L = std::hypot(v.x(), v.y());
        if (L < 1e-6) {
            centers.push_back(A);
            continue;
        }
        QPointF dir(v.x() / L, v.y() / L);
        int nSteps = static_cast<int>(std::floor(L / step));
        centers.push_back(A);
        for (int s = 1; s <= nSteps; ++s)
            centers.push_back(A + dir * (s * step));
        centers.push_back(B);
    }

    for (size_t i = 0; i < centers.size(); ++i)
        grid.insert(Circle(centers[i], R), static_cast<int>(i));

    QVector<QVector<int> > matrix(gridH, QVector<int>(gridW, 0.0));
    double cellW = bbox.width() / gridW;
    double cellH = bbox.height() / gridH;
    double stepX = cellW / samplesPerCell;
    double stepY = cellH / samplesPerCell;

    for (int y = 0; y < gridH; ++y) {
        for (int x = 0; x < gridW; ++x) {
            double sum = 0.0;
            for (int sy = 0; sy < samplesPerCell; ++sy) {
                for (int sx = 0; sx < samplesPerCell; ++sx) {
                    QPointF pt(bbox.left() + x * cellW + (sx + 0.5) * stepX,
                               bbox.top() + y * cellH + (sy + 0.5) * stepY);
                    int count = 0;
                    for (int idx: grid.query(pt)) {
                        QPointF c = centers[idx];
                        qreal dx = pt.x() - c.x();
                        qreal dy = pt.y() - c.y();
                        if (dx * dx + dy * dy <= R * R) ++count;
                    }
                    sum += count;
                }
            }
            matrix[y][x] = sum; // / (samplesPerCell * samplesPerCell);
        }
    }
    return matrix;
}
