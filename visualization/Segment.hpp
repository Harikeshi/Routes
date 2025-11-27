#ifndef SEGMENT_H
#define SEGMENT_H

#include <QPointF>

namespace DeepWidgets {
class Segment
{
public:
    Segment(const QPointF& a, const QPointF& b) : a(a), b(b)
    {
    }

    QPointF a;
    QPointF b;
};
} // namespace DeepWidgets
#endif // SEGMENT_H