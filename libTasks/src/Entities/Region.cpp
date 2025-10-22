#include <Task/Entities/Region.hpp>

#include <Task/Operations/RouteOperations.hpp>
//#include <Task/Operations/UnitConverter.hpp>
#include <Task/Schemes/Search/Config.hpp>

/*! Core */
#include <AbstractOperations/Algorithms/Area.hpp>
#include <AbstractOperations/Algorithms/CoveredBy.hpp>
#include <AbstractOperations/ChangeEachPoint.hpp>
#include <Geometry/Operations/AtPoints2D.hpp>
#include <MathTools/AngularUnits/RadiansOperations.hpp>
#include <MathTools/Constants.hpp>

/*! std */
#include <cmath>
#include <stdexcept>

namespace Schemes {
namespace Search {
namespace InRegion {

Region::Region(Polygon _polygon, Point2D _beginPoint, Point2D _endPoint)
{
    std::string message;
    bool correct = AbstractOperations::correct(_polygon, message);
    if (!correct)
    {
        throw std::runtime_error("Полигон не скорректирован. " + message);
    }
    polygon = _polygon;
    setEntrance(_beginPoint);
    setExit(_endPoint);
}

bool Region::initializeDoorPoint(const Point2D& point, size_t& idx)
{
    Ring& ring = polygon.outer();
    const double absoluteError = size() * PROJECTION_SHIFT;
    for (size_t i = 0; i < ring.size() - 1; i++)
    {
        const BorderedLine ringLine(ring[i], ring[(i + 1)]);
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

Region& Region::operator=(const Region& region)
{
    if (this == &region)
        return *this;

    this->polygon = region.polygon;
    this->entrance = region.entrance;
    this->exit = region.exit;

    return *this;
}

void Region::setPolygon(Polygon& _polygon)
{
    std::string message;
    bool correct = AbstractOperations::correct(_polygon, message);
    if (!correct)
    {
        throw std::runtime_error("Полигон не скорректирован. " + message);
    }
    polygon = std::move(_polygon);
}

void Region::setEntrance(const Point2D& point)
{
    /// @todo нужно удалять старую точку входа из полигона
    /// Если она еще не была добавлена в полигон, то чтобы не удалять точку [0],
    /// для полей индексов входа/выхода использовать не size_t, а int
    /// и задавать -1 по умолчанию
    bool isInserted = initializeDoorPoint(point, entrance);
    // если точка выхода на стороне ринга после точки входа или на той же стороне, но ближе к следующей точке, то нужно увеличить индекс
    if (exit >= entrance && isInserted)
    //    if (exit >= entrance)
    {
        exit++;
    }
}

void Region::setExit(const Point2D& point)
{
    ///@todo нужно удалять старую точку выхода из полигона
    bool isInserted = initializeDoorPoint(point, exit);
    if (entrance >= exit && isInserted)
    {
        entrance++;
    }
}

OtzType Region::classify() const
{
    double polygonSquare = AbstractOperations::area(polygon.outer());
    /// @todo нахождение выпуклой оболочки
    double convexHullSquare = polygonSquare;
    double ratio = polygonSquare / convexHullSquare;

    if (polygon.inners().size() > 0 || ratio < NONCONVEXIVITY_RATIO)
    {
        return OtzType::SearchZigzag;
    }
    else
    {
        return OtzType::SearchShift;
    }
}

//void Region::metersToMiles()
//{
//    AbstractOperations::changeEachPoint(polygon, Operations::metersToMiles);
//    //Operations::metersToMiles(entrance.point);
//    //Operations::metersToMiles(exit.point);
//}

Radian Region::entranceCourse() const
{
    const Ring2D& ring = polygon.outer();

    Radian alpha(Geometry::Line(ring[entrance], ring[(entrance + ring.size() - 2) % (ring.size() - 1)]).getSlope());
    Radian beta(Geometry::Line(ring[entrance], ring[entrance + 1]).getSlope());

    return bisector(alpha, beta);

    //    Radian course = bisector(alpha, beta);

    //    Geometry::Line guideLine(ring[entrance], course);
    //    const double delta = size() * MIN_RELATIVE_ENTRANCE_LENGTH;
    //    Point2D forwardPoint = guideLine.findPoint(ring[entrance], delta, true, true);
    //    Point2D backwardPoint = guideLine.findPoint(ring[entrance], delta, true, false);

    //    if (AbstractOperations::coveredBy(backwardPoint, ring))
    //    {
    //        course = guideLine.getReversedSlope();
    //    }
    //    else if (!AbstractOperations::coveredBy(forwardPoint, ring))
    //    {
    //        throw std::runtime_error("Не определен начальный курс в районе");
    //    }
    //    return course;
}

std::vector<Point2D> Region::routeToExit(const Point2D& position) const
{
    /// @todo прокладка путей из библиотеки триангуляции
    std::vector<Point2D> route{position, polygon.outer()[exit]};
    return route;
}

BorderedLine Region::tackToExit(const Point2D& position) const
{
    return BorderedLine(position, this->polygon.outer()[exit]);
}

bool Region::penultimatePosition(const Point2D& point, Point2D& positionCurr, const double remainingPath) const
{
    auto dist = Geometry::distance(point, positionCurr) + Geometry::distance(positionCurr, polygon.outer()[exit]);
    if (dist > remainingPath)
    {
        BorderedLine tackToExit = Region::tackToExit(point);
        Line guideline{point, positionCurr};
        Radian courseToExit = tackToExit.getAngleTo(guideline);
        double distToExit = tackToExit.getLength();
        double theoremCos = (pow(remainingPath, 2) - pow(distToExit, 2)) / (remainingPath - distToExit * cos(courseToExit.get())) / 2;
        positionCurr = guideline.findPoint(point, theoremCos);
        return true;
    }
    else
    {
        return false;
    }
}

Point2D Region::penultimatePosition(const Point2D& positionFirst, const Point2D& positionSecond, const double& lengthError, const double& remainingPath, TriangularGeometry::PathFinder& path) const
{
    BorderedLine bl(positionFirst, positionSecond);
    Point2D positionMid = bl.findPointAtLine(bl.getLength() / 2, true);
    std::vector<Point2D> pathFromMid = path.findPath(positionMid, polygon.outer()[exit]);
    double lengthThroughMid = Geometry::distance(positionFirst, positionMid) + Operations::pathLength(pathFromMid);
    if (lengthThroughMid - remainingPath < -lengthError)
    {
        return penultimatePosition(positionMid, positionSecond, lengthError, remainingPath - Geometry::distance(positionFirst, positionMid), path); // remainingPath - distance(positionFirst, positionMid)
    }
    else if (lengthThroughMid - remainingPath > lengthError)
    {
        return penultimatePosition(positionFirst, positionMid, lengthError, remainingPath, path);
    }
    else
        return positionMid;
}

std::optional<std::pair<Radian, double>> Region::longerTack(const Point2D& coordinates, const Radian& course, const Radian& turnAngle) const
{
    std::pair<Radian, double> res;

    const Radian nextCourseRight = (course + turnAngle).normalize();
    const Radian nextCourseLeft = (course - turnAngle).normalize();

    std::optional<PolygonIntersection> nearestIntersectionRight = nearestIntersectionAhead(coordinates, nextCourseRight);
    std::optional<PolygonIntersection> nearestIntersectionLeft = nearestIntersectionAhead(coordinates, nextCourseLeft);

    if (!nearestIntersectionLeft.has_value() && !nearestIntersectionRight.has_value())
    {
        return std::nullopt;
    }

    double tackMaxRight{0}, tackMaxLeft{0};
    if (nearestIntersectionRight.has_value())
    {
        tackMaxRight = maxTackLength(coordinates, nearestIntersectionRight.value());
    }
    if (nearestIntersectionLeft.has_value())
    {
        tackMaxLeft = maxTackLength(coordinates, nearestIntersectionLeft.value());
    }
    // Если в обоих направлениях слишком малая длина галса, возвращаем nullopt
    if (tackMaxLeft < Schemes::Search::TACK_DIST_MIN && tackMaxRight < Schemes::Search::TACK_DIST_MIN)
    {
        return std::nullopt;
    }
    if (tackMaxRight >= tackMaxLeft)
    {
        res.first = nextCourseRight;
        res.second = tackMaxRight;
    }
    else
    {
        res.first = nextCourseLeft;
        res.second = tackMaxLeft;
    }
    return std::optional<std::pair<Radian, double>>(res);
}

std::optional<std::pair<Radian, double>> Region::nextCourse(const Point2D& coordinates, const Radian& course, const Radian& turnAngle, bool isZigzag) const
{
    std::optional<std::pair<Radian, double>> nextTackMax = longerTack(coordinates, course, turnAngle);
    if (nextTackMax.has_value() && nextTackMax->second >= TACK_DIST_LEFT)
    {
        return nextTackMax;
    }
    else
    {
        nextTackMax = longerTack(coordinates, course, Radian(M_PI) - turnAngle);
        if (nextTackMax.has_value() && nextTackMax->second >= TACK_DIST_LEFT && isZigzag)
        {
            return nextTackMax;
        }
        else
        {
            Radian backwardDir = (course + Radian{M_PI}).normalize();
            auto intersectionBackward = nearestIntersectionAhead(coordinates, backwardDir);
            if (!intersectionBackward.has_value())
            {
                backwardDir = (backwardDir + Radian{0.1}).normalize();
                intersectionBackward = nearestIntersectionAhead(coordinates, backwardDir);
                if (!intersectionBackward.has_value())
                {
                    return std::nullopt;
                }
            }
            double length = maxTackLength(coordinates, intersectionBackward.value());
            return std::make_pair(backwardDir, length);
        }
    }
}

double Region::maxTackLength(const Point2D& start, const PolygonIntersection& nearestIntersection) const
{
    if (nearestIntersection.ringIndex > polygon.inners().size())
    {
        throw std::runtime_error("Нет такого индекса полигона! (calculateTackMax)");
    }

    const BorderedLine guideline(start, nearestIntersection.point);
    Ring ring;
    if (nearestIntersection.ringIndex == 0)
    {
        ring = polygon.outer();
    }
    else
    {
        ring = polygon.inners()[nearestIntersection.ringIndex - 1];
    }
    BorderedLine edge = BorderedLine(ring[nearestIntersection.vertexIndex], ring[(nearestIntersection.vertexIndex + 1)]);

    /// @todo расчет угла beta c учетом любого расположения точки пересечения, в том числе:
    ///  1) в вершине
    ///  2) близко к вершине, соединяющей с другим ребром, которое в итоге и определяет траверзное расстояние.
    ///  3) случай парраллельности guideline & edge
    const Radian beta = guideline.getAngleTo(edge);
    const double dist = guideline.getLength();

    double result = dist - Schemes::Search::TRAVERSA_MIN / std::abs(sin(beta.get()));
    // Если траверзное расстояние уже меньше минимального, возвращаем 0
    if (result < 0)
    {
        result = 0;
    }
    return result;
}

std::vector<PolygonIntersection> Region::intersectionsAll(const Line& line, const size_t& index) const
{
    Ring ring;
    if (index == 0)
    {
        ring = polygon.outer();
    }
    else
    {
        ring = polygon.inners()[index - 1];
    }
    std::vector<PolygonIntersection> result;
    for (size_t i = 0; i < ring.size() - 1; i++)
    {
        const BorderedLine ringLine(ring[i], ring[(i + 1)]);
        if (ringLine.isSameTo(line))
        {
            PolygonIntersection firstCrossP{ring[i], index, i, true};
            result.push_back(firstCrossP);
            if (i != ring.size() - 1)
            {
                PolygonIntersection secondCrossP{ring[(i + 1)], index, (i + 1), true};
                result.push_back(secondCrossP);
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
            if (ringLine.nearBorderLine(point, ACCURACY_DOUBLE * size()))
            {
                if (point != ringLine.getFinish())
                {
                    PolygonIntersection intersectionP = {point, index, i};
                    if (point == ringLine.getStart())
                    {
                        intersectionP.isVertex = true;
                    }
                    result.push_back(intersectionP);
                }
            }
        }
    }
    return result;
}

std::vector<PolygonIntersection> Region::intersectionsAll(const Line& line) const
{
    std::vector<PolygonIntersection> result;
    result = intersectionsAll(line, 0);
    if (!polygon.inners().empty())
    {
        for (size_t i = 0; i < polygon.inners().size(); i++)
        {
            std::vector<PolygonIntersection> vectorInners = intersectionsAll(line, i + 1);
            result.insert(result.end(), vectorInners.begin(), vectorInners.end());
        }
    }
    return result;
}

bool Region::isSeen(const Point2D& positionCurr) const
{
    bool isVertex = false;
    BorderedLine bline(positionCurr, polygon.outer()[exit]);
    auto interPoints = intersectionsAll(bline);
    for (int i = 0; i < interPoints.size(); i++)
    {
        if (interPoints[i].ringIndex != 0)
        {
            int indexRing = interPoints[i].ringIndex - 1;
            int indexFirst = (interPoints[i].vertexIndex + 1) % (polygon.inners()[indexRing].size() - 1);
            int indexLast = (interPoints[i].vertexIndex - 1) % (polygon.inners()[indexRing].size() - 1);
            Point2D pointNext = polygon.inners()[indexRing][indexFirst];
            Point2D pointPrev = polygon.inners()[indexRing][indexLast];
            auto coeffs = bline.getAllCoeffs();
            auto a = std::get<0>(coeffs);
            auto b = std::get<1>(coeffs);
            auto c = std::get<2>(coeffs);

            auto first = a * pointNext.getX() + b * pointNext.getY() + c;
            auto second = a * pointPrev.getX() + b * pointPrev.getY() + c;
            if ((first > 0 && second > 0) || (first < 0 && second < 0))
            {
                isVertex = true;
                continue;
            }
            else if (first == 0 || second == 0)
            {
                isVertex = true;
                continue;
            }
            else
                return false;
            if (isVertex)
                break;
            else
                return false;
        }
        else if (Geometry::distance(interPoints[i].point, polygon.outer()[exit]) < ACCURACY_DOUBLE * size() ||
                 !bline.nearBorderLine(interPoints[i].point, ACCURACY_DOUBLE * size()))
            continue;
        else if (interPoints[i].point == polygon.outer()[interPoints[i].vertexIndex])
            continue;
        else
            return false;
    }
    return true;
}

std::optional<PolygonIntersection> Region::nearestIntersectionAhead(const Point2D& start, const Radian& direction) const
{
    const Line guideLine{start, direction};
    PolygonIntersection nearestIntersection;
    bool hasIntersection = false;
    double minDist = size();
    std::vector<PolygonIntersection> intersections = intersectionsAll(guideLine);
    for (size_t i = 0; i < intersections.size(); ++i)
    {
        if (Geometry::distance(start, intersections[i].point) < size() * ACCURACY_DOUBLE)
        {
            continue;
        }
        BorderedLine tack(start, intersections[i].point);
        double tackLen = tack.getLength();
        if (tack.isForward() == guideLine.isForward() && tackLen < minDist)
        {
            nearestIntersection = intersections[i];
            minDist = tackLen;
            hasIntersection = true;
        }
    }
    //    if (Geometry::distance(start, nearestIntersection.point) < TRAVERSA_MIN)
    //    {
    //        hasIntersection = false;
    //    }
    return hasIntersection ? std::optional(nearestIntersection) : std::nullopt;
}

double Region::size() const
{
    double minX = MAX_SIZE, minY = MAX_SIZE;
    double maxX = -MAX_SIZE, maxY = -MAX_SIZE;
    for (unsigned int i = 0; i < polygon.outer().size(); i++)
    {
        double x = polygon.outer()[i].getX();
        double y = polygon.outer()[i].getY();
        if (x < minX)
        {
            minX = x;
        }
        else if (x > maxX)
        {
            maxX = x;
        }
        if (y < minY)
        {
            minY = y;
        }
        else if (y > maxY)
        {
            maxY = y;
        }
    }

    return maxX - minX + maxY - minY;
}
} // namespace InRegion
} // namespace Search
} // namespace Schemes
