#pragma once

#include <QDebug>
#include <QPainter>
#include <QStack>
#include <QVector2D>
#include <QWidget>

#include "./ModelObject.hpp"
#include "./Models/Object.hpp"
#include "./Models/Target.hpp"
#include "./RouteState.hpp"
#include "./SegmentObject.hpp"

namespace Scene {
namespace Objects {
class RouteState;
class WithOutDrawState;
class FullDrawState;
class CurrentDrawState;

class RouteObject : public QObject
{
    // Q_OBJECT
public:
    RouteObject(QObject* parent = nullptr, RouteState* state = new CurrentDrawState())
        : QObject(parent), state(state), head(new ModelObject())
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
        const SegmentObject* segment = segments[currentSegmentIndex];
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
        if (segments.isEmpty())
        {
            return QPointF{}; // QPointF{0, 0};
        }

        // Проверку, если путь закончен то последнюю точку, в остальных случаях сегмент[current].getCurrent()
        if (currentSegmentIndex >= segments.size())
        {
            return segments.back()->getCurrentPoint();
        }

        return segments[currentSegmentIndex]->getCurrentPoint();
    }

    // TODO: Пересмотреть
    void initialize(const QVector<Models::Segment>& segments, const QColor& color, const Models::Object& parameters)
    {
        for (const auto& segment : segments)
        {
            this->segments.push_back(new SegmentObject(this, segment));
        }

        this->setColor(color);
        this->setRadius(parameters.detectionRange);

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
        SegmentObject* segment = new SegmentObject(this, {start, end, speed});

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
    const QVector<SegmentObject*> getSegments() const
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
    StateType getStateType() const
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

    void initHead(const Models::Object parameters)
    {
        head->initialize(parameters);
    }

    void initHead(const Models::Target& target)
    {
        head->initialize(target);
    }

    // Objects перечисление
    bool setModel(const Objects object, const double radius, const double size = 100)
    {
        if (head != nullptr)
        {
            head->setModel(object, size);
            head->setRadius(radius);

            return true;
        }

        return false;
    }
    void swapCoordinates()
    {
        for (size_t i = 0; i != segments.size(); ++i)
        {
            segments[i]->swapCoordinates();
        }

        //head->swapCoordinates();
    }

protected:
    // Memento
    RouteState* state; // Текущее состояние

    QVector<SegmentObject*> segments; // Логический путь
    ModelObject* head;                // Головной объект
    QColor color;                     // TODO: Перенести в сегмент

    size_t currentSegmentIndex; // Индекс текущего сегмента
    double currentTime;         // Прошло времени с начала маршрута

    double currentLength;
};
} // namespace Objects
} // namespace Scene