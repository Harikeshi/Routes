#pragma once

// Task
#include <Task/Outputs/Route.hpp>
#include <Task/Schemes/Search/InRegion/Input.hpp>

namespace Entities {
using Input = Schemes::Search::InRegion::Input;
using SearchRing = Entities::SearchRing;
using Line = Entities::Line;

class EfficiencyIndicators
{
protected:
    PrimaryEntities::Box<Point2D> boxPolygon;
    std::vector<std::vector<double>> observationDensity;
    std::vector<std::vector<bool>> mask;
    double routeSquare;
    int cellLength;

    /*!
     * \brief boxTack
     * \details Расчитывает бокс минимального размера, в котором лежит галс с учетом зоны обнаржения, представленный в виде двух точек
     * \param tack [in] отрезок
     * \param detRange [in] радиус обнаружения
     * \return [out] две точки: нижняя левая вершина, верхняя правая вершина
     */
    std::pair<Point2D, Point2D> boxTack(const Geometry::BorderedLine& tack, const double& detRange);

    /*!
     * \brief initializeMask инициализация маски
     * \param sizeX [in] количество ребер решетки по оси X внутри бокса полигона с учетом буфера ширины дальности обнаружения (внешний бокс)
     * \param sizeY [in] количество ребер решетки по оси Y внутри внешнего бокса
     */
    void initializeMask(const size_t& sizeX, const size_t& sizeY);

    /*!
     * \brief calculateMask расчет маски
     * \details Решетка буллевых значений в зависимости от принадлежности точек решетки полигону.
     * \param polygon [in] полигон
     * \param minX [in] минимальная координата по оси X
     * \param minY [in] минимальная координата по оси Y
     * \param sizeX [in] количество ребер решетки по оси X внутри бокса полигона с учетом буфера ширины дальности обнаружения (внешний бокс)
     * \param sizeY [in] количество ребер решетки по оси Y внутри внешнего бокса
     */
    void calculateMask(const PrimaryEntities::Polygon<Point2D>& polygon, const double& minX, const double& minY, const int& sizeX, const int& sizeY);

    /*!
     * \brief initializeObservationDensity инициализация матрицы плотности наблюдения
     * \param sizeX [in]  число ребер решетки по оси х внутри бокса полигона с учетом буфера ширины дальности обнаружения (внешний бокс)
     * \param sizeY [in] количество ребер решетки по оси Y внутри внешнего бокса
     */
    void initializeObservationDensity(const int& sizeX, const int& sizeY);

    /*!
     * \brief addObservationTack
     * \details Добавить время наблюдения каждой точки бокса, содержащего галс с учетом зоны обнаржения
     * \param minCorner [in] нижняя левая вершина бокса
     * \param firstPoint [in] начальная точка галса
     * \param secondPoint [in] конечная точка галса
     * \param detRange [in] радиус обнаружения
     */
    void addObservationTack(const Point2D& firstPoint, const Point2D& secondPoint, const double& searchVelocity, const double& detRange);

    /*!
     * \brief sumObservationTack
     * \details Сложить вклады во время наблюдения от всех галсов
     * \param box [in] бокс полигона с учетом буфера ширины дальности обнаружения
     * \param route [in] маршрут
     * \param detRange [in] дальность обнаружения
     */
    void sumObservationTack(const Outputs::Route& route, const double& detRange);

    /*!
     * \brief observationIntegral Наработка наблюдения (площадь х время).
     * \details Интеграл по матрице плотности наблюдения по всему боксу полигона с учетом буфера ширины дальности обнаружения (внешний бокс).
     * \return [out]
     */
    double observationIntegral();

public:
    /*!
     * \brief EfficiencyIndicators
     * \details Конструктор класса показателей эффективности схемы
     * \param cl [in] размер клетки
     */
    EfficiencyIndicators(const int& cl)
        : cellLength(cl){};

    /*!
     * \brief calculateRouteSquare
     * \details Расчет площади области которая была покрыта поисковым маршрутом
     * \param polygon [in] полигон
     * \param route [in] маршрут
     * \param detRange [in] дальность обнаружения
     */
    void calculateRouteSquare(const PrimaryEntities::Polygon<Point2D>& polygon, Outputs::Route& route, const double& detRange);

    /*!
     * \brief calculateObservationDensity
     * \details Расчет матрицы плотности поиска
     * \param polygon [in] полигон
     * \param route [in] маршрут
     * \param detRange [in] дальность обнаружения
     */
    void calculateObservationDensity(PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange);

    /*!
     * @note пока что некорректная интерпретация метрики
     *
     * \brief averageProductivity
     * \details Расчёт средней производительности поиска
     * \param polygon [in] полигон
     * \param route [in] маршрут
     * \param detRange [in] дальность обнаружения
     * \param searchTime [in] поисковое время
     * \return [out]
     */
    double averageProductivity(PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange, const double& searchTime);

    /*!
     * \brief averageTime
     * \details Расчёт среднего времени наблюдения каждой точки в боксе
     * \param polygon [in] полигон
     * \param route [in] маршрут
     * \param detRange [in] дальность обнаружения
     * \return [out] среднее время
     */
    double averageTime(PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange);

    /*!
     * \brief timeStatistics
     * \details Расчет среднего времени наблюдения и СКО (среднеквадратичное отклонение) времени наблюдения
     * \param polygon [in] полигон
     * \param route [in] маршрут
     * \param detRange [in] дальность обнаружения
     * \return [out] среднее время и СКО
     */
    std::pair<double, double> timeStatistics(PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange);

    /*!
     * \brief getObservationDensity
     * \return [out] плотность
     */
    std::vector<std::vector<double>> getObservationDensity() const;

    /*!
     * \brief getMask
     * \return [out] маска
     */
    std::vector<std::vector<bool>> getMask() const;

    /*!
     * \brief getRouteSquare
     * \return [out] площадь маршрута
     */
    double getRouteSquare() const;
};

} // namespace Entities
