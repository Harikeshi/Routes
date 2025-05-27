#pragma once

#include <QPainter>
#include <QPolygonF>
#include <QWidget>

#include <map>

#include "./Models/Object.hpp"
#include "./Models/Target.hpp"
#include "./SegmentObject.hpp"

namespace Scene {
namespace Objects {
enum Objects
{
    Arrow,
    Enemy,
    Ship
};

class ModelObject : public QObject
{
    using Segment = SegmentObject;

    // TODO: Класс движушегося объекта
    // Точка берется с Route
    // ObjectWidget только отрисовывает
    QPolygonF model; // Это форма объекта

    std::map<Objects, std::function<QPolygonF(const double)>> models;

    double radiusHAS; // Радиус ГАС

    double currentVelocity;
    double maxVelocity;

public:
    ModelObject(QObject* parent = nullptr)
        : QObject(parent)
    {
        registerModels();
    }

    void swapCoordinates()
    {
        for (size_t i = 0; i != model.size(); ++i)
        {
            auto x = model[i].x();
            model[i].setX(model[i].y());
            model[i].setY(x);
        }
    }

    double getRadius() const
    {
        return radiusHAS;
    }

    void setRadius(const double radius)
    {
        radiusHAS = radius;
    }

    void initialize(const Models::Object& parameters)
    {
        radiusHAS = parameters.detectionRange;
        currentVelocity = parameters.currentVelocity;
        currentVelocity = parameters.maxVelocity;
    }

    void initialize(const Models::Target& target)
    {
        radiusHAS = target.maxNoiseReduced; // detectionRange;
        currentVelocity = target.currentVelocity;
        currentVelocity = target.maxVelocity;
    }

    void registerModels()
    {
        // Arrow
        models[Objects::Arrow] = [](const double size) {
            QPolygonF arrow;

            arrow << QPointF(0, 0)
                  << QPointF(-size, size / 2)
                  << QPointF(-size, -size / 2);

            return arrow;
        };

        // Enemy
        models[Objects::Enemy] = [](const double size) {
            QPolygonF enemy;

            enemy << QPointF(0, 0)
                  << QPointF(-1.5 * size, 0)
                  << QPointF(-size, +size)
                  << QPointF(0, +size);

            return enemy;
        };
    }

    void draw(QPainter& painter, const Segment& segment, const QColor& color)
    {
        drawHas(painter, segment.getCurrentPoint());

        drawModel(painter, segment, color);
    }

    void drawHas(QPainter& painter, const QPointF& center)
    {
        // TODO: Color?
        painter.setBrush(Qt::NoBrush);

        painter.drawEllipse(center, radiusHAS, radiusHAS);
    }

    void setModel(const QPolygonF& polygon)
    {
        model = polygon;
    }

    // TODO: initmodel
    bool setModel(const Objects object, const double size = 100)
    {
        auto mod = models.find(object);

        if (mod != models.end())
        {
            this->model = mod->second(size);

            return true;
        }

        return false;
    }

    // TODO: Переместить в Object
    void drawModel(QPainter& painter, const Segment& segment, const QColor& color)
    {
        // // Отрисовка стрелки
        QTransform transform; // перенос в точку

        double angle = std::atan2(segment.getEnd().y() - segment.getStart().y(), segment.getEnd().x() - segment.getStart().x());
        transform.translate(segment.getCurrentPoint().x(), segment.getCurrentPoint().y());
        transform.rotateRadians(angle); // повернуть на угол

        auto temp = model;
        model = transform.map(model);

        // Заполнить полигон
        painter.setBrush(color);
        painter.drawPolygon(model);

        std::swap(model, temp);
    }
};
} // namespace Objects
} // namespace Scene