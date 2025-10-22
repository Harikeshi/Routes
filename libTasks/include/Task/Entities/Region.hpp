#pragma once

/*! Core */
#include <Geometry/Objects/BorderedLine.hpp>
#include <PrimaryEntities/Entities/Polygon.hpp>
#include <TriangularGeometry/PathFinder.hpp>

/*! std */
#include <optional>

namespace Schemes {
namespace Search {
namespace InRegion {

using Ring = PrimaryEntities::Ring<Point2D>;
using Polygon = PrimaryEntities::Polygon<Point2D>;
using BorderedLine = Geometry::BorderedLine;
using Line = Geometry::Line;

struct PolygonIntersection
{
    Point2D point{0, 0};
    size_t ringIndex{0}; // i = 0 если пересечение на внешнем ринге(outer), i = 1..k если на внутреннем (inner[i])
    size_t vertexIndex{0};
    bool isVertex{false};
    inline bool operator==(const PolygonIntersection& object) const
    {
        return this->point == object.point &&
               this->ringIndex == object.ringIndex &&
               this->vertexIndex == object.vertexIndex &&
               this->isVertex == object.isVertex;
    }
};

/// @todo расширять, куда поместить
enum OtzType
{
    SearchZigzag = 0,
    SearchShift = 1
};

class Region
{
private:
    Polygon polygon;
    size_t entrance;
    size_t exit;

    bool initializeDoorPoint(const Point2D& point, size_t& idx);

public:
    Region()
    {
    }
    Region(Polygon _polygon, Point2D _beginPoint, Point2D _endPoint);

    /// Конструктор копирования
    Region(const Region& region)
        : polygon(region.polygon), entrance(region.entrance), exit(region.exit)
    {
    }
    /// Присваивание копированием
    Region& operator=(const Region& polygon);
    /// Конструктор перемещения
    Region(Region&& rhs) = default;
    /// Присваивание перемещением
    Region& operator=(Region&& rhs) = default;

    ~Region() = default;

public:
    /*!
     * \brief validatePolygon
     * \details
     */
    void validatePolygon() const
    {
    }

    void validateEntrance() const
    {
        if (entrance >= polygon.outer().size())
        {
            throw std::runtime_error("Точка входа не лежит на границе района");
        }
    }

    void validateExit() const
    {
        if (exit >= polygon.outer().size())
        {
            throw std::runtime_error("Точка выхода не лежит на границе района");
        }
    }

    void validate() const
    {
        validatePolygon();
        validateEntrance();
        validateExit();
    }

    /// Методы получения и присваивания
    void setPolygon(Polygon& _polygon);

    void setEntrance(const Point2D& point);

    void setExit(const Point2D& point);

    Polygon const& getPolygon() const
    {
        return polygon;
    }

    size_t const& getEntrance() const
    {
        return entrance;
    }

    size_t const& getExit() const
    {
        return exit;
    }

    Ring outer() const
    {
        return polygon.outer();
    }

    std::vector<Ring> inners() const
    {
        return polygon.inners();
    }

    /// Методы для алгоритмов
    /*!
   * \brief classify
   * \details Классификация региона для выбора способа
   * \return Тип поиска (?)
   */
    OtzType classify() const;

    /*!
     * \brief metersToMiles - перевод координат полигона и точек входа/выхода из метров в мили
     */
    void metersToMiles();

    /*!
     * \brief entranceCourse
     * \details Построения курса из точки старта
     * \return [out] Курс (направление)
     */
    Radian entranceCourse() const;

    /*!
     * \brief routeToEnd
     * \details Прокладка пути к точке выхода с учетом того, что "надо огибать остров"
     * \param position [in] текущая позиция
     * \return [out] Вектор точек маршрута
     */
    std::vector<Point2D> routeToExit(const Point2D& position) const;

    /*!
     * \brief tackToExit
     * \details Прямой галс от заданной позиции до точки выхода из района
     * \param position [in] текущая позиция
     * \return [out] отрезок
     */
    BorderedLine tackToExit(const Point2D& position) const;

