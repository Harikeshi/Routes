#pragma once

#include <QPainter>
#include <QWidget>

#include "Models/Limits.hpp"
#include "Models/Perimeter.hpp"

class GridWidget : public QWidget
{
    Q_OBJECT

    QColor color;
    int steps; // Количество шагов
    // double minX, minY, maxX, maxY;

public:
    GridWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        color = QColor(220, 220, 220);
        steps = 20;
    }

    // void initialize(const std::pair<std::pair<double, double>, std::pair<double, double>>& limits)
    // {
    //     minX = limits.first.first;
    //     minY = limits.first.second;
    //     maxX = limits.second.first;
    //     maxY = limits.second.second;
    // }

    void setColor(const QColor& col)
    {
        color = col;
    }

    void setSteps(const int s)
    {
        steps = s;
    }

    void draw(QPainter& painter, const Limits& limits)
    {
        //
        QPen pen(color, 2);
        pen.setCosmetic(true);

        pen.setStyle(Qt::DotLine);

        QFont font = painter.font();

        font.setPointSize(8);

        painter.setPen(pen);
        painter.setFont(font);
        //     minX = limits.first.first;
        //     minY = limits.first.second;
        //     maxX = limits.second.first;
        //     maxY = limits.second.second;
        // Вертикальные линии (ось X)
        auto step = (limits.maxX - limits.minX) / steps;

        double current = limits.minX;

        for (int i = 0; i <= steps; ++i)
        {
            QPointF p1(current, limits.minY);
            QPointF p2(current, limits.maxY);

            painter.drawLine(p1, p2);

            current += step;
        }

        // Горизонтальные линии (ось Y)
        step = (limits.maxY - limits.minY) / steps;

        current = limits.minY;

        for (int i = 0; i <= steps; ++i)
        {
            QPointF p1(limits.minX, current);
            QPointF p2(limits.maxX, current);

            painter.drawLine(p1, p2);

            current += step;
        }
    }

    QImage drawToImage(int width, int height, const Limits& limits)
    {
        QImage image(width, height, QImage::Format_ARGB32);
        image.fill(Qt::white);

        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setViewport(0, 0, width, height);

        draw(painter, limits);

        return image;
    }
};
