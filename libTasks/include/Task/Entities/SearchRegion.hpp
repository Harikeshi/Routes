#pragma once

#include <Task/Entities/SearchRing.hpp>
#include <Task/Schemes/Search/Config.hpp>

/*! Core */
#include <Geometry/Objects/BorderedLine.hpp>
#include <PrimaryEntities/Entities/Polygon.hpp>
#include <TriangularGeometry/PathFinder.hpp>

/*! GEOS */
#include <geos/operation/buffer/BufferParameters.h>

/*! std */
#include <cmath>
#include <optional>

namespace Entities {

using namespace Geometry;
using namespace Schemes::Search;

/*!
 * Пересечение в полигоне.
 * TODO: Пока еще используется
 */
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

/*!
 * \brief Нахождение пересечений, используя Core или Geos
 */
enum IntersectionMethod
{
    Core = 0,
    Geos = 1
};
/*!
 * Класс Поискового региона.
 */
class SearchRegion
{
    /*!
     * Индекс вершины входа.
     */
    size_t entrance;
    //==============================================================================
    /*!
     * Индекс вершины выхода.
     */
    size_t exit;
    //==============================================================================
    /*!
     * Внешнее поисковое кольцо.
     */
    SearchRing mOuterRing;
    //==============================================================================
    /*!
     * Внутренние поисковые кольца.
     */
    std::vector<SearchRing> mInnerRings;

public:
    /*!
     * Конструктор по-умолчанию.
     */
    explicit SearchRegion() // = delete
        : entrance(), exit(), mOuterRing(), mInnerRings()
    {
    }
    //==============================================================================
    /**
     * @brief Пользовательский конструктор из std::initializer_list<SearchRing>.
     * @param rings
     * @param start
     * @param end
     */
    explicit SearchRegion(const std::initializer_list<SearchRing>& rings, const Point2D& start, const Point2D& end);
    //==============================================================================
    /**
     * @brief Пользовательский конструктор из PrimaryEntities::Polygon<Point2D>.
     * @param polygon TODO: Полигон, если можем изменять, то можно подавать по ссылке.
     * @param beginPoint
     * @param endPoint
     */
    explicit SearchRegion(const PrimaryEntities::Polygon<Point2D>& polygon, const Point2D& beginPoint, const Point2D& endPoint);
    //==============================================================================
    /**
     * @brief
     * @param region
     */
    SearchRegion(const SearchRegion& region) = default;
    //==============================================================================
    /**
     * @brief
     * @param rhs
     */
    SearchRegion(SearchRegion&& rhs) = default;
    //==============================================================================
    /**
     * @brief
     * @param polygon
     * @return
     */
    SearchRegion& operator=(const SearchRegion& polygon) = default;
    //==============================================================================
    /**
     * @brief
     * @param rhs
     * @return
     */
    SearchRegion& operator=(SearchRegion&& rhs) = default;
    //==============================================================================
    /**
     * @brief
     */
    ~SearchRegion() = default;
    //==============================================================================
private:
    /**
     * @brief
     * @return
     */
    PrimaryEntities::Polygon<Point2D> toPolygon2D() const;
    //==============================================================================
    /**
     * @brief Метод корректировки согласно принятой в Core.
     */
    void correct();
    //==============================================================================
public:
    /**
     * @brief Метод проверяет корректность ориентации колец согласно Core.
     * Внешнее кольцо - CW, внутренние кольца - CCW
     * @return
     */
    bool hasCorrectOrientation() const;
    //==============================================================================
    /**
     * @brief Перегруженный оператор вывода в поток.
     * @param os
     * @param region
     * @return std::ostream&
     */
    friend std::ostream& operator<<(std::ostream& os, const SearchRegion& region)
    {
        os << "[\n";

        os << region.outer() << "\n";

        for (const auto& ring : region.inners())
            os << ring << "\n";

        os << "]";

        os << " Entrance: [" << region.getEntrancePoint().getX() << ", " << region.getEntrancePoint().getY() << "]\n";
        os << " Exit: [" << region.getExitPoint().getX() << ", " << region.getExitPoint().getY() << "]\n";

        return os;
    }

public:
    //==============================================================================
    /*!
     *\brief Получение внешнего кольца по константной ссылке
     *\return Внешнее кольцо
     */
    SearchRing const& outer() const;
    //==============================================================================
    /*!
    *\brief Получение внешнего кольца по ссылке
    *\return Внешнее кольцо
    */
    SearchRing& outer();
    //==============================================================================
    /*!
     *\brief Получение внутренних колец по константной ссылке.
    *\return std::vector<SearchRing> const&
    */
    std::vector<SearchRing> const& inners() const;
    //==============================================================================
    /*!
    *\brief Получение внутренних колец по ссылке.
    *\return std::vector<SearchRing>&
    */
    std::vector<SearchRing>& inners();
    //==============================================================================
    /**
     * @brief Метод изменяет точку входа.
     */
    void setEntrance(const Point2D& point);
    //==============================================================================
    /**
     * @brief Метод изменяет точку выхода.
     */
    void setExit(const Point2D& point);
    //==============================================================================
    /**
     * @brief Метод возвращает индекс точки входа.
     * @return size_t
     */
    size_t getEntrance() const;
    //==============================================================================
    /**
     * @brief Метод возвращает индекс точки выхода.
     * @return size_t
     */
    size_t getExit() const;
    //==============================================================================
    /**
     * @brief Метод устанавливает кольца региона из заданного полигона.
     * @param polygon Полигон Core.
     */
    void setFromPolygon(const PrimaryEntities::Polygon<Point2D>& polygon);
    //==============================================================================
    /**
     * @brief Метод возвращает полигон Core, полученный на основе колец региона.
     * @return PrimaryEntities::Polygon<Point2D>
     */
    PrimaryEntities::Polygon<Point2D> getPolygon() const;
    //==============================================================================
    /*validate*/
    // Валидация второго уровня, логическая
    void validatePolygon() const;
    void validateEntrance() const;
    void validateExit() const;
    void validate() const;
    //==============================================================================
    /*!
     * \brief intersectionsAllCore
     * \details Поиск всех точек пересечения с полигоном с использованием функции пересечения Core
     * \param line [in] линия пересечения ринга
     * \return [out] Вектор точек пересечения полигона
     */
    std::vector<RingIntersection> intersectionsAllCore(const Line& line) const;
    //==============================================================================
    /*!
     * \brief intersectionsAll
     * \details Расчет пересечений прямой с полигоном
     * \param start [in] Точка относительно которой ищем пересечение
     * \param direction [in] Направление поиска
     * \param intersectionMethod [in] используемый метод пересечения (Core или Geos)
     * \return [out] Точка пересечения в пространстве и на регионе
     */
    std::vector<RingIntersection> intersectionsAll(const Point2D& start, const Radian& direction, const IntersectionMethod& intersectionMethod = Core) const;
    //==============================================================================
    /*!
     * \brief nearestIntersectionAhead
     * \details Расчет ближайшего пересечения со стороной региона по заданному направлению
     * \param start [in] Точка относительно которой ищем пересечение
     * \param direction [in] Направление поиска
     * \param intersectionMethod [in] используемый метод пересечения (Core или Geos)
     * \return [out] объект RingIntersection с одним элементом пересечения - ближайшая точка пересечения в направлении
     */
    RingIntersection nearestIntersectionAhead(const Point2D& start, const Radian& direction, const IntersectionMethod& intersectionMethod = Core) const;
    //==============================================================================
    /*!
     * \brief Метод возвращает все пересечения отрезка с кольцами региона с заданной точностью на основе GEOS.
     * \param p0 [in] Точка начала отрезка.
     * \param p1 [in] Точка конца отрезка.
     * \param eps [in] Заданная точность.
     * \return [out] std::vector<RingIntersection>
    */
    std::vector<RingIntersection> intersectionsAllGEOS(const Point2D& p0, const Point2D& p1, const double eps = 1e-14) const;
    //==============================================================================
    /**
     * @brief Метод возвращает кольцо региона по индексу.
     * 0-outer
     * @param index Индекс кольца.
     * @return SearchRing
     */
    SearchRing getRing(size_t index) const;
    //==============================================================================
    /**
     * @brief Метод возвращает точку входа в поисковый регион.
     * @return Point2D
     */
    Point2D const& getEntrancePoint() const;
    //==============================================================================
    /**
     * @brief Метод возвращает точку выхода из поискового региона.
     * @return Point2D
     */
    Point2D const& getExitPoint() const;
    //==============================================================================
    /**
     * @brief Метод проверки видимости точки выхода из региона из заданной позиции.
     * @param positionCurr Точка заданной позиции.
     * @param eps Заданная точность.
     * @return bool
     */
    bool isSeen(const Point2D& positionCurr) const;
    //==============================================================================
    /*!
     * \brief longerTack выбор того направления (направо, налево) где длиннее галс
     * \param coordinates
     * \param course
     * \param turnAngle
     * \param intersectionMethod используемый метод пересечения (Core или Geos)
     * \return std::optional<std::pair<Radian, double>>
     */
    std::optional<std::pair<Radian, double>> longerTack(const Point2D& coordinates, const Radian& course, const Radian& turnAngle, IntersectionMethod intersectionMethod = Core) const;
    //==============================================================================
    //    /**
    //     * @brief Путь, проложенный (с триангуляцией) к точке выхода
    //     * @param position
    //     * @return std::vector<Point2D>
    //     */
    //    std::vector<Point2D> routeToExit(const Point2D& position) const;
    //==============================================================================
    /*!
     * \brief nextCourse
     * \details Расчет следующего курса, с обработкой случая нахождения в "тупике".
     * \param coordinates [in] Текущая координата
     * \param course [in] Текущий курс
     * \param turnAngle [in] Заданный угол поворота
     * \param checkSupplementaryCourse - построение курса по смежному углу поворота M_PI - turnAngle. true - для Зигзага, false - для Сдвига
     * \param intersectionMethod используемый метод пересечения (Core или Geos)
     * \return std::optional<std::pair<Radian, double>>
     */
    std::optional<std::pair<Radian, double>> nextCourse(const Point2D& coordinates, const Radian& course, const Radian& turnAngle, bool checkSupplementaryCourse = true, IntersectionMethod intersectionMethod = Core) const;
    //==============================================================================
    /**
     * @details Определение начального курса от точки входа. В полигоне с внешним кольцом ориентированным по часовой стрелке строится биссектриса угла при точке входа. Направления (v[i], v[i-1]), биссектриса, (v[i], v[i+1]) ориентированы по часовой стрелке, где v[i] - точка входа, v[i-1], v[i+1] - следующая за и предыдущая к точке входа вершины соответственно
     * @return Radian
     */
    Radian entranceCourse() const;
    //==============================================================================
    /*!
    * \brief tackToExit прямой галс от заданной позиции до точки выхода из района
    * \param position
    * \return отрезок
    */
    BorderedLine tackToExit(const Point2D& position) const;
    //==============================================================================
    /*!
    * \brief расчет предпоследней маршрутной точки, время пути через которую к точке выхода из района будет в пределах заданного времени обследования. Используется теорема косинусов
    * \param pos - текущая позиция
    * \param course - текущий курс
    * \param remainingPath - наибольшая длина пути до точки выхода, чтобы уложиться в заданное время
    */
    bool penultimatePosition(const Point2D& point, Point2D& positionCurr, double remainingPath) const;
    //==============================================================================
    /*!
    * \brief расчет предпоследней маршрутной точки, время пути через которую к точке выхода из района будет в пределах заданного времени обследования. Используется вещественный поиск
    * \param routeBack - последняя точка добавленная в маршрут
    * \param positionCurr - точка, полученная из теоремы косинусов
    * \param lastOptimalPath -
    * \param remainingPath - наибольшая длина пути до точки выхода, чтобы уложиться в заданное время
    * \param path - область триангуляции
    * \param mFinal - точка выхода
    * \return возвращает точку последнего галса
    */
    std::vector<Point2D> pathToExitBisection(const Point2D& positionFirst, const Point2D& positionSecond, const double& lengthError, const double& remainingPath, TriangularGeometry::PathFinder& path) const;

