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
        : QObject(parent)
    {
        color = QColor(220, 220, 220);
        steps = 10;
        margin = 2;
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

    void draw(QPainter& painter, const Limits& limits)
    {
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

            auto font = painter.font();

            painter.setFont(font);

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

public:
    // TODO: В отрисовку сетки добавить поворот чисел слева
    void draw(const Limits& limits, const QRect& rect, const QString& H, const QString& V)
    {
        this->image = QImage(rect.width(), rect.height(), QImage::Format_ARGB32);

        image.fill(Qt::transparent);

        QPainter painter(&image);

        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);

        // Установка шрифта для подписей
        QFont font("Arial", 8);
        painter.setFont(font);
        QFontMetrics fm(font);

        // Отступы
        const int topMargin = 15;
        const int rightMargin = 10;
        // TODO: Привязать отступы слева и справа к высоте цифр заданного шрифта
        const int bottomMargin = fm.height();
        const int leftMargin = fm.height();
        // const int leftMargin = fm.horizontalAdvance(QString::number(limits.getMaxDifference()) + "00");

        // 1 pixel по ширине
        auto wPixel = limits.getXDifference() / rect.width();
        // 2 pixel по высоте
        auto hPixel = limits.getYDifference() / rect.height();

        // Начало по ширине из minX
        // Вертикальные линии

        // TODO: можно не отрисовывать
        // 1. Начало отрисовки leftMargin
        // первая линия {leftMargin, height - bottomMargin, leftMargin, topMargin}
        // Последняя линия {width - rightMargin, height - bootomMargin, width-rightMargin, topMargin}
        // 2. Следующие линии от minX до maxX
        // Серединные линии
        // Вертикальные

        auto mainLimints = limits.limitsWithMargins(margin);

        auto beginX = std::fabs(mainLimints.minX - limits.minX) / wPixel; // начало в пикселях
        auto availableWidth = mainLimints.getXDifference() / wPixel;      //

        auto xLogicalStep = mainLimints.getXDifference() / steps; // шаг по X в координатах
        auto xScreenStep = availableWidth / steps;                // шаг по X в пикселях

        auto beginY = std::fabs(mainLimints.maxY - limits.maxY) / hPixel;
        auto availableHeight = mainLimints.getYDifference() / hPixel; // в пикселях

        auto yLogicalStep = mainLimints.getYDifference() / steps;
        auto yScreenStep = availableHeight / steps;

        auto maxX = rect.width() - rightMargin;
        auto maxY = rect.height() - bottomMargin;
        double minX = leftMargin;
        double minY = topMargin;

        // Отрисовка сетки
        painter.setPen(QPen(Qt::lightGray, 1, Qt::DotLine));

        // Вертикальные линии
        for (int i = 0; i <= steps; ++i)
        {
            int x = beginX + i * xScreenStep;
            painter.drawLine(x, minY, x, maxY);

            QString label = QString::number(mainLimints.minX + i * xLogicalStep); // X
            int labelWidth = fm.horizontalAdvance(label);

            // if (i != 0 && i != steps)
            painter.drawText(x - labelWidth / 2, rect.height() - bottomMargin / 2, label);
        }

        // Горизонтальные линии
        for (int i = 0; i <= steps; ++i)
        {
            int y = beginY + i * yScreenStep;
            painter.drawLine(minX, y, maxX, y);

            // Подписи по оси Y (левая ось)
            QString label = QString::number(mainLimints.maxY - i * yLogicalStep); // максимум -
            int labelWidth = fm.horizontalAdvance(label);

            drawCenteredRotatedText(painter, leftMargin / 2, y, -90, label); // поворот 90 влево
        }

        // Рисование осей
        painter.setPen(QPen(Qt::black, 2));

        // Подписи осей
        QFont axisFont("Arial", 12, QFont::Bold);
        painter.setFont(axisFont);

        // Название оси X
        painter.drawText(rect.width() - rightMargin - xScreenStep / 2, rect.height() - bottomMargin / 3, H);

        // Название оси Y
        painter.drawText(leftMargin / 2, topMargin + yScreenStep / 2, V);

        painter.end();
    }

    QImage getImage() const
    {
        return image;
    }

private:
    void
    drawCenteredRotatedText(QPainter& painter, qreal centerX, qreal centerY, qreal angle, const QString& text)
    {
        painter.save();

        QFontMetrics fm(painter.font());
        QRect textRect = fm.boundingRect(text);

        // Перенос начала координат в центр поворота
        painter.translate(centerX, centerY);

        painter.rotate(angle);

        painter.drawText(-textRect.width() / 2, textRect.height() / 2 - fm.descent(), text);

        painter.restore();
    }

private:
    QColor color;
    int steps; // Количество шагов

    double margin = 2; // Отступ в процентах
    QImage image;      // Кэш изображения
};
} // namespace Scene::Objects