#pragma once

#include <QApplication>
#include <QLinearGradient>
#include <QPainter>
#include <QStackedLayout>
#include <QWidget>
#include <algorithm>
#include <cmath>
#include <vector>

class SmoothHeatmapWidget : public QWidget
{
public:
    explicit SmoothHeatmapWidget(QWidget* parent = nullptr) : QWidget(parent)
    {
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_OpaquePaintEvent, false);
        setStyleSheet("background: transparent;");
        const int rows = 20, cols = 20;
        matrix.assign(rows, std::vector<int>(cols));
        minVal = 256;
        maxVal = 0;
        for (int y = 0; y < rows; ++y)
        {
            for (int x = 0; x < cols; ++x)
            {
                int v = int(50 + 200 * std::sin(x / 3.0) * std::cos(y / 3.0));
                matrix[y][x] = v;
                minVal = std::min(minVal, v);
                maxVal = std::max(maxVal, v);
            }
        }
    }

protected:
    void paintEvent(QPaintEvent*) override
    {
    }

public:
    void draw(QPainter& p)
    {
        if (matrix.empty() || matrix[0].empty())
            return;

        p.save();
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setRenderHint(QPainter::SmoothPixmapTransform, true);
        const int rows = int(matrix.size());
        const int cols = int(matrix[0].size());
        const double cellW = std::max(1.0, double(width()) / double(cols));
        const double cellH = std::max(1.0, double(height()) / double(rows));

        // Heatmap
        p.setOpacity(0.6);
        for (int y = 0; y < rows - 1; ++y)
        {
            for (int x = 0; x < cols - 1; ++x)
            {
                QColor c00 = normColor(matrix[y][x]);
                QColor c10 = normColor(matrix[y][x + 1]);
                QColor c01 = normColor(matrix[y + 1][x]);
                QColor c11 = normColor(matrix[y + 1][x + 1]);
                const int iw = std::max(1, int(std::ceil(cellW)));
                const int ih = std::max(1, int(std::ceil(cellH)));
                QImage cellImg(iw, ih, QImage::Format_ARGB32);
                cellImg.fill(Qt::transparent);
                QPainter cp(&cellImg);
                for (int py = 0; py < ih; ++py)
                {
                    double fy = (ih == 1) ? 0.0 : double(py) / double(ih - 1);
                    QColor left = lerpColor(c00, c01, fy);
                    QColor right = lerpColor(c10, c11, fy);
                    QLinearGradient grad(0, 0, iw, 0);
                    grad.setColorAt(0.0, left);
                    grad.setColorAt(1.0, right);
                    cp.setPen(Qt::NoPen);
                    cp.setBrush(grad);
                    cp.drawRect(0, py, iw, 1);
                }
                cp.end();
                p.drawImage(QRectF(x * cellW, y * cellH, cellW, cellH), cellImg);
            }
        }

        // Contours — одно значение на каждую изолинию
        p.setOpacity(1.0);
        QFont f = p.font();
        f.setPointSize(8);
        p.setFont(f);
        //        for (int level : contourLevels())
        //        {
        //            QColor contourColor = normColor(level);
        //            p.setPen(QPen(contourColor, 1.0));
        //            bool labelDrawn = false;
        //            for (int y = 0; y < rows - 1; ++y)
        //            {
        //                for (int x = 0; x < cols - 1; ++x)
        //                {
        //                    double v0 = matrix[y][x];
        //                    double v1 = matrix[y][x + 1];
        //                    double v2 = matrix[y + 1][x + 1];
        //                    double v3 = matrix[y + 1][x];
        //                    int idx = 0;
        //                    if (v0 > level)
        //                        idx |= 1;
        //                    if (v1 > level)
        //                        idx |= 2;
        //                    if (v2 > level)
        //                        idx |= 4;
        //                    if (v3 > level)
        //                        idx |= 8;
        //                    if (idx == 0 || idx == 15)
        //                        continue;
        //                    auto interp = [&](double a, double b, double offset, bool horizontal) -> QPointF {
        //                        double den = (b - a);
        //                        double t = (std::abs(den) < 1e-9) ? 0.5 : (level - a) / den;
        //                        t = std::clamp(t, 0.0, 1.0);
        //                        if (horizontal)
        //                            return QPointF((x + t) * cellW, (y + offset) * cellH);
        //                        else
        //                            return QPointF((x + offset) * cellW, (y + t) * cellH);
        //                    };
        //                    QPointF e0 = interp(v0, v1, 0.0, true);
        //                    QPointF e1 = interp(v1, v2, 1.0, false);
        //                    QPointF e2 = interp(v3, v2, 1.0, true);
        //                    QPointF e3 = interp(v0, v3, 0.0, false);
        //                    auto drawSeg = [&](const QPointF& a, const QPointF& b) {
        //                        p.drawLine(a, b);
        //                        if (!labelDrawn)
        //                        {
        //                            QPointF mid = (a + b) * 0.5;
        //                            QString txt = QString::number(level);
        //                            QFontMetrics fm(p.font());
        //                            QRect tr = fm.boundingRect(txt).adjusted(-2, -1, 2, 1);
        //                            QPoint tl = (mid + QPointF(2, -2)).toPoint();
        //                            QRect labRect(tl, tr.size());
        //                            p.save();
        //                            p.setPen(Qt::NoPen);
        //                            p.setBrush(QColor(255, 255, 255, 180));
        //                            p.drawRect(labRect);
        //                            p.restore();
        //                            p.setPen(contourColor);
        //                            p.drawText(labRect.topLeft() + QPoint(2, fm.ascent()), txt);
        //                            p.setPen(QPen(contourColor, 1.0));
        //                            labelDrawn = true;
        //                        }
        //                    };
        //                    switch (idx)
        //                    {
        //                    case 1:
        //                    case 14:
        //                        drawSeg(e3, e0);
        //                        break;
        //                    case 2:
        //                    case 13:
        //                        drawSeg(e0, e1);
        //                        break;
        //                    case 3:
        //                    case 12:
        //                        drawSeg(e3, e1);
        //                        break;
        //                    case 4:
        //                    case 11:
        //                        drawSeg(e1, e2);
        //                        break;
        //                    case 6:
        //                    case 9:
        //                        drawSeg(e0, e2);
        //                        break;
        //                    case 7:
        //                    case 8:
        //                        drawSeg(e3, e2);
        //                        break;
        //                    case 5:
        //                        drawSeg(e0, e1);
        //                        drawSeg(e3, e2);
        //                        break;
        //                    case 10:
        //                        drawSeg(e0, e3);
        //                        drawSeg(e1, e2);
        //                        break;
        //                    }
        //                }
        //            }
        //        }
        p.restore();
    }

    QColor normColor(int val) const
    {
        if (maxVal == minVal)
            return QColor(200, 200, 200);
        double t = double(val - minVal) / double(maxVal - minVal);
        t = std::clamp(t, 0.0, 1.0);
        double r = t;
        double g = 1.0;
        double b = 0.0;
        double brightness = 1.0 - 0.35 * t;
        r = std::clamp(r * brightness, 0.0, 1.0);
        g = std::clamp(g * brightness, 0.0, 1.0);
        return QColor(int(r * 255), int(g * 255), int(b * 255), 255);
    }

    static QColor lerpColor(const QColor& a, const QColor& b, double t)
    {
        t = std::clamp(t, 0.0, 1.0);
        return QColor(
            int(a.red() + (b.red() - a.red()) * t),
            int(a.green() + (b.green() - a.green()) * t),
            int(a.blue() + (b.blue() - a.blue()) * t),
            255);
    }

    std::vector<int> contourLevels() const
    {
        int n = 10;
        if (maxVal <= minVal)
            return {};
        std::vector<int> levels;
        for (int i = 1; i < n; ++i)
        {
            int v = int(std::round(minVal + i * (maxVal - minVal) / double(n)));
            levels.push_back(v);
        }
        return levels;
    }

private:
    std::vector<std::vector<int>> matrix;
    int minVal = 0, maxVal = 1;
};