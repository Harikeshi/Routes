// SegmentView.h
#pragma once
#include <QPointF>
#include <QVector>
#include <QWidget>

namespace NewWidgets {
struct Point
{
    double x{0.0};
    double y{0.0};
};

struct Segment
{
    Point a;
    Point b;
};

class SegmentView : public QWidget
{
    Q_OBJECT
public:
    explicit SegmentView(QWidget* parent = nullptr);

    void setSegments(const QVector<Segment>& segs);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QVector<Segment> m_segments;

    // World-to-screen transform parameters
    double m_scale{1.0}; // pixels per world unit (uniform)
    double m_tx{0.0};    // translation in pixels (X)
    double m_ty{0.0};    // translation in pixels (Y)
    bool m_initialFitDone{false};

    // Margins for axes labels
    int m_marginLeft{50};
    int m_marginBottom{30};
    int m_marginRight{15};
    int m_marginTop{15};

    // Interaction
    bool m_panning{false};
    QPoint m_lastMousePos;

    // Bounding box of figure
    double m_minX{0.0}, m_minY{0.0}, m_maxX{0.0}, m_maxY{0.0};

    // Helpers
    void computeBoundingBox();
    void fitFigureToWidgetOnce();
    QPointF worldToScreen(const QPointF& w) const;
    QPointF screenToWorld(const QPointF& s) const;
    void drawGrid(QPainter& p);
    void drawAxesLabels(QPainter& p);
    void drawSegments(QPainter& p);

    // Grid step helpers
    double chooseNiceStep(double targetPixelsPerMajor, double scale) const;
    QString formatNumber(double v) const;
};

// SegmentView.cpp
#include "SegmentView.h"
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <cmath>
#include <limits>

SegmentView::SegmentView(QWidget* parent) : QWidget(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    // Example default segments (a simple polyline rectangle)
    QVector<Segment> demo{
        {{0, 0}, {100, 0}},
        {{100, 0}, {100, 60}},
        {{100, 60}, {0, 60}},
        {{0, 60}, {0, 0}}};
    setSegments(demo);
}

void SegmentView::setSegments(const QVector<Segment>& segs)
{
    m_segments = segs;
    computeBoundingBox();
    m_initialFitDone = false; // re-fit on next show
    update();
}

void SegmentView::computeBoundingBox()
{
    if (m_segments.isEmpty())
    {
        m_minX = m_minY = 0.0;
        m_maxX = m_maxY = 1.0;
        return;
    }
    m_minX = std::numeric_limits<double>::infinity();
    m_minY = std::numeric_limits<double>::infinity();
    m_maxX = -std::numeric_limits<double>::infinity();
    m_maxY = -std::numeric_limits<double>::infinity();
    for (const auto& s : m_segments)
    {
        m_minX = std::min(m_minX, std::min(s.a.x, s.b.x));
        m_minY = std::min(m_minY, std::min(s.a.y, s.b.y));
        m_maxX = std::max(m_maxX, std::max(s.a.x, s.b.x));
        m_maxY = std::max(m_maxY, std::max(s.a.y, s.b.y));
    }
    // Ensure origin (0,0) is included to keep bottom-left meaningful
    m_minX = std::min(m_minX, 0.0);
    m_minY = std::min(m_minY, 0.0);
}

void SegmentView::fitFigureToWidgetOnce()
{
    // Fit only once (on first show); later resizes do not change figure scale.
    int w = width();
    int h = height();
    int availW = std::max(1, w - (m_marginLeft + m_marginRight));
    int availH = std::max(1, h - (m_marginTop + m_marginBottom));

    double worldW = std::max(1e-9, m_maxX - 0.0); // since origin is bottom-left
    double worldH = std::max(1e-9, m_maxY - 0.0);

    double sx = static_cast<double>(availW) / worldW;
    double sy = static_cast<double>(availH) / worldH;
    m_scale = std::min(sx, sy);

    // Position origin at bottom-left margins; tx,ty allow nudging if needed.
    m_tx = 0.0;
    m_ty = 0.0;

    m_initialFitDone = true;
}

QPointF SegmentView::worldToScreen(const QPointF& w) const
{
    // Bottom-left origin:
    double sX = m_marginLeft + m_tx + w.x() * m_scale;
    double sY = height() - m_marginBottom - m_ty - w.y() * m_scale;
    return QPointF(sX, sY);
}

QPointF SegmentView::screenToWorld(const QPointF& s) const
{
    double wX = (s.x() - m_marginLeft - m_tx) / m_scale;
    double wY = (height() - m_marginBottom - m_ty - s.y()) / m_scale;
    return QPointF(wX, wY);
}

void SegmentView::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    // Background
    p.fillRect(rect(), QColor(250, 250, 250));

