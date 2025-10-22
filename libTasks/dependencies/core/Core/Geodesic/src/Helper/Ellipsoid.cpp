#include <Geodesic/Helper/Ellipsoid.hpp>
#include "EllipsoidInner.hpp"
//! Tools
#include <Geodesic/GeoLineTools/LoxodromeTools.hpp>
#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>
//! Cartographic
#include <Cartographic/database.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
Ellipsoid::Ellipsoid(double majorRadius, double flattening)
: mParams(std::make_shared<EllipsoidInner>(majorRadius, flattening))
{
}
//==============================================================================
Ellipsoid::Ellipsoid(BasicEllipsoid type)
: mParams(nullptr)
{
  //! Получение параметров требуемого эллипсоида
  std::shared_ptr<Cartographic::IEllipsoid> ellipsoid;
  switch (type)
  {
  case EL_KRASOVSKY:
    ellipsoid = std::make_shared<Cartographic::Ellipsoid::Krasovsky>();
    break;
  case EL_GENERAL_GLOBE:
    ellipsoid = std::make_shared<Cartographic::Ellipsoid::GeneralGlobe>();
    break;
  default: //!< По умолчанию используется WGS 84 
    ellipsoid = std::make_shared<Cartographic::Ellipsoid::WGS_84>();
    break;
  }

  //! Формируем параметры эллипсоида
  const double flattening (1. - sqrt(1 - ellipsoid->E2()));
  mParams = std::make_shared<EllipsoidInner>(ellipsoid->A(), flattening);
}
//==============================================================================
std::shared_ptr<LoxodromeTools> Ellipsoid::createLoxodromeTools() 
const
{
  return std::make_shared<LoxodromeTools>(*this);
}
//==============================================================================
std::shared_ptr<OrthodromyTools> Ellipsoid::createOrthodromyTools() 
const
{
  return std::make_shared<OrthodromyTools>(*this);
}
//==============================================================================
double Ellipsoid::getMajorRadius() const
{
  return mParams->a;
}
//==============================================================================
double Ellipsoid::getMinorRadius() const
{
  return mParams->b;
}
//==============================================================================
double Ellipsoid::getFlatenning() const
{
  return mParams->f;
}
//==============================================================================
} // ! Geodesic
//==============================================================================

