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

class State
{
    StateType state;

public:
    State(const StateType& state)
        : state{state}
    {
    }

    StateType type() const
    {
        return state;
    }

    // TODO: Добавить индекс текущего сегмента чтобы не считались все
    virtual void draw(QPainter& painter, const QVector<SegmentObject*>& segments, const QColor& color) = 0;
    virtual ~State() = default;
};

// Без отрисовки nullptr
class WithOutDrawState : public State
{
public:
    WithOutDrawState()
        : State(StateType::Clean)
    {
    }

    void draw(QPainter& painter, const QVector<SegmentObject*>& segments, const QColor& color) override
    {
        QPen pen = QPen(color, 2);
        pen.setCosmetic(true);

        painter.setPen(pen);
    }
};

class CurrentDrawState : public State
{
public:
    CurrentDrawState()
        : State(StateType::Current)
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

class FullDrawState : public State
{
public:
    FullDrawState()
        : State(StateType::Full)
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
    State* state;
    Qt::Key key;

public:
    Memento(State* state_, Qt::Key key)
        : state(state_), key(key)
    {
    }

    Qt::Key getKey() const
    {
        return key;
    }

    State* getState() const
    {
        return state;
    }
};
} // namespace Scene::Objects
