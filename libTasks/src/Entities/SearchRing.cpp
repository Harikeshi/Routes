#include "Task/Entities/RegionClassifier.hpp"
#include <Task/Entities/SearchRing.hpp>
#include <Task/Operations/Geometry/SegmentOperations.hpp>
#include <Task/Schemes/Search/Config.hpp>

/*! Core */
#include <AbstractOperations/Algorithms/Correct.hpp>
#include <AbstractOperations/Algorithms/Helper/CorrectHelper.hpp>
#include <Geometry/Objects/BorderedLine.hpp>

/*! GEOS */
#include <geos/algorithm/LineIntersector.h> // TODO: вынести в SegmentOperations
#include <geos/algorithm/MinimumDiameter.h>
#include <geos/algorithm/Orientation.h>

namespace Entities {

using Coordinate = geos::geom::CoordinateXY;
using LineIntersector = geos::algorithm::LineIntersector;

//==============================================================================
// Проверить обход
SearchRing SearchRing::getConvexHullGEOS() const
{
    const auto factory = geos::geom::GeometryFactory::getDefaultInstance(); // geos::geom::GeometryFactory::Ptr

    const auto points = factory->createMultiPoint(this->toCoordinateSequence()); // MultiPoint для обработки повторяющихся точек

    // Выпуклая оболочка geos
    auto hullCoordinates = points->convexHull()->getCoordinates();

    // убрать дублирующую последнюю точку
    if (hullCoordinates->size() >= 3)
    {
        hullCoordinates->pop_back();
    }

    // Результирующий вектор
    return this->fromCoordinateSequence(hullCoordinates);
}

// Первая и последняя точки не дублируются
// TODO: Проверить обход
PrimaryEntities::Ring<std::pair<Point2D, bool>> SearchRing::getRingWithConvexHullGEOS() const
{
    auto temp = *this;
    temp.correct();

    // TODO: Убрать повторяющиеся точки
    auto hullRing = getConvexHullGEOS();

    // Формируем результирующий вектор
    Ring<std::pair<Point2D, bool>> resultRing;

    for (const auto& it : temp)
    {
        resultRing.emplace_back(it, false);
    }

    // Помечаем точки, вошедшие в выпуклую оболочку
    for (auto& resultPoint : resultRing)
    {
        for (const auto& hullPoint : hullRing)
        {
            if (resultPoint.first.getX() == hullPoint.getX() && resultPoint.first.getY() == hullPoint.getY())
            {
                resultPoint.second = true;
                break;
            }
        }
    }

    return resultRing;
}

typename SearchRing::OBB SearchRing::getOBB() const
{
    const auto factory = geos::geom::GeometryFactory::getDefaultInstance(); // geos::geom::GeometryFactory::Ptr

    std::unique_ptr<geos::geom::MultiPoint> points = factory->createMultiPoint(this->toCoordinateSequence());

    auto mbrCoords = geos::algorithm::MinimumDiameter::getMinimumRectangle(points.get())->getCoordinates();

    Point2D p0(mbrCoords->getX(0), mbrCoords->getY(0));
    Point2D p1(mbrCoords->getX(1), mbrCoords->getY(1));
    Point2D p2(mbrCoords->getX(2), mbrCoords->getY(2));

    // Вычисляем оси OBB
    Point2D axis1(p1.getX() - p0.getX(), p1.getY() - p0.getY());
    Point2D axis2(p2.getX() - p1.getX(), p2.getY() - p1.getY());

    // Нормализуем оси
    double len1 = std::hypot(axis1.getX(), axis1.getY());
    double len2 = std::hypot(axis2.getX(), axis2.getY());

    if (len1 > 0)
    {
        axis1.set(axis1.getX() / len1, axis1.getY() / len1);
    }
    if (len2 > 0)
    {
        axis2.set(axis2.getX() / len2, axis2.getY() / len2);
    }

    // TODO: Вычисляем центр
    double centerX = (p0.getX() + p1.getX() + p2.getX() + mbrCoords->getX(3)) / 4.0;
    double centerY = (p0.getY() + p1.getY() + p2.getY() + mbrCoords->getY(3)) / 4.0;

    // Длины полуосей
    double halfLen1 = len1 / 2.0;
    double halfLen2 = len2 / 2.0;

    auto obb = OBB{Point2D(centerX, centerY), {axis1, halfLen1}, {axis2, halfLen2}};

    return obb;
}

// Прямоугольник ориентированный вдоль главных осей
// Результат - Обход против часовой стрелки, Начало в любой точке
SearchRing SearchRing::getOrientedBoundingBoxGEOS() const
{
    const auto factory = geos::geom::GeometryFactory::getDefaultInstance(); // geos::geom::GeometryFactory::Ptr

    // TODO: по сути у нас не бывает ring == 1
    if (this->size() >= 2)
    {
        std::unique_ptr<geos::geom::MultiPoint> points = factory->createMultiPoint(this->toCoordinateSequence());

        auto obb = geos::algorithm::MinimumDiameter::getMinimumRectangle(points.get())->getCoordinates();

        // Убрать последнюю дублирующую точку
        if (obb->size() >= 3)
            obb->pop_back();

        return this->fromCoordinateSequence(obb);
    }

    // Результирующий вектор
    return *this;
}

// Площадь
double SearchRing::getArea() const
{
    double area = 0.0;

    for (size_t i = 0; i < this->size(); ++i)
    {
        size_t next = (i + 1) % this->size();
        area += this->at(i).getX() * this->at(next).getY() - this->at(next).getX() * this->at(i).getY();
    }

    return std::abs(area) / 2.0;
}

bool SearchRing::isConvex() const
{
    // TODO: Можно убрать. Кольцо по-условию не может быть меньше 3 точек
    if (this->size() < 3)
        return false;

    int sign = 0;

    for (size_t i = 0; i < this->size(); ++i)
    {
        const Point2D& p0 = this->at(i);
        const Point2D& p1 = this->at((i + 1) % this->size());
        const Point2D& p2 = this->at((i + 2) % this->size());

        double cross = (p1.getX() - p0.getX()) * (p2.getY() - p1.getY()) - (p1.getY() - p0.getY()) * (p2.getX() - p1.getX());

        if (cross != 0)
        {
            if (sign == 0)
            {
                sign = cross > 0 ? 1 : -1;
            }
            else if ((cross > 0 && sign == -1) || (cross < 0 && sign == 1))
            {
                return false;
            }
        }
    }

    return true;
}

// Углы между смежными гранями по каждой вершине начиная с [0], радианы
std::vector<Radian> SearchRing::getAngles() const
{
    // В соответственных вершинах
    std::vector<Radian> angles;

    Point2D prev;
    Point2D curr;
    Point2D next;

    for (size_t i = 0; i < this->size() - 1; ++i)
    {
        if (i == 0)
        {
            prev = this->at((this->size() - 2));
            curr = this->at(0);
            next = this->at(1);
        }
        else
        {
            prev = this->at((i - 1 + this->size()) % this->size());
            curr = this->at(i);
            next = this->at((i + 1) % this->size());
        }

        // std::cout << (i - 1 + this->size()) % this->size() << "-" << i << "-" << (i + 1) % this->size() << std::endl;

        double dx1 = prev.getX() - curr.getX();
        double dy1 = prev.getY() - curr.getY();
        double dx2 = next.getX() - curr.getX();
        double dy2 = next.getY() - curr.getY();

        double dot = dx1 * dx2 + dy1 * dy2;
        double cross = dx1 * dy2 - dy1 * dx2;

        double angle = std::atan2(cross, dot);
        angles.push_back(Radian(std::abs(angle)));
    }

    return angles;
}

bool SearchRing::hasIntersection(const Point2D& segStart, const Point2D& segEnd) const
{
    for (size_t i = 0; i < this->size() - 1; ++i)
    {
        if (Operations::segmentsIntersect(this->at(i), this->at((i + 1) % this->size()), segStart, segEnd))
        {
            return true;
        }
    }

    return false;
}

/* private */
geos::geom::CoordinateSequence SearchRing::toCoordinateSequence() const
{
    geos::geom::CoordinateSequence coords;

    for (const Point2D& pt : *this)
    {
        coords.add(geos::geom::Coordinate(pt.getX(), pt.getY()));
    }

    return coords;
}

/* private */
SearchRing SearchRing::fromCoordinateSequence(const std::unique_ptr<geos::geom::CoordinateSequence>& sequence) const
{
    SearchRing result;

    for (size_t i = 0; i != sequence->size(); ++i)
    {
        result.emplace_back(sequence->getAt(i).x, sequence->getAt(i).y);
    }

    result.correct();

    return result;
}

/* private */
std::unique_ptr<geos::geom::LinearRing> SearchRing::toGeosGeometry() const
{
    geos::geom::GeometryFactory::Ptr geometryFactory = geos::geom::GeometryFactory::create();

    std::unique_ptr<geos::geom::LinearRing> geosRing = toGeosRing(*this);

    if (geosRing == nullptr)
        throw std::runtime_error("toGeosGeometry: Ring was not converted");

    return geosRing;
}

/* Методы Пети */
RingIntersection SearchRing::intersectionsAllCore(const Geometry::Line& line) const
{
    RingIntersection result; // TODO: Всегда возвращаем с нулем, возвращать надо vector или set, В зависимости от того какая структура для хранения в RingIntersection
    for (size_t i = 0; i < this->size() - 1; ++i)
    {
        const Geometry::BorderedLine ringLine(this->at(i), this->at(i + 1));

        if (ringLine.isSameTo(line))
        {
            Intersection firstCrossP{this->at(i), i, true};
            //result.intersections.push_back(firstCrossP);
            result._intersections.insert(firstCrossP);
            if (i != this->size() - 1)
            {
                Intersection secondCrossP{this->at(i + 1), (i + 1), true};
                //result.intersections.push_back(secondCrossP);
                result._intersections.insert(secondCrossP);
            }
            i++;
        }
        else if (ringLine.isParallel(line))
        {
            continue;
        }
        else
        {
            Point2D point = line.crossPoint(ringLine);
            if (ringLine.nearBorderLine(point, ACCURACY_DOUBLE * halfPerimeter()))
            {
                if (point != ringLine.getFinish())
                {
                    Intersection intersectionP = Intersection{point, i};
                    if (point == ringLine.getStart())
                    {
                        intersectionP.isVertex = true;
                    }
                    //result.intersections.push_back(intersectionP);
                    result._intersections.insert(intersectionP);
                }
            }
        }
    }

    return result;
}

double SearchRing::halfPerimeter() const
{
    auto [minX, maxX] = std::minmax_element(this->begin(), this->end(), [](const Point2D& first, const Point2D& second) {
        return first.getX() < second.getX();
    });
    auto [minY, maxY] = std::minmax_element(this->begin(), this->end(), [](const Point2D& first, const Point2D& second) {
        return first.getY() < second.getY();
    });
    return maxX->getX() - minX->getX() + maxY->getY() - minY->getY();
}

std::set<Intersection, Greater> SearchRing::_intersectionsAllGEOS(const Point2D& a, const Point2D& b, double eps) const
{
    LineIntersector intersector;
    std::set<Intersection, Greater> setIntersections;

    // TODO: Замкнутое кольцо так как, иначе не проверяется последнее ребро.
    for (size_t vertexIndex = 0; vertexIndex < this->size() - 1; ++vertexIndex)
    {
        intersector.computeIntersection(a.getCoordinates(), b.getCoordinates(), this->at(vertexIndex).getCoordinates(), this->at(vertexIndex + 1).getCoordinates());

        if (intersector.hasIntersection())
        {
            // Брать нулевую точку. Так как у нас не может быть на одном отрезке два пересечения.
            // Создаем пересечение. Проверка только с первый вершиной.
            auto intersectionPoint = Point2D(intersector.getIntersection(0).x, intersector.getIntersection(0).y);

            Intersection intersection(Point2D(intersector.getIntersection(0).x, intersector.getIntersection(0).y), vertexIndex, intersector.getIntersection(0).equals2D(this->at(vertexIndex).getCoordinates(), eps));

            // Проверяем перед добавлением. Ищем в set
            auto it = find_if(setIntersections.begin(), setIntersections.end(), [intersection](const Intersection& inter) { return intersection.point == inter.point; });

            // Если нет в set Внести
            if (it == setIntersections.end())
            {
                setIntersections.insert(intersection);
            }
            else
            {
                // Изменить флаг, если новое пересечение это вершина, а найденная не вершина
                if (intersection.isVertex && !it->isVertex)
                {
                    setIntersections.erase(it);
                    setIntersections.insert(intersection);
                }
            }
        }
    }

    return setIntersections;
}

RingIntersection SearchRing::intersectionsAllGEOS(const Point2D& a, const Point2D& b, double eps) const
{
    LineIntersector intersector;

    std::set<Intersection, Greater> setIntersections;

    for (size_t vertexIndex = 0; vertexIndex < this->size() - 1; ++vertexIndex)
    {
        intersector.computeIntersection(a.getCoordinates(), b.getCoordinates(), this->at(vertexIndex).getCoordinates(), this->at(vertexIndex + 1).getCoordinates());

        if (intersector.hasIntersection())
        {
            // Брать нулевую точку, так как у нас не может быть на одном отрезке два пересечения
            auto inter0 = intersector.getIntersection(0);

            // Создаем пересечение
            Intersection intersection(Point2D(inter0.x, inter0.y), vertexIndex, inter0.equals2D(this->at(vertexIndex).getCoordinates(), eps));

            // Проверяем перед добавлением. Ищем в set
            auto it = find_if(setIntersections.begin(), setIntersections.end(), [intersection](const Intersection& inter) { return intersection.point == inter.point; });

            if (it == setIntersections.end())
            {
                // Внести
                setIntersections.insert(intersection);
            }
            else
            {
                // Изменить флаг, если вершина и найденная не вершина
                if (intersection.isVertex && !it->isVertex)
                {
                    setIntersections.erase(it);
                    setIntersections.insert(intersection);
                }
            }
        }
    }

    std::vector<Intersection> intersections(setIntersections.begin(), setIntersections.end());
    //std::copy(setIntersections.begin(), setIntersections.end(), std::back_inserter(intersections));

    // TODO: RingIntersection Избыточность так как всегда будет 0
    RingIntersection result{intersections, 0};

    return result;
}

bool SearchRing::hasCorrectOrientation(bool isInner) const
{
    if (isInner)
        return !AbstractOperations::Helper::hasCorrectOrientation(this->toGeosGeometry().get());

    return AbstractOperations::Helper::hasCorrectOrientation(this->toGeosGeometry().get());
}

void SearchRing::correct(bool isInner)
{
    std::string message;

    // Кольцо одно корректируем как CW
    bool isCorrect = AbstractOperations::correct(*static_cast<Ring2D*>(this), message);

    if (!isCorrect)
    {
        //*this = SearchRing{}; // TODO: Вернуть пустое или оставить без изменения
        throw std::runtime_error(message);
    }

    if (isInner)
    {
        std::reverse(this->begin(), this->end());
    }
}

Point2D SearchRing::getVertex(size_t index) const
{
    if (this->empty())
    {
        throw std::runtime_error("Обращение по индексу к вершине к пустому кольцу.");
    }

    // TODO: можно установить index % this->size(), Если мы панки
    if (index >= this->size())
    {
        throw std::invalid_argument("Задан слишком большой индекс вершины в кольце.");
    }

    return this->at(index);
}

bool SearchRing::initializeDoorPoint(const Point2D& point, size_t& idx)
{
    Ring& ring = *this;
    const double absoluteError = halfPerimeter() * Schemes::Search::PROJECTION_SHIFT;
    for (size_t i = 0; i < ring.size() - 1; i++)
    {
        const Geometry::BorderedLine ringLine(ring[i], ring[(i + 1)]);
        if (Geometry::distance(point, ringLine.getStart()) < absoluteError)
        {
            idx = i;
            return false;
        }
        else if (Geometry::distance(point, ringLine.getFinish()) >= absoluteError && ringLine.nearBorderLine(point, absoluteError))
        {
            ring.insert(ring.begin() + i + 1, point);
            idx = i + 1;
            return true;
        }
    }
    idx = ring.size() + 1;
    return false;
}

size_t SearchRing::indexOf(const Point2D& point) const
{
    for (size_t i = 0; i != this->size(); ++i)
    {
        if ((*this)[i] == point)
            return i % this->size();
    }

    throw std::runtime_error("Vertex not found!");
}

SearchRing SearchRing::deepenConcavities(const size_t& entrance, const size_t& exit, const double& traversaMin) const
{
    auto concavities = Entities::getConcavities(*this);

    for (size_t i = 0; i < concavities.size() - 1; ++i)
    {
        if (concavities[i].second != 0 && i != entrance && i != exit)
        {
            size_t firstIndex = (i - 1) % this->size();
            size_t secondIndex = (i + 1) % this->size();
            Line line((*this)[firstIndex], (*this)[secondIndex]);
            auto concaveLine = line.perpendicularLine((*this)[concavities[i].first]);
            Point2D changedPoint = concaveLine.findPoint((*this)[concavities[i].first], traversaMin, false, false);
            const_cast<SearchRing*>(this)->insert(this->begin() + concavities[i].first, changedPoint);
        }
    }

    return *this;
}

} // namespace Entities
