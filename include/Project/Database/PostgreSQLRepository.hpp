#pragma once

#include <memory>
#include <pqxx/pqxx>
#include <string>

#include <QPointF>
#include <QString>

#include "IRepository.hpp"
#include "Project/Models/Message.hpp"
#include "Project/Models/Object.hpp"
#include "Project/Models/Perimeter.hpp"
#include "Project/Models/Report.hpp"
#include "Project/Models/Request.hpp"
#include "Project/Models/Route.hpp"
#include "Project/Models/Segment.hpp"
#include "Project/Models/Target.hpp"

namespace Database {
using Message = Models::Message;
using Object = Models::Object;
using Perimeter = Models::Perimeter;
using Report = Models::Report;
using Request = Models::Request;
using Route = Models::Route;
using Segment = Models::Segment;
using Target = Models::Target;

class PostgreSQLRepository : public IRepository
{
public:
    PostgreSQLRepository() : Database::IRepository("")
    {
    }

    PostgreSQLRepository(const QString& connectionString)
        : Database::IRepository{connectionString}, m_connection(std::make_unique<pqxx::connection>(connectionString.toStdString()))
    {
        if (!m_connection->is_open())
        {
            throw std::runtime_error("Failed to connect to " + std::string(m_connection->dbname()) + " database");
        }
    }

    size_t getLastId(const std::string& table)
    {
        pqxx::work txn(*m_connection);
        auto result = txn.exec("SELECT id FROM " + table + " ORDER BY id DESC LIMIT 1");

        if (result.empty())
        {
            return 0; // Если таблица пуста
        }

        return result[0]["id"].as<size_t>();
    }

    // TODO: API: save, findById, findAll
    // TODO: Добавить owner
    size_t save(const Message& message, pqxx::work& txn)
    {
        int message_id = 0;

        message_id = txn.exec_params(
                            "INSERT INTO messages (code, text, type) VALUES ($1, $2, $3) RETURNING id",
                            message.code,
                            message.text.toStdString(),
                            message.type.toStdString())[0]["id"]
                         .as<size_t>();

        //        txn.commit();

        return message_id;
    }

    Message findMessageById(size_t id, pqxx::work& txn)
    {
        auto result = txn.exec_params("SELECT id, code, text, type FROM messages WHERE id = $1", id);

        if (result.empty())
        {
            throw std::runtime_error("Message not found");
        }

        auto row = result[0];
        return Message{
            row["id"].as<size_t>(),
            row["code"].as<int>(),
            QString::fromStdString(row["text"].as<std::string>()),
            QString::fromStdString(row["type"].as<std::string>())};
    }

    QVector<Message> findAllMessages()
    {
        pqxx::work txn(*m_connection);
        auto result = txn.exec("SELECT id, code, text, type FROM messages");

        QVector<Message> messages;
        for (const auto& row : result)
        {
            messages.append(Message{row["id"].as<size_t>(),
                                    row["code"].as<int>(),
                                    QString::fromStdString(row["text"].as<std::string>()),
                                    QString::fromStdString(row["type"].as<std::string>())});
        }
        return messages;
    }

    // ObjectRepository implementation
    size_t save(const Object& obj, pqxx::work& txn)
    {
        size_t obj_id = 0;

        obj_id = txn.exec_params(
                        "INSERT INTO objects (detection_range, max_velocity, current_velocity, "
                        "search_velocity, turning_radius, min_length_section) "
                        "VALUES ($1, $2, $3, $4, $5, $6) RETURNING id",
                        obj.detectionRange,
                        obj.maxVelocity,
                        obj.currentVelocity,
                        obj.search_velocity,
                        obj.turning_radius,
                        obj.min_lenght_section)[0]["id"]
                     .as<size_t>();
        //        txn.commit();

        return obj_id;
    }

    Object findObjectById(size_t id, pqxx::work& txn)
    {
        auto result = txn.exec_params(
            "SELECT id, detection_range, max_velocity, current_velocity, "
            "search_velocity, turning_radius, min_length_section "
            "FROM objects WHERE id = $1",
            id);

        if (result.empty())
        {
            throw std::runtime_error("Object not found");
        }

        auto row = result[0];
        return Object{
            row["id"].as<size_t>(),
            row["detection_range"].as<double>(),
            row["max_velocity"].as<double>(),
            row["current_velocity"].as<double>(),
            row["search_velocity"].as<double>(),
            row["turning_radius"].as<double>(),
            row["min_length_section"].as<double>()};
    }

