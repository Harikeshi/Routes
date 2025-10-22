#include "Point/GeoPoint2D.hpp"
#include "HelperFunctions.hpp"
//==============================================================================
bool GeoPoint2D::isCorrect(LongitudeRange range /*= REGULAR*/) const
{
  return (   isCorrectLatitude(Radian(mPoint.y))
          && isCorrectLongitude(Radian(mPoint.x), range));
}
//==============================================================================
void GeoPoint2D::normalize(LongitudeRange range /*= NORMAL*/)
{
  double value(mPoint.x
               - floor((mPoint.x + M_PI) / (2 * M_PI)) * 2 * M_PI);
  switch (range)
  {
  case REGULAR:
    mPoint.x = value;
    break;
  case NORMAL:
    if (value < 0.)
      mPoint.x = value + 2 * M_PI;
    else
      mPoint.x = value;
    break;
  default:
    break;
  }
}
//==============================================================================
