#pragma once

#include <QColor>
#include <QPainter>
#include <QPointF>
#include <QTimer>
#include <QVector>
#include <QWidget>

#include "../Objects/Models/Object.hpp"
#include "../Objects/RouteObject.hpp"

#include "./Limits.hpp"

namespace Scene {
namespace Entities {
// QObject
class Routes : public QWidget
{
    using StateType = Objects::StateType;
    using RouteState = Objects::RouteState;
    using RouteObject = Objects::RouteObject;
    using Object = Objects::Models::Object;
    using ShipParameters = Objects::Models::Object;
    using Limits = Entities::Limits;
    using Route = Objects::Models::Route;
    using Objects = Objects::Objects;

    Q_OBJECT

    // Цвета для БЭНКов не меняются
    const QVector<QColor> palette = {Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::cyan, Qt::magenta};

    // TODO: напрашивается RoutesWidget
    Object parameters;             // Возможно построение routes делать из ViWidget
    ShipParameters shipParameters; // Возможно построение routes делать из ViWidget

    QVector<RouteObject*> routes{};
    size_t numberRoutes; // количество не законченных плавание

signals:
    //void sendCurrentPoint(const std::pair<size_t, QPointF>&);
    void sendCurrentInformation(QVector<std::pair<QPointF, double>>&);
    void sendCurrentTime(const double&);
    void stop(void);

public:
    explicit Routes(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        routes = QVector<RouteObject*>();
        numberRoutes = 0;
    }

    void draw(QPainter& painter)
    {
        for (const auto& route : routes)
        {
            route->draw(painter);

            //    /*    sendIntersectionResult(QString("Target Position: [" + QString("%1, %2").arg(target->getCurrentPosition().x()).arg(target->getCurrentPosition().y()) + ", Ship[" +
            //                                       QString("] position:") + QString("%1, %2").arg(route->getCurrentPosition().x()).arg(route->getCurrentPosition().y()) +
            //                                      QString(", Radius: ") + QString("%1").arg(route->getRadius())));*/
        }
    }

    // Сброс текущих значений до начальных current->start
    // clear
    void clear()
    {
        for (auto& route : routes)
        {
            if (!route->getSegments().isEmpty())
            {
                route->clear();
            }
        }
    }

    void setParameters(const Object& params)
    {
        parameters = params;
    }

    void swapCoordinates()
    {
        for (auto& route : routes)
        {
            route->swapCoordinates();
        }
    }

    void setRoutes(const QVector<Route>& routes_)
    {
        // Очищаем старые значения
        routes.clear();

        // Устанавливаем новые
        for (const auto& route_ : routes_)
        {
            auto route = new RouteObject(this);

            route->initialize(route_.getSegments(), palette[routes.size() % palette.size()], parameters);

            this->routes.append(route);
        }

        numberRoutes = routes.size();

        this->clear(); // После загрузки обновляем
    }

    void setModel(const Objects model, const double size)
    {
        for (size_t i = 0; i != routes.size(); ++i)
        {
            routes[i]->setModel(model, parameters.detectionRange, size);
        }
    }

    void move(const double& speedMultiplier)
    {
        QVector<std::pair<QPointF, double>> positions(routes.size());

        for (size_t i = 0; i != routes.size(); ++i)
        {
            // Когда путь заканчивается таймер останавливается
            if (!routes[i]->update(speedMultiplier))
            {
                positions[i] = std::make_pair(routes[i]->getCurrentPosition(), routes[i]->getSpeed());
                --numberRoutes;

                if (numberRoutes == 0)
                {
                    qDebug() << "Общая длина пути: " << routes[i]->getCurrentLength() << " метров.";
                    qDebug() << "Время в пути: " << routes[i]->getCurrentTime() << " секунд.";

                    emit stop();
                }
            }
            // TODO: {номер маршрута, координаты}
            emit sendCurrentInformation(positions);
        }

        emit sendCurrentTime(routes.first()->getCurrentTime());
    }

    bool isEmpty() const
    {
        return routes.isEmpty();
    }

    void setStateType(RouteState* state)
    {
        for (const auto route : routes)
        {
            route->setState(state);
        }
    }

    // У всех состояние отрисовки одинаковое сейчас
    StateType getStateType() const
    {
        if (routes.isEmpty())
        {
            return StateType::Clean;
        }

        return routes.first()->getStateType();
    }

    void setLimits(Limits& limits) const
    {
        for (const auto& route : routes)
        {
            for (const auto& segment : route->getSegments())
                limits.initFromSegment(segment->getSegment());
        }
    }
};
} // namespace Entities
} // namespace Scene