#pragma once

/*! Core */
#include <Point/Point2D.hpp>

/*! std */
#include <set>

namespace Entities {
/*!
 * Структура одиночного пересечения.
 */
struct Intersection
{
    /*!
     * Точка пересечения.
     */
    Point2D point;
    //==============================================================================
    /*!
     * Индекс вершины ребра.
     */
    size_t vertexIndex;
    //==============================================================================
    /*!
     * Точка пересечения является вершиной.
     */
    bool isVertex;
    //==============================================================================
    // Требуется использовать только с optional, так как нельзя отследить иначе, что пересечения не было, в данной реализации
    /*!
     * Конструктор по-умолчанию.
     * @param point
     * @param vertId
     * @param isVert
     */
    explicit Intersection(const Point2D& point = {0, 0}, const size_t vertId = 0, bool isVert = false)
        : point(point), vertexIndex(vertId), isVertex(isVert)
    {
        // TODO: необязательная процедура
        // bringToZero();
    }
    //==============================================================================
    /*!
     * Метод возвращает точку пересечения.
     * @return Point2D
    */
    Point2D getPoint() const
    {
        return point;
    }
    //==============================================================================
    /*!
     * Метод возвращает индекс вершины.
     * @return size_t
    */
    size_t getIndex() const
    {
        return vertexIndex;
    }
    //==============================================================================
    // TODO: Поправить позже
    /*!
     * Метод проверки, является ли вершиной.
     * @return bool
    */
    bool vertex() const
    {
        return isVertex;
    }
    //==============================================================================
    /*!
     * Оператор сравнения.
     * @param object
     * @return bool
    */
    inline bool operator==(const Intersection& object) const
    {
        return this->point == object.point &&
               this->vertexIndex == object.vertexIndex &&
               this->isVertex == object.isVertex;
    }
    //==============================================================================
    /*!
     *
     * @param os
     * @return
    */
    std::ostream& operator<<(std::ostream& os) const
    {
        os << "[" << this->point.getX() << ", " << this->point.getY() << "], VertexIndex: " << this->vertexIndex << ", IsVertex: "
           << (this->isVertex ? "true" : "false") << ".\n";

        return os;
    }
    //==============================================================================
};

/*!
 * Структура для оператора сравнения для множества.
 */
struct Greater
{
    /*!
     * Оператор сравнения.
     * @param a
     * @param b
     * @return bool
    */
    bool operator()(const Intersection& a, const Intersection& b) const
    {
        return (a.point.getX() < b.point.getX()) || (a.point.getX() == b.point.getX() && a.point.getY() < b.point.getY());
    }
    //==============================================================================
};

/*!
 * Структура пересечений в кольце.
*/
struct RingIntersection
{
    // TODO: Пока на выбор или сет или вектор
    // TODO: использовать set<>, удалить vector<>
    //std::vector<Intersection> intersections;

