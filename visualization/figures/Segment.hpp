#pragma once

#include "Element.hpp"
#include "../config.hpp"
#include "geometry.hpp"

#include <QDebug>

/*!
 * Дуги мы не можем изменять, чтобы придать ускорение или замедление объекту,
 * но можем поделить Дугу в зависимости от ускорения/замедления.
 * Отрезок можем использовать для составления отрезков для ускорения/замедления.
 */
class Segment : public Element {
    QPointF A;
    QPointF B;

    double _speed;

public:
    Segment(const QPointF &a = QPointF{}, const QPointF &b = QPointF{}, double s = 0)
            : Element(), A(a), B(b), _speed(s) {
    }

    bool contains(const QPointF &point, double eps = 1e-09) {
        // Векторные координаты
        QPointF AP = point - A;
        QPointF AB = B - A;

        // 1. Проверка коллинеарности: векторное произведение должно быть ≈ 0
        double cross = AP.x() * AB.y() - AP.y() * AB.x();
        if (qAbs(cross) > eps)
            return false;

        // 2. Проверка проекции: точка должна быть между a и b
        double dot = AP.x() * AB.x() + AP.y() * AB.y();
        if (dot < -eps)
            return false;

        double ab_len2 = AB.x() * AB.x() + AB.y() * AB.y();
        if (dot - ab_len2 > eps)
            return false;

        return true;
    }

    /*!
     * Смещение на отрезке от from на расстояние range.
     * @param from
     * @param to
     * @param range
     * @return
     */
    QPointF shift(const QPointF &from, double range) {
        if (range > length()) {
            throw std::runtime_error("Расстояние больше длины отрезка!");
        }

        if (!contains(from))
            throw std::runtime_error("Точка не лежит на отрезке!");

        auto direction = angle(B - A);

        QPointF result(geometry::vector(from, range, direction));

//        qDebug() << result;
        if (!contains(result)) {
            throw std::runtime_error("Полученная точка выходит за границы отрезка!");
        }

        return result;
    }

    std::vector<Segment> upper(double from, double to, size_t number) {
        std::vector<Segment> result;

        auto range = (to * to - from * from) / (2 * acceleration);

        if (range >= length()) {
            range = length();
        }

        QPointF start = A;
        auto step = range / (number);
        for (size_t i = 0; i != number; ++i) {
            auto end = shift(start, step);

            auto to = std::sqrt(std::fabs(from * from + step * 2 * acceleration));
            result.emplace_back(start, end, to);
            from = to;
            start = end;
        }
        // TODO: Последний участок
        if (range < length()) {
            result.emplace_back(result.back().B, B, to);
        }

        return result;
    }

    // TODO: Не тестировал толком, надо проверять
    std::vector<Segment> split(double v0, double v1, int n) {
        std::vector<Segment> result;

        //        double dx = original.end.x - original.start.x;
        //        double dy = original.end.y - original.start.y;
        auto AB = B - A;
        double L = std::hypot(AB.x(), AB.y());
        double a = (v1 > v0) ? acceleration : -deceleration;
        double L_target = (v1 * v1 - v0 * v0) / (2 * a);

        bool canReach = (a > 0) ? (L_target <= L) : (L_target <= L);

        double remainingLength = L;
        double currentV = v0;
        auto currentStart = A;

        // Единичный вектор направления движения
        auto norm_segm = AB / L;
        //        double ux = dx / L;
        //        double uy = dy / L;

        // Участок с изменением скорости
        double accelLength = canReach ? L_target : L;
        double segLength = accelLength / n;

        for (int i = 0; i < n; ++i) {
            double nextV = std::sqrt(std::max(0.0, currentV * currentV + 2 * a * segLength));
            if ((a > 0 && nextV > v1) || (a < 0 && nextV < v1))
                nextV = v1;

            QPointF nextEnd{currentStart.x() + norm_segm.x() * segLength,
                            currentStart.y() + norm_segm.y() * segLength};

            result.push_back({currentStart, nextEnd, currentV});

            currentStart = nextEnd;
            currentV = nextV;
            remainingLength -= segLength;
        }

        // Остаток с постоянной скоростью
        if (canReach && remainingLength > 1e-6) {
            segLength = remainingLength / n; // можно выбрать другое кол-во сегментов
            for (int i = 0; i < n; ++i) {
                QPointF nextEnd{currentStart.x() + norm_segm.x() * segLength,
                                currentStart.y() + norm_segm.y() * segLength};
                result.push_back({currentStart, nextEnd, v1});
                currentStart = nextEnd;
            }
        }

        return result;
    }

    // TODO: Требуется проверка
    QPointF move(double time) const override {
        double distance = time * _speed;
        return QPointF{A.x() + distance * sin(geometry::angle(B - A)), A.y() + distance * cos(geometry::angle(B - A))};
    }

    static std::vector<Segment>
    generateSpeedTransitionSegments(const QPointF &start, const QPointF &end, double v0, double v1, int n) {
        std::vector<Segment> result;

        double totalLength = geometry::distance(start, end);
        if (totalLength < 1e-9 || n <= 0)
            return result;

        // Направление движения
        double dx = (end.x() - start.x()) / totalLength;
        double dy = (end.y() - start.y()) / totalLength;

        // Длина одного сегмента
        double segLength = totalLength / n;

        // Знак ускорения
        double a = (v1 > v0) ? acceleration : -deceleration;

        double currentV = v0;
        QPointF currentStart = start;

        for (int i = 0; i < n; ++i) {
            // Расчёт скорости на следующем сегменте
            double nextV = std::sqrt(std::max(0.0, currentV * currentV + 2 * a * segLength));
            if ((a > 0 && nextV > v1) || (a < 0 && nextV < v1))
                nextV = v1;

            // Вычисление координат конца сегмента
            QPointF nextEnd{
                    currentStart.x() + dx * segLength,
                    currentStart.y() + dy * segLength};

            result.push_back({currentStart, nextEnd, currentV});

            currentStart = nextEnd;
            currentV = nextV;
        }

        return result;
    }

    void show() const override {
        qDebug() << "Segment: A" << A << ", B" << B << ", speed: " << _speed;
    }

    double length() const override {
        return std::hypot(A.x() - B.x(), A.y() - B.y());
    }

    void draw(QPainter &painter) override {
        painter.drawLine(A, B);
    }

    QPointF start() const override {
        return A;
    }

    QPointF end() const override {
        return B;
    }

    double speed() const override {
        return _speed;
    }
};