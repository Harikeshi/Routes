#pragma once

#include "Models/Object.hpp"
#include "SegmentWidget.hpp"

#include <QPainter>
#include <QPolygonF>
#include <QWidget>

#include <map>

namespace Visual {

enum Objects
{
    Arrow,
    Enemy,
    Ship
};

class ObjectWidget : public QWidget
{
    using Segment = Visual::SegmentWidget;

    // TODO: Класс движушегося объекта
    // Точка берется с Route
    // ObjectWidget только отрисовывает
    QPolygonF model; // Это форма объекта

    std::map<Visual::Objects, std::function<QPolygonF(const double)>> models;

    double radiusHAS; // Радиус ГАС

    double currentVelocity;
    double maxVelocity;

public:
    ObjectWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        registerModels();
    }

    void initialize(const Visual::Models::Object& parameters)
    {
        radiusHAS = parameters.getDetectionRange();
        currentVelocity = parameters.getCurrentVelocity();
        currentVelocity = parameters.getMaxVelocity();
    }

    void registerModels()
    {
        // Arrow
        models[Visual::Objects::Arrow] = [](const double size) {
            QPolygonF arrow;

            arrow << QPointF(0, 0)
                  << QPointF(-size, size / 2)
                  << QPointF(-size, -size / 2);

            return arrow;
        };

        // Enemy
        models[Visual::Objects::Enemy] = [](const double size) {
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
        qDebug() << "Позиция подается в Object: " << segment.getCurrentPoint();

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
        double angle = std::atan2(segment.getEnd().y() - segment.getStart().y(), segment.getEnd().x() - segment.getStart().x());

        qDebug() << "Угол поворота: " << angle;
        qDebug() << "Позиция подается в drawModel: " << segment.getCurrentPoint();

        QTransform transform; // перенос в точку
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
} // namespace Visual
