#pragma once

#include <QPainter>
#include <QVector>
#include <QWidget>
#include <algorithm>

#include "./ColorFunctions.hpp"

namespace Widgets {

//! TODO: Для построения матрицы использовать поле полное, для удобства наложения
//! То есть вычислить размер поля для конкретной отрисовки.
// -> loaded() -> limits(с учетом отступов и прочее) -> расчет матрицы с учетом этого поля и наложение.

class HeatmapWidget : public QWidget
{
    Q_OBJECT
public:
    HeatmapWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
    }

    void setData(const QVector<double>& data, int rows, int cols)
    {
        m_data = data;
        m_rows = rows;
        m_cols = cols;

        computeRange();
        generateImage();
        update();
    }

    double minValue() const
    {
        return m_min;
    }
    double maxValue() const
    {
        return m_max;
    }

protected:
    void paintEvent(QPaintEvent*) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::SmoothPixmapTransform, true);

        if (!m_img.isNull())
            p.drawImage(rect(), m_img);
    }

    void draw(QPainter& painter)
    {
        painter.save();

        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

        if (!m_img.isNull())
            painter.drawImage(rect(), m_img);

        painter.restore();
    }

private:
    QVector<double> m_data;
    int m_rows = 0, m_cols = 0;
    double m_min = 0, m_max = 1;
    QImage m_img;

    void computeRange()
    {
        if (m_data.isEmpty())
            return;

        m_min = *std::min_element(m_data.begin(), m_data.end());
        m_max = *std::max_element(m_data.begin(), m_data.end());

        if (m_min == m_max)
            m_max = m_min + 1e-9;
    }

    void generateImage()
    {
        if (m_rows <= 0 || m_cols <= 0)
            return;

        m_img = QImage(m_cols, m_rows, QImage::Format_ARGB32);

        for (int y = 0; y < m_rows; ++y)
        {
            QRgb* line = reinterpret_cast<QRgb*>(m_img.scanLine(y));
            for (int x = 0; x < m_cols; ++x)
            {
                double v = m_data[y * m_cols + x];
                double t = (v - m_min) / (m_max - m_min);
                // QColor c = greenToRed(t);
                QColor c = Widgets::blueToViolet(t);

                line[x] = c.rgb();
            }
        }
    }
};
} // namespace Widgets
