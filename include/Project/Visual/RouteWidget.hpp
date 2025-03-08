#pragma once

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
        : QWidget(parent), state(state)
    {
    }

    // Вычислить текущую точку,
    bool update(const double speedMultiplier)
    {
        qDebug() << "Текущий индекс: " << currentSegmentIndex;
        if (currentSegmentIndex >= segments.size())
        {
            steps = 0;
            qDebug() << "Выход из update()";
            return false;
        }

        // Вычисление новой позиции точки
        const SegmentWidget* segment = segments[currentSegmentIndex];
        QVector2D direction(segment->getEnd() - segment->getStart());

        auto lastCurrentPoint = segments[currentSegmentIndex]->getCurrentPoint();

        direction.normalize();

        QVector2D step = direction * segment->getSpeed() * speedMultiplier; // за минуту с учетом множителя скорости
        QPointF nextPosition = position + step.toPointF();

        if ((position == segments.back()->getEnd()) || (QVector2D(nextPosition - segment->getStart()).length() >= QVector2D(segment->getEnd() - segment->getStart()).length()))
        {
            position = segment->getEnd();

            //currentLength += segments[currentSegmentIndex]->length();          //*
            segments[currentSegmentIndex]->setCurrentPoint(segment->getEnd()); //*

            qDebug() << "Текущая точка сегмента: " << segments[currentSegmentIndex]->getCurrentPoint();

            currentLength += std::hypot(lastCurrentPoint.x() - segment->getEnd().x(), lastCurrentPoint.y() - segment->getEnd().y());
            currentSegmentIndex++;

            qDebug() << "Измененный индекс: " << currentSegmentIndex;
        }
        else
        {
            position = nextPosition;
            // Расстояние от конца текущего сегмента, до точки position
            segments[currentSegmentIndex]->setCurrentPoint(nextPosition); //*

            currentLength += std::hypot(lastCurrentPoint.x() - nextPosition.x(), lastCurrentPoint.y() - nextPosition.y());
        }

        //qDebug() << "Шаг [" << steps++ << "]: начало: " << lastCurrentPoint << ", конец: " << segments[currentSegmentIndex]->getCurrentPoint() << ", скорость: " << segment->getSpeed() * speedMultiplier;

        // TODO: Убрать вычислить из текущего положения segment->current
        // Кажется будет медленнее
        // Добавляем время
        // time = currentLength + std::hypot(position.x() - segments[currentSegmentIndex].getCurrentPoint().x(), position.y() - segments[currentSegmentIndex].getCurrentPoint().y())/ speed
        currentTime += std::hypot(position.x() - path.back().x(), position.y() - path.back().y()) / segment->getSpeed(); // TODO: Потенциальное деление на 0
        // qDebug() << "Предыдущая текущая точка: " << lastCurrentPoint << ", Новая текущая точка: " << segments[currentSegmentIndex]->getCurrentPoint();

        qDebug() << "Текущая Длина:" << currentLength;

        path.append(position);

        return true;
    }

    // TODO: position заменить
    QPointF getCurrentPosition() const
    {
        // Проверку, если путь закончен то последнюю точку, в остальных случаях сегмент[current].getCurrent()

        if (currentSegmentIndex >= segments.size())
        {
            return segments.back()->getCurrentPoint();
        }

        return segments[currentSegmentIndex]->getCurrentPoint();
    }

    // TODO: Пересмотреть
    void initialize(const QVector<Visual::Models::Segment>& segments, const QColor& color, const double radius)
    {
        for (const auto& segment : segments)
        {
            this->segments.push_back(new SegmentWidget(this, segment));
        }

        this->setColor(color);
        this->setRadius(radius);

        clear();
    }

    void reset()
    {
        clear();

        segments.clear(); // TODO: это reset
        setNull(position);
    }

    // TODO: Только очистка
    void clear()
    {
        // Сброс к начальным значениям пути
        currentTime = 0;
        currentLength = 0;
        currentSegmentIndex = 0; // Выбран первый отрезок
        path.clear();            // Обнуляем путь

        if (!segments.isEmpty())
        {
            for (const auto& segment : segments)
            {
                segment->clear();
            }

            position = segments[0]->getStart(); // Позиция в начальной точке
            path.append(position);
        }
    }

    /* get/set */
    size_t getCurrentIndex() const
    {
        return currentSegmentIndex;
    }

    QPointF getPosition() const
    {
        return position;
    }

    void setPosition(const QPointF& point)
    {
        position = point;
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
        return radius;
    }

    void setRadius(const double& r)
    {
        radius = r;
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
        // return currentLength / currentSpeed;
        return currentTime;
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
    void setNullPosition()
    {
        setNull(position);
    }

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
        state->draw(painter, segments, color);
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

protected:
    // Memento
    RouteState* state; // Текущее состояние

    QVector<SegmentWidget*> segments; // Логический путь

    QColor color; // TODO: Перенести в сегмент

    size_t currentSegmentIndex; // Индекс текущего сегмента
    double currentLength;       // TODO: не используется. Длина пройденного пути

    // TODO: при инициализации маршрута брать данные о скорости и радиусе ГАС из json
    double currentTime;    // Прошло времени с начала маршрута
    double radius = 10;    // Радиус ГАС
    QPointF position;      // Текущая позиция
    QVector<QPointF> path; // Путь пройденный от начала до текущей позиции
    int steps{0};          // TODO: для тестов
};
} // namespace Visual
