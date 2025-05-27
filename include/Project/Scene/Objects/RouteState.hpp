#pragma once

#include "./SegmentObject.hpp"

namespace Scene {
namespace Objects {
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

    void draw(QPainter& painter, const QVector<SegmentObject*>& segments, const QColor& color) override
    {
        for (size_t i = 0; i < segments.size(); ++i)
        {
            auto segment = segments.at(i);
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
        for (size_t i = 0; i < segments.size(); ++i)
        {
            auto segment = segments.at(i);
            segment->drawFull(painter, color);
        }
    }
};

#include <QWidget>

// Memento
class Memento
{
private:
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
} // namespace Objects
} // namespace Scene