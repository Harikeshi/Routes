#pragma once

#include <QImage>
#include <QPainter>
#include <QWidget>

#include "../Entities/Limits.hpp"

namespace Scene::Objects {
class GridObject final : public QObject
{
    using Limits = Entities::Limits;

    Q_OBJECT

public:
    GridObject(QObject* parent = nullptr)
        : QObject(parent),
          penColor(200, 200, 200), axisColor(Qt::black), steps(10.)
    {
        color = QColor(220, 220, 220);
        image = QImage(800, 800, QImage::Format_ARGB32);
    }

    void setColor(const QColor& colour)
    {
        color = colour;
    }

    void setSteps(const int number)
    {
        steps = number;
    }

public:
    QImage getImage() const
    {
        return image;
    }

private:
    double round(double x)
    {
        if (x == 0.)
            return 0.;
        double power = std::pow(10., std::floor(std::log10(x)));
        double step = power / 2.0;

        if (step < 5)
            step = 5; // минимальный шаг

        return std::ceil(x / step) * step;
    }

public:
    void draw(QPainter& painter, const QTransform& transform, const QTransform& inverseTransform, const QRect& widgetRect)
    {
        painter.save();
        QRectF worldRect = inverseTransform.mapRect(QRectF(widgetRect));

        double left = worldRect.left();
        double right = worldRect.right();
        double bottom = worldRect.bottom();
        double top = worldRect.top();

        double spacing = round(qMin(qAbs(right - left), qAbs(top - bottom)) / steps);

        double startX = std::floor(left / spacing) * spacing;
        double endX = std::ceil(right / spacing) * spacing;
        double startY = std::floor(top / spacing) * spacing;
        double endY = std::ceil(bottom / spacing) * spacing;

        painter.setTransform(QTransform());

        // Вертикальные
        painter.setPen(QPen(penColor, 1));
        for (double x = startX; x <= endX; x += spacing)
        {
            QPointF p1 = transform.map(QPointF(x, bottom));
            QPointF p2 = transform.map(QPointF(x, top));
            painter.drawLine(p1, p2);
        }

        // Горизонтальные
        for (double y = startY; y <= endY; y += spacing)
        {
            QPointF p1 = transform.map(QPointF(left, y));
            QPointF p2 = transform.map(QPointF(right, y));
            painter.drawLine(p1, p2);
        }

        // Оси
        painter.setPen(QPen(axisColor, 2));

        // X
        if (worldRect.top() <= 0 && worldRect.bottom() >= 0)
        {
            QPointF x1 = transform.map(QPointF(left, 0));
            QPointF x2 = transform.map(QPointF(right, 0));
            painter.drawLine(x1, x2);
        }

        // Y
        if (worldRect.left() <= 0 && worldRect.right() >= 0)
        {
            QPointF y1 = transform.map(QPointF(0, bottom));
            QPointF y2 = transform.map(QPointF(0, top));
            painter.drawLine(y1, y2);
        }

        // Координаты
        drawLabels(painter, transform, startX, endX, startY, endY, spacing, widgetRect);

        painter.restore();
    }

    void drawLabels(QPainter& painter, const QTransform& transform, double startX, double endX, double startY, double endY, double spacing, const QRect& widgetRect)
    {
        painter.setPen(QPen(Qt::black));
        QFont font = painter.font();
        font.setPointSize(8);
        painter.setFont(font);

        int widgetWidth = widgetRect.width();
        int widgetHeight = widgetRect.height();

        // X
        for (double x = startX; x <= endX; x += spacing)
        {
            if (std::abs(x) < 1e-6)
                continue;

            QPointF labelPos = transform.map(QPointF(x, 0));
            if (labelPos.x() >= 0 && labelPos.x() <= widgetWidth)
            {
                QString label = QString::number(x, 'f', std::abs(x) < 1 ? 1 : 0);
                QRect textRect = painter.fontMetrics().boundingRect(label);
                painter.drawText(labelPos.x() - textRect.width() / 2,
                                 widgetHeight - 2,
                                 label);
            }
        }

        // Y
        for (double y = startY; y <= endY; y += spacing)
        {
            if (std::abs(y) < 1e-6)
                continue;

            QPointF labelPos = transform.map(QPointF(0, y));
            if (labelPos.y() >= 0 && labelPos.y() <= widgetHeight)
            {
                QString label = QString::number(y, 'f', std::abs(y) < 1 ? 1 : 0);
                QRect textRect = painter.fontMetrics().boundingRect(label);
                painter.drawText(2, labelPos.y() + textRect.height() / 3, label);
            }
        }

        QPointF originPos = transform.map(QPointF(0, 0));
        if (originPos.x() >= 0 && originPos.x() <= widgetWidth &&
            originPos.y() >= 0 && originPos.y() <= widgetHeight)
        {
            painter.drawText(originPos.x() + 3, originPos.y() - 3, "0");
        }
    }

private:
    QColor penColor;
    QColor axisColor;

    QColor color;
    int steps; // Количество шагов

    QImage image;      // Кэш изображения
};
} // namespace Scene::Objects
