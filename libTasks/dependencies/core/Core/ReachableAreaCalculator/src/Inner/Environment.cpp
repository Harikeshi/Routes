#ifndef Q_MOC_RUN
#include <Environment.hpp>
#endif

#include <Inner/GeometryEnvironment.hpp>
//==============================================================================
namespace ReachableAreaCalculator
{
//==============================================================================
Environment::Environment(const std::vector<Polygon2D>& polygons)
{
    inner = new GeometryEnvironment(polygons);
}
//==============================================================================
Environment::~Environment()
{
    delete inner;
}
//==============================================================================
void Environment::addPolygon(const Polygon2D& polygon)
{
    inner->addPolygon(polygon);
}
//==============================================================================
const GeometryEnvironment* Environment::getInner() const
{
    return inner;
}
//==============================================================================
}
//==============================================================================