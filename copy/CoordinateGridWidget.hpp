#pragma once

#include <QCheckBox>
#include <QPainter>
#include <QVBoxLayout>
#include <QWidget>
#include <QtMath>

class CoordinateGridWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CoordinateGridWidget(QWidget* parent = nullptr)
        : QWidget(parent), isGeographic(true)
    {
        QCheckBox* toggleButton = new QCheckBox("Географические координаты", this);
        toggleButton->setChecked(true);
        connect(toggleButton, &QCheckBox::toggled, this, [this](bool checked) {
            isGeographic = checked;
            update();
        });

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(toggleButton);
        layout->addStretch();
        setLayout(layout);
    }

protected:
    // Преобразование широты в Y-координату по Меркатору
    double mercatorY(double lat) const
    {
        lat = qBound(-89.5, lat, 89.5); // Ограничиваем широту для избежания бесконечности
        return log(tan(M_PI / 4 + qDegreesToRadians(lat) / 2));
    }

    // Обратное преобразование Y-координаты в широту
    double inverseMercatorY(double y) const
    {
        return qRadiansToDegrees(2 * atan(exp(y)) - M_PI / 2);
    }

    void paintEvent(QPaintEvent*) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QPen gridPen(QColor(200, 200, 200), 1, Qt::DotLine);
        QPen axisPen(Qt::black, 2);
        QFont labelFont("Arial", 8);
        painter.setFont(labelFont);

        int width = this->width();
        int height = this->height();
        int margin = 40;

        painter.setPen(axisPen);
        painter.drawLine(margin, height - margin, width - margin, height - margin); // X
        painter.drawLine(margin, height - margin, margin, margin);                  // Y

        int gridSteps = 10;
        double xStep = (width - 2 * margin) / static_cast<double>(gridSteps);

        painter.setPen(gridPen);

        if (isGeographic)
        {
            painter.drawText(width - margin - 30, height - margin + 20, "Долгота");
            painter.drawText(margin - 30, margin + 10, "Широта");

            // Географические координаты с проекцией
            double minLat = -85.0, maxLat = 85.0; // Ограничиваем широту для Меркатора
            double minY = mercatorY(minLat);
            double maxY = mercatorY(maxLat);
            double yScale = (height - 2 * margin) / (maxY - minY);

            for (int i = 0; i <= gridSteps; ++i)
            {
                // Ось X (долгота) - равномерно
                int x = margin + i * xStep;
                painter.drawLine(x, height - margin, x, margin);

                double lon = -180.0 + i * (360.0 / gridSteps);
                QString xLabel = QString::number(lon, 'f', 1) + "°";
                painter.drawText(x - 20, height - margin + 15, xLabel);

                // Ось Y (широта) - с проекцией Меркатора
                double yRatio = i / static_cast<double>(gridSteps);
                double yValue = minY + yRatio * (maxY - minY);
                int y = height - margin - yRatio * (height - 2 * margin);
                painter.drawLine(margin, y, width - margin, y);

                double lat = inverseMercatorY(yValue);
                QString yLabel = QString::number(lat, 'f', 1) + "°";
                painter.drawText(margin - 35, y + 5, yLabel);
            }
        }
        else
        {
            // Метрические координаты (как в исходном коде)
            painter.drawText(width - margin - 10, height - margin + 20, "X");
            painter.drawText(margin - 20, margin + 10, "Y");

            double yStep = (height - 2 * margin) / static_cast<double>(gridSteps);

            for (int i = 0; i <= gridSteps; ++i)
            {
                int x = margin + i * xStep;
                int y = height - margin - i * yStep;

                painter.drawLine(x, height - margin, x, margin);
                painter.drawLine(margin, y, width - margin, y);

                QString xLabel = QString::number(i * (100.0 / gridSteps), 'f', 1);
                QString yLabel = QString::number(i * (100.0 / gridSteps), 'f', 1);

                painter.drawText(x - 20, height - margin + 15, xLabel);
                painter.drawText(margin - 35, y + 5, yLabel);
            }
        }
    }

private:
    bool isGeographic;
};