    QVector<Object> findAllObjects()
    {
        pqxx::work txn(*m_connection);
        auto result = txn.exec(
            "SELECT id, detection_range, max_velocity, current_velocity, "
            "search_velocity, turning_radius, min_length_section FROM objects");

        QVector<Object> objects;
        for (const auto& row : result)
        {
            objects.append(Object{row["id"].as<size_t>(),
                                  row["detection_range"].as<double>(),
                                  row["max_velocity"].as<double>(),
                                  row["current_velocity"].as<double>(),
                                  row["search_velocity"].as<double>(),
                                  row["turning_radius"].as<double>(),
                                  row["min_length_section"].as<double>()});
        }
        return objects;
    }

    // PerimeterRepository implementation
    size_t save(const Perimeter& perimeter, pqxx::work& txn)
    {
        size_t perimeter_id = 0;

        // Save perimeter
        perimeter_id = txn.exec_params(
                              "INSERT INTO perimeters (entrance_x, entrance_y, exit_x, exit_y) "
                              "VALUES ($1, $2, $3, $4) RETURNING id",
                              perimeter.entrance.x(),
                              perimeter.entrance.y(),
                              perimeter.exit.x(),
                              perimeter.exit.y())[0][0]
                           .as<size_t>();

        // Delete old rings
        txn.exec_params("DELETE FROM perimeter_rings WHERE perimeter_id = $1", perimeter_id);

        // Save new rings
        for (int ringIndex = 0; ringIndex < perimeter.rings.size(); ++ringIndex)
        {
            const auto& ring = perimeter.rings[ringIndex];

            auto ringResult = txn.exec_params(
                "INSERT INTO perimeter_rings (perimeter_id, ring_index) "
                "VALUES ($1, $2) RETURNING id",
                perimeter_id,
                ringIndex);

            size_t ringId = ringResult[0]["id"].as<size_t>();

            for (int pointIndex = 0; pointIndex < ring.size(); ++pointIndex)
            {
                const auto& point = ring[pointIndex];
                txn.exec_params(
                    "INSERT INTO perimeter_ring_points (ring_id, point_index, x, y) "
                    "VALUES ($1, $2, $3, $4)",
                    ringId,
                    pointIndex,
                    point.x(),
                    point.y());
            }
        }

        txn.commit();

        return perimeter_id;
    }

    Perimeter findPerimeterById(size_t id, pqxx::work& txn)
    {
        // Get perimeter basics
        auto perimeterResult = txn.exec_params(
            "SELECT id, entrance_x, entrance_y, exit_x, exit_y FROM perimeters WHERE id = $1", id);

        if (perimeterResult.empty())
        {
            throw std::runtime_error("Perimeter not found");
        }

        auto row = perimeterResult[0];
        Perimeter perimeter;
        perimeter.id = row["id"].as<size_t>();
        perimeter.entrance = QPointF(row["entrance_x"].as<double>(), row["entrance_y"].as<double>());
        perimeter.exit = QPointF(row["exit_x"].as<double>(), row["exit_y"].as<double>());

        // Get rings
        auto ringsResult = txn.exec_params(
            "SELECT r.id, r.ring_index FROM perimeter_rings r "
            "WHERE r.perimeter_id = $1 ORDER BY r.ring_index",
            id);

        for (const auto& ringRow : ringsResult)
        {
            size_t ringId = ringRow["id"].as<size_t>();
            QPolygonF ring;

            auto pointsResult = txn.exec_params(
                "SELECT x, y FROM perimeter_ring_points "
                "WHERE ring_id = $1 ORDER BY point_index",
                ringId);

            for (const auto& pointRow : pointsResult)
            {
                ring << QPointF(pointRow["x"].as<double>(), pointRow["y"].as<double>());
            }

            perimeter.rings.append(ring);
        }

        return perimeter;
    }

