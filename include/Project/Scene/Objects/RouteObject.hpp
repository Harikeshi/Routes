#pragma once

#include <QMouseEvent>
#include <QStack>
#include <QToolTip>
#include <QWidget>

#include "./ModelObject.hpp"
#include "./SegmentObject.hpp"
#include "Project/Models/Object.hpp"
#include "Project/Models/Target.hpp"
#include "RouteState.hpp"

#include <complex>

namespace Scene::Objects {
class State;
class WithOutDrawState;
class FullDrawState;
class CurrentDrawState;

class RouteObject : public QWidget
{
    // Q_OBJECT
public:
    RouteObject(QWidget* parent = nullptr, State* state = new CurrentDrawState())
        : QWidget(parent), state(state), head(new ModelObject())
    {
        // setMouseTracking(true);
    }

    // Вычислить текущую точку,
    // time = time * muliplier
    bool move(double time)
    {
        if (currentSegmentIndex >= segments.size())
        {
            return false;
        }

        double accumulatedTime = 0.0;

        for (size_t i = 0; i < segments.size(); ++i)
        {
            double dx = segments[i]->getEnd().x() - segments[i]->getStart().x();
            double dy = segments[i]->getEnd().y() - segments[i]->getStart().y();

            double segmentTime = segments[i]->length() / segments[i]->getSpeed();
            currentSegmentIndex = i;
            // Проверяем, находится ли нужное время в этом сегменте
            if (time <= accumulatedTime + segmentTime)
            {
                // Вычисляем прогресс прохождения сегмента (0..1)
                double progress = (time - accumulatedTime) / segmentTime;

                // Меняем точку
                segments[i]->setCurrentPoint(QPointF{
                    segments[i]->getStart().x() + progress * dx,
                    segments[i]->getStart().y() + progress * dy});
                break;
            }

            // считаем, что отрезок пройден
            segments[i]->setCurrentPoint(segments[i]->getEnd());
            // Увеличиваем накопленное время
            accumulatedTime += segmentTime;
        }

        //TODO: До currentSegmentIndex

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
    void initialize(const QVector<Models::Segment>& segments, const QColor& color, const Models::Object& parameters, double pointSize)
    {
        for (const auto& segment : segments)
        {
            this->segments.push_back(new SegmentObject(this, segment));
            length += segment.length();
        }

        this->setColor(color);
        this->setRadius(parameters.detection_range);

        this->head->initialize(parameters);

        clear();
    }

    double getLength() const
    {
        return length;
    }

    void reset()
    {
        length = 0;
        currentSegmentIndex = 0; // Выбран первый отрезок
        segments.clear();        // TODO: это reset

        head = new ModelObject(); // Головной объект
    }

    /*!
         * Метод сброса текущей точки к стартовой.
         */
    void clear()
    {
        // Сброс к начальным значениям пути
        length = 0;
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
        SegmentObject* segment = new SegmentObject(this, {0, start, end, speed});

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
        head->show();

        for (const auto& segment : segments)
        {
            segment->show();
        }
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
    static void setNull(QPointF& position)
    {
        position.setX(-1e30);
        position.setY(-1e30);
    }

public:
    StateType getStateType() const
    {
        return state->type();
    }

    void setState(State* state)
    {
        this->state = state;
    }

    void draw(QPainter& painter)
    {
        // Отрисовка пути
        state->draw(painter, segments, color);

        // Отрисовка Объекта
        if (state->type() != StateType::Clean)
        {
            drawHead(painter);
        }
    }

    void drawHead(QPainter& painter)
    {
        // Отрисовка Объекта
        if (!segments.isEmpty() && currentSegmentIndex <= segments.size())
        {
            auto index = currentSegmentIndex;

            // Если достигли конечной точки
            if (currentSegmentIndex >= segments.size())
            {
                --index;
            }

            head->draw(painter, *segments.at(index), color);
        }
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

        // for (auto& point : pointsWidget)
        //     point->swapCoordinates();

        //head->swapCoordinates();
    }

    double getFullTime() const
    {
        double time{0};
        for (const auto& segment : segments)
            time += segment->getFullTime();

        return time;
    }

protected:
    // Memento
    State* state; // Текущее состояние

    QVector<SegmentObject*> segments; // Логический путь
    ModelObject* head;                // Головной объект
    QColor color;                     // TODO: Перенести в сегмент

    // TODO: Рассчитывать все капсулы при загрузке, и крайнюю капсулу рассчитывать перед отрисовкой.
    // TODO: при изменении капсул высылать sendChangedCapsules
    size_t currentSegmentIndex; // Индекс текущего сегмента

    double length{0};
};
} // namespace Scene::Objects
