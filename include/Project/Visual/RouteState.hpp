#pragma once

#include "SegmentWidget.hpp"

namespace Visual {

enum State
{
    Clean,
    Current,
    Full
};

class RouteState
{
    State state;

public:
    RouteState(const State& state)
        : state{state}
    {
    }

    State type() const
    {
        return state;
    }

    virtual void draw(QPainter& painter, const QVector<Visual::SegmentWidget*>& segments, const QColor& color) = 0;
    virtual ~RouteState() = default;
};

// Без отрисовки nullptr
class WithOutDrawState : public RouteState
{
public:
    WithOutDrawState()
        : RouteState(State::Clean)
    {
    }

    void draw(QPainter& painter, const QVector<Visual::SegmentWidget*>& segments, const QColor& color) override
    {
    }
};

class CurrentDrawState : public RouteState
{
public:
    CurrentDrawState()
        : RouteState(State::Current)
    {
    }

    void draw(QPainter& painter, const QVector<Visual::SegmentWidget*>& segments, const QColor& color) override
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
        : RouteState(State::Full)
    {
    }
    void draw(QPainter& painter, const QVector<Visual::SegmentWidget*>& segments, const QColor& color) override
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
} // namespace Visual