    // RouteRepository implementation
    size_t save(const Route& route, pqxx::work& txn)
    {
        // TODO: Взять последний index
        // Save route
        size_t route_id = txn.exec_params("INSERT INTO routes (id) VALUES (DEFAULT) RETURNING id")[0][0].as<size_t>();

        // Delete old segments
        txn.exec_params("DELETE FROM route_segment_relations WHERE route_id = $1", route_id);
        txn.exec_params(
            "DELETE FROM route_segments WHERE id IN ("
            "SELECT segment_id FROM route_segment_relations WHERE route_id = $1"
            ")",
            route_id);

        // Save segments
        for (int i = 0; i < route.segments.size(); ++i)
        {
            const auto& segment = route.segments[i];

            size_t segment_id = txn.exec_params(
                                       "INSERT INTO route_segments (start_x, start_y, end_x, end_y, base_speed) "
                                       "VALUES ($1, $2, $3, $4, $5) RETURNING id",
                                       segment.start.x(),
                                       segment.start.y(),
                                       segment.end.x(),
                                       segment.end.y(),
                                       segment.baseSpeed)[0][0]
                                    .as<size_t>();

            txn.exec_params(
                "INSERT INTO route_segment_relations (route_id, segment_id, segment_order) "
                "VALUES ($1, $2, $3)",
                route_id,
                segment_id,
                i);
        }

        //        txn.commit();

        return route_id;
    }

    Route findRouteById(size_t id, pqxx::work& txn)
    {
        // Verify route exists
        auto routeResult = txn.exec_params("SELECT id FROM routes WHERE id = $1", id);
        if (routeResult.empty())
        {
            throw std::runtime_error("Route not found");
        }

        Route route;
        route.id = id;

        // Get segments
        auto segmentsResult = txn.exec_params(
            "SELECT s.id, s.start_x, s.start_y, s.end_x, s.end_y, s.base_speed "
            "FROM route_segments s "
            "JOIN route_segment_relations r ON s.id = r.segment_id "
            "WHERE r.route_id = $1 ORDER BY r.segment_order",
            id);

        for (const auto& row : segmentsResult)
        {
            route.segments.push_back(Segment{row["id"].as<size_t>(),
                                             QPointF(row["start_x"].as<double>(), row["start_y"].as<double>()),
                                             QPointF(row["end_x"].as<double>(), row["end_y"].as<double>()),
                                             row["base_speed"].as<double>()});
        }

        return route;
    }

    // RequestRepository implementation
    size_t save(const Request& request)
    {
        auto request_id = this->getLastId("requests") + 1;

        pqxx::work txn(*m_connection);

        // Save perimeter
        auto perimeter_id = save(request.perimeter, txn);

        // Save target
        auto target_id = save(request.target, txn);

        // Save ship
        auto ship_id = save(request.ship, txn);

        // Save request
        txn.exec_params(
            "INSERT INTO requests (id, name, time, perimeter_id, target_id, ship_id) "
            "VALUES ($1, $2, $3, $4, $5, $6)",
            request_id,
            getCurrentUsername(),
            request.time,
            perimeter_id,
            target_id,
            ship_id);

        // Save border points
        txn.exec_params("DELETE FROM request_border_points WHERE request_id = $1", request_id);
        for (int i = 0; i < request.border.size(); ++i)
        {
            const auto& point = request.border[i];
            txn.exec_params(
                "INSERT INTO request_border_points (request_id, point_index, x, y) "
                "VALUES ($1, $2, $3, $4)",
                request_id,
                i,
                point.x(),
                point.y());
        }

        txn.commit();

        return request_id;
    }

    Request findRequestById(size_t id)
    {
        pqxx::work txn(*m_connection);

        auto requestResult = txn.exec_params(
            "SELECT id, time, perimeter_id, target_id, ship_id FROM requests WHERE id = $1", id);

        if (requestResult.empty())
        {
            throw std::runtime_error("Request not found");
        }

        auto row = requestResult[0];
        Request request;
        request.id = row["id"].as<size_t>();
        request.time = row["time"].as<double>();
        request.perimeter = findPerimeterById(row["perimeter_id"].as<size_t>(), txn);
        request.target = findTargetById(row["target_id"].as<size_t>(), txn);
        request.ship = findObjectById(row["ship_id"].as<size_t>(), txn);

        // Get border points
        auto borderResult = txn.exec_params(
            "SELECT x, y FROM request_border_points "
            "WHERE request_id = $1 ORDER BY point_index",
            id);

        for (const auto& borderRow : borderResult)
        {
            request.border.append(QPointF(
                borderRow["x"].as<double>(),
                borderRow["y"].as<double>()));
        }

        return request;
    }

