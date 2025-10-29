#include <Task/Entities/RegionClassifier.hpp>
#include <Task/Entities/SearchRegion.hpp>

#include <Task/Exceptions/AlgorithmException.hpp>
#include <Task/Operations/Geometry/SegmentOperations.hpp>
#include <Task/Operations/RouteOperations.hpp>
#include <Task/Schemes/Search/Config.hpp>

/*! Core */
#include <AbstractOperations/Algorithms/CoveredBy.hpp>
#include <AbstractOperations/Algorithms/Helper/BufferHelper.hpp>
#include <AbstractOperations/ChangeEachPoint.hpp>
#include <Geometry/Operations/AtPoints2D.hpp>

/*! std */
#include <stdexcept>

namespace Entities {
/* ctor */
SearchRegion::SearchRegion(const std::initializer_list<SearchRing>& rings, const Point2D& start, const Point2D& end)
    : mOuterRing(rings.size() > 0 ? *rings.begin() : SearchRing()),
      mInnerRings(rings.size() > 0 ? rings.begin() + 1 : rings.begin(), rings.end())
{
    // По стандарту Simple Feature Access(SFA) внешние кольца должны быть ориентированы против часовой стрелки(CCW)
    // Внутренние кольца должны быть ориентированы по часовой стрелке(CW). Данное соглашение поддерживается в GEOS
    if (start == end)
    {
        // Объект считается не созданным == деструктор не вызывается, члены удаляются в обратном порядке, объект удаляется
        throw std::invalid_argument("Вход и выход не может быть в одной точке.");
    }

    /// @note: почему так? Причина в проверке на самопересечения, Обязательно надо запускать с Polygon2D
    /// Инициализации из SearchRing скорее всего не будет
    auto polygon = toPolygon2D();

    this->setFromPolygon(polygon);

    /// @note: как я понял вход не может быть, в вершине, (но при добавлении становится вершиной),
    /// выход может быть в вершине
    setEntrance(start);
    setExit(end);
    parameters = RegionParameters();
}

SearchRegion::SearchRegion(const PrimaryEntities::Polygon<Point2D>& polygon, const Point2D& beginPoint, const Point2D& endPoint)
{
    this->setFromPolygon(polygon);

    /// @note: Точки должны корректироваться? Почему точки не скорректированы
    // TODO: переименовать setEntrancePoint, setExitPoint
    setEntrance(beginPoint);
    setExit(endPoint);
    parameters = RegionParameters();
}

SearchRegion::SearchRegion(const PrimaryEntities::Polygon<Point2D>& polygon, const Point2D& beginPoint, const Point2D& endPoint, const RegionParameters& parameters)
{
    this->setFromPolygon(polygon);

    /// @note: Точки должны корректироваться? Почему точки не скорректированы
    // TODO: переименовать setEntrancePoint, setExitPoint
    setEntrance(beginPoint);
    setExit(endPoint);
    setParameters(parameters);
}

/* get/set */
SearchRing const& SearchRegion::outer() const
{
    return mOuterRing;
}

std::vector<SearchRing> const& SearchRegion::inners() const
{
    return mInnerRings;
}

SearchRing& SearchRegion::outer()
{
    return mOuterRing;
}

std::vector<SearchRing>& SearchRegion::inners()
{
    return mInnerRings;
}

size_t SearchRegion::getEntrance() const
{
    return entrance;
}

size_t SearchRegion::getExit() const
{
    return exit;
}

RegionParameters SearchRegion::getParameters() const
{
    return parameters;
}

/* validates */
void SearchRegion::validatePolygon() const
{
}
void SearchRegion::validateEntrance() const
{
    if (entrance >= this->outer().size())
    {
        throw Exceptions::AlgorithmFailure(Exceptions::AlgorithmFailureEnum::DoorPointIncorrect);
    }
}

void SearchRegion::validateExit() const
{
    if (exit >= this->outer().size())
    {
        throw Exceptions::AlgorithmFailure(Exceptions::AlgorithmFailureEnum::DoorPointIncorrect);
    }
}

void SearchRegion::validate() const
{
    validatePolygon();
    validateEntrance();
    validateExit();
}

void SearchRegion::setFromPolygon(const PrimaryEntities::Polygon<Point2D>& polygon)
{
    std::string message;

    auto polygonCopy = std::move(polygon);

    bool correct = AbstractOperations::correct(polygonCopy, message);

    if (!correct)
    {
        throw std::runtime_error("Полигон не скорректирован. " + message);
    }

    this->mOuterRing = static_cast<SearchRing>(polygonCopy.outer());

    mInnerRings.clear();

    for (auto& inner : polygonCopy.inners())
    {
        this->mInnerRings.emplace_back(static_cast<SearchRing>(inner));
    }
}

Polygon2D SearchRegion::getPolygon() const
{
    Polygon2D result;

    ///@note: чет такое себе, но куда деваться?

    result.outer() = this->mOuterRing;

    for (const auto& inner : this->mInnerRings)
        result.inners().push_back(inner);

    std::string message;

    AbstractOperations::correct(result, message);

    return result;
}

void SearchRegion::setEntrance(const Point2D& point)
{
    bool isInserted = this->mOuterRing.initializeDoorPoint(point, entrance);
    // если точка выхода на стороне ринга после точки входа или на той же стороне, но ближе к следующей точке, то нужно увеличить индекс
    if (exit >= entrance && isInserted)
    //    if (exit >= entrance)
    {
        exit++;
    }
}

void SearchRegion::setExit(const Point2D& point)
{
    bool isInserted = this->mOuterRing.initializeDoorPoint(point, exit);
    if (entrance >= exit && isInserted)
    {
        entrance++;
    }
}

void SearchRegion::setParameters(const RegionParameters& parametersIn)
{
    parameters = parametersIn;
}

std::vector<RingIntersection> SearchRegion::intersectionsAllCore(const Line& line) const
{
    std::vector<RingIntersection> result;

    std::vector<Intersection> vectorOuters = mOuterRing.intersectionsAllCore(line).getIntersectionsVector();

    if (!vectorOuters.empty())
    {
        RingIntersection outerIntersections(vectorOuters, 0);
        result.push_back(outerIntersections);
    }
    if (!mInnerRings.empty())
    {
        for (size_t i = 0; i < mInnerRings.size(); i++)
        {
            std::vector<Intersection> vectorInners = mInnerRings[i].intersectionsAllCore(line).getIntersectionsVector();

            if (!vectorInners.empty())
            {
                RingIntersection innerIntersections(vectorInners, i + 1);
                result.push_back(innerIntersections);
            }
        }
    }
    return result;
}

std::vector<RingIntersection> SearchRegion::intersectionsAll(const Point2D& start, const Radian& direction, const IntersectionMethod& method) const
{
    const Line guideLine{start, direction};

    std::vector<RingIntersection> intersections;
    if (method == Core)
    {
        intersections = intersectionsAllCore(guideLine);
    }
    else
    {
        Point2D finish = guideLine.findPoint(start, this->halfPerimeter());
        intersections = intersectionsAllGEOS(start, finish);
    }
    return intersections;
}

RingIntersection SearchRegion::nearestIntersectionAhead(const Point2D& start, const Radian& direction, const IntersectionMethod& method) const
{
    std::vector<RingIntersection> intersections = intersectionsAll(start, direction, method);

    const Line guideLine{start, direction};
    double minDist = mOuterRing.halfPerimeter();

    RingIntersection rInter;

    for (size_t i = 0; i < intersections.size(); ++i)
    {
        for (const auto& intersection : intersections[i]._intersections)
        //        for (size_t j = 0; j < intersections[i].size(); ++j)
        {
            if (Geometry::distance(start, intersection.getPoint()) < mOuterRing.halfPerimeter() * ACCURACY_DOUBLE)
            {
                continue;
            }

            BorderedLine tack(start, intersection.getPoint());
            double tackLen = tack.getLength();

            // проверка, что точки лежат в нужной стороне от стартовой
            /// @note может, перейти к пересечению с отрезком (ошибки уже нет)
            if (tack.isForward() == guideLine.isForward() && tackLen < minDist)
            {
                rInter = RingIntersection{intersection, i};
                minDist = tackLen;
            }
        }
    }

    return rInter;
}

// * GEOS
//==============================================================================
std::vector<RingIntersection> SearchRegion::intersectionsAllGEOS(const Point2D& p0, const Point2D& p1, const double eps) const
{
    std::vector<RingIntersection> result;
    auto intersectionSet = mOuterRing._intersectionsAllGEOS(p0, p1, eps);

    if (!intersectionSet.empty())
    {
        RingIntersection outerIntersections(intersectionSet, 0);
        result.push_back(outerIntersections);
    }

    if (mInnerRings.empty())
    {
        return result;
    }

    for (size_t i = 0; i < mInnerRings.size(); i++)
    {
        auto innerSet = mInnerRings[i]._intersectionsAllGEOS(p0, p1, eps);

        if (!innerSet.empty())
        {
            RingIntersection innerIntersections(innerSet, i + 1);
            result.push_back(innerIntersections);
        }
    }

    return result;
}

double SearchRegion::effectiveWidth(const Point2D& point, const Radian& transversalDirection, const double& detectionRange, const IntersectionMethod& im) const
{
    Line transversalLine{point, transversalDirection};
    std::vector<RingIntersection> intersections = intersectionsAll(point, transversalDirection, im);
    double minLength1 = detectionRange;
    double minLength2 = detectionRange;
    for (size_t i = 0; i < intersections.size(); ++i)
    {
        for (const auto& intersection : intersections[i]._intersections)
        {
            const Point2D& intersectionPoint = intersection.getPoint();
            BorderedLine tack{point, intersectionPoint};
            double length = tack.getLength();
            if (tack.isForward() == transversalLine.isForward() && length < minLength1)
            {
                minLength1 = length;
            }
            else if (tack.isForward() != transversalLine.isForward() && length < minLength2)
            {
                minLength2 = length;
            }
        }
    }
    return minLength1 + minLength2;
}

SearchRing SearchRegion::getRing(size_t index) const
{
    if (mOuterRing.size() == 0)
    {
        throw std::invalid_argument("Регион не содержит колец.");
    }

    if (index > mInnerRings.size())
    {
        throw std::invalid_argument("Задан слишком большой индекс кольца в регионе.");
    }

    if (index == 0)
    {
        return this->mOuterRing;
    }

    return this->mInnerRings[index - 1];
}

Point2D const& SearchRegion::getEntrancePoint() const
{
    return this->mOuterRing[entrance];
}

Point2D const& SearchRegion::getExitPoint() const
{
    return this->mOuterRing[exit];
}

bool SearchRegion::isSeen(const Point2D& positionCurr) const
{
    bool isVertex = false;
    BorderedLine bline(positionCurr, mOuterRing[exit]);
    auto interPoints = intersectionsAllCore(bline);
    for (size_t i = 0; i < interPoints.size(); i++)
    {
        for (size_t j = 0; j < interPoints[i]._intersections.size(); j++)
        {
            if (interPoints[i].ringIndex != 0)
            {
                int indexRing = interPoints[i].ringIndex - 1;
                int indexFirst = (interPoints[i].getIntersectionsVector()[j].vertexIndex + 1) % (mInnerRings[indexRing].size() - 1);
                int indexLast = (interPoints[i].getIntersectionsVector()[j].vertexIndex - 1) % (mInnerRings[indexRing].size() - 1);
                Point2D pointNext = mInnerRings[indexRing][indexFirst];
                Point2D pointPrev = mInnerRings[indexRing][indexLast];
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
            else if (fabs(Geometry::distance(positionCurr, interPoints[i].getIntersectionsVector()[j].point) - Geometry::distance(positionCurr, mOuterRing[exit])) < ACCURACY_DOUBLE * halfPerimeter() ||
                     !bline.nearBorderLine(interPoints[i].getIntersectionsVector()[j].point, ACCURACY_DOUBLE * halfPerimeter()))
                continue;
            else if (interPoints[i].getIntersectionsVector()[j].point == mOuterRing[interPoints[i].getIntersectionsVector()[j].vertexIndex])
                continue;
            else
                return false;
        }
    }
    return true;
}

std::optional<std::pair<Radian, double>> SearchRegion::longerTack(const Point2D& coordinates, const Radian& course, const Radian& turnAngle, IntersectionMethod intersectionMethod) const
{
    std::pair<Radian, double> res;

    const Radian nextCourseRight = (course + turnAngle).normalize();
    const Radian nextCourseLeft = (course - turnAngle).normalize();

    RingIntersection nearestIntersectionRight = this->nearestIntersectionAhead(coordinates, nextCourseRight, intersectionMethod);
    RingIntersection nearestIntersectionLeft = this->nearestIntersectionAhead(coordinates, nextCourseLeft, intersectionMethod);

    if (nearestIntersectionLeft.empty() && nearestIntersectionRight.empty())
    {
        return std::nullopt;
    }

    double tackMaxRight{0}, tackMaxLeft{0};

    if (!nearestIntersectionRight.empty())
    {
        tackMaxRight = this->maxTackLength(coordinates, nearestIntersectionRight);
    }
    if (!nearestIntersectionLeft.empty())
    {
        tackMaxLeft = this->maxTackLength(coordinates, nearestIntersectionLeft);
    }
    // Если в обоих направлениях слишком малая длина галса, возвращаем nullopt
    if (tackMaxLeft < parameters.tackDistMin && tackMaxRight < parameters.tackDistMin)
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

//std::vector<Point2D> SearchRegion::routeToExit(const Point2D& position) const
//{
//    /// @todo прокладка путей из библиотеки триангуляции
//    return std::vector<Point2D>{position, this->getExitPoint()};
//}

std::optional<std::pair<Radian, double>> SearchRegion::nextCourse(const Point2D& coordinates, const Radian& course, const Radian& turnAngle, bool checkSupplementaryCourse, IntersectionMethod intersectionMethod) const
{
    std::optional<std::pair<Radian, double>> nextTackMax = this->longerTack(coordinates, course, turnAngle, Core);

    if (nextTackMax.has_value())
    {
        return nextTackMax;
    }
    else
    {
        nextTackMax = this->longerTack(coordinates, course, Radian(M_PI) - turnAngle, intersectionMethod);
        if (nextTackMax.has_value() && checkSupplementaryCourse)
        {
            return nextTackMax;
        }
        else
        {
            Radian backwardDir = (course + Radian{M_PI}).normalize();
            auto intersectionBackward = this->nearestIntersectionAhead(coordinates, backwardDir, intersectionMethod);

            if (intersectionBackward.empty())
            {
                backwardDir = (backwardDir + Radian{0.1}).normalize();
                intersectionBackward = this->nearestIntersectionAhead(coordinates, backwardDir, intersectionMethod);
                if (intersectionBackward.empty())
                {
                    return std::nullopt;
                }
            }

            double length = this->maxTackLength(coordinates, intersectionBackward);

            return std::make_pair(backwardDir, length);
        }
    }
}

size_t SearchRegion::size() const
{
    if (mOuterRing.size() < 1)
        return 0;

    return mInnerRings.size() + 1;
}

std::pair<Point2D, Point2D> SearchRegion::boxSizes() const
{
    auto [minX, maxX] = std::minmax_element(mOuterRing.begin(), mOuterRing.end(), [](const Point2D& first, const Point2D& second) {
        return first.getX() < second.getX();
    });
    auto [minY, maxY] = std::minmax_element(mOuterRing.begin(), mOuterRing.end(), [](const Point2D& first, const Point2D& second) {
        return first.getY() < second.getY();
    });
    return std::make_pair(Point2D{minX->getX(), minY->getY()}, Point2D{maxX->getX(), maxY->getY()});
}

double SearchRegion::halfPerimeter() const
{
    auto hp = boxSizes();
    return hp.second.getX() - hp.first.getX() + hp.second.getY() - hp.first.getY();
}

double SearchRegion::maxTackLength(const Point2D& start, const RingIntersection& intersection) const
{
    // TODO: такой проверки не должно быть
    if (intersection.getIndex() > this->mInnerRings.size())
    {
        throw std::runtime_error("Нет такого индекса полигона! (calculateTackMax)");
    }

    // Считаем, что пересечение единственное
    const BorderedLine guideline(start, intersection[0].getPoint());

    SearchRing ring = this->getRing(intersection.getIndex());

    BorderedLine edge = BorderedLine(ring.getVertex(intersection[0].getIndex()),
                                     ring.getVertex(intersection[0].vertexIndex + 1));

    const Radian beta = guideline.getAngleTo(edge);
    const double dist = guideline.getLength();
    double traversaOffset;
    if (intersection.ringIndex == 0)
    {
        traversaOffset = parameters.traversaMin;
    }
    else
    {
        // после применения подхода "надувания" внутренних полигонов, нужно убрать траверзное расстояние до них
        // делаем отступ, чтобы не "вылететь" за границу полигона.
        // умножаем на 2, чтобы отработала функция nearestIntersectionAhead на следующем шаге
        traversaOffset = 2 * ACCURACY_DOUBLE * this->halfPerimeter();
    }

    double result = dist - traversaOffset / std::abs(sin(beta.get()));
    // Если траверзное расстояние уже меньше минимального, возвращаем 0
    if (result < 0)
    {
        result = 0;
    }
    return result;
}

bool SearchRegion::penultimatePosition(const Point2D& point, Point2D& positionCurr, double remainingPath) const
{
    auto dist = Geometry::distance(point, positionCurr) + Geometry::distance(positionCurr, this->getExitPoint());
    if (dist > remainingPath)
    {
        BorderedLine tackToExit = this->tackToExit(point);
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

std::vector<Point2D> SearchRegion::pathToExitBisection(const Point2D& positionFirst, const Point2D& positionSecond, const double& lengthError, const double& remainingPath, TriangularGeometry::PathFinder& path) const
{
    /// @note: Из-за рекурсии может быть случай когда точки будут совпадать positionFirst и positionSecond
    BorderedLine bl(positionFirst, positionSecond);

    Point2D positionMid = bl.findPointAtLine(bl.getLength() / 2, true);

    std::vector<Point2D> pathFromMid = path.findPath(positionMid, static_cast<Point2D>(this->getExitPoint()));

    double lengthThroughMid = Geometry::distance(positionFirst, positionMid) + Operations::pathLength(pathFromMid);

    if (lengthThroughMid - remainingPath < -lengthError)
    {
        return pathToExitBisection(positionMid, positionSecond, lengthError, remainingPath - Geometry::distance(positionFirst, positionMid), path);
    }
    else if (lengthThroughMid - remainingPath > lengthError)
    {
        return pathToExitBisection(positionFirst, positionMid, lengthError, remainingPath, path);
    }
    else
    {
        return pathFromMid;
    }
}

std::vector<Point2D> SearchRegion::pathToExit(TriangularGeometry::PathFinder& path, std::vector<Point2D>& route, Point2D& positionCurr, const double& lengthError, const double& remainingPath) const
{
    if (this->penultimatePosition(route.back(), positionCurr, remainingPath) && this->isSeen(positionCurr))
    {
        route.push_back(positionCurr);
        route.push_back(this->getPolygon().outer()[exit]);
    }
    else
    {
        auto pathToExit = this->pathToExitBisection(route.back(), positionCurr, lengthError, remainingPath, path);
        route.insert(route.end(), pathToExit.begin(), pathToExit.end());
    }
    return route;
}

Radian SearchRegion::entranceCourse() const
{
    //    const Ring2D& ring = static_cast<const Ring2D&>(this->outer());
    const SearchRing& ring = this->outer();

    Radian alpha(Geometry::Line(ring[entrance], ring[(entrance + ring.size() - 2) % (ring.size() - 1)]).getSlope());
    Radian beta(Geometry::Line(ring[entrance], ring[entrance + 1]).getSlope());

    return bisector(alpha, beta);
}

BorderedLine SearchRegion::tackToExit(const Point2D& position) const
{
    return BorderedLine(position, this->getExitPoint());
}

void SearchRegion::correct()
{
    std::vector<Ring2D> inners;

    for (const auto& inner : mInnerRings)
        inners.emplace_back(inner);

    // toPolygon
    Polygon2D polygon;
    polygon.outer() = std::move(mOuterRing);
    polygon.inners() = std::move(inners);

    this->setFromPolygon(polygon);
}

Polygon2D SearchRegion::toPolygon2D() const
{
    std::vector<Ring2D> inners;

    for (const auto& inner : mInnerRings)
        inners.emplace_back(inner);

    // toPolygon
    Polygon2D polygon;

    polygon.outer() = std::move(mOuterRing);
    polygon.inners() = std::move(inners);

    return polygon;
}

bool SearchRegion::hasCorrectOrientation() const
{
    for (const auto& inner : mInnerRings)
    {
        if (inner.hasCorrectOrientation())
        {
            return false;
        }
    }

    return mOuterRing.hasCorrectOrientation();
}

double SearchRegion::patrollingLength(const int& countOfRepeats, const Point2D& pointFirst, const Point2D& pointSecond) const
{
    double tack = distance(pointFirst, pointSecond);
    if (countOfRepeats % 2 == 1)
        return countOfRepeats * tack + distance(pointSecond, mOuterRing[exit]);
    else
        return countOfRepeats * tack + distance(pointFirst, mOuterRing[exit]);
}

std::pair<Point2D, Point2D> SearchRegion::longestMidline(const SearchRing::OBB& obb) const
{
    auto [axis, length] = obb.side1.second > obb.side2.second ? obb.side1 : obb.side2;

    Point2D start(obb.center.getX() - axis.getX() * length, obb.center.getY() - axis.getY() * length);
    Point2D end(obb.center.getX() + axis.getX() * length, obb.center.getY() + axis.getY() * length);

    return {start, end};
}

std::vector<Polygon2D> SearchRegion::crop() const
{
    std::vector<Polygon2D> inflatePolygon(1);
    inflatePolygon[0].outer() = mOuterRing;
    MultiPolygon2D multiPoly;
    for (const auto& innerPolygon : mInnerRings)
    {
        AbstractOperations::Helper::buffer(static_cast<Ring2D>(innerPolygon), multiPoly, parameters.traversaMin, geos::operation::buffer::BufferParameters::DEFAULT_QUADRANT_SEGMENTS, geos::operation::buffer::BufferParameters::CAP_FLAT, geos::operation::buffer::BufferParameters::JOIN_MITRE, 5., true, true);
        inflatePolygon[0].inners().push_back(multiPoly[0].outer());
    }

    static_cast<Entities::SearchRing>(inflatePolygon[0].outer()).deepenConcavities(entrance, exit, parameters.traversaMin);

    return inflatePolygon;
}

} // namespace Entities
