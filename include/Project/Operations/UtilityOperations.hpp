#pragma once

#include <Project/Models/Report.hpp>
#include <Project/Models/Request.hpp>
#include <Task/Entities/Efficiency.hpp>
#include <Task/Operations/JsonOperations.hpp>

namespace Operations {

inline void show_(const PrimaryEntities::Polygon<Point2D>& polygon)
{
    //! Полигон
    std::cout << "outer: [";
    for (const auto& point : polygon.outer())
        std::cout << "[" << point.getX() << ", " << point.getY() << "], ";
    std::cout << "]" << std::endl;
    std::cout << "inners: [";
    for (const auto& inner : polygon.inners())
    {
        for (const auto& point : inner)
            std::cout << "[" << point.getX() << ", " << point.getY() << "], ";
        std::cout << "]" << std::endl;
    }
    std::cout << "]" << std::endl;
}

/*!
 * \brief routeFromReport
 * \param report
 * \return
 */
inline PrimaryEntities::Polygon<Point2D> polygonFromRequest(const Models::Request& request)
{
    if (request.getPerimeter().getRings().isEmpty())
        throw std::runtime_error("Perimeter.Rings.isEmpty()");

    PrimaryEntities::Polygon<Point2D> result;
    Schemes::Search::InRegion::Input input;
    auto json = request.toNJson();

    try
    {
        input.fromJson(json);
        result = input.region.getPolygon();
    }
    catch (const std::exception& e)
    {
        throw;
    }

    return result;
}

/*!
 * \brief routeFromReport
 * \param report
 * \return
 */
inline Outputs::Route routeFromReport(const Models::Report& report)
{
    std::vector<Point2D> route;
    std::vector<double> vels;

    for (auto const segment : report.routes()[0].getSegments())
    {
        route.push_back(Point2D{segment.getStart().x(), segment.getStart().y()});
        vels.push_back(segment.baseSpeed);
    }

    route.push_back(Point2D{report.routes()[0].getSegments().last().getEnd().x(), report.routes()[0].getSegments().last().getEnd().y()});

    return Outputs::Route{route, vels};
}

/*!
 * \brief buildMatrix
 * \param polygon
 * \param route
 * \param detRange
 * \return
 */
inline std::vector<std::vector<double>> buildMatrix(PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange)
{
    //return {{1, 1, 1}, {2, 2, 2}, {1, 1, 1}};

    Entities::EfficiencyIndicators indicators(100);
    // bbox
    indicators.calculateObservationDensity(polygon, route, detRange);

    return indicators.getObservationDensity();
}

/*!
 * \brief buildAverageTime
 * \param polygon
 * \param route
 * \param detRange
 * \param searchVelocity
 * \return
 */
inline double buildAverageTime(PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange)
{
    Entities::EfficiencyIndicators indicators(100);

    return indicators.averageTime(polygon, route, detRange);
}

inline std::pair<double, double> buildTimeStatistics(PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange)
{
    Entities::EfficiencyIndicators indicators(100);

    return indicators.timeStatistics(polygon, route, detRange);
}
} // namespace Operations