    // Grid first (extends with widget)
    drawGrid(p);

    // Segments (fixed scale unless user zooms)
    drawSegments(p);

    // Axes labels on left and bottom
    drawAxesLabels(p);
}

void SegmentView::drawSegments(QPainter& p)
{
    QPen segPen(QColor(30, 30, 30), 2.0);
    p.setPen(segPen);

    for (const auto& s : m_segments)
    {
        QPointF a = worldToScreen(QPointF(s.a.x, s.a.y));
        QPointF b = worldToScreen(QPointF(s.b.x, s.b.y));
        p.drawLine(a, b);
    }
}

double SegmentView::chooseNiceStep(double targetPixelsPerMajor, double scale) const
{
    // Choose a world step so that step * scale ~ targetPixelsPerMajor
    // Use 1, 2, 5 × 10^k pattern
    double targetWorld = targetPixelsPerMajor / std::max(1e-12, scale);
    double pow10 = std::pow(10.0, std::floor(std::log10(targetWorld)));
    double candidates[3] = {1.0, 2.0, 5.0};
    double best = pow10;
    double bestDiff = std::numeric_limits<double>::infinity();
    for (double c : candidates)
    {
        double step = c * pow10;
        double diff = std::fabs(step * scale - targetPixelsPerMajor);
        if (diff < bestDiff)
        {
            bestDiff = diff;
            best = step;
        }
    }
    return best;
}

QString SegmentView::formatNumber(double v) const
{
    // Compact formatting for axis labels
    if (std::fabs(v) < 1e-6)
        return QStringLiteral("0");
    // Decide precision based on magnitude
    double mag = std::fabs(v);
    int prec = (mag < 1.0) ? 3 : (mag < 10.0 ? 2 : 0);
    return QString::number(v, 'f', prec);
}

void SegmentView::drawGrid(QPainter& p)
{
    // Area where the grid is drawn (inside margins)
    int left = m_marginLeft;
    int right = width() - m_marginRight;
    int bottom = height() - m_marginBottom;
    int top = m_marginTop;

    QRect gridRect(left, top, right - left, bottom - top);

    // Outline
    p.setPen(QPen(QColor(220, 220, 220), 1));
    p.drawRect(gridRect);

    // Major step ~ 80 px
    double majorTargetPx = 90.0;
    double step = chooseNiceStep(majorTargetPx, m_scale);

    // Draw vertical lines from world x = 0 to max
    // Determine visible world range
    double worldMinX = screenToWorld(QPointF(left, bottom)).x();
    double worldMaxX = screenToWorld(QPointF(right, bottom)).x();
    double worldMinY = screenToWorld(QPointF(left, bottom)).y();
    double worldMaxY = screenToWorld(QPointF(left, top)).y();

    // Start indices aligned to step
    double startX = std::floor(worldMinX / step) * step;
    double startY = std::floor(worldMinY / step) * step;

    QPen minorPen(QColor(235, 235, 235), 1);
    QPen majorPen(QColor(210, 210, 210), 1);
    QPen axisPen(QColor(180, 180, 180), 1);

    QFont labelFont = font();
    labelFont.setPointSize(std::max(8, labelFont.pointSize()));
    p.setFont(labelFont);

    // Axes lines (x=0 and y=0)
    // Vertical axis (x=0)
    {
        QPointF a = worldToScreen(QPointF(0, worldMinY));
        QPointF b = worldToScreen(QPointF(0, worldMaxY));
        p.setPen(axisPen);
        p.drawLine(QPointF(a.x(), bottom), QPointF(a.x(), top));
    }
    // Horizontal axis (y=0)
    {
        QPointF a = worldToScreen(QPointF(worldMinX, 0));
        QPointF b = worldToScreen(QPointF(worldMaxX, 0));
        p.setPen(axisPen);
        p.drawLine(QPointF(left, a.y()), QPointF(right, a.y()));
    }

    // Vertical grid lines and bottom labels
    p.setPen(minorPen);
    for (double x = startX; x <= worldMaxX + step; x += step)
    {
        QPointF sA = worldToScreen(QPointF(x, worldMinY));
        QPointF sB = worldToScreen(QPointF(x, worldMaxY));
        bool isMajor = std::fabs(std::fmod(x, step * 5.0)) < 1e-9;
        p.setPen(isMajor ? majorPen : minorPen);
        p.drawLine(QPointF(sA.x(), bottom), QPointF(sA.x(), top));
        // Label on bottom for major lines (including x=0)
        if (isMajor || std::fabs(x) < 1e-9)
        {
            QString lbl = formatNumber(x);
            QRectF textRect(sA.x() - 25, bottom + 2, 50, m_marginBottom - 2);
            p.setPen(QPen(QColor(80, 80, 80)));
            p.drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, lbl);
        }
    }

    // Horizontal grid lines and left labels
    p.setPen(minorPen);
    for (double y = startY; y <= worldMaxY + step; y += step)
    {
        QPointF sA = worldToScreen(QPointF(worldMinX, y));
        QPointF sB = worldToScreen(QPointF(worldMaxX, y));
        bool isMajor = std::fabs(std::fmod(y, step * 5.0)) < 1e-9;
        p.setPen(isMajor ? majorPen : minorPen);
        p.drawLine(QPointF(left, sA.y()), QPointF(right, sA.y()));
        // Label on left for major lines (including y=0)
        if (isMajor || std::fabs(y) < 1e-9)
        {
            QString lbl = formatNumber(y);
            QRectF textRect(2, sA.y() - 10, m_marginLeft - 6, 20);
            p.setPen(QPen(QColor(80, 80, 80)));
            p.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, lbl);
        }
    }
}

