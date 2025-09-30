#pragma once

#include <QPainter>
#include <QPolygonF>
#include <QWidget>

#include <map>

#include "./SegmentObject.hpp"
#include "Project/Models/Object.hpp"
#include "Project/Models/Target.hpp"

namespace Scene::Objects {
enum Objects
{
    Arrow,
    Enemy,
    Ship
};
/*!
 * Класс, который определяет, как будет отображаться Объект.
 */
class ModelObject final : public QObject
{
    using Segment = SegmentObject;

    // TODO: Класс движущегося объекта
    // Точка берется с Route
    // ObjectWidget только для изображения
    QPolygonF model; // Это форма объекта

    std::map<Objects, std::function<QPolygonF(double)>> models;

    double radiusHAS; // Радиус ГАС

    double currentVelocity{0};
    double maxVelocity{0};

public:
    ModelObject(QObject* parent = nullptr)
        : QObject(parent)
    {
        registerModels();
    }

    void show() const
    {
        qDebug() << "Model: QPolygonF:" << model;
        qDebug() << "Model: Radius: " << radiusHAS;
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
        radiusHAS = parameters.detection_range;
        currentVelocity = parameters.search_velocity;
        maxVelocity = parameters.max_velocity;
    }

    void initialize(const Models::Target& target)
    {
        radiusHAS = target.maxNoiseReduced; // detectionRange;
        currentVelocity = target.currentVelocity;
        maxVelocity = target.maxVelocity;
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

    void drawHas(QPainter& painter, const QPointF& center) const
    {
        // TODO: Color?
        painter.setBrush(Qt::NoBrush);

        painter.drawEllipse(center, radiusHAS, radiusHAS);
    }

    void setModel(const QPolygonF& polygon)
    {
        model = polygon;
    }

    // TODO: init_model
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
} // namespace Scene::Objects