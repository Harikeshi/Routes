#pragma once

#include <QStack>
#include <QWidget>

#include "./ModelObject.hpp"
#include "./SegmentObject.hpp"
#include "Project/Models/Object.hpp"
#include "Project/Models/Target.hpp"
#include "RouteState.hpp"

#include <complex>

namespace Scene::Objects {
class RouteState;
class WithOutDrawState;
class FullDrawState;
class CurrentDrawState;

class RouteObject : public QWidget
{
    // Q_OBJECT
public:
    RouteObject(QWidget* parent = nullptr, RouteState* state = new CurrentDrawState())
        : QWidget(parent), state(state), head(new ModelObject())
    {
        // setMouseTracking(true);
    }

    void mousePressEvent(QMouseEvent* event, const QPointF& point)
    {
        if (pointsWidget.isEmpty())
            return;

        // TODO: Не верно распознает
        int pointIndex = findPointAt(point, pointsWidget[0]->getRadius());

        if (pointIndex != -1)
        {
            QString tooltipText = QString("Point %1 :[ X: %2, Y: %3 ]")
                                      .arg(pointIndex + 1)
                                      .arg(pointsWidget[pointIndex]->x())
                                      .arg(pointsWidget[pointIndex]->y());
            QToolTip::showText(event->globalPos(), tooltipText, this);
        }

        update();
    }
    /*!
     *
     * @param pos
     * @param radius
     * @return
     */
    int findPointAt(const QPointF& pos, double radius)
    {
        for (int i = 0; i < pointsWidgetSize(); ++i)
        {
            int dx = pos.x() - pointsWidget.at(i)->x();
            int dy = pos.y() - pointsWidget.at(i)->y();

            if (dx * dx + dy * dy <= (radius + radius * 0.05) * (radius + radius * 0.05))
            {
                return i;
            }
        }

        return -1;
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
                segments[i]->setCurrentPoint(QPointF{segments[i]->getStart().x() + progress * dx,
                                                     segments[i]->getStart().y() + progress * dy});
                break;
            }

            // считаем, что отрезок пройден
            segments[i]->setCurrentPoint(segments[i]->getEnd());
            // Увеличиваем накопленное время
            accumulatedTime += segmentTime;
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
    void initialize(const QVector<Models::Segment>& segments, const QColor& color, const Models::Object& parameters, double pointSize)
    {
        //! Установить Первую точку
        if (!segments.isEmpty())
        {
            pointsWidget.push_back(new PointWidget(segments.at(0).getStart(), pointSize, color));
        }

        for (const auto& segment : segments)
        {
            pointsWidget.push_back(new PointWidget(segment.getEnd(), pointSize, color)); //! Точки

            this->segments.push_back(new SegmentObject(this, segment));
            length += segment.length();
        }

        this->setColor(color);
        this->setRadius(parameters.detectionRange);

        this->head->initialize(parameters);

        clear();
    }

    double getLength() const
    {
        return length;
    }

    void reset()
    {
        clear();

        segments.clear(); // TODO: это reset
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

    size_t pointsWidgetSize() const
    {
        auto type = getStateType();

        if (type == StateType::Current)
        {
            return currentSegmentIndex + 1;
        }
        else if (type == StateType::Full)
        {
            return segments.size() + 1;
        }

        return 0;
    }

    /*!
    *   QFont font("Arial", 50, QFont::Bold); // Размер 50
    *   painter.setFont(font);
    *
    *   // Рисуем цифру "5" в позиции (50, 70)
    *   painter.drawText(50, 70, "5");
    * @param painter
     */

    void printNumber(QPainter& painter, const QPointF& real, size_t number)
    {
        painter.setRenderHint(QPainter::Antialiasing);

        QFont font("Arial", 1, QFont::Bold); // Размер 50
        painter.setFont(font);

        painter.drawText(real.x(), real.y(), QString("%1").arg(number));
    }

    void drawPoints(QPainter& painter)
    {
        if (pointsWidget.isEmpty())
            return;

        if (currentSegmentIndex > 0)
        {
            pointsWidget.at(0)->draw(painter, 1);
        }

        for (size_t i = 1; i != pointsWidgetSize(); ++i)
        {
            pointsWidget.at(i)->draw(painter, i + 1);
        }
    }

    QVector<PointWidget*> getPointsWidget()
    {
        return pointsWidget;
    }

    void draw(QPainter& painter)
    {
        // Отрисовка пути
        state->draw(painter, segments, color);

        if (showPoints)
            drawPoints(painter);

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

        for (auto& point : pointsWidget)
            point->swapCoordinates();

        //head->swapCoordinates();
    }

    double getFullTime() const
    {
        double time{0};
        for (const auto& segment : segments)
            time += segment->getFullTime();

        return time;
    }

    void changeShowPoints()
    {
        showPoints = !showPoints;
    }

protected:
    // Memento
    RouteState* state; // Текущее состояние

    QVector<SegmentObject*> segments; // Логический путь
    ModelObject* head;                // Головной объект
    QColor color;                     // TODO: Перенести в сегмент

    size_t currentSegmentIndex; // Индекс текущего сегмента

    QVector<PointWidget*> pointsWidget;

    double length;

    bool showPoints = true;
};
} // namespace Scene::Objects
