#include <Geodesic/GeodesicCartesianTransition/Transition.hpp>
//! Helper
#include "Helper/EllipsoidInner.hpp"
//! std
#include <cmath>
//==============================================================================
namespace Geodesic
{
//==============================================================================
//! TODO: при реализации данной функции в Geometry убрать
/*!
* \brief Функция для приведения значения к диапазону.
*        Значение, выходящее за пределы диапазона, обрезается по границе
* \param[in] value - значение
* \param[in] min - минимально допустимое значение
* \param[in] max - максимально допустимое значение
* \return Значение в пределах диапазона
*/
double toRange(double value, double min, double max)
{
  if (value < min)
    return min;
  else if (value > max)
    return max;
  return value;
};
//==============================================================================
Transition::Transition(const std::shared_ptr<Ellipsoid>& ellipsoid,
                       const double limit /*= ACCURACY_DOUBLE*/)
: mEllipsoid(ellipsoid)
, mLimit(limit < 0 ? 0. : limit)
{
  //! Если эллипсоид не задан, то собирается эллипсоид WGS-84
  if (!mEllipsoid)
    mEllipsoid = std::make_shared<Ellipsoid>(EL_WGS_84);
}
//==============================================================================
Point3D Transition::fromGeodesicToCartesian(const GeoPoint3D& geoPoint)
{
  //! Нормализация геодезических координат
  GeoPoint3D normGeoPoint(geoPoint);
  normGeoPoint.normalize(REGULAR);
  
  //! Геодезические координаты
  double latitude(normGeoPoint.getLatitude().get());    //!< B - широта
  double longitude(normGeoPoint.getLongitude().get());  //!< L - долгота
  double height(normGeoPoint.getHeight());              //!< H - высота

  //! Вычисляем радиус кривизны первого вертикала N = a / W (в метрах) 
  //! a (большая полуось эллипсоида)
  //! W (первая основная функция широты) = sqrt(1 - e^2 * (sinB)^2)

  double cosB(cos(latitude));
  double sinBsinB(toRange(1. - cosB * cosB, 0., 1.));
  double sinB(sqrt(sinBsinB));
  if (latitude < 0.)
    sinB *= -1;

  double cosL(cos(longitude));
  double sinL(sqrt(toRange(1. - cosL * cosL, 0., 1.)));
  if (longitude < 0.)
    sinL *= -1;

  double N(mEllipsoid->mParams->aDEVe
           / sqrt(mEllipsoid->mParams->ree - sinBsinB));
  double NHcosB((N + height) * cosB); //< (N + H) * cosB

  //! Формирование декартовых координат точки
  //! x = (N + H) * cosB * cosL
  //! y = (N + H) * cosB * sinL
  //! x = (N + (1 - e ^ 2) + H) sinB

  //! Собираем результат
  return Point3D(NHcosB * cosL,                                         //!< x
                 NHcosB * sinL,                                         //!< y
                 (N * (1. - mEllipsoid->mParams->ee) + height) * sinB); //!< z
}
//==============================================================================
GeoPoint3D Transition::fromCartesianToGeodesic(const Point3D& point)
{
  //! Декартовые координаты
  double x(point.getX()); //!< x
  double y(point.getY()); //!< y
  double z(point.getZ()); //!< z
  
  //! Геодезические координаты
  double latitude;  //!< широта   (B)
  double longitude; //!< долгота  (L)
  double height;    //!< высота   (H)
  
  //! Вычисление вспомогательной величины D = sqrt(x ^ 2 + y ^ 2) 
  double xxyy(x * x + y * y); //!<  x ^ 2 + y ^ 2
  double D(sqrt(xxyy));       //!<  sqrt(x ^ 2 + y ^ 2) 

  //! Случай, когда D = 0;
  if (fabs(D) <= std::numeric_limits<double>::epsilon())
  {
    //! B = Pi / 2 * z / |z|
    //! H = z * sinB - a * sqrt (1 - e ^ 2 * (sinB) ^ 2) 
    //! (sinB) ^ 2 = 1
    //! Общая часть для вычисления H: a * sqrt (1 - e ^ 2)
    double helpH(mEllipsoid->mParams->a * sqrt(1. - mEllipsoid->mParams->ee));

    //! Определение знака, в зависимости от знака z
    if (z < 0)
    {
      latitude = -M_PI / 2.;
      //! sinB = -1
      height = -z - helpH;
    }
    else
    {
      latitude = M_PI / 2.;
      //! sinB = 1
      height = z - helpH;
    }
    //! longitude = 0;
    return GeoPoint3D(Radian(latitude), Radian(0.), height);
  }

  //! Вычисление долготы по ГОСТу в зависимости от знаков y и x 
  //! (!) Долгота попадает в диапазон [0; 2 * Pi] (!)
  //! La = |arcsin(y / D)|
  //! (y = 0) И (x < 0) => L = Pi
  //! (y = 0) И (x < 0) => L = Pi
  //! (y = 0) И (x < 0) => L = Pi
  //! (y = 0) И (x < 0) => L = Pi
  //! (y = 0) И (x < 0) => L = Pi
  //! (y = 0) И (x < 0) => L = Pi
  //! После требуется нормализация в диапазон [-Pi; Pi)

  //! Аналог, но вычисляет долготу в диапазоне [-Pi; Pi)
  longitude = y > 0. ? acos(x / D) : - acos(x / D);

  //! Случай когда z = 0
  //! B = 0, H = D - a
  if (fabs(z) <= std::numeric_limits<double>::epsilon())
    return GeoPoint3D(Radian(0.), Radian(longitude), 
                      D - mEllipsoid->mParams->a); 
  
  //! Вычисление вспомогательных величин r, c, p
  double r(sqrt(xxyy + z * z)); //!< r = sqrt(x ^ 2 + y ^ 2 + z ^ 2)
  double c(asin(z / r));        //!< c = arcsin(z / r)
  double p(mEllipsoid->mParams->aee / 2 / r); //!< p = a * e ^ 2 / (2 * r)

  //! Итеративный процесс с использованием вспомогательных величин s1 и s2
  double s1(0.);
  double s2(0.);
  double b;             //!< Текуще значение широты: b = c + s1  
  double sinb;          //!< sin(b)
  double denominatorS2; //! sqrt(1. - e ^ 2 * sin(b) ^ 2)
  do 
  {
    s1 = s2;
    b = c + s1; 
    sinb = sin(b);
    denominatorS2 = sqrt(1. - mEllipsoid->mParams->ee * sinb * sinb);
    s2 = asin(p * sin(2 * b) / denominatorS2);
  } while (fabs(s2 - s1) > mLimit);
  
  //! B = b, H = D * cosB + z * sinB - a * sqrt(1. - e ^ 2 * sin(B) ^ 2)
  //! cos(B) = sqrt(1 - sin(B) ^ 2)
  double cosB(sqrt(toRange(1. - sinb * sinb, 0., 1.))); 
  height = D * cosB + z * sinb - mEllipsoid->mParams->a * denominatorS2;
  
  return GeoPoint3D(Radian(b), Radian(longitude), height);
}
//==============================================================================
}
//==============================================================================

