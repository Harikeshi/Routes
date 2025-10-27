#pragma once

#include <QRect>
#include <QTransform>

#include "Limits.hpp"

namespace Scene::Entities {
class CoordinateSystem
{
public:
    CoordinateSystem() = default;

    // TODO: 1 какой логический путь пройден за тик -> вернуть из функции update, далее реальный путь разделить на текущую скорость с ускорением и прибавить к общему времени
    // TODO: узнать сколько реально проходит с той или иной цифрой скорости
    // TODO: берем шаг в 0.1 минуты, считаем реально сколько проходит за такой шаг, далее откладываем на сегменте это расстояние в переводе на экранное расстояние
    // TODO: множитель увеличивает 0.1 на какое-то значение и рассчитывается с учетом этого множителя
    /*!
     * Перевод из экранных координат в реальные.
     * @param screen_pos
     * @return
     */
    QPointF toLogical(const QPointF& screen_pos) const
    {
        return transform.inverted().map(screen_pos);
    }

    /*!
     * Установка.
     * @param rect
     * @param limits
     */
    void setTransform(const QRectF& rect, const Limits& limits)
    {
        height = std::fabs(limits.maxY - limits.minY);
        width = std::fabs(limits.maxX - limits.minX);

        //TODO: для реализации отступов margin
        const auto contentRect = QRectF(
            0,
            0,
            rect.width(),
            rect.height());

        const auto scaleX = contentRect.width() / width;   //maxX - minX;
        const auto scaleY = contentRect.height() / height; //maxY - minY;

        transform = QTransform().translate(-limits.minX * scaleX, contentRect.height() + limits.minY * scaleY).scale(scaleX, -scaleY);
    }

    QTransform getTransform()
    {
        return transform;
    }
    double getHeight() const
    {
        return height;
    }

    double getWidth() const
    {
        return width;
    }

private:
    QTransform transform;

    double height;
    double width;
};
} // namespace Scene::Entities
