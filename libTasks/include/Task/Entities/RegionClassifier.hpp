#pragma once

#include <Task/Entities/SearchRegion.hpp>
#include <Task/Operations/Geometry/SegmentOperations.hpp>

namespace Entities {

// TODO: Далее эти функции пойдут в классификатор
/*!
 * @brief Функция возвращает индекс первой вершины выпуклой оболочки.
 *\param[in] convexBegin Начальная точка выпуклой оболочки.
 * @return size_t
*/
inline size_t indexFirstConvexVertex(const Point2D& convexBegin, const SearchRing& ring)
{
    for (size_t j = 0; j != ring.size(); ++j)
    {
        if (convexBegin == ring.at(j))
        {
            return j;
        }
    }

    return 0;
}
//==============================================================================
/*!
 * @brief Функция возвращает все расстояния вершин от выпуклой оболочки.
 * @return std::pair<size_t, double> Пара номер вершины, значение отклонения точки от стороны выпуклой оболочки.
*/
inline std::vector<std::pair<size_t, double>> getConcavities(const SearchRing& ring)
{
    const auto& convex = ring.getConvexHullGEOS(); // Вершины выпуклой оболочки
    const size_t ringSize = ring.size();

    std::vector<std::pair<size_t, double>> concavities;

    // Найдём индекс первой вершины оболочки в кольце
    size_t convexIndex = 0;
    size_t startIndex = Entities::indexFirstConvexVertex(convex.at(convexIndex), ring);
    concavities.emplace_back(startIndex, 0.0);

    size_t from = startIndex;
    size_t curr = (from + 1) % ringSize;
    size_t between = curr;

    // Обходим кольцо до возвращения к стартовой вершине
    while (curr != startIndex && convexIndex + 1 < convex.size())
    {
        const auto& ringPoint = ring.at(curr);
        const auto& nextConvex = convex.at(convexIndex + 1);

        if (ringPoint == nextConvex) // сравнение с epsilon
        {
            size_t to = curr;

            // Обрабатываем промежуточные точки между from и to
            while (between != to)
            {
                const auto& pt = ring.at(between);
                double dist = Operations::pointToSegmentDistance(pt, ring.at(from), nextConvex);
                concavities.emplace_back(between, dist);
                between = (between + 1) % ringSize;
            }

            concavities.emplace_back(to, 0.0);
            from = to;
            curr = (to + 1) % ringSize;
            between = curr;
            ++convexIndex;
        }
        else
        {
            curr = (curr + 1) % ringSize;
        }
    }

    return concavities;
}

//==============================================================================
// TODO: Или даже inline будут использоваться только тут по идее
/*!
 * @brief Метод возвращает максимальное расстояние от вершин до выпуклой оболочки.
 * @return double
*/
inline double getMaxConcavity(const Entities::SearchRing& ring)
{
    // Если выпуклое Кольцо
    if (ring.isConvex())
        return 0.0;

    auto concavities = getConcavities(ring);

    return std::max_element(concavities.begin(), concavities.end(), [](std::pair<size_t, double> p1, std::pair<size_t, double> p2) { return p1.second < p2.second; })->second;
}
//==============================================================================
/*!
 * @brief Метод возвращает максимальное расстояние от вершин до выпуклой оболочки.
 * @return double
*/
inline double getMaxConcavityGEOS(const Entities::SearchRing& ring)
{
    // Если выпуклое Кольцо
    if (ring.isConvex())
        return 0.0;

    auto convexHull = ring.getRingWithConvexHullGEOS();

    size_t i{0};

    // Находим первую вершину выпуклой оболочки
    for (; i != convexHull.size(); ++i)
    {
        if (convexHull.at(i).second)
        {
            break;
        }
    }

    size_t curr{i};
    size_t end{(i++) % ring.size()};
    double maxDistance{0.0};

    // Проходим до этой вершины (т.е полный круг)
    for (; i % ring.size() != end; ++i)
    {
        // Находим следующую вершину
        if (convexHull.at(i % ring.size()).second)
        {
            // От начала
            size_t start = curr;
            ++curr; // Переходим на следующую точку

            for (; curr != i % ring.size(); ++curr)
            {
                // Измеряем расстояние и проверяем
                double dist = Operations::pointToSegmentDistance(convexHull.at(curr).first, convexHull.at(start).first, convexHull.at(i % ring.size()).first);
                if (dist > maxDistance)
                {
                    maxDistance = dist;
                }
            }
        }
    }

    return maxDistance;
}
//==============================================================================
/*!
 * Перечисление тип региона.
 */
enum RegionType
{
    Type1, // Гребенка
    Type2, // Сдвиг
    Type3  // Зигзаг
};

/*!
 * Класс классификации поискового региона.
 */
class RegionClassifier
{
public:
    RegionClassifier(double range = 100)
        : widthHAS(range)
    {
    }

