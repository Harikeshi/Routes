#pragma once

#include <QColor>
#include <QPointF>
#include <QTimer>
#include <QVector>
#include <QWidget>

#include "../Objects/RouteObject.hpp"
#include "Project/Models/Object.hpp"
// #include "Project/Scene/Objects/PointWidget.hpp"

#include "./Limits.hpp"

namespace Scene::Entities {
// QObject
class Routes final : public QWidget
{
    using StateType = Objects::StateType;
    using RouteState = Objects::State;
    using RouteObject = Objects::RouteObject;
    using Object = Models::Object;
    using ShipParameters = Models::Object;
    using Limits = Entities::Limits;
    using Route = Models::Route;
    using Objects = Objects::Objects;
    // using PointWidget = Scene::Objects::PointWidget;

    Q_OBJECT

    // Цвета для БЭНКов не меняются
    const QVector<QColor> palette = {Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::cyan, Qt::magenta};

    // TODO: напрашивается RoutesWidget
    Object parameters; // Возможно построение routes делать из ViWidget

    QVector<RouteObject*> routes{};

    size_t numberRoutes; // количество не законченных

signals:
    void sendIndexCurrentPositionSpeed(size_t, const QPointF&, double);

    void complete(void);

public:
    Routes(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        parameters = Object{};
        routes = QVector<RouteObject*>();
        numberRoutes = 0;
    }

    double getRadius() const
    {
        return parameters.detection_range;
    }

    void reset()
    {
        routes.clear();

        numberRoutes = 0;
        parameters.reset();
    }

    void changeShowPoints()
    {
        for (const auto& route : routes)
            route->changeShowPoints();
    }

    void draw(QPainter& painter)
    {
        for (const auto& route : routes)
        {
            route->draw(painter);

            // sendIntersectionResult(QString("Target Position: [" + QString("%1, %2").arg(target->getCurrentPosition().x()).arg(target->getCurrentPosition().y()) + ", Ship[" +
            //                                       QString("] position:") + QString("%1, %2").arg(route->getCurrentPosition().x()).arg(route->getCurrentPosition().y()) +
            //                                      QString(", Radius: ") + QString("%1").arg(route->getRadius())));
        }
    }

    double getMaximumTime() const
    {
        double maximum{0};
        for (const auto& route : routes)
            if (maximum < route->getFullTime())
                maximum = route->getFullTime();

        return maximum;
    }

    /*!
         * Метод сброса текущей точки к стартовой для всех маршрутов.
         */
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

    /*!
         * Метод смены координат  X<->Y
         */
    void swapCoordinates()
    {
        //! Пути
        for (auto& route : routes)
        {
            route->swapCoordinates();
        }
    }

    // TODO:
    void setRoutes(const QVector<Route>& routes_, double pointSize)
    {
        // Очищаем старые значения
        routes.clear();

        // this->clear();
        // Устанавливаем новые
        for (const auto& route_ : routes_)
        {
            auto route = new RouteObject(this);

            route->initialize(route_.getSegments(), palette[routes.size() % palette.size()], parameters, pointSize);

            this->routes.append(route);
        }

        numberRoutes = routes.size();

        this->clear(); // После загрузки обновляем
    }

    size_t size() const
    {
        return routes.size();
    }

    const QVector<RouteObject*>& getRoutes() const
    {
        return routes;
    }

    void setModel(const Objects model, const double size)
    {
        for (const auto& route : routes)
        {
            route->setModel(model, parameters.detection_range, size);
        }
    }

    void mousePressEvent(QMouseEvent* event, const QPointF& point)
    {
        for (const auto& route : routes)
        {
            route->mousePressEvent(event, point);
        }
    }

    void move(double time)
    {
        for (size_t i = 0; i != routes.size(); ++i)
        {
            if (!routes[i]->move(time))
            {
                --numberRoutes;
                // TODO: Сюда не попадает!
                if (numberRoutes == 0)
                {
                    qDebug() << "Общая длина пути: " << routes[i]->getLength() << " метров.";
                    emit complete();
                }
            }

            emit sendIndexCurrentPositionSpeed(i + 1, routes.at(i)->getCurrentPosition(), routes.at(i)->getSpeed());
            // TODO: {номер маршрута, координаты}
            //emit sendCurrentInformation(positions);
        }
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
} // namespace Scene::Entities
