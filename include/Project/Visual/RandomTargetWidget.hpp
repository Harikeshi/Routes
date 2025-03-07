#pragma once

#include <QPointF>
#include <QVector2D>
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
class RandomTarget
{
public:
    int getRandomNumber(int min, int max)
    {
        return std::rand() % (max - min + 1) + min;
    }

    bool isPointInPolygon(const QPointF& point, const QPolygonF& polygon)
    {
        return polygon.containsPoint(point, Qt::OddEvenFill);
    }

    QPointF generateRandomPoint(const Perimeter& perimeter)
    {
        bool isValidPoint = false;

        // TODO: проверка на размер периметра

        while (!isValidPoint)
        {
            randomPoint = QPointF(getRandomNumber(0, 500), getRandomNumber(0, 500));

            // Проверяем, находится ли точка внутри внешнего многоугольника
            if (isPointInPolygon(randomPoint, perimeter.getInners()[0]))
            {
                isValidPoint = true;

                // Проверяем, находится ли точка внутри любого из внутренних многоугольников
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

        return randomPoint;
    }

    // TODO: speed limits
    QPointF movePoint(const QPointF& currentPoint)
    {
        float speed = static_cast<float>(getRandomNumber(1, 10)) / 1.0f; // Случайная скорость в диапазоне от 0 до 1

        float dx = getRandomNumber(-2, 2);
        float dy = getRandomNumber(-2, 2);

        QVector2D direction{dx, dy};
        direction.normalize();
        QVector2D step = direction * (speed / 60.0);

        QPointF newPoint = currentPoint + step.toPointF();

        if (isPointInPolygon(newPoint, perimeter.getInners()[0]))
        {
            bool insideInnerPolygon = false;
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
                // randomPath.push_back(randomPoint);
                return newPoint;
            }
        }

        return currentPoint;
    }
};
} // namespace Visual