    // TODO: Можно сделать заключение типа, подходит для ... не подходит для ...
    // Основной метод классификации
    RegionType classify(const SearchRegion& region, double radiusHas) const
    {
        //TODO: тут какая-то классификация
        // 1. Гребенка
        // 2. Сдвиг
        // 3. Зигзаг
        if (isType1(region, radiusHas))
            return RegionType::Type1;

        if (isType2(region))
        {
            return RegionType::Type2;
        }

        return RegionType::Type3;
    }

protected:
    bool isType2(const SearchRegion& region) const
    {
        // 1. Ближе к квадратному.
        // 2. Углы между сторонами не должны быть меньше допустимого.
        // 3. Отсутствие или минимум по площади внутренних полигонов.
        SearchRing::OBB obb = region.outer().getOBB();

        double ratio = std::min(obb.side1.second, obb.side2.second) / std::max(obb.side1.second, obb.side2.second);

        if (ratio < this->squaredRatioThreshold)
        {
            //            std::cout << "Отношение сторон: вычисленное(" << ratio << ") < требуемое(" << this->squaredRatioThreshold << ")" << std::endl;
            return false;
        }

        if (hasSharpAngles(region.outer()))
        {
            //            std::cout << "Регион имеет острые углы!" << std::endl;
            return false;
        }
        if (isTooConcave(region.outer()))
        {
            //            std::cout << "Одна из стенок имеет большую вогнутость!" << std::endl;
            return false;
        }

        // TODO: Может быть просто отсутствие внутренних колец?
        if (hasSignificantInnerRings(region))
        {
            //            std::cout << "Слишком много внутренних полигонов. " << std::endl;
            return false;
        }

        return true;
    }

    // Для Гребенки
    // TODO: Можно сообщение собирать
    bool isType1(const SearchRegion& region, double radiusHAS) const
    {
        // TODO: Условия
        // 1. Ширина меньше или равна ГАС.
        // 2. Возможность прохода по осевой линии.
        // 3. Является узким.

        SearchRing::OBB obb = region.outer().getOBB();

        // Определяем, какая ось является шириной (меньшая)
        // Большая длина, меньшая ширина
        double halfWidth = std::min(obb.side1.second, obb.side2.second);
        double halfLength = std::max(obb.side1.second, obb.side2.second);

        // Проверка соотношения сторон
        // TODO: тут можно за уши притянуть процент от порогово значения, как процент подходящего
        if (halfWidth / halfLength > narrowRatioThreshold) // "Узкость"
        {
            return false;
        }

        // Проверка ширины прохода
        if (2 * halfWidth > radiusHAS)
        {
            return false;
        }

        // Проверка возможности прохода по центру без пересечения внутренних колец
        if (!canPassThroughCenter(region.inners(), obb))
        {
            return false;
        }

        return true;
    }

    bool hasSignificantInnerRings(const SearchRegion& region) const
    {
        if (region.inners().empty())
            return false;

        double outerArea = region.outer().getArea();
        double innerArea = 0.0;

        for (const SearchRing& ring : region.inners())
        {
            innerArea += ring.getArea();
        }

        return innerArea / outerArea < maxInnerAreaRatio;
    }

    bool hasSharpAngles(const SearchRing& ring) const
    {
        std::vector<Radian> angles = ring.getAngles();

        return std::any_of(angles.begin(), angles.end(), [this](Radian angle) { return angle.normalizeWithShift() < permissibleAngle; });
    }

    bool isTooConcave(const SearchRing& outer) const
    {
        double maxConcavity = getMaxConcavity(outer);

        // TODO: Непонятно как брать максимально допустимый
        double maxAllowed = std::sqrt(outer.getArea()) * maxConvacityRatio;

        return maxConcavity > maxAllowed;
    }

protected:
    std::pair<Point2D, Point2D> longestMidline(const SearchRing::OBB& obb) const
    {
        auto [axis, length] = obb.side1.second > obb.side2.second ? obb.side1 : obb.side2;

        Point2D start(obb.center.getX() - axis.getX() * length, obb.center.getY() - axis.getY() * length);
        Point2D end(obb.center.getX() + axis.getX() * length, obb.center.getY() + axis.getY() * length);

        return {start, end};
    }

    // Проверка возможности прохода по центру OBB
    bool canPassThroughCenter(const std::vector<SearchRing>& inners, const SearchRing::OBB& obb) const
    {
        // TODO:
        // Создаем отрезок, представляющий путь по центру OBB
        // TODO: Вычислить длинную сторону
        auto startEnd = longestMidline(obb);

        // Проверяем пересечение с внутренними кольцами
        // TODO: Проверка на пересечение с outer
        return std::none_of(inners.begin(), inners.end(), [startEnd](const SearchRing& inner) { return inner.hasIntersection(startEnd.first, startEnd.second); });
    }

private:
    double widthHAS = 100;                      // Ширина ГАС
    double narrowRatioThreshold = 0.3;          // Отношение сторон для Гребенки
    double squaredRatioThreshold = 0.7;         // Отношение сторон для Shift
    Radian permissibleAngle = Radian(M_PI / 6); // Допустимый угол между соседними стенками
    double maxConvacityRatio = 0.1;             // Максимальный порог вогнутости - вычисляется относительно значений длины и ширины
    double maxInnerAreaRatio = 0.1;             // Максимальная площадь внутренних колец
};
} // namespace Entities
