#pragma once

#include "RouteWidget.hpp"

// TODO: пересмотреть, вылетает

class TargetWidget : public RouteWidget
{
public:
    TargetWidget(QWidget* parent = nullptr)
        : RouteWidget{parent}
    {
        // TODO: Чтобы была за пределами
        setNull(position);

        setColor(Qt::black);
    }

    void draw(QPainter& painter, const bool showLines)
    {
        QPen pen = QPen(color, 2);
        pen.setCosmetic(true);

        painter.setPen(pen);

        qDebug() << "Target::draw()::path.size(): " << path.size();
        if (showLines)
            if (!path.isEmpty())
            {
                for (int i = 0; i < path.size() - 1; ++i)
                {
                    painter.drawLine(path[i], path[i + 1]);
                }
            }

        // TODO: Сделать невидимую? не отрисовывать т нет проблем)
        painter.setBrush(Qt::red);
        painter.drawEllipse(position, .2, .2);
    }

    void clear()
    {
        reset();

        setNull(position); // TODO: переработать

        RouteWidget::clear();
    }
};
