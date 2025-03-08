#pragma once

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

class Memento
{
private:
    RouteState* state;

public:
    Memento(RouteState* state_)
        : state(state_)
    {
    }

    RouteState* getState() const
    {
        return state;
    }
};

class Context // TODO: RouteWidget
{
private:
    RouteState* state_; // Текущее состояние

    RouteState* contextData_; // Данные контекста

public:
    Context(RouteState* state)
        : state_(state)
    {
    }

    void setState(RouteState* state)
    {
        if (contextData_ != nullptr)
        {
            // Откат к предыдущей
            state_ = contextData_;
            contextData_ = nullptr;
        }
        else
        {
            contextData_ = state_;
            state_ = state;
        }
    }

    void setData(RouteState* data)
    {
        contextData_ = data;
    }

    RouteState* getData() const
    {
        return contextData_;
    }

    void draw()
    {
        // state_->draw(*this);
    }

    // Сохраняет текущее состояние в Memento
    Memento* saveState() const
    {
        return new Memento(contextData_);
    }

    // Восстанавливает состояние из Memento
    void restoreState(Memento* memento)
    {
        contextData_ = memento->getState();
        qDebug() << "Состояние восстановлено.";
    }
};

class RouteWidget : public QWidget
{
    // Q_OBJECT
public:
    RouteWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
    }

    // Вычислить текущую точку,
    bool update(const double speedMultiplier)
    {
        if (currentSegmentIndex >= segments.size())
        {
            steps = 0;
            return false;
        }

        // Вычисление новой позиции точки
        const SegmentWidget* segment = segments[currentSegmentIndex];
        QVector2D direction(segment->getEnd() - segment->getStart());

        direction.normalize();

        QVector2D step = direction * segment->getSpeed() * speedMultiplier; // за минуту с учетом множителя скорости
        qDebug() << "Скорость на отрезке: " << segment->getSpeed();
        QPointF nextPosition = position + step.toPointF();

        if ((position == segments.back()->getEnd()) || (QVector2D(nextPosition - segment->getStart()).length() >= QVector2D(segment->getEnd() - segment->getStart()).length()))
        {
            position = segment->getEnd();
            currentLength += segments[currentSegmentIndex]->length();            //*
            segments[currentSegmentIndex++]->setCurrentPoint(segment->getEnd()); //*
        }
        else
        {
            position = nextPosition;
            // Расстояние от конца текущего сегмента, до точки position
            currentLength += std::hypot(position.x() - segments[currentSegmentIndex]->getCurrentPoint().x(), position.y() - segments[currentSegmentIndex]->getCurrentPoint().y()); //*
            segments[currentSegmentIndex]->setCurrentPoint(nextPosition);                                                                                                          //*
        }

        qDebug() << "Шаг [" << steps++ << "]: начало: " << position << ", конец: " << nextPosition << ", скорость: " << segment->getSpeed() * speedMultiplier;

        // TODO: Убрать вычислить из текущего положения segment->current
        // Кажется будет медленнее
        // Добавляем время
        // time = currentLength + std::hypot(position.x() - segments[currentSegmentIndex].getCurrentPoint().x(), position.y() - segments[currentSegmentIndex].getCurrentPoint().y())/ speed
        currentTime += std::hypot(position.x() - path.back().x(), position.y() - path.back().y()) / segment->getSpeed(); // TODO: Потенциальное деление на 0

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

    void draw(QPainter& painter, const bool showLines, const bool full)
    {
        QPen pen = QPen(color, 2);
        pen.setCosmetic(true);

        painter.setPen(pen);

        qDebug() << "Route::draw():: path.size(): " << path.size();

        // Отрисовка пройденного пути
        if (!path.isEmpty())
        {
            // TODO: Отличается отроисовкой полного пути
            if (full)
            {
                // show logical path
                for (int j = 0; j < segments.size(); ++j)
                {
                    painter.drawLine(segments[j]->getStart(), segments[j]->getEnd());
                }

                return;
            }

            if (showLines)
                for (int j = 0; j < path.size() - 1; ++j)
                {
                    auto start = path[j];
                    auto end = path[j + 1];

                    painter.drawLine(start, end);
                }
        }
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

        reset();
    }

    // TODO: По сути это clear, а ресет это clear + segments.clear()
    void reset1()
    {
        // Сброс к начальным значениям пути
        // Обнуляем все кроме segments
        currentTime = 0;

        if (!segments.isEmpty())
        {
            currentSegmentIndex = 0;            // Выбран первый отрезок
            position = segments[0]->getStart(); // Позиция в начальной точке

            path.clear(); // Обнуляем путь
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

    // void addSegment(const SegmentWidget& segment)
    // {
    //     segments.push_back(segment);
    // }

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

    // TODO: Только очистка
    void clear1()
    {
        segments.clear(); // TODO: это reset
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
    void setState(RouteState* state)
    {
        if (contextData_ != nullptr)
        {
            // Откат к предыдущей
            state_ = contextData_;
            contextData_ = nullptr;
        }
        else
        {
            contextData_ = state_;
            state_ = state;
        }
    }

    void setData(RouteState* data)
    {
        contextData_ = data;
    }

    RouteState* getData() const
    {
        return contextData_;
    }

    void draw(QPainter& painter)
    {
        state_->draw(painter, this);
    }

    // Сохраняет текущее состояние в Memento
    Memento* saveState() const
    {
        return new Memento(contextData_);
    }

    // Восстанавливает состояние из Memento
    void restoreState(Memento* memento)
    {
        contextData_ = memento->getState();
        qDebug() << "Состояние восстановлено.";
    }

protected:
    // Memento
    RouteState* state_; // Текущее состояние

    RouteState* contextData_; // Данные контекста

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

class RouteState
{
public:
    virtual void draw(QPainter& painter, RouteWidget* widget) = 0;
    virtual ~RouteState() = default;
};

// Без отрисовки nullptr
class WithOutDrawState : public RouteState
{
public:
    void draw(QPainter& painter, RouteWidget* widget) override
    {
    }
};

class CurrentDrawState : public RouteState
{
public:
    void draw(QPainter& painter, RouteWidget* widget) override
    {
        // TODO: Проход по всем отрисовка start->current
        // Отрисовка только головы
        // Отрисовка ShipWidgetSegment

        for (size_t i = 0; i < widget->getCurrentIndex(); ++i)
        {
            widget->getSegments().at(i)->draw(painter, widget->getColor());
        }

        widget->getSegments().at(widget->getCurrentIndex())->draw(painter, widget->getColor());
    }
};

class FullDrawState : public RouteState
{
public:
    void draw(QPainter& painter, RouteWidget* widget) override
    {
        // Отрисовка только головы
        // TODO: Проход по всем отрисовка start->end

        for (size_t i = 0; i < widget->getSegments().size(); ++i)
        {
            // TODO: Отрисовать все со стрелками
            widget->getSegments().at(i)->draw(painter, widget->getColor());
        }
    }
};
} // namespace Visual
