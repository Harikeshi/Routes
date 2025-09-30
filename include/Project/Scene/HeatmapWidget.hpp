#pragma once

#include <QtWidgets>
#include <cmath>
#include <vector>

class HeatmapWidget : public QWidget
{
    Q_OBJECT
public:
    HeatmapWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        pts_ = {
            {20, 40}, {160, 80}, {220, 30}, {320, 130}, {420, 100}, {20, 40}};

        R_ = 12.0;

        gridW_ = 200; // количество ячеек по ширине
        gridH_ = 100; // по высоте

        // TODO: Перенести в Capsules Сюда передавать только капсулы для отрисовки из routes.
        rebuildCapsules();   //! Перенести
        reloadUnifiedPath(); //! Перенести
        rebuildMatrix();
    }

    //! Убрать
    void setRadius(double radius)
    {
        R_ = radius;
    }

    void setCapsules(const std::vector<QPainterPath>& capsules)
    {
        capsules_ = capsules;
        rebuildMatrix();
    }

    void setPoints(const std::vector<QPointF>& points)
    {
        pts_ = points;

        rebuildCapsules();
        rebuildMatrix();
    }

    void setGrid(double width, double height)
    {
        gridW_ = width;
        gridH_ = height;
    }

public:
    void draw(QPainter& p)
    {
        p.save();

        QRectF bbox = totalBBox_;
        double cellW = bbox.width() / gridW_;
        double cellH = bbox.height() / gridH_;

        int maxVal = 0;
        for (auto& row : matrix_)
            for (int v : row)
                if (v > maxVal)
                    maxVal = v;

        for (int y = 0; y < gridH_; ++y)
        {
            for (int x = 0; x < gridW_; ++x)
            {
                int v = matrix_[y][x];

                if (v <= 0.0)
                    continue;

                double t = maxVal > 0 ? (double)v / maxVal : 0.0;
                QColor c = heatColor(t);
                ;

                p.fillRect(QRectF(bbox.left() + x * cellW,
                                  bbox.top() + y * cellH,
                                  cellW,
                                  cellH),
                           c);
            }
        }

        //! Перенести
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(100, 180, 240, 180));
        p.drawPath(unifiedPath_);

        p.restore();
    }

    //! Перенести
    void rebuildCapsules()
    {
        capsules_.clear();
        totalBBox_ = rect(); //QRectF();
        for (size_t i = 0; i + 1 < pts_.size(); ++i)
        {
            QPainterPath cap = makeCapsule(pts_[i], pts_[i + 1], R_);
            capsules_.push_back(cap);
            totalBBox_ = totalBBox_.united(cap.boundingRect());
        }
    }

    void rebuildMatrix()
    {
        matrix_.assign(gridH_, std::vector<int>(gridW_, 0));
        double cellW = totalBBox_.width() / gridW_;
        double cellH = totalBBox_.height() / gridH_;

        for (int y = 0; y < gridH_; ++y)
        {
            for (int x = 0; x < gridW_; ++x)
            {
                QPointF center(totalBBox_.left() + (x + 0.5) * cellW,
                               totalBBox_.top() + (y + 0.5) * cellH);
                int count = 0;
                for (auto& cap : capsules_)
                {
                    if (cap.contains(center))
                        ++count;
                }
                matrix_[y][x] = count;
            }
        }
    }

private:
    //! Перенести
    QPainterPath makeCapsule(const QPointF& a, const QPointF& b, qreal R)
    {
        QPainterPath path;
        if (a == b)
        {
            path.addEllipse(a, R, R);
            return path;
        }
        QPointF v = b - a;
        qreal L = std::hypot(v.x(), v.y());
        QPointF dir(v.x() / L, v.y() / L);
        QPointF n(-dir.y(), dir.x());

        QPointF aL = a + n * R;
        QPointF aR = a - n * R;
        QPointF bL = b + n * R;
        QPointF bR = b - n * R;

        path.moveTo(aL);
        path.lineTo(bL);
        path.lineTo(bR);
        path.lineTo(aR);
        path.closeSubpath();

        QRectF arcRectA(a.x() - R, a.y() - R, 2 * R, 2 * R);
        QRectF arcRectB(b.x() - R, b.y() - R, 2 * R, 2 * R);

        QPainterPath capA;
        capA.moveTo(aR);
        capA.arcTo(arcRectA, std::atan2(-(aR.y() - a.y()), aR.x() - a.x()) * 180 / M_PI, 180);
        QPainterPath capB;
        capB.moveTo(bL);
        capB.arcTo(arcRectB, std::atan2(-(bL.y() - b.y()), bL.x() - b.x()) * 180 / M_PI, 180);

        path = path.united(capA);
        path = path.united(capB);
        return path;
    }

    QColor heatColor(double t)
    {
        // t в [0,1], 0 — зелёный, 1 — красный
        int r = static_cast<int>(255 * t);
        int g = static_cast<int>(255 * (1.0 - t));

        return QColor(r, g, 0);
    }

    //! Перенести
    void reloadUnifiedPath()
    {
        unifiedPath_.clear();
        for (const auto cap : capsules_)
        {
            unifiedPath_ = unifiedPath_.united(cap);
        }
        unifiedPath_ = unifiedPath_.simplified();
    }

private:
    //! Перенести
    std::vector<QPointF> pts_;
    qreal R_;
    QPainterPath unifiedPath_;
    std::vector<QPainterPath> capsules_;

    QRectF totalBBox_;
    //! Оставить
    int gridW_, gridH_;
    std::vector<std::vector<int>> matrix_;
};