    /*!
     * \brief penultimatePosition
     * \details расчет предпоследней маршрутной точки, время пути через которую к точке выхода из района будет в пределах заданного времени обследования. Используется теорема косинусов
     * \param position [in] текущая позиция
     * \param positionCurr [in] текущий курс
     * \param remainingPath [in] наибольшая длина пути до точки выхода, чтобы уложиться в заданное время
     * \return [out] Возвращает true, если может посчитать с помощью теоремы косинусов, иначе false
     */
    bool penultimatePosition(const Point2D& point, Point2D& positionCurr, const double remainingPath) const;

    /*!
     * \details расчет предпоследней маршрутной точки, время пути через которую к точке выхода из района будет в пределах заданного времени обследования. Используется вещественный поиск
     * \param routeBack [in] последняя точка добавленная в маршрут
     * \param positionCurr [in] точка, полученная из теоремы косинусов
     * \param lastOptimalPath [in] ошибка оптимального маршрута
     * \param remainingPath [in] наибольшая длина пути до точки выхода, чтобы уложиться в заданное время
     * \param path [in] область триангуляции
     * \param mFinal [in] точка выхода
     * \return [out] возвращает точку последнего галса
     */
    Point2D penultimatePosition(const Point2D& routeBack, const Point2D& positionCurr, const double& lengthError, const double& remainingPath, TriangularGeometry::PathFinder& path) const;

    /*!
     * \brief longerTack
     * \details выбор того направления (направо, налево) где длиннее галс
     * \param coordinates [in] Текущая координата
     * \param course [in] Текущий курс
     * \param turnAngle [in] Заданный угол поворота
     * \return [out] пару, следующий курс и максимальную длину галса
     */
    std::optional<std::pair<Radian, double>> longerTack(const Point2D& coordinates, const Radian& course, const Radian& turnAngle) const;

    /*!
     * \brief nextCourse
     * \details Расчет следующего курса, с обработкой случая нахождения в "тупике".
     * \param coordinates [in] Текущая координата
     * \param course [in] Текущий курс
     * \param turnAngle [in] Заданный угол поворота
     * \return [out] пару, следующий курс и максимальную длину галса
     */
    std::optional<std::pair<Radian, double>> nextCourse(const Point2D& coordinates, const Radian& course, const Radian& turnAngle, bool isZigzag = true) const;

    /*!
     * \brief maxTackLength
     * \details Расчет максимально возможной длина галса, чтобы не сблизиться с границей района
     * \param start [in] Точка от которой считается расстояние
     * \param nearestIntersection [in] Место пересечения с полигоном
     * \param traversaMin [in] Траверсное расстояние (или минимальное расстояние до границы)
     * \return [out] максимальную длину галса
     */
    double maxTackLength(const Point2D& start, const PolygonIntersection& nearestIntersection) const;

    /*!
     * \brief intersectionsAll
     * \details Поиск всех точек пересечения с рингом
     * \param line [in] линия пересечения ринга
     * \param index [in] индекс ринга
     * \return [out] Вектор точек пересечения ринга
     */
    std::vector<PolygonIntersection> intersectionsAll(const Line& line, const size_t& index) const;

    /*!
     * \brief intersectionsAll
     * \details Поиск всех точек пересечения с полигоном
     * \param line [in] линия пересечения ринга
     * \return [out] Вектор точек пересечения полигона
     */
    std::vector<PolygonIntersection> intersectionsAll(const Line& line) const;

    /*!
     * \brief isSeen
     * \details Проверка видимости точки относительно точки выхода
     * \param positionCurr [in] точка внутри полигона
     * \return [out] true - точка видна, false - точка не видна
     */
    bool isSeen(const Point2D& positionCurr) const;

    /*!
     * \brief nearestIntersectionInDirection
     * \details Расчет ближайшего пересечения со стороной региона по заданному направлению
     * \param start [in] Точка относительно которой ищем пересечение
     * \param direction [in] Направление поиска
     * \return [out] Точка пересечения в пространстве и на регионе
     */
    std::optional<PolygonIntersection> nearestIntersectionAhead(const Point2D& start, const Radian& direction) const;

    /*!
     * \brief size
     * \return [out] Размер отрезка, который гарантированно не поместится внутри полигона
     */
    double size() const;
};
} // namespace InRegion
} // namespace Search
} // namespace Schemes
