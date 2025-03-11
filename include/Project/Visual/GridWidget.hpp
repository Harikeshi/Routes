#pragma once

#include <QPainter>
#include <QWidget>

#include "Models/Limits.hpp"

namespace Visual {
class GridWidget : public QWidget
{
    using Limits = Visual::Models::Limits;

    Q_OBJECT

    QColor color;
    int steps; // Количество шагов

public:
    GridWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        color = QColor(220, 220, 220);
        steps = 20;
    }

    void setColor(const QColor& col)
    {
        color = col;
    }

    void setSteps(const int s)
    {
        steps = s;
    }

    void paintEvent(QPaintEvent* event) override
    {
        // Создаем QImage, если он еще не создан или размер изменился
        if (image.isNull() || image.size() != size())
        {
            image = QImage(size(), QImage::Format_ARGB32);
            image.fill(Qt::white); // Заполняем изображение белым цветом

            QPainter imagePainter(&image);
            // Применяем трансформацию к QPainter
            imagePainter.setTransform(transform);
            // Отрисовываем содержимое виджета на QImage
            draw(imagePainter, limits);
        }

        // Отрисовываем QImage на виджете
        QPainter widgetPainter(this);
        widgetPainter.drawImage(0, 0, image);
    }

    // Limits limits;
    // QTransform transform;
    // QImage image;
    void initialize(const QTransform& transform, const Limits& limits)
    {
        // Устанавливаем новую трансформацию и обновляем буфер
        this->limits = limits;
        this->transform = transform;
        image = QImage();

        update();
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

            // x - минимум
            // painter.drawText(QPointF{limits.minX, current}, QString("%1").arg(i));
            painter.drawLine(p1, p2);

            current += step;
        }
    }

    void drawVertical()
    {
    }

    void drawHorizontal()
    {
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

private:
    Limits limits;
    QTransform transform;
    QImage image;
};
} // namespace Visual