    size_t save(const Report& report)
    {
        size_t report_id = this->getLastId("reports") + 1;

        pqxx::work txn(*m_connection);

        // Verify request exists
        auto requestResult = txn.exec_params("SELECT id FROM requests WHERE id = $1", report.request_id);

        if (requestResult.empty())
        {
            throw std::runtime_error("Request not found");
        }

        // Save report
        txn.exec_params(
            "INSERT INTO reports (id, request_id) VALUES ($1, $2) "
            "ON CONFLICT (id) DO UPDATE SET request_id = $2",
            report_id,
            report.request_id);

        // Save routes
        for (const auto& route : report._routes)
        {
            auto route_id = save(route, txn);

            txn.exec_params(
                "INSERT INTO report_route_relations (report_id, route_id) "
                "VALUES ($1, $2) ON CONFLICT (report_id, route_id) DO NOTHING",
                report_id,
                route_id);
        }

        // Save messages
        for (const auto& message : report._messages)
        {
            auto message_id = save(message, txn);
            txn.exec_params(
                "INSERT INTO report_message_relations (report_id, message_id) "
                "VALUES ($1, $2) ON CONFLICT (report_id, message_id) DO NOTHING",
                report_id,
                message_id);
        }

        txn.commit();

        return report_id;
    }

    Report findReportById(size_t id)
    {
        pqxx::work txn(*m_connection);

        auto reportResult = txn.exec_params(
            "SELECT id, request_id FROM reports WHERE id = $1", id);

        if (reportResult.empty())
        {
            throw std::runtime_error("Report not found");
        }

        auto row = reportResult[0];
        Report report;
        report.id = row["id"].as<size_t>();
        report.request_id = row["request_id"].as<size_t>();

        // Get routes
        auto routesResult = txn.exec_params(
            "SELECT route_id FROM report_route_relations WHERE report_id = $1", id);

        for (const auto& routeRow : routesResult)
        {
            report._routes.append(findRouteById(routeRow["route_id"].as<size_t>(), txn));
        }

        // Get messages
        auto messagesResult = txn.exec_params(
            "SELECT message_id FROM report_message_relations WHERE report_id = $1", id);

        for (const auto& messageRow : messagesResult)
        {
            report._messages.append(findMessageById(messageRow["message_id"].as<size_t>(), txn)); // FindMessageById
        }

        return report;
    }

    QVector<Report> findReportsByRequestId(size_t request_id)
    {
        pqxx::work txn(*m_connection);

        auto reportsResult = txn.exec_params(
            "SELECT id FROM reports WHERE request_id = $1 ORDER BY created_at", request_id);

        QVector<Report> reports;
        for (const auto& row : reportsResult)
        {
            reports.append(findReportById(row["id"].as<size_t>()));
        }

        return reports;
    }

    //private:
    std::unique_ptr<pqxx::connection> m_connection;

    size_t save(const Target& target, pqxx::work& txn)
    {
        size_t target_id = 0;

        target_id = txn.exec_params(
                           "INSERT INTO targets (detection_point_x, detection_point_y, course_min, course_max, root_mean_square_error, current_velocity, max_velocity, min_noise_reduced, max_noise_reduced, obsolescence_time, avoidance_distance) VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11) RETURNING id",
                           target.detectionPoint.x(),
                           target.detectionPoint.y(),
                           target.courses.first,
                           target.courses.second,
                           target.rootMeanSquareError,
                           target.currentVelocity,
                           target.maxVelocity,
                           target.minNoiseReduced,
                           target.maxNoiseReduced,
                           target.obsolescenceTime,
                           target.avoidanceDistance)[0][0]
                        .as<size_t>();

        txn.commit();

        return target_id;
    }

    Target findTargetById(size_t id, pqxx::work& txn)
    {
        auto result = txn.exec_params(
            "SELECT "
            "id, detection_point_x, detection_point_y, course_min, course_max, "
            "root_mean_square_error, current_velocity, max_velocity, "
            "min_noise_reduced, max_noise_reduced, obsolescence_time, avoidance_distance "
            "FROM targets WHERE id = $1",
            id);

        if (result.empty())
        {
            throw std::runtime_error("Target not found");
        }

        auto row = result[0];
        return Target{
            row["id"].as<size_t>(),
            QPointF(row["detection_point_x"].as<double>(), row["detection_point_y"].as<double>()),
            {row["course_min"].as<double>(), row["course_max"].as<double>()},
            row["root_mean_square_error"].as<double>(),
            row["current_velocity"].as<double>(),
            row["max_velocity"].as<double>(),
            row["min_noise_reduced"].as<double>(),
            row["max_noise_reduced"].as<double>(),
            row["obsolescence_time"].as<double>(),
            row["avoidance_distance"].as<double>()};
    }
};
} // namespace Database