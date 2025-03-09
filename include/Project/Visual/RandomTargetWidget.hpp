#pragma once

#include <QPointF>
#include <QPolygonF>
#include <QVector2D>
#include <QWidget>

#include "Models/Limits.hpp"
#include "PerimeterWidget.hpp"
#include "SegmentWidget.hpp"
// TODO: Добавить рандом таргет targets.push(point);

// TODO:
//    1. находим рандомную точку;
//    2. берем вектор к ней; QVector2D
//    3. находим точку на векторе относительно пройденного расстояния с соответствующей скорости
//    4. проверяем, что нет пересечений на пути;
//        - есть пересечения, прокладываем путь, до пересечения
//        - нет пересечения, принимаем путь
//    5. возвращаем новую точку.
namespace Visual {
class RandomTargetWidget : public QWidget
{
public:
    RandomTargetWidget(QWidget* parent = nullptr)
        : QWidget(parent), currentPosition{1e30, 1e30}
    {
    }

    int getRandomNumber(int min, int max)
    {
        return std::rand() % (max - min + 1) + min;
    }

    bool isPointInPolygon(const QPointF& point, const QPolygonF& polygon)
    {
        return polygon.containsPoint(point, Qt::OddEvenFill);
    }

    // TODO: Брать лимиты Limits
    QPointF generateRandomPoint(const PerimeterWidget& perimeter, const Models::Limits& limits)
    {
        bool isValidPoint = false;

        // TODO: проверка на размер периметра
        QPointF randomPoint;

        while (!isValidPoint)
        {
            randomPoint = QPointF(getRandomNumber(limits.minX, limits.maxX), getRandomNumber(limits.minY, limits.maxY));

            // Проверяем, находится ли точка внутри внешнего многоугольника
            if (isPointInPolygon(randomPoint, perimeter.getInners()[0]))
            {
                isValidPoint = true;

                // Проверяем, находится ли точка внутри любого из внутренних многоугольников
                if (perimeter.getInners().size() > 1)
                {
                    for (size_t i = 1; i != perimeter.getInners().size(); ++i)
                    {
                        if (isPointInPolygon(randomPoint, perimeter.getInners()[i]))
                        {
                            isValidPoint = false;
                            break;
                        }
                    }
                }
            }
        }

        return randomPoint;
    }

    // TODO: speed limits
    void movePoint(const PerimeterWidget& perimeter, const Models::Limits& limits, const double speedWithMultiplier)
    {
        // TODO: Передавать за время набрать путь с маленькими промежутками
        // TODO: или брать рандомное направление в диапазоне +- 15-30 градусов в первоначальном направлении
        // TODO: или идет в направлении и попадая своим радиусаом в радиус меняет
        //float speed = static_cast<float>(getRandomNumber(10, 18)) / 1.0f; // Случайная скорость в диапазоне 10-18

        auto next = generateRandomPoint(perimeter, limits);

        QVector2D direction{next.x() - currentPosition.x(), next.y() - currentPosition.y()};
        direction.normalize();

        QVector2D step = direction * (speedWithMultiplier);
        QPointF newPoint = currentPosition + step.toPointF();

        if (isPointInPolygon(newPoint, perimeter.getInners()[0]))
        {
            bool insideInnerPolygon = false;

            if (perimeter.getInners().size() > 1)
                for (size_t i = 1; i != perimeter.getInners().size(); ++i)
                {
                    if (isPointInPolygon(newPoint, perimeter.getInners()[i]))
                    {
                        insideInnerPolygon = true;
                        break;
                    }
                }

            if (!insideInnerPolygon)
            {
                path.push_back(newPoint);
                currentPosition = newPoint;
            }
        }
    }

    void setStartPoint(const PerimeterWidget& perimeter, const Models::Limits& limits)
    {
        path.clear();

        currentPosition = generateRandomPoint(perimeter, limits);
        path.push_back(currentPosition);
    }

    void reset()
    {
        path.clear();
        currentPosition.setX(1e30);
        currentPosition.setY(1e30);
    }

    void draw(QPainter& painter)
    {
        painter.setBrush(Qt::darkGreen);
        if (path.size() > 1)
            for (size_t i = 0; i != path.size() - 1; ++i)
            {
                painter.drawLine(path[i], path[i + 1]);
            }
        // Заполнить полигон
        painter.drawEllipse(currentPosition, 50, 50);
    }

    QPointF getCurrentPosition() const
    {
        return currentPosition;
    }

private:
    QPointF currentPosition;
    QVector<QPointF> path;
};
} // namespace Visual