void SegmentView::drawAxesLabels(QPainter& p)
{
    // Axis titles near origin
    p.setPen(QPen(QColor(60, 60, 60)));
    QFont f = font();
    f.setBold(true);
    p.setFont(f);

    // X label
    p.drawText(QRect(m_marginLeft, height() - m_marginBottom + 4, width() - m_marginLeft - m_marginRight, m_marginBottom - 4),
               Qt::AlignRight | Qt::AlignTop,
               "X");

    // Y label
    p.save();
    // Rotate to draw "Y" vertically along left margin
    p.translate(12, m_marginTop);
    p.rotate(-90);
    p.drawText(QRect(0, 0, height() - m_marginTop - m_marginBottom, m_marginLeft - 12),
               Qt::AlignLeft | Qt::AlignTop,
               "Y");
    p.restore();
}

void SegmentView::resizeEvent(QResizeEvent*)
{
    // Keep figure scale unchanged; grid will naturally extend due to larger rect.
    update();
}

void SegmentView::showEvent(QShowEvent*)
{
    if (!m_initialFitDone)
    {
        fitFigureToWidgetOnce();
    }
}

void SegmentView::wheelEvent(QWheelEvent* event)
{
    // Zoom at cursor
    const QPointF cursorS = event->position();
    QPointF beforeW = screenToWorld(cursorS);

    // Typical zoom factor per notch
    int delta = event->angleDelta().y(); // +120 or -120 per notch
    double factor = std::pow(1.125, delta / 120.0);

    // Limit scale
    double newScale = std::clamp(m_scale * factor, 0.05, 1000.0);

    // Adjust translation to keep 'beforeW' under cursor
    // s = L + tx + w*scale; we want s fixed, so solve for tx, ty with new scale.
    QPointF afterS(cursorS);
    double oldScale = m_scale;
    m_scale = newScale;

    // Recompute tx, ty so that world point 'beforeW' maps to the same screen point
    // after zoom: s.x = L + tx + w.x * m_scale => tx = s.x - L - w.x * m_scale
    m_tx = afterS.x() - m_marginLeft - beforeW.x() * m_scale;
    // For Y: s.y = H - B - ty - w.y * m_scale => ty = H - B - s.y - w.y * m_scale
    m_ty = height() - m_marginBottom - afterS.y() - beforeW.y() * m_scale;

    update();
    event->accept();
}

void SegmentView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_panning = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    }
}

void SegmentView::mouseMoveEvent(QMouseEvent* event)
{
    if (m_panning)
    {
        QPoint delta = event->pos() - m_lastMousePos;
        m_lastMousePos = event->pos();

        // Panning in screen space adjusts tx, ty directly
        m_tx += delta.x();
        m_ty -= delta.y(); // note Y inversion: moving mouse up decreases screen y, so increase ty accordingly
        update();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void SegmentView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_panning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    }
}

// main.cpp
} // namespace NewWidgets