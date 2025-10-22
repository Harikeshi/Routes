#ifndef INTERSECTOR_HPP
#define INTERSECTOR_HPP
#include <Point/Point2D.hpp>

namespace PrimaryEntities
{

struct Intersection
{
    Point2D point{0, 0};
    size_t vertexIndex{0};
    bool isVertex{false};
    Intersection() = default;
    Intersection (const Point2D point, const size_t vertId, bool isVert = false) : point(point), vertexIndex(vertId), isVertex(isVert) {};
    inline bool operator == (const Intersection& object) const
    {
        return this->point == object.point &&
               this->vertexIndex == object.vertexIndex &&
               this->isVertex == object.isVertex;
    }
};

struct RingIntersection : Intersection
{
    std::vector<Intersection> intersections;
    size_t ringIndex{0}; // i = 0 если пересечение на внешнем ринге(outer), i = 1..k если на внутреннем (inner[i])
    RingIntersection() = default;
    RingIntersection(const std::vector<Intersection>& vInter, const size_t& index) : intersections(vInter), ringIndex(index) {};
    RingIntersection(const Intersection& inter, const size_t& index) : ringIndex(index)
    {
        intersections.push_back(inter);
    };
};

}
#endif // INTERSECTOR_HPP
