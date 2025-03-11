#pragma once

#include "ObjectWidget.hpp"
#include "RouteState.hpp"
#include "SegmentWidget.hpp"

#include <QDebug>
#include <QPainter>
#include <QStack>
#include <QVector2D>
#include <QWidget>

namespace Visual {

class RouteState;
class WithOutDrawState;
class FullDrawState;
class CurrentDrawState;

class RouteWidget : public QWidget
{
    // Q_OBJECT
public:
    RouteWidget(QWidget* parent = nullptr, RouteState* state = new CurrentDrawState())
        : QWidget(parent), state(state), head(new ObjectWidget())
    {
    }

    // Вычислить текущую точку,
    bool update(const double speedMultiplier)
    {
        if (currentSegmentIndex >= segments.size())
        {
            return false;
        }

        // Вычисление новой позиции точки
        const SegmentWidget* segment = segments[currentSegmentIndex];
        QVector2D direction(segment->getEnd() - segment->getStart());
        direction.normalize();

        auto lastCurrentPoint = segments[currentSegmentIndex]->getCurrentPoint();

        QVector2D step = direction * segment->getSpeed() * speedMultiplier; // за минуту с учетом множителя скорости
        QPointF nextPosition = lastCurrentPoint + step.toPointF();

        if (QVector2D(nextPosition - segment->getStart()).length() >= QVector2D(segment->getEnd() - segment->getStart()).length())
        {
            segments[currentSegmentIndex++]->setCurrentPoint(segment->getEnd()); //*
            currentTime += std::hypot(lastCurrentPoint.x() - segment->getEnd().x(), lastCurrentPoint.y() - segment->getEnd().y()) / segment->getSpeed();
            currentLength += std::hypot(lastCurrentPoint.x() - segment->getEnd().x(), lastCurrentPoint.y() - segment->getEnd().y());
        }
        else
        {
            segments[currentSegmentIndex]->setCurrentPoint(nextPosition); //*

            currentTime += std::hypot(lastCurrentPoint.x() - nextPosition.x(), lastCurrentPoint.y() - nextPosition.y()) / segment->getSpeed();
            currentLength += std::hypot(lastCurrentPoint.x() - nextPosition.x(), lastCurrentPoint.y() - nextPosition.y());
        }

        return true;
    }

    // TODO: position заменить
    QPointF getCurrentPosition() const
    {
        // Проверку, если путь закончен то последнюю точку, в остальных случаях сегмент[current].getCurrent()
        if (segments.isEmpty())
        {
            return QPointF{-1e30, -1e30};
        }

        if (currentSegmentIndex >= segments.size())
        {
            return segments.back()->getCurrentPoint();
        }

        return segments[currentSegmentIndex]->getCurrentPoint();
    }

    // TODO: Пересмотреть
    void initialize(const QVector<Visual::Models::Segment>& segments, const QColor& color, const Visual::Models::Object& parameters)
    {
        for (const auto& segment : segments)
        {
            this->segments.push_back(new SegmentWidget(this, segment));
        }

        this->setColor(color);
        this->setRadius(parameters.getDetectionRange());

        this->head->initialize(parameters);

        clear();
    }

    void reset()
    {
        clear();

        segments.clear(); // TODO: это reset
    }

    // TODO: Только очистка
    void clear()
    {
        // Сброс к начальным значениям пути
        currentTime = 0;
        currentLength = 0;
        currentSegmentIndex = 0; // Выбран первый отрезок

        if (!segments.isEmpty())
        {
            for (const auto& segment : segments)
            {
                segment->clear();
            }
        }
    }

    /* get/set */
    size_t getCurrentIndex() const
    {
        return currentSegmentIndex;
    }

    void addSegment(QPointF start, QPointF end, double speed)
    {
        SegmentWidget* segment = new SegmentWidget(this, {start, end, speed});

        segments.push_back(segment);
    }

    QColor getColor() const
    {
        return color;
    }

    void setColor(const QColor& c)
    {
        color = c;
    }
    const QVector<SegmentWidget*> getSegments() const
    {
        return segments;
    }

    double getRadius() const
    {
        return head->getRadius();
    }

    void setRadius(const double& radius)
    {
        head->setRadius(radius);
    }

    bool isEmpty() const
    {
        return segments.isEmpty();
    }

    void show() const
    {
        for (const auto& segment : segments)
        {
            segment->show();
        }
    }

    double getCurrentTime() const
    {
        return currentTime;
    }

    double getCurrentLength() const
    {
        return currentLength;
    }

    double getSpeed() const
    {
        if (segments.size() != 0 && currentSegmentIndex < segments.size())
        {
            return segments[currentSegmentIndex]->getSpeed();
        }

        return segments.back()->getSpeed();
    }

protected:
    void setNull(QPointF& position)
    {
        position.setX(-1e30);
        position.setY(-1e30);
    }

public:
    State getStateType() const
    {
        return state->type();
    }

    void setState(RouteState* state)
    {
        this->state = state;
    }

    void draw(QPainter& painter)
    {
        // Отрисовка пути
        state->draw(painter, segments, color);

        // Отрисовка Объекта
        if (!segments.isEmpty() && currentSegmentIndex <= segments.size())
        {
            auto index = currentSegmentIndex;

            // Если достигли конечной точки
            if (currentSegmentIndex == segments.size())
            {
                --index;
            }

            head->draw(painter, *segments.at(index), color);
        }
    }

    // Сохраняет текущее состояние в Memento
    Memento* saveState(const Qt::Key& key) const
    {
        return new Memento(state, key);
    }

    // Восстанавливает состояние из Memento
    void restoreState(Memento* memento)
    {
        state = memento->getState();
    }

    void initHead(const Visual::Models::Object parameters)
    {
        head->initialize(parameters);
    }

    bool setModel(const Visual::Objects object, const double size = 100)
    {
        return head->setModel(object, size);
    }

protected:
    // Memento
    RouteState* state; // Текущее состояние

    QVector<SegmentWidget*> segments; // Логический путь
    ObjectWidget* head;               // Головной объект
    QColor color;                     // TODO: Перенести в сегмент

    size_t currentSegmentIndex; // Индекс текущего сегмента
    double currentTime;         // Прошло времени с начала маршрута

    double currentLength;
};
} // namespace Visual
