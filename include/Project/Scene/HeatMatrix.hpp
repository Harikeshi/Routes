#pragma once

#include <QPainterPath>
#include <QPointF>
#include <vector>

class HeatMatrix {
public:
    static QVector<QVector<int> > calculate(const QVector<QPainterPath> &capsules, const QRectF &totalBBox,
                                            int gridH = 200, int gridW = 100) {
        QVector<QVector<int> > matrix;
        matrix.assign(gridH, QVector<int>(gridW, 0));

        //! Размер клетки
        double cellW = totalBBox.width() / gridW;
        double cellH = totalBBox.height() / gridH;

        for (int y = 0; y < gridH; ++y) {
            for (int x = 0; x < gridW; ++x) {
                QPointF center(totalBBox.left() + (x + 0.5) * cellW,
                               totalBBox.top() + (y + 0.5) * cellH);
                int count = 0;
                for (auto &cap: capsules) {
                    if (cap.contains(center))
                        ++count;
                }
                matrix[y][x] = count;
            }
        }

        return matrix;
    }
};
