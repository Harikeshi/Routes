#pragma once

#include "Objects/SegmentObject.hpp"
#include <QtWidgets>

class Capsule {
public:
    static QPainterPath make(const QPointF &a, const QPointF &b, qreal R) {
        QPainterPath path;
        if (a == b) {
            //! TODO: Вырожденная капсула просто окружность
            path.addEllipse(a, R, R);
            return path;
        }

        // Вектор отрезка и его нормаль
        const QPointF v = b - a;
        const qreal L = std::hypot(v.x(), v.y());
        const QPointF dir = QPointF(v.x() / L, v.y() / L);
        const QPointF n(-dir.y(), dir.x()); // нормаль влево

        // Четыре угла прямоугольника
        const QPointF aL = a + n * R;
        const QPointF aR = a - n * R;
        const QPointF bL = b + n * R;
        const QPointF bR = b - n * R;

        // Тело прямоугольника
        path.moveTo(aL);
        path.lineTo(bL);
        path.lineTo(bR);
        path.lineTo(aR);
        path.closeSubpath();

        // Полуокружность у B (центральный угол вдоль нормали)
        {
            QPainterPath cap;
            cap.moveTo(bL);
            // дуга рисуется проще через addEllipse + вычитание, но сделаем дугу явно
            // Сформируем дугу через addEllipse + интерсекцию: однако QPainterPath не дает сегмент дуги напрямую с центром/углом.
            // Практичней: используем addEllipse и потом ограничимся секторовой дугой через arcTo на ограничивающем прямоугольнике.
        }
        // В Qt удобнее нарисовать капы дугами:
        // Построим дугу через arcTo на прямоугольнике, описывающем окружность радиуса R.

        auto addHalfCircle = [&](QPainterPath &t, const QPointF &c, const QPointF &from, const QPointF &to) {
            // Построим локальный bbox окружности
            QRectF rc(c.x() - R, c.y() - R, 2 * R, 2 * R);
            // Вычислим углы в градусах
            auto angleDeg = [](const QPointF &p) { return std::atan2(-p.y(), p.x()) * 180.0 / M_PI; };

            qreal a1 = angleDeg(from - c);
            qreal a2 = angleDeg(to - c);

            // Направление положительное (по часовой? в Qt угол растет против часовой стрелки)
            // Хотим полукруг слева направо через внешнюю сторону: берём кратчайшую дугу 180°
            // В нашем случае from=aL->bL и bR->aR соответствуют нормали; проще задать явно:
            // Но чтобы без путаницы, используем QPainterPath::arcMoveTo/arcTo.

            // Пойдём от from к to против часовой. Сдвинем t в from:
            t.moveTo(from);
            // Найдём стартовый угол прямоугольника rc, ближайший к from
            // Упростим: вычислим угол центра->from и используем arcTo на 180°
            qreal start = std::atan2(-(from.y() - c.y()), (from.x() - c.x())) * 180.0 / M_PI;
            t.arcTo(rc, start, 180.0);
        };

        QPainterPath res = path;

        // Полуокружности:
        {
            QPainterPath capB;
            addHalfCircle(capB, b, bL, bR);
            res = res.united(capB);
        } {
            QPainterPath capA;
            addHalfCircle(capA, a, aR, aL); // направление противоположное
            res = res.united(capA);
        }
        return res.simplified();
    }
};

class CapsulesPath : public QWidget {
    Q_OBJECT

public:
    CapsulesPath(QWidget *parent = nullptr)
        : QWidget(parent) {
    }

    //! Перенести
    void rebuildPath(const QVector<Scene::Objects::SegmentObject *> &segments, size_t index, bool cur = true) {
        unifiedPath_.clear();

        if (cur) {
            for (size_t i = 0; i < index; ++i) {
                QPainterPath cap = Capsule::make(segments[i]->getStart(), segments[i]->getCurrentPoint(), R_);
                unifiedPath_ = unifiedPath_.united(cap);
            }
            unifiedPath_ = unifiedPath_.simplified();
            return;
        }

        for (const auto &segment: segments) {
            QPainterPath cap = Capsule::make(segment->getStart(), segment->getCurrentPoint(), R_);
            unifiedPath_ = unifiedPath_.united(cap);
        }

        unifiedPath_ = unifiedPath_.simplified();
    }

    void setRadius(double radius) {
        R_ = radius;
    }

    //! TODO: Подавать move в качестве аргумента
    void draw(QPainter &p) {
        p.save();

        p.setPen(Qt::NoPen);
        p.setBrush(QColor(100, 180, 240, 180));
        p.drawPath(unifiedPath_);

        p.restore();
    }

private:
    qreal R_;
    QPainterPath unifiedPath_;
};
