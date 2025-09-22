#pragma once

#include <QtWidgets>

class Capsules : public QWidget
{
    Q_OBJECT
public:
    Capsules(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        R_ = 14.0;
        QVector<QPointF> pts_ = {
            {40, 80}, {200, 120}, {280, 60}, {380, 160}, {520, 130}};

        setMinimumSize(800, 400);
    }

public:
    void setPoints(const QVector<QPointF>& pts)
    {
        pts_ = pts;
    }

    void setRadius(double radius)
    {
        R_ = radius;
    }

    void draw(QPainter& p)
    {
        p.save();

        p.setRenderHint(QPainter::Antialiasing, true);

        // Левая половина: stroker
        {
            QPainterPath center;
            center.moveTo(pts_[0]);
            for (int i = 1; i < pts_.size(); ++i)
                center.lineTo(pts_[i]);

            QPainterPathStroker s;
            s.setWidth(2 * R_);
            s.setCapStyle(Qt::RoundCap);
            s.setJoinStyle(Qt::RoundJoin);
            QPainterPath thick = s.createStroke(center);

            p.translate(0, 0);
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(240, 90, 70, 200));
            p.drawPath(thick);

            QPen guide(Qt::blue, 1.5, Qt::DashLine);
            guide.setCosmetic(true);
            p.setPen(guide);
            p.setBrush(Qt::NoBrush);
            p.drawPath(center);
        }
        p.restore();

        p.save();
        // Правая половина: capsules (для сравнения)
        {
            p.translate(0, height() / 2);
            QPainterPath thick = makeThickPathCapsules(pts_, R_);
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(90, 160, 240, 180));
            p.drawPath(thick);

            QPainterPath center;
            center.moveTo(pts_[0]);
            for (int i = 1; i < pts_.size(); ++i)
                center.lineTo(pts_[i]);
            QPen guide(Qt::darkGreen, 1.5, Qt::DashLine);
            guide.setCosmetic(true);
            p.setPen(guide);
            p.setBrush(Qt::NoBrush);
            p.drawPath(center);
        }
        p.restore();
    }

    QPainterPath makeCapsule(const QPointF& a, const QPointF& b, qreal R)
    {
        QPainterPath path;
        if (a == b)
        {
            // Дегенерат: просто круг
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

        auto addHalfCircle = [&](QPainterPath& t, const QPointF& c, const QPointF& from, const QPointF& to) {
            // Построим локальный bbox окружности
            QRectF rc(c.x() - R, c.y() - R, 2 * R, 2 * R);
            // Вычислим углы в градусах
            auto angleDeg = [](const QPointF& p) { return std::atan2(-p.y(), p.x()) * 180.0 / M_PI; };

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
        }
        {
            QPainterPath capA;
            addHalfCircle(capA, a, aR, aL); // направление противоположное
            res = res.united(capA);
        }
        return res.simplified();
    }

    QPainterPath makeThickPathCapsules(const QVector<QPointF>& pts, qreal R)
    {
        QPainterPath result;
        for (int i = 0; i + 1 < pts.size(); ++i)
        {
            QPainterPath seg = makeCapsule(pts[i], pts[i + 1], R);
            result = result.isEmpty() ? seg : result.united(seg);
        }
        return result.simplified();
    }

private:
    qreal R_;
    QPainterPath unifiedPath_;
    std::vector<QPainterPath> capsules_;
    QVector<QPointF> pts_;
};