    /*!
     * \brief pathToExit
     * \param path
     * \param route
     * \param positionCurr
     * \param lengthError
     * \param remainingPath
     * \return
     */
    std::vector<Point2D> pathToExit(TriangularGeometry::PathFinder& path, std::vector<Point2D>& route, Point2D& positionCurr, const double& lengthError, const double& remainingPath) const;
    //==============================================================================
    /*!
    * \brief maxTackLength
    * \details Расчет максимально возможной длина галса, чтобы не сблизиться с границей района
    * \param start [in] Точка от которой считается расстояние
    * \param nearestIntersection [in] Место пересечения с полигоном
    * \param traverseMin [in] Траверсное расстояние (или минимальное расстояние до границы)
    * \return double
    */
    double maxTackLength(const Point2D& start, const RingIntersection& intersection) const;
    //==============================================================================
    /*!
     * @brief Метод возвращает количество колец в полигоне.
     * @return size_t
     */
    size_t size() const;
    //==============================================================================
    /*!
     * @brief Метод возвращает расстояние между наиболее удаленными точками внешнего кольца.
     * @return double
     */
    double halfPerimeter() const;
    /*!
     * \brief patrollingLength
     * \details Расчитывает расстояние от точки БЭК до точки выхода
     *          с учётом чётного (не чётного) кол-ва повторений
     * \param countOfRepeats [in] колличество повторений
     * \param pointFirst [in] первая точка БЭК
     * \param pointSecond [in] вторая  точка БЭК
     * \return [out] длина от точки БЭК до точки выхода
     */
    double patrollingLength(const int& countOfRepeats, const Point2D& pointFirst, const Point2D& pointSecond) const;
    /*!
     * \brief longestMidline
     * \param obb
     * \return
     */
    std::pair<Point2D, Point2D> longestMidline(const SearchRing::OBB& obb) const;
    //==============================================================================
    /*!
     * \brief inflatePolygon
     * \return
     */
    std::vector<Polygon2D> crop(const double& distance) const;
};
} // namespace Entities
