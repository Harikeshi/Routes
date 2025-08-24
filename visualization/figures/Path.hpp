#pragma once

#include "Element.hpp"
#include "Segment.hpp"
#include "Arc.hpp"

class Path {
    std::vector<std::shared_ptr<Element>> _elements;

public:
    void add(const std::shared_ptr<Segment> &segment) {
        _elements.push_back(segment);
    }

    void add(const std::shared_ptr<Arc> &arc) {
        _elements.push_back(arc);
    }

    void add(const Arc &arc) {
        _elements.push_back(std::make_shared<Arc>(arc.start(), arc.end(), arc.center(), arc.speed(), arc.clockwise()));
    }

    void add(const Segment &seg) {
        _elements.push_back(std::make_shared<Segment>(seg.start(), seg.end(), seg.speed()));
    }

    void add_range(const std::vector<std::shared_ptr<Element>> &elements) {
        for (const auto &element: elements)
            _elements.push_back(element);
    }

    std::vector<std::shared_ptr<Element>> elements() const {
        return _elements;
    }

    void add(const Path &path) {
        for (const auto &element: path._elements) {
            _elements.push_back(element);
        }
    }

    void add(const std::vector<Arc> &arcs) {
        for (const auto &arc: arcs) {
            this->add(arc);
        }
    }

    void add(const std::vector<Segment> &segments) {
        for (const auto &segment: segments) {
            this->add(segment);
        }
    }

    void add(const std::vector<WayPoint> &elements) {
        QPointF start{};

        if (!elements.empty()) {
            start = _elements.back()->end();
        }

        for (const auto &element: elements) {
            add(std::make_shared<Segment>(start, start + element.position, element.speed));
            start = _elements.back()->end();
        }
    }

    void draw(QPainter &painter) {
        for (const auto &element: _elements) {
            element->draw(painter);
        }
    }

    std::shared_ptr<Element> last() {
        return _elements.back();
    }

    std::shared_ptr<Element> first() {
        return _elements.front();
    }

    void show() const {
        for (const auto &element: _elements) {
            element->show();
        }
    }
};
