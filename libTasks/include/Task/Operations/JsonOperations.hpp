#pragma once

#include <AbstractOperations/Algorithms/Correct.hpp>
#include <Task/Entities/SearchRegion.hpp>
#include <Task/Entities/SearchRing.hpp>
#include <Task/Outputs/Route.hpp>

/*! Core */
#include <Point/Point2D.hpp>
#include <PrimaryEntities/GeometryConverter.hpp>
#include <PrimaryEntities/PointConverter.hpp>

/*! std */
#include <fstream>

#include <nlohmann/json.hpp>

namespace Operations {

using SearchRing = Entities::SearchRing;
using SearchRegion = Entities::SearchRegion;

/**
 * @brief Чтение из файла в объект json.
 *
 * @param path Путь к файлу json;
 * @return nlohmann::json Объект json.
 */
inline nlohmann::json fromfile(const std::string& path)
{
    nlohmann::json object;

    std::ifstream file(path);

    if (!file.is_open())
        throw std::runtime_error("File doesn't open.");

    file >> object;

    if (object.empty())
        throw std::runtime_error("Json Object is empty.");

    file.close();

    return object;
}

/**
 * @brief Функция сериализации Point2D в объект json.
 *
 * @param point Объект Point2D;
 * @return nlohmann::json Объект json.
 */
inline nlohmann::json getJson(const Point2D& point) noexcept
{
    double firstCoord(point.getCoordinates().x);
    double secondCoord(point.getCoordinates().y);

    nlohmann::json jsonArray = nlohmann::json::array();

    jsonArray.push_back(firstCoord);

    jsonArray.push_back(secondCoord);

    return jsonArray;
}

/**
 * @brief Функция сериализации вектора чисел в объект json.
 *
 * @param vec Объект vector<double>;
 * @return nlohmann::json Объект json.
 */
inline nlohmann::json getJson(const std::vector<double>& vec) noexcept
{
    nlohmann::json jsonArray = nlohmann::json::array();

    for (const auto& v : vec)
    {
        jsonArray.push_back(v);
    }

    return jsonArray;
}

/**
 * @brief Функция сериализации вектора Point2D в объект json.
 *
 * @param vec Объект vector<Point2D>;
 * @return nlohmann::json Объект json.
 */
inline nlohmann::json getJson(const std::vector<Point2D>& vec) noexcept
{
    nlohmann::json jsonArray = nlohmann::json::array();

    for (size_t i = 0; i != vec.size(); ++i)
    {
        jsonArray.push_back(getJson(vec[i]));
    }

    return jsonArray;
}

/**
 * @brief Функция десериализации Point2D из объекта json.
 *
 * @param j Объект json;
 * @return Point2D Объект Point2D.
 */
inline Point2D getPointFromJson(const nlohmann::json& j)
{
    double firstCoord = j[0];
    double secondCoord = j[1];

    geos::geom::GeometryFactory::Ptr geometryFactory = geos::geom::GeometryFactory::create();

    std::unique_ptr<geos::geom::Point> geosPoint = geometryFactory->createPoint(geos::geom::CoordinateXY(firstCoord, secondCoord));

    Point2D point(PrimaryEntities::fromGeosGeometry<Point2D>(geosPoint.get()));

    return point;
}

/**
 * @brief Функция десериализации Coordinate из объекта json.
 *
 * @param j Объект json;
 * @return Объект geos::geom::Coordinate.
*/
inline geos::geom::Coordinate getCoordinateFromJson(const nlohmann::json& j)
{
    double firstCoord = j[0].get<double>();
    double secondCoord = j[1].get<double>();

    return geos::geom::Coordinate{firstCoord, secondCoord};
}

/**
 * @brief Функция десериализации std::vector<geos::geom::Coordinate> из объекта json.
 *
 * @param j Объект json;
*/
inline void setFromJson(std::vector<geos::geom::Coordinate>& coordinates, const nlohmann::json& json)
{
    nlohmann::json vec(json);

    coordinates.reserve(vec.size());

    for (std::size_t pointIndex(0); pointIndex < vec.size(); ++pointIndex)
    {
        coordinates.emplace_back(getCoordinateFromJson(vec[pointIndex]));
    }
}

/**
 * @brief Функция десериализации std::vector<std::vector<geos::geom::Coordinate>> из объекта json.
 *
 * @param j Объект json;
*/
inline void setFromJson(std::vector<std::vector<geos::geom::Coordinate>>& vec, const nlohmann::json& json)
{
    vec.reserve(json.size());

    for (std::size_t vectorIndex{0}; vectorIndex < json.size(); ++vectorIndex)
    {
        auto arr = json[vectorIndex]; // json содержащий очередной массив

        std::vector<geos::geom::Coordinate> v; // Вектор, который будем добавлять
        v.resize(arr.size());

        for (std::size_t pointIndex{0}; pointIndex < arr.size(); ++pointIndex)
        {
            v[pointIndex].x = arr[pointIndex][0].get<double>();
            v[pointIndex].y = arr[pointIndex][1].get<double>();
        }

        vec.emplace_back(v);
    }
}

/**
 * @brief Функция сохранения объекта json в файл.
 *
 * @param path Путь к файлу;
 * @param json Объект json;
 * @return true
 * @return false
 */
inline bool toFile(const std::string& path, const nlohmann::json& json)
{
    std::fstream output(path, std::ios_base::out | std::ios_base::trunc);

    if (!output.is_open())
        return false;

    output << json;

    output.close();

    return true;
}

/*!
 * Метод для сохранения файла json в домашнюю директорию в зависимости от операционной системы.
 * @param json
 * @param fileName
 */
inline void saveJsonToHome(const nlohmann::json& json, const std::string& fileName)
{
    try
    {
#ifdef _WIN32
        const char* path = std::getenv("USERPROFILE");
#else
        const char* path = getenv("HOME");
#endif
        Operations::toFile(std::string(path) + "/" + fileName, json);
    }
    catch (...)
    {
    }
}

// Region -> json["search_region"]
inline void validateRegionJson(const nlohmann::json& json)
{
    // TODO: Добавить проверки validate Point2D
    if (json.find("borders") == json.end())
        return throw std::runtime_error("Поле \"borders\" отсутствует.");

    if (!json.at("borders").is_array())
        return throw std::runtime_error("Поле \"borders\" не является массивом.");

    if (json.at("borders").size() == 0)
        return throw std::runtime_error("Пустое поле \"borders\".");

    //! TODO: Проверка на пустые кольца
    for (const auto& border : json.at("borders"))
    {
        if (border.empty())
        {
            throw std::runtime_error("Одно из колец пустое.");
        }
    }

    if (json.find("entry_point") == json.end())
        return throw std::runtime_error("Поле \"entry_point\" отсутствует.");

    if (json.find("exit_point") == json.end())
        return throw std::runtime_error("Поле \"exit_point\" отсутствует.");
}

// Outer
inline void setRegionOuterFromJson(PrimaryEntities::Polygon<Point2D>& polygon, const nlohmann::json& json)
{
    const nlohmann::json& ring(json[0]);

    std::size_t countOfVertices(ring.size());

    polygon.outer().reserve(countOfVertices);

    for (std::size_t vertexIndex(0); vertexIndex < countOfVertices; ++vertexIndex)
    {
        const nlohmann::json& vertex(ring[vertexIndex]);
        polygon.outer().emplace_back(getPointFromJson(vertex));
    }
}

// Inner
inline void setRegionInnersFromJson(PrimaryEntities::Polygon<Point2D>& polygon, const nlohmann::json& json)
{
    polygon.inners().reserve(json.size() - 1);

    for (std::size_t ringIndex{1}; ringIndex < json.size(); ++ringIndex)
    {
        const auto& ring = json[ringIndex];

        PrimaryEntities::Ring<Point2D> innerRing;
        innerRing.resize(ring.size());

        for (std::size_t vertexIndex{0}; vertexIndex < ring.size(); ++vertexIndex)
        {
            nlohmann::json vertex(ring[vertexIndex]);

            auto point = getPointFromJson(vertex);
            innerRing[vertexIndex].set(point.getX(), point.getY());
        }

        polygon.inners().emplace_back(innerRing);
    }
}

/*
inline void setRegionFromJson(Schemes::Search::InRegion::Region& region, const nlohmann::json& json)
{
    validateRegionJson(json);

    // Если валидация прошла
    PrimaryEntities::Polygon<Point2D> polygon{};

    setRegionOuterFromJson(polygon, json["borders"]);

    if (json["borders"].size() > 1)
    {
        setRegionInnersFromJson(polygon, json["borders"]);
    }

    region.setPolygon({polygon});
    region.setEntrance(getPointFromJson(json["entry_point"]));
    region.setExit(getPointFromJson(json["exit_point"]));
}
*/

// SearchRegion
inline void setRingFromJson(SearchRing& ring, const nlohmann::json& json)
{
    ring.clear();

    nlohmann::json jsonRing(json[0]);

    ring.reserve(jsonRing.size());

    for (std::size_t vertexIndex(0); vertexIndex < jsonRing.size(); ++vertexIndex)
    {
        nlohmann::json vertex(jsonRing[vertexIndex]);

        ring.emplace_back(getPointFromJson(vertex));
    }
}

inline void setRegionOuterFromJson(SearchRegion& region, const nlohmann::json& json)
{
    nlohmann::json ring(json[0]);

    std::size_t countOfVertices(ring.size());

    region.outer().clear();
    region.outer().reserve(countOfVertices);

    for (std::size_t vertexIndex(0); vertexIndex < countOfVertices; ++vertexIndex)
    {
        nlohmann::json vertex(ring[vertexIndex]);
        region.outer().emplace_back(getPointFromJson(vertex));
    }
}

inline void setRegionInnersFromJson(SearchRegion& region, const nlohmann::json& json)
{
    region.inners().clear();
    region.inners().reserve(json.size() - 1);

    for (std::size_t ringIndex{1}; ringIndex < json.size(); ++ringIndex)
    {
        auto ring = json[ringIndex];

        SearchRing innerRing;
        innerRing.resize(ring.size());

        for (std::size_t vertexIndex{0}; vertexIndex < ring.size(); ++vertexIndex)
        {
            nlohmann::json vertex(ring[vertexIndex]);

            auto point = getPointFromJson(vertex);
            innerRing[vertexIndex].set(point.getX(), point.getY());
        }

        region.inners().emplace_back(innerRing);
    }
}

/// @todo сделать через конструктор от Полигона и точек входа/выхода
inline void setRegionFromJson(SearchRegion& region, const nlohmann::json& json)
{
    validateRegionJson(json);

    SearchRegion regionTmp;
    //    setRegionOuterFromJson(region, json["borders"]);
    setRegionOuterFromJson(regionTmp, json["borders"]);

    if (json["borders"].size() > 1)
    {
        //        setRegionInnersFromJson(region, json["borders"]);
        setRegionInnersFromJson(regionTmp, json["borders"]);
    }

    region.setFromPolygon(regionTmp.getPolygon());
    region.setEntrance(getPointFromJson(json["entry_point"]));
    region.setExit(getPointFromJson(json["exit_point"]));
}

inline void setVectorPointsFromJson(std::vector<Point2D>& coordinates, const nlohmann::json& json)
{
    nlohmann::json vec(json);
    coordinates.clear();

    coordinates.reserve(vec.size());

    for (std::size_t pointIndex(0); pointIndex < vec.size(); ++pointIndex)
    {
        coordinates.emplace_back(getPointFromJson(vec[pointIndex]));
    }
}

/**
 * @brief Функция десериализации Polygon2D из объекта json.
 *
 * @param j Объект json;
*/
inline void setPolygonFromJson(PrimaryEntities::Polygon<Point2D>& polygon, const nlohmann::json& json)
{
    auto sizeJson = json["search_region"]["borders"].size();

    polygon.outer().resize(json["search_region"]["borders"][0].size());
    polygon.inners().resize(sizeJson - 1);

    for (size_t i = 0; i < sizeJson; ++i)
    {
        if (i == 0)
        {
            for (size_t j = 0; j < json["search_region"]["borders"][0].size(); ++j)
            {
                polygon.outer()[j].set(json["search_region"]["borders"][0][j][0].get<double>(),
                                       json["search_region"]["borders"][0][j][1].get<double>());
            }
        }
        else
        {
            auto sizeInners = json["search_region"]["borders"][i].size();
            polygon.inners()[i - 1].resize(sizeInners);
            for (size_t j = 0; j < sizeInners; ++j)
            {
                polygon.inners()[i - 1][j].set(json["search_region"]["borders"][i][j][0].get<double>(),
                                               json["search_region"]["borders"][i][j][1].get<double>());
            }
        }
    }
    if (!AbstractOperations::correct(polygon))
    {
        throw std::runtime_error("Полигон не скорректирован. ");
    }
}

inline void setRouteFromJson(Outputs::Route& route, const nlohmann::json& json)
{
    auto sizePoints = json["routes"][0]["points"].size();
    auto sizeVelocities = json["routes"][0]["velocities"].size();

    for (size_t i = 0; i < sizePoints; ++i)
    {
        route.points.push_back(Point2D{json["routes"][0]["points"][i][0].get<double>(),
                                       json["routes"][0]["points"][i][1].get<double>()});
    }

    for (size_t i = 0; i < sizeVelocities; ++i)
    {
        route.velocities.push_back(json["routes"][0]["velocities"][i].get<double>());
    }
}

} // namespace Operations
