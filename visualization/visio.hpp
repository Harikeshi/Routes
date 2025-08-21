#pragma once

#include <QtCore>
#include <QtMath>

struct Formation
{
    QVector<QPointF> figure; // Точки фигуры

    size_t count;
    double width;
    double height;

public:
    virtual void init(size_t size, double width, double height) = 0;

    virtual QVector<QPointF> real(const QPointF& center) = 0;

    virtual void show() = 0;
};

struct Linier : public Formation
{
    void init(size_t size, double width, double height) override
    {
        if (size <= 0)
            throw std::runtime_error("Задайте количество кораблей.");

        if (size == 1)
        {
            figure.push_back({0, 0});

            return;
        }

        auto step = width / (size - 1);

        // TODO: Добавить наклон
        auto half = width / 2;

        figure.push_back({-half, 0});

        for (size_t i = 1; i < size - 1; ++i)
        {
            figure.push_back({-half + step * i, 0});
        }

        figure.push_back({+half, 0});
    }

    void show() override
    {
        for (const auto& point : figure)
        {
            qDebug() << point;
        }
    }

    QVector<QPointF> real(const QPointF& center) override
    {
        QVector<QPointF> points;

        for (const auto& point : figure)
        {
            points.push_back(center + point);
        }

        return points;
    }
};

static inline double segmentLength(const QPointF& a, const QPointF& b)
{
    const QPointF d = b - a;
    return std::hypot(d.x(), d.y());
}

static inline double directionAngle(const QPointF& a, const QPointF& b)
{
    const QPointF d = b - a;
    return std::atan2(d.y(), d.x());
}

static inline QPointF rotate(const QPointF& v, double theta)
{
    const double c = std::cos(theta), s = std::sin(theta);
    return QPointF(c * v.x() - s * v.y(), s * v.x() + c * v.y());
}

// Возвращает траекторию для каждой точки фигуры.
// offsets: смещения точек от центра (в локальной системе фигуры).
// path: полилиния движения центра.
// arcStep: шаг дискретизации по дуге (в тех же единицах, что и path).
QVector<QVector<QPointF>>
buildPointTrajectories(const QVector<QPointF>& offsets,
                       const QVector<QPointF>& path,
                       bool alignOrientation,
                       double arcStep = 1.0,
                       double fixedAngle = 0.0)
{
    QVector<QVector<QPointF>> result;
    result.resize(offsets.size());
    if (path.size() < 2 || offsets.isEmpty())
        return result;

    // Предварительно добавим стартовую точку
    const QPointF start = path.front();
    double currentAngle = fixedAngle;

    // Для фиксированной ориентации заранее повернём смещения
    QVector<QPointF> rotatedOffsets = offsets;
    if (!alignOrientation)
    {
        for (int k = 0; k < offsets.size(); ++k)
            rotatedOffsets[k] = rotate(offsets[k], fixedAngle);
    }

    for (int seg = 0; seg + 1 < path.size(); ++seg)
    {
        const QPointF A = path[seg];
        const QPointF B = path[seg + 1];
        const double L = segmentLength(A, B);
        if (L <= 0.0)
            continue;

        // Угол сегмента
        const double segAngle = directionAngle(A, B);

        // Кол-во шагов на сегмент
        const int steps = qMax(1, int(std::ceil(L / arcStep)));

        for (int i = 0; i <= steps; ++i)
        {
            const double t = (steps == 0) ? 0.0 : double(i) / double(steps);
            const QPointF C = A + t * (B - A);

            // Актуальные повернутые смещения
            if (alignOrientation)
            {
                currentAngle = segAngle;
                for (int k = 0; k < offsets.size(); ++k)
                {
                    const QPointF off = rotate(offsets[k], currentAngle);
                    const QPointF p = C + off;
                    // Избегаем дубликатов на границах: добавлять всё, кроме (i==0 && seg>0)
                    if (!(i == 0 && seg > 0))
                        result[k].push_back(p);
                }
            }
            else
            {
                for (int k = 0; k < offsets.size(); ++k)
                {
                    const QPointF p = C + rotatedOffsets[k];
                    if (!(i == 0 && seg > 0))
                        result[k].push_back(p);
                }
            }
        }
    }

    return result;
}