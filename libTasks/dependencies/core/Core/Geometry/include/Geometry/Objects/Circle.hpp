#ifndef GEOMETRY_OBJECTS_CIRCLE_HPP
#define GEOMETRY_OBJECTS_CIRCLE_HPP
//==============================================================================
#include <Geometry/Objects/Line.hpp>
//! Point
#include <Point/Point2D.hpp>
//! PrimaryEntities
#include <PrimaryEntities/CircleLite.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
/*!
 * \brief Круг на декартовой плоскости.
 */
class Circle
{
//==============================================================================
public:
//==============================================================================
 /*!
  *\brief Конструктор круга на декартовой плоскости
  *\param[in] center - Центр круга (точка на плоскости)
  *\param[in] radius - Радиус круга [м]
  *\throws GeometryFailure # NegativeRadius
  */
  Circle( const Point2D & center, const double radius );
//==============================================================================
 /*!
  *\brief Конструктор круга по трем точкам
  *\param[in] pA - первая точка
  *\param[in] pB - вторая точка
  *\param[in] pC - третья точка
  *\throws GeometryFailure # CircumscribedCircleForBadTriangle
  */
  Circle( const Point2D& pA, const Point2D& pB, const Point2D& pC );
//==============================================================================
//==============================================================================
 /*!
  *\brief Получить радиус круга
  *\return - радиус [м]
  */
  inline double radius( ) const noexcept
  {
    return mRadius;
  }
//==============================================================================
 /*!
  *\brief Задать радиус окружности
  *\param[in] radius - радиус [м]
  *\throws GeometryFailure # NegativeRadius
  */
  virtual void setRadius( const double radius );
//==============================================================================
 /*!
  *\brief Получить центр окружности
  *\return - центр (точка на плоскости)
  */
  inline Point2D center( ) const noexcept
  {
    return mCenter;
  }
//==============================================================================
 /*!
  *\brief Задать центр окружности
  *\return - центр (точка на плоскости)
  */
  inline void setCenter( const Point2D& center ) noexcept
  {
    mCenter = center;
  }
//==============================================================================
 /*!
  *\brief Задать центр и радиус окружности
  *\param[in] center - первая точка
  *\param[in] radius - радиус [м]
  *\throws GeometryFailure # NegativeRadius
  */
  void setParams( const Point2D & center, const double radius );
//==============================================================================
 /*!
  *\brief Получить окружность
  *\return - окружность
  */
  inline PrimaryEntities::CircleLite2D getCircle( ) const
  {
    return PrimaryEntities::CircleLite2D( mCenter, mRadius );
  }
//==============================================================================
 /*!
  *\brief Задать окружность
  *\param[in] circle - окружность
  *\throws GeometryFailure # NegativeRadius
  */
  void setCircle( const PrimaryEntities::CircleLite2D& circle );
//==============================================================================
 /*!
  *\brief Задать круг тремя точками
  *\param[in] a - первая точка
  *\param[in] b - вторая точка
  *\param[in] c - третья точка
  *\throws GeometryFailure # CircumscribedCircleForBadTriangle
  */
  void buildNearTriangle( const Point2D& a, const Point2D& b, const Point2D& c );
//==============================================================================
//==============================================================================
 /*!
  *\brief Лежит ли точка в пределах окружности
  *\details Учитываются точки внутри окружности, на ней, и возле с некой погрешностью
  *\param[in] point - проверяемая точка (на плоскости)
  *\param[in] eps - допустимая погрешность [м]
  */
  bool isInsideCircle( const Point2D& point, const double eps = 0 );
//==============================================================================
 /*!
  *\brief Лежит ли точка на окружности
  *\details Учитываются точки на окружности с некой погрешностью
  *\param[in] point - проверяемая точка (на плоскости)
  *\param[in] eps - допустимая погрешность [м]
  */
  bool isOnCircle( const Point2D& point, const double eps = 0 );
//==============================================================================
 /*!
  *\brief Найти точку касания окружности и прямой, проходящей через
  *       заданную точку
  *\param[in] point - точка, через которую должна проходить касательная
  *           (не лежит на окружности)
  *\param[in] toLeftSide - ищем левую касательную, если смотреть
  *           от point к centerPoint
  *\return - точка касания на окружности
  *\throws GeometryFailure # TangentThroughInsidePoint
  */
  Point2D tangentPoint( const Point2D& point, const bool toLeftSide );
//==============================================================================
 /*!
  *\brief Найти касательную к окружности, проходящую через данную точку
  *
  *\param[in] point - точка, через которую должна проходить касательная
  *           (не лежит на окружности)
  *\param[in] toLeftSide - ищем левую касательную, если смотреть
  *           от point к centerPoint
  *\return - касательная к окружности
  *\throws GeometryFailure # TangentThroughInsidePoint
  */
  Line tangentLine( const Point2D& point, const bool toLeftSide );
//==============================================================================
 /*!
  *\brief Найти касательную к окружности с заданным углом наклона
  *\param[in] lineSlope  - угол наклона прямой
  *\param[in] toLeftSide - ищем левую касательную (относительно прямой с
  *                        заданным углом наклона, проходящей через центр
  *                        окружности)
  *\return - касательная к окружности
  */
  Line tangentLine(Radian lineSlope, const bool toLeftSide);
//==============================================================================
//==============================================================================
protected:
//==============================================================================
  Point2D mCenter; //!< Центр круга (точка на плоскости)
  double  mRadius; //!< Радиус круга [м]
//==============================================================================
};
//==============================================================================
} //! Geometry
//==============================================================================
#endif //! GEOMETRY_OBJECTS_CIRCLE_HPP
