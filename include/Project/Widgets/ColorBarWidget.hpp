#pragma once

#include <QPainter>
#include <QWidget>

#include "./ColorFunctions.hpp"

namespace Widgets {
class ColorBarWidget : public QWidget
{
    Q_OBJECT
public:
    ColorBarWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
    }

    void setRange(double minV, double maxV)
    {
        m_min = minV;
        m_max = maxV;
        update();
    }

protected:
    void paintEvent(QPaintEvent*) override
    {
        QPainter p(this);
        int w = width() / 2;
        int h = height();

        // Рисуем вертикальный градиент
        QLinearGradient grad(0, 0, 0, h);
        for (int i = 0; i <= 100; i++)
        {
            double t = i / 100.0;
            // grad.setColorAt(1.0 - t, greenToRed(t));
            grad.setColorAt(1.0 - t, Widgets::blueToViolet(t));
        }
        p.fillRect(0, 0, w, h, grad);

        // Рисуем рамку
        p.setPen(Qt::black);
        p.drawRect(0, 0, w, h);

        // Подписи min/max
        p.setPen(Qt::black);
        p.drawText(w + 5, 15, QString::number(m_max, 'g', 4));
        p.drawText(w + 5, h - 5, QString::number(m_min, 'g', 4));
    }

private:
    double m_min = 0, m_max = 1;
};
} // namespace Widgets
