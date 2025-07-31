#pragma once

#include "PointWidget.hpp"
#include "SegmentObject.hpp"

namespace Scene::Objects {
enum StateType
{
    Clean,
    Current,
    Full
};

class RouteState
{
    StateType state;

public:
    RouteState(const StateType& state)
        : state{state}
    {
    }

    StateType type() const
    {
        return state;
    }

    // TODO: Добавить индекс текущего сегмента чтобы не считались все
    virtual void draw(QPainter& painter, const QVector<SegmentObject*>& segments, const QColor& color) = 0;
    virtual ~RouteState() = default;
};

// Без отрисовки nullptr
class WithOutDrawState : public RouteState
{
public:
    WithOutDrawState()
        : RouteState(StateType::Clean)
    {
    }

    void draw(QPainter& painter, const QVector<SegmentObject*>& segments, const QColor& color) override
    {
        QPen pen = QPen(color, 2);
        pen.setCosmetic(true);

        painter.setPen(pen);
    }
};

class CurrentDrawState : public RouteState
{
public:
    CurrentDrawState()
        : RouteState(StateType::Current)
    {
    }
    // todo: общая функция draw и drawPoints drawSegments
    void draw(QPainter& painter, const QVector<SegmentObject*>& segments, const QColor& color) override
    {
        for (const auto& segment : segments)
        {
            segment->drawCurrent(painter, color);
        }
    }
};

class FullDrawState : public RouteState
{
public:
    FullDrawState()
        : RouteState(StateType::Full)
    {
    }

    void draw(QPainter& painter, const QVector<SegmentObject*>& segments, const QColor& color) override
    {
        for (const auto& segment : segments)
        {
            segment->drawFull(painter, color);
        }
    }
};

#include <QWidget>

// Memento
class Memento
{
    RouteState* state;
    Qt::Key key;

public:
    Memento(RouteState* state_, Qt::Key key)
        : state(state_), key(key)
    {
    }

    Qt::Key getKey() const
    {
        return key;
    }

    RouteState* getState() const
    {
        return state;
    }
};
} // namespace Scene::Objects
