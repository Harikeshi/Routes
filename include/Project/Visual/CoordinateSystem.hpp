#pragma once

#include <QRect>
#include <QTransform>

#include "Models/Limits.hpp"
#include "Models/Perimeter.hpp"

class CoordinateSystem
{
public:
    // QTransform transform;
    CoordinateSystem() = default;

    // CoordinateSystem(const Limits& limits)
    //     : minX{limits.minX}, minY{limits.minY}, maxX{limits.maxX}, maxY{limits.maxY}
    // {
    // }

    // TODO: 1 какой логический путь пройден за тик -> вернуть из функции апдейт, далее реальный путь разделить на текущую скорость с ускорением и прибавить к общему времени
    // TODO: узнать сколько реально проходит с той или иной цифрой скорости
    // TODO: берем шаг в 0.1 минуты, считаем реально сколько проходит за такой шаг, далее откладываем на сегменте это расстояние в переводе на экранное расстояние
    // TODO: множитель увеличивает 0.1 на какое-то значение и рассчитывается с учетом этого множителя

    // void setLimits(const double miX, const double maX, const double miY, const double maY)
    // {
    //     minX = miX;
    //     maxX = maX;
    //     minY = miY;
    //     maxY = maY;
    // }

    // void setLimits(const std::pair<double, double> mins, const std::pair<double, double> maxs)
    // {
    //     minX = mins.first;
    //     minY = mins.second;
    //     maxX = maxs.first;
    //     maxY = maxs.second;
    // }

    QPointF toLogical(const QPointF& screen_pos) const
    {
        return transform.inverted().map(screen_pos);
    }

    void setTransform(const QRectF& rect, const Limits& limits)
    {
        double height = std::fabs(limits.maxY - limits.minY);
        double width = std::fabs(limits.maxX - limits.minX);

        //TODO: для реализации отступов margin
        QRectF contentRect = QRectF(
            0,
            0,
            rect.width(),
            rect.height());

        auto scaleX = contentRect.width() / width;   //maxX - minX;
        auto scaleY = contentRect.height() / height; //maxY - minY;

        qDebug() << "------------------------------------------------------------";
        qDebug() << "Данные ширины: " << contentRect.width() << "(" << width << ")";
        qDebug() << "Данные высоты: " << contentRect.height() << "(" << height << ")";
        qDebug() << "Множитель по Х: " << scaleX << ", множитель по Y: " << scaleY;

        transform = QTransform().translate(-limits.minX * scaleX, contentRect.height() + limits.minY * scaleY).scale(scaleX, -scaleY);
    }

    QTransform getTransform()
    {
        return transform;
    }

    // double getScaleX() const
    // {
    //     return scaleX;
    // }

    // double getScaleY() const
    // {
    //     return scaleY;
    // }

private:
    QTransform transform;

    // double scaleX;
    // double scaleY;

    // double minX;
    // double minY;
    // double maxX;
    // double maxY;
};
