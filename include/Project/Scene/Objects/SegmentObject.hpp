#pragma once

#include <QPainter>
#include <QPointF>
#include <QWidget>
#include <QPainterPath>
#include <QtMath>
#include "Project/Models/Segment.hpp"
#include "../Capsules.hpp"
#include "Project/Scene/Capsules.hpp"
#include "Project/Scene/Capsules.hpp"

namespace Scene::Objects {
    /*!
     * Класс отображения отрезка пути.
     */
    class SegmentObject final : public QObject {
        using Segment = Models::Segment;

    public:
        explicit SegmentObject(QObject *parent = nullptr, const Segment &segment = {})
            : QObject(parent),
              segment{segment},
              current{segment.getStart()} {
            this->segment = segment;
        }

        explicit SegmentObject(const QPointF &start, const QPointF &end, double speed, QObject *parent = nullptr)
            : segment{0, start, end, speed},
              current{start}, QObject(parent) {
        }

        void clear() {
            current = segment.getStart();
        }

        void initialize(const Segment &segment) {
            this->segment = segment;
            current = segment.getStart();
        }

        QPainterPath create(const QPointF &a, const QPointF &b, qreal R) {
            QPainterPath path;
            if (a == b) {
                path.addEllipse(a, R, R);
                return path;
            }
            QPointF v = b - a;
            qreal L = std::hypot(v.x(), v.y());
            QPointF dir(v.x() / L, v.y() / L);
            QPointF n(-dir.y(), dir.x());

            QPointF aL = a + n * R;
            QPointF aR = a - n * R;
            QPointF bL = b + n * R;
            QPointF bR = b - n * R;

            path.moveTo(aL);
            path.lineTo(bL);
            path.lineTo(bR);
            path.lineTo(aR);
            path.closeSubpath();

            QRectF arcRectA(a.x() - R, a.y() - R, 2 * R, 2 * R);
            QRectF arcRectB(b.x() - R, b.y() - R, 2 * R, 2 * R);

            QPainterPath capA;
            capA.moveTo(aR);
            capA.arcTo(arcRectA, std::atan2(-(aR.y() - a.y()), aR.x() - a.x()) * 180 / M_PI, 180);
            QPainterPath capB;
            capB.moveTo(bL);
            capB.arcTo(arcRectB, std::atan2(-(bL.y() - b.y()), bL.x() - b.x()) * 180 / M_PI, 180);

            path = path.united(capA);
            path = path.united(capB);

            return path;
        }

        void cDrawFull(QPainter &painter, double radius,
                       const QColor &color = QColor(100, 150, 255, 180)) {
            auto capsule = create(segment.start, segment.end, radius);

            painter.fillPath(capsule, color);
        }

        void cDrawCurrent(QPainter &painter, double radius, const QColor &color = QColor(100, 150, 255, 180)) {
            auto capsule = create(segment.start, current, radius);

            painter.fillPath(capsule, color);
        }

        void drawCurrent(QPainter &painter, const QColor &color) const {
            setPen(painter, color);
            painter.drawLine(segment.start, current);
        }

        void drawFull(QPainter &painter, const QColor &color) const {
            setPen(painter, color);

            painter.drawLine(segment.start, segment.end);
        }

        double getSpeed() const {
            return segment.getSpeed();
        }

        QPointF getEnd() const {
            return segment.getEnd();
        }

        QPointF getStart() const {
            return segment.getStart();
        }

        void show() const {
            segment.show();
        }

        double length() const {
            return segment.length();
        }

        double currentLength() const {
            return std::hypot(current.x() - segment.getStart().x(), current.y() - segment.getStart().y());
        }

        void setCurrentPoint(const QPointF &point) {
            current = point;
        }

        QPointF getCurrentPoint() const {
            return current;
        }

        double getCurrentTime() const {
            return currentLength() / segment.getSpeed();
        }

        double getFullTime() const {
            return segment.time();
        }

        Segment getSegment() const {
            return segment;
        }

        void setCurrent(const QPoint &curr) {
            current = curr;
        }

        void setSegment(const Segment &seg) {
            segment = seg;
        }

    private:
        void setPen(QPainter &painter, const QColor &color) const {
            QPen pen = QPen(color, 2);
            pen.setCosmetic(true);

            painter.setPen(pen);
        }

    public:
        void swapCoordinates() {
            segment.swapCoordinates();
            auto x = current.x();

            current.setX(current.y());
            current.setY(x);
        }

    private:
        Segment segment;

        QPointF current;
    };
} // namespace Scene::Objects
