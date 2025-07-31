#pragma once

#include <QPainter>
#include <QWidget>

namespace Scene::Objects {
/*!
 * Класс отображения точки поворота пути.
 */
class PointWidget final : public QWidget // Оставляем Widget так как требуется интерактив
{
    Q_OBJECT
public:
    explicit PointWidget(const QPointF& point, double radius, const QColor& color, QWidget* parent = nullptr)
        : real{point}, innerRadius{radius}, fillColor{color}, QWidget(parent)
    {
    }
    void draw(QPainter& painter, size_t number)
    {
        painter.save();

        // Внутренний эллипс (заливка)
        painter.setPen(Qt::NoPen);
        painter.setBrush(fillColor);
        painter.drawEllipse(real, innerRadius, innerRadius);

        painter.restore();
    }

public:
    double x() const
    {
        return real.x();
    }

    double y() const
    {
        return real.y();
    }

    QPointF getPoint() const
    {
        return real;
    }

    double getRadius() const
    {
        return innerRadius;
    }

    void setRadius(double radius)
    {
        innerRadius = radius;
    }

    void setColor(const QColor& color)
    {
        fillColor = color;
    }

    void swapCoordinates()
    {
        auto x = real.x();

        real.setX(real.y());
        real.setY(x);
    }

private:
    double innerRadius;

    QPointF real; // Реальные координаты.

    // TODO:
    QPointF global; // Глобальные координаты.

    QColor fillColor = Qt::black;
};
} // namespace Scene::Objects