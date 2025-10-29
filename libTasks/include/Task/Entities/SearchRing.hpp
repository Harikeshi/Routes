#pragma once

#include <Task/Entities/Intersection.hpp>

/*! Core */
#include <Geometry/Objects/Line.hpp>
#include <PrimaryEntities/Entities/Ring.hpp>

namespace Entities {
/*!
 * Класс поискового кольца
 */
class SearchRing : public PrimaryEntities::Ring<Point2D>
{
public:
    //==============================================================================
    /*!
     * @brief Конструктор по-умолчанию.
     */
    explicit SearchRing() // = delete
        : Ring<Point2D>()
    {
        // TODO: Пустое кольцо не может быть, возможно следует удалить конструктор по умолчанию
    }
    //==============================================================================
    /*!
     * @brief Пользовательский конструктор из std::initializer_list<Point2D>.
     *\param[in] ring
     */
    SearchRing(const std::initializer_list<Point2D>& ring)
        : PrimaryEntities::Ring<Point2D>{ring}
    {
        // TODO: Кольцо может быть как CW так и CCW ориентации
    }
    //==============================================================================
    /*!
     * @brief Пользовательский конструктор из PrimaryEntities::Ring<Point2D>.
     *\param[in] ring
     * @return 
     */
    explicit SearchRing(const PrimaryEntities::Ring<Point2D>& ring)
        : PrimaryEntities::Ring<Point2D>{ring}
    {
    }
    //==============================================================================
    /*!
     * Конструктор копирования.
     */
    SearchRing(const SearchRing&) = default;
    //==============================================================================
    /*!
     * Конструктор перемещения.
     */
    SearchRing(SearchRing&&) noexcept = default;
    //==============================================================================
    /*!
     * Перемещающее присваивание.
     * @return SearchRing
     */
    SearchRing& operator=(SearchRing&&) noexcept = default;
    //==============================================================================
    /*!
     * Копирующее присваивание.
     * @return SearchRing&
     */
    SearchRing& operator=(const SearchRing&) = default;
    //==============================================================================
    /*!
     * @brief Метод возвращает выпуклую оболочку.
     * @return SearchRing
     */
    SearchRing getConvexHullGEOS() const;
    //==============================================================================
    /*!
     * @brief Метод возвращает кольцо, точки выпуклой оболочки помечены флагом.
     * @return PrimaryEntities::Ring<std::pair<Point2D, bool>>
     */
    PrimaryEntities::Ring<std::pair<Point2D, bool>> getRingWithConvexHullGEOS() const;
    //==============================================================================
    /*!
     * @brief структура Oriented Bounding Box.
     */
    struct OBB
    {
        Point2D center; // Центроид(Реальные координаты).

