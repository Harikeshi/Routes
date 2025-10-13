#pragma once

#include <QPainter>
#include <QWidget>
#include <algorithm>

namespace Widgets {
// Линейная интерполяция между двумя цветами
inline QColor lerpColor(const QColor& c1, const QColor& c2, double t)
{
    t = std::clamp(t, 0.0, 1.0);
    int r = std::lround(c1.red() * (1.0 - t) + c2.red() * t);
    int g = std::lround(c1.green() * (1.0 - t) + c2.green() * t);
    int b = std::lround(c1.blue() * (1.0 - t) + c2.blue() * t);
    return QColor(r, g, b);
}

// от светло-зелёного до красного
inline QColor greenToRed(double t)
{
    return lerpColor(QColor(144, 238, 144), QColor(255, 0, 0), t);
}

// от голубого до фиолетового
inline QColor blueToViolet(double t)
{
    return lerpColor(QColor(0, 191, 255), // DeepSkyBlue
                     QColor(148, 0, 211), // DarkViolet
                     t);
}
} // namespace Widgets