    /*!
     * Множество пересечений.
     * Отсортированы лексикографически.
    */
    std::set<Intersection, Greater> _intersections;
    //==============================================================================
    /*!
     * Индекс кольца.
    */
    size_t ringIndex{0}; // i = 0 если пересечение на внешнем ринге(outer), i = 1..k если на внутреннем (inner[i])
                         //==============================================================================
    // TODO: Дефолтный конструктор порождает ошибки с индексами
    /*!
     * Конструктор по-умолчанию.
    */
    RingIntersection() = default;
    //==============================================================================
    /*!
     * Пользовательский конструктор из множества пересечений.
     * @param vInter
     * @param index
    */
    RingIntersection(const std::set<Intersection, Greater>& vInter, const size_t& index)
        : /*intersections(vInter.begin(), vInter.end()), */ _intersections(vInter), ringIndex(index)
    {
    }
    //==============================================================================
    /*!
     * Пользовательский конструктор из вектора пересечений. TODO: или то или то
     * @param vInter
     * @param index
    */
    RingIntersection(const std::vector<Intersection>& vInter, const size_t& index)
        : /*intersections(vInter),*/ _intersections(vInter.begin(), vInter.end()), ringIndex(index)
    {
    }
    //==============================================================================
    /*!
     * Пользовательский конструктор из единичного пересечения.
     * @param inter
     * @param index
    */
    RingIntersection(const Intersection& inter, const size_t& index)
        : ringIndex(index)
    {
        //intersections.push_back(inter);
        _intersections.insert(inter);
    }
    //==============================================================================
    /*!
     * Метод добавления пересечения.
     * @param intersection Конкретное пересечение.
     * @return std::pair<std::set<Intersection, Greater>::iterator, bool>
    */
    std::pair<std::set<Intersection, Greater>::iterator, bool> insert(const Intersection& intersection)
    {
        return _intersections.insert(intersection);
    }
    //==============================================================================
    // TODO: Использование optional не требуется
    /*!
     * Метод проверки на отсутствие пересечений.
     * @return size_t
    */
    size_t empty() const
    {
        return _intersections.empty();
    }
    //==============================================================================
    /*!
      * Метод возвращает количество пересечений в кольце.
      * @return size_t
    */
    size_t size() const
    {
        return _intersections.size();
    }
    //==============================================================================
    /*!
     * Метод возвращает индекс кольца.
     * @return size_t
    */
    size_t getIndex() const
    {
        return ringIndex;
    }
    //==============================================================================
    /*!
     * Метод возвращает множество пересечений.
     * @return std::set<Intersection, Greater>
    */
    std::set<Intersection, Greater> getIntersections() const
    {
        return _intersections;
    }
    //==============================================================================
    /*!
     * Метод возвращает вектор пересечений.
     * @return std::vector<Intersection>
    */
    std::vector<Intersection> getIntersectionsVector() const
    {
        std::vector<Intersection> vectorOuters(_intersections.begin(), _intersections.end());

        //std::copy(_intersections.begin(), _intersections.end(), vectorOuters);

        return vectorOuters;
    }
    //==============================================================================
    /*!
     * Оператор возвращает пересечение по индексу.
     * @param index Индекс конкретного пересечения.
     * @return Intersection
    */
    const Intersection& operator[](size_t index) const
    {
        if (index >= _intersections.size())
            throw std::out_of_range("out of range.");

        return *(std::next(_intersections.begin(), static_cast<long>(index)));
    }
    //==============================================================================
    /*!
     * Перегруженный оператор вывода в поток.
     * @param os
     * @param ring
     * @return std::ostream&
    */
    friend std::ostream& operator<<(std::ostream& os, const RingIntersection& ring)
    {
        for (const auto& intersection : ring._intersections)
        {
            os << "[" << ring.ringIndex << "][" << intersection.point.getX() << ", " << intersection.point.getY()
               << "], VertexIndex: " << intersection.vertexIndex << ", IsVertex: "
               << (intersection.isVertex ? "true" : "false") << ".\n";
        }

        return os;
    }
};
//==============================================================================
/*!
 * Структура пересечения в регионе. TODO: не используется, В процессе разработки.
*/
struct RegionIntersections
{
    // TODO: может быть пустой, есть проверка empty()
    std::vector<RingIntersection> rIntersections;

    // TODO: все пересечения в i-м кольце
    std::set<Intersection, Greater> getRingIntersections(size_t index) const
    {
        // Пройти по всем вектору и выдать, если есть
        for (const auto& intersections : rIntersections)
        {
            if (intersections.getIndex() == index)
                return intersections.getIntersections();
        }

        return std::set<Intersection, Greater>{};
    }
    //==============================================================================
    /*!
     * Метод возвращает количество пересечений колец.
     * @return size_t
     */
    size_t size() const
    {
        size_t number{0};

        for (const auto& rIntersection : rIntersections)
        {
            number += rIntersection.size();
        }

        return number;
    }
    //==============================================================================
    /*!
     * Метод добавления пересечений в кольце.
     * @param intersection Пересечения в кольце.
     * @return RingIntersection
     */
    RingIntersection insert(const RingIntersection& intersection)
    {
        return rIntersections.emplace_back(intersection);
    }
    //==============================================================================
    /*!
    * Метод проверяет является ли кольцо пустым
    * @return bool
    */
    bool isEmpty() const
    {
        return rIntersections.empty();
    }
    //==============================================================================
};
} // namespace Entities
