#pragma once

#include "Segment.hpp"

class Route
{
public:
    void addSegment(const Segment& segment)
    {
        segments.push_back(segment);
    }

    const QVector<Segment> getSegments() const
    {
        return segments;
    }

    void setSegments(const QVector<Segment>& segments)
    {
        this->segments = segments;
    }

private:
    QVector<Segment> segments;
};