        std::pair<Point2D, double> side1; // struct{axis, halfLength} - Нормализованная ось и половина длины оси
        std::pair<Point2D, double> side2;
    };
    //==============================================================================
    /*!
     * @brief Метод получения Oriented Bounding Box.
     * @return OBB
     */
    OBB getOBB() const;
    //==============================================================================
    /*!
     * @brief Метод получения Oriented Bounding Box с помощью GEOS.
     * @return SearchRing
     */
    SearchRing getOrientedBoundingBoxGEOS() const;
    //==============================================================================
    /**
     * @brief Метод получения площади кольца.
     * @return double
     */
    double getArea() const;
    //==============================================================================
    /*!
     * @brief Метод проверки выпуклости кольца.
     * @return bool
     */
    bool isConvex() const; // TODO: заменить на соответствующие методы из Core
    //==============================================================================
    /*!
     * @brief Метод получения внутренних углов кольца в соответствующих вершинах.
     * @return std::vector<double>
     */
    std::vector<Radian> getAngles() const;
    //==============================================================================
    /*!
     * @brief Метод проверки кольца на пересечение с отрезком.
     *\param[in] segStart Точка начала отрезка.
     *\param[in] segEnd Точка конца отрезка.
     * @return bool
     */
    bool hasIntersection(const Point2D& segStart, const Point2D& segEnd) const;
    //==============================================================================
private:
    //! Вспомогательные функции для работы с кольцом в geos
    //==============================================================================
    /*!
     * @brief Метод возвращает последовательность координат GEOS из кольца.
     * @return geos::geo
     */
    geos::geom::CoordinateSequence toCoordinateSequence() const;
    //==============================================================================
    /*!
     * @brief Метод возвращает кольцо из последовательности координат GEOS.
     *\param[in] sequence
     * @return SearchRing
     */
    SearchRing fromCoordinateSequence(const std::unique_ptr<geos::geom::CoordinateSequence>& sequence) const;
    //==============================================================================
    /*!
     * Метод возвращает геометрию GEOS полученную из кольца.
     * @return std::unique_ptr<geos::geom::LinearRing>
     */
    std::unique_ptr<geos::geom::LinearRing> toGeosGeometry() const;

public:
    // Методы Пети
    //==============================================================================
    /*!
   * \brief intersectionsAll
   * \details Поиск всех точек пересечения с рингом
   * \param line [in] линия пересечения ринга
   * \param index [in] индекс ринга
   * \return [out] Вектор точек пересечения ринга
   */
    RingIntersection intersectionsAllCore(const Geometry::Line& line) const;
    //==================================================================================
    /*!
   * \brief TODO: Диагональ ограничивающего прямоугольника(Диаметр).
   * \return [out] Размер отрезка, который гарантированно не поместится внутри полигона
   */
    double halfPerimeter() const;
    //==================================================================================
    /**
     * @brief Метод возвращает множество пересечений в кольце.
     * @param a Точка начала отрезка.
     * @param b Точка конца отрезка.
     * @param eps Допустимая точность.
     * @return std::set<Intersection, Greater>
     */
    std::set<Intersection, Greater> _intersectionsAllGEOS(const Point2D& a, const Point2D& b, double eps = 1e-14) const;
    //==================================================================================
    /**
     * @brief Метод возвращает все пересечения отрезка с кольцом.
     * @param a Точка начала отрезка.
     * @param b Точка конца отрезка.
     * @param eps Допустимая точность.
     * @return RingIntersection
     */
    RingIntersection intersectionsAllGEOS(const Point2D& a, const Point2D& b, double eps = 1e-14) const;

public:
    //==================================================================================
    /**
     * @brief Метод проверки ориентации кольца(CW).
     * @param isInner 
     * @return bool
     */
    bool hasCorrectOrientation(bool isInner = false) const;
    //==================================================================================
    /*!
     * @brief Метод перевода кольца в правильную ориентацию(CW).
     * Используется для внешнего кольца.
     */
    void correct(bool isInner = false);
    //==================================================================================
    /*!
     * Метод возвращает вершину по индексу.
     * @param index Индекс вершины.
     * @return Point2D
     */
    Point2D getVertex(size_t index) const;
    //==================================================================================
    /*!
     * Метод возвращает первый индекс вершины ребра, на котором лежит заданная точка.
     * @param point Точка.
     * @param absoluteError Допустимое отклонение.
     * @return size_t
     */
    bool initializeDoorPoint(const Point2D& point, size_t& idx);
    //==================================================================================
    /*!
    * Метод возвращает индекс вершины, если заданная точка является вершиной.
    * @param point Точка
    * @return size_t
    */
    size_t indexOf(const Point2D& point) const;
    //==================================================================================
    /*!
     * Перегруженный оператор вывода в поток.
     * @param os
     * @param ring
     * @return std::ostream&
     */
    friend std::ostream& operator<<(std::ostream& os, const SearchRing& ring)
    {
        for (const auto& point : ring)
        {
            os << "[" << point.getX() << ", " << point.getY() << "]";
        }

        return os;
    }
    /*!
     * \brief deepenConcavities
     * \return
     */
    SearchRing deepenConcavities(const size_t& entrance, const size_t& exit, const double& traversaMin) const;
};

} // namespace Entities
