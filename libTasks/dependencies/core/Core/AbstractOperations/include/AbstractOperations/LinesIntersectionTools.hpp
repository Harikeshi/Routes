#ifndef ABSTRACT_OPERATIONS_LINEINTERSECTIONTOOLS_HPP
#define ABSTRACT_OPERATIONS_LINEINTERSECTIONTOOLS_HPP
//==============================================================================
#include <AbstractOperations/AbstractOperationsException.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
#include <MathTools/BasicMath/Range.hpp>
#include <MathTools/BasicMath/AngleTypes.hpp>
//! PrimaryEntities
#include <PrimaryEntities/WayPoint.hpp>
#include <PrimaryEntities/Entities/MultiPoint.hpp>
//! std
#include <vector>
#include <memory>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
using PrimaryEntities::WayPoint;
//==============================================================================
//==============================================================================
// объявляем функции, которые будут переопределены в модулях Geometry и Geodesic
// Если реализация одной из функций для данного типа точки окажется недоступной
// из места вызова пересекатора, получим ошибку компиляции
//==============================================================================
template <typename Point>
class IntersectorHelper
{
public:
//==============================================================================
/*!
 *\brief расстояние между точками 
 *\param[in] first - первая точка
 *\param[in] second - вторая точка
 *\return расстояние между двумя точками [м]
 */
  static
  double distance( const Point& first, const Point& second );
//==============================================================================
/*!
 *\brief Углы наклона соединяющей линии в начальной и конечной точках
 *\param[in] first - первая точка
 *\param[in] second - вторая точка
 *\return пара углов наклона или азимутов
 */
  static
  std::pair<Radian, Radian> inclinations(
    const Point& first, const Point& second );
//==============================================================================
/*!
 *\brief Углы наклона соединяющей линии в начальной и конечной точках и
 *       расстояние между ними
 *\param[in] first - первая точка
 *\param[in] second - вторая точка
 *\return - кортеж, содержащий пару углов наклона [радианы] и длину [м]
 */
  static
  std::tuple<Radian, Radian, double> inclinationsAndDistance(
    const Point& first, const Point& second );
//==============================================================================
/*!
 *\brief Найти точку, делящую интервал в заданном соотношении
 *\details Найти на AB точку C такую, что portion = |AC| / |AB|
 *\param[in] beg - начало интервала
 *\param[in] end - конец интервала
 *\param[in] portion - заданное соотношение, принадлежит [0, 1]
 *\return найденная точка
 */
  static
  Point pointByPortion( const Point& beg, const Point& end,
    const double portion );
//==============================================================================
/*!
 *\brief Найти точку пересечения отрезков
 *\details Предполагается, что подаваемые отрезки имеют ровно одну точку 
 *         пересечения.
 *         Если отрезки не пересекаются, функции следует генерировать исключение
 *\param[in] firstBeg - начало первого отрезка
 *\param[in] firstEnd - конец первого отрезка
 *\param[in] secondBeg - начало второго отрезка
 *\param[in] secondEnd - конец второго отрезка
 *\param[out] crossP - найденное пересечение (координаты)
 */
  static
  void findSingleIntersection( const Point& firstBeg, const Point& firstEnd,
    const Point& secondBeg, const Point& secondEnd,
    Point& crossP );
//==============================================================================
};
//==============================================================================
/*!
 *\brief Варианты пересечений отрезков
 *\details В алгоритмах используется порядок внутри некоторых групп!! 
 *         Произвольные изменения списка не безопасны
 */
enum IntersectionType
{
  WithoutIntersection = 0, // нет ни одной общей точки

  // у отрезков одна общая внутрення (для обоих отрезков) точка
  Intersection = 0x0010, // классическое внутреннее пересечение

  // у отрезков одна общая граничная точка
  CommonBegPoint = 0x0001, // отрезки начинаются в одной точке и дальше расходятся
  FirstAfterSecondPoint = 0x0002, // начало 1-го отрезка совпадает с концом 2-го
  SecondAfterFirstPoint = 0x0004, // начало 2-го отрезка совпадает с концом 1-го
  CommonEndPoint = 0x0008, // отрезки заканчиваются в одной точке, но до этого расходятся

  // один отрезок упирается в другой
  FirstStartsFromSecond = 0x0020, // начало 1-го отрезка лежит внутри 2-го
  SecondStartsFromFirst = 0x0040, // начало 2-го отрезка лежит внутри 1-го
  FirstFinishesAtSecond = 0x0080, // конец 1-го отрезка лежит внутри 2-го
  SecondFinishesAtFirst = 0x0100, // конец 2-го отрезка лежит внутри 1-го

  // частичное наложение ( без внешних границ )
  CommonBegOverlay = 0x0200, // наложение начал ( при противоположных направлениях)
  FirstAfterSecondOverlay = 0x0400, // наложение конца 2-го и начала 1-го
  SecondAfterFirstOverlay = 0x0800, // наложение конца 1-го и начала 2-го
  CommonEndOverlay = 0x1000, // наложение  концов ( при противоположных направлениях )

  // полное вхождение без внешних границ
  FirstInsideSecond = 0x2000, // 1-й отрезок целиком внутри 2-го
  SecondInsideFirst = 0x4000, // 2-й отрезок целиком внутри 1-го

  // полное вхождение с общей границей
  FrstInScndWithCommonBeg = 0x8000, // 1-й отрезок внутри 2-го и у них общее начало
  FrstInScndWithCommonEnd = 0x10000, // 1-й отрезок внутри 2-го и у них общий конец
  FrstInScndAndFrstBegIsScndEnd = 0x20000, // 1-й отрезок внутри 2-го и начало 1-го совпадает с концом 2-го
  FrstInScndAndFrstEndIsScndBeg = 0x40000, // 1-й отрезок внутри 2-го и конец 1-го совпадает с началом 2-го

  ScndInFrstWithCommonBeg = 0x80000, // 2-й отрезок внутри 1-го и у них общее начало
  ScndInFrstWithCommonEnd = 0x100000, // 2-й отрезок внутри 1-го и у них общий конец
  ScndInFrstAndFrstBegIsScndEnd = 0x200000, // 2-й отрезок внутри 1-го и начало 1-го совпадает с концом 2-го
  ScndInFrstAndFrstEndIsScndBeg = 0x400000, // 2-й отрезок внутри 1-го и конец 1-го совпадает с началом 2-го

  // полное наложение
  Coincide = 0x800000, // отрезки полностью совпадают
  Contradirectional = 0x1000000, // отрезки совпадают с точностью до направления

  // агрегирующие типы для удобной трактовки результата
  EndToEnd = CommonBegPoint | CommonEndPoint | FirstAfterSecondPoint
  | SecondAfterFirstPoint, // граница одного отрезка также является границей другого
  TShaped = SecondStartsFromFirst | SecondFinishesAtFirst | FirstStartsFromSecond
  | FirstFinishesAtSecond, // граница одного отрезка расположена на другом
  Overlay = FirstAfterSecondOverlay | SecondAfterFirstOverlay | CommonBegOverlay
  | CommonEndOverlay | FirstInsideSecond | SecondInsideFirst
  | FrstInScndWithCommonBeg | FrstInScndWithCommonEnd | FrstInScndAndFrstBegIsScndEnd
  | FrstInScndAndFrstEndIsScndBeg | ScndInFrstWithCommonBeg | ScndInFrstWithCommonEnd
  | ScndInFrstAndFrstBegIsScndEnd | ScndInFrstAndFrstEndIsScndBeg,  // наложение
  Same = Coincide | Contradirectional, // совпадение (с точностью до направления)

  // самые генеральные признаки
  PointIntersection = Intersection | EndToEnd | TShaped, // пересечение в одной точке
  IntervalIntersection = Overlay | Same, // наложение на определенном интервале
};
//==============================================================================
//TODO возможно, на плоскости быстрее найти пересечение, чем оценить его наличие
/*!
 *\brief Определение наличия и типа пересечения для двумерных ненулевых отрезков
 *\details На плоскости используются отрезки прямых, на эллипсоиде - ортодромий
 *\param[in] firstBeg - начало первого отрезка
 *\param[in] firstEnd - конец первого отрезка
 *\param[in] secondBeg - начало второго отрезка
 *\param[in] secondEnd - конец второго отрезка
 *\return тип найденного пересечения (или его отсутствие)
 */
template <typename Point>
IntersectionType checkIntersection(
  const Point& firstBeg, const Point& firstEnd,
  const Point& secondBeg, const Point& secondEnd )
{
  // случаи совпадающих границ проверяются отдельно
  IntersectionType bordersStatus = WithoutIntersection;
  if( firstBeg == secondBeg )
    bordersStatus = CommonBegPoint;
  if( firstBeg == secondEnd )
    bordersStatus = IntersectionType ( bordersStatus | FirstAfterSecondPoint );
  if( firstEnd == secondBeg )
    bordersStatus = IntersectionType( bordersStatus | SecondAfterFirstPoint );
  if( firstEnd == secondEnd )
    bordersStatus = IntersectionType( bordersStatus | CommonEndPoint );
  if( bordersStatus ) // хотя бы одна пара точек совпала
  {
    if( bordersStatus == ( CommonBegPoint | CommonEndPoint ) )
      return Coincide;

    if( bordersStatus == ( FirstAfterSecondPoint | SecondAfterFirstPoint ) )
      return Contradirectional;

    // если совпала только одна пара, нужно исследовать углы и по ним проверять наложение
    std::tuple<Radian,Radian,double> frst 
      = IntersectorHelper<Point>::inclinationsAndDistance( firstBeg, firstEnd );
    std::tuple<Radian, Radian, double>  scnd 
      = IntersectorHelper<Point>::inclinationsAndDistance( secondBeg, secondEnd );

    switch( bordersStatus )
    {
    case CommonBegPoint:
      if( BasicMath::isEqual( std::get<0>( frst ), std::get<0>( scnd ) ) )
      {
        if( std::get<2>( frst ) < std::get<2>( scnd ) )
          return FrstInScndWithCommonBeg;
        else
          return ScndInFrstWithCommonBeg;
      }
      else
        return CommonBegPoint;

    case CommonEndPoint:
      if( BasicMath::isEqual( std::get<1>( frst ), std::get<1>( scnd ) ) )
      {
        if( std::get<2>( frst ) < std::get<2>( scnd ) )
          return FrstInScndWithCommonEnd;
        else
          return ScndInFrstWithCommonEnd;
      }
      else
        return CommonEndPoint;

    case FirstAfterSecondPoint:
      if( BasicMath::isEqual(
        std::get<0>( frst ).normalize(),
        ( std::get<1>( scnd ) + Radian(M_PI) ).normalize() ) )
      {
        if( std::get<2>( frst ) < std::get<2>( scnd ) )
          return FrstInScndAndFrstBegIsScndEnd;
        else
          return ScndInFrstAndFrstBegIsScndEnd;
      }
      else
        return FirstAfterSecondPoint;

    case SecondAfterFirstPoint:
      if( BasicMath::isEqual(
        ( std::get<1>( frst ) ).normalize(),
        ( std::get<0>( scnd ) + M_PI ).normalize() ) )
      {
        if( std::get<2>( frst ) < std::get<2>( scnd ) )
          return FrstInScndAndFrstEndIsScndBeg;
        else
          return ScndInFrstAndFrstEndIsScndBeg;
      }
      else
        return SecondAfterFirstPoint;
    }
  }
  
  /*
  Если отрезки пересекаются, то натянутый на их концы четырехугольник окажется 
  выпуклым.
  При обходе вершин выпуклого четырехугольника поворот осуществляется все время
  в одну и ту же сторону
  */
  
  /* Пары углов, соответствующие каждой стороне.
  * Технически для разных типов точки они будут разными: на плоскости - углы 
  * наклона, на эллипсоиде - азимуты. Но т.к. для решения задачи требуется 
  * их нормализованная разность, приводить к единой СК не требуется
  */
  std::vector < std::pair<Radian, Radian> > sideDirections( 4 );
  
  sideDirections[0] 
    = IntersectorHelper<Point>::inclinations( firstBeg, secondBeg );
  sideDirections[1] 
    = IntersectorHelper<Point>::inclinations( secondBeg, firstEnd );
  sideDirections[2] 
    = IntersectorHelper<Point>::inclinations( firstEnd, secondEnd );
  sideDirections[3] 
    = IntersectorHelper<Point>::inclinations( secondEnd, firstBeg );
  
  // нормализованные углы поворота в каждой вершине четырехугольника
  std::vector< Radian > turnAngles( 4 );
  using namespace BasicMath;
  std::vector< AngleType > turnAnglesTypes( 4 );
  
  bool allPositive = true;
  bool allNegative = true;
  size_t zeroNum = 0;
  size_t straightNum = 0;
  std::vector<size_t> nullIds;
  
  for( size_t i = 0; i < 4; i++ )
  {
    turnAngles[i] = ( sideDirections[i].first
      - sideDirections[( i + 3 ) % 4].second ).normalize( );
    turnAnglesTypes[i] = checkAndCorrectAngle( turnAngles[i] );
    switch( turnAnglesTypes[i] )
    {
    case ArbitraryAngle:
      allPositive &= ( turnAngles[i] > 0 );
      allNegative &= ( turnAngles[i] < 0 );
      break;
    case ZeroAngle:
      zeroNum++;
      nullIds.push_back( i );
      break;
    case StraightAngle:
      straightNum++;
    }
  }
  
  switch (zeroNum)
  {
  case 1:
    return IntersectionType ( FirstStartsFromSecond << nullIds[0] );
  case 2:
    if( turnAnglesTypes[0] == turnAnglesTypes[2] )
    {
      // один отрезок полностью расположен внутри другого
      return IntersectionType( FirstInsideSecond << nullIds[0] );
    }
    else
    {
      // случай частичного наложения
      return IntersectionType( 
        CommonBegOverlay << ( nullIds[0] + nullIds[1] / 2 ) );
    }
    return Overlay;
  default:
    // если все углы по 180, то отрезки лежат на одной прямой, общих точек нет.
    // Если все углы одного знака - пересечение есть
    return ( ( allPositive || allNegative ) && ( ! straightNum ) )
      ? Intersection 
      : WithoutIntersection;
  }
}
//==============================================================================
/*!
 *\brief Найти соотношение, в котором точка делит интервал (пройденное к целому)
 *\details Считается, что заданная точка расположена внутри интервала
 *\param[in] beg - начало интервала
 *\param[in] end - конец интервала
 *\param[in] point - заданная точка
 *\return - найденное соотношение
 */
template <typename Point>
double portionByPoint( const Point& beg, const Point& end, const Point& point )
{
  double totLen = IntersectorHelper<Point>::distance( beg, end );
  double pointLen = IntersectorHelper<Point>::distance( beg, point );
  return pointLen / totLen;
}
//==============================================================================
/*!
 *\brief Определить тип пересечения и его положение
 *\details Для простого пересечения вовзращается его относительная дальность на 
 *         первом и втором отрезках. Для наложения - относительные дальности 
 *         начала и конца наложения. Начало определяется по направлению первого.
 *         Пример: на середине первого отрезка начался второй, длиной в четверть
 *         первого и идущий в противоположную сторону => { 1/4, 1/2, 1, 0 }
 *\param[in] firstBeg - начало первого отрезка
 *\param[in] firstEnd - конец первого отрезка
 *\param[in] secondBeg - начало второго отрезка
 *\param[in] secondEnd - конец второго отрезка
 *\param[out] portions - относительные дальности пересечения (при наличии)
 *\param[out] crossP - первая общая точка (при наличии)
 *\return тип найденного пересечения (или его отсутствие)
 */
template <typename Point>
IntersectionType findIntersection(
  const Point& firstBeg, const Point& firstEnd,
  const Point& secondBeg, const Point& secondEnd, 
  std::vector<double>& portions, 
  Point& crossP)
{
  // TODO: наверно здесь же нужно формирвоать саму точку пересечения

  portions.clear();
  crossP = Point( );

  IntersectionType type = checkIntersection( firstBeg, firstEnd,
    secondBeg, secondEnd );
  if( !type )
    return WithoutIntersection;

  switch( type )
  {
    case CommonBegPoint:
      portions = { 0, 0 };
      crossP = firstBeg;
      return type;

    case FirstAfterSecondPoint:
      portions = { 0, 1 };
      crossP = firstBeg;
      return type;

    case SecondAfterFirstPoint:
      portions = { 1, 0 };
      crossP = firstEnd;
      return type;

    case CommonEndPoint:
      portions = { 1, 1 };
      crossP = firstEnd;
      return type;

    case FirstStartsFromSecond:
      portions = { 0, portionByPoint( secondBeg, secondEnd, firstBeg ) };
      crossP = firstBeg;
      return type;

    case FirstFinishesAtSecond:
      portions = { 1, portionByPoint( secondBeg, secondEnd, firstEnd ) };
      crossP = firstEnd;
      return type;

    case SecondStartsFromFirst:
      portions = { portionByPoint( firstBeg, firstEnd, secondBeg ), 0 };
      crossP = secondBeg;
      return type;

    case SecondFinishesAtFirst:
      portions = { portionByPoint( firstBeg, firstEnd, secondEnd ), 1 };
      crossP = secondEnd;
      return type;

    case CommonBegOverlay:
      portions = { 0, portionByPoint( firstBeg, firstEnd, secondBeg ),
                  portionByPoint( secondBeg, secondEnd, firstBeg ), 0 };
      crossP = firstBeg;
      return type;

    case FirstAfterSecondOverlay:
      portions = { 0, portionByPoint( firstBeg, firstEnd, secondEnd ),
                  portionByPoint( secondBeg, secondEnd, firstBeg ), 1 };
      crossP = firstBeg;
      return type;

    case SecondAfterFirstOverlay:
      portions = { portionByPoint( firstBeg, firstEnd, secondBeg ), 1,
                  0, portionByPoint( secondBeg, secondEnd, firstEnd ) };
      crossP = secondBeg;
      return type;

    case CommonEndOverlay:
      portions = { portionByPoint( firstBeg, firstEnd, secondEnd ), 1,
                  1, portionByPoint( secondBeg, secondEnd, firstEnd ) };
      crossP = secondEnd;
      return type;

    case FirstInsideSecond:
      portions = { 0, 1, portionByPoint( secondBeg, secondEnd, firstBeg ),
                  portionByPoint( secondBeg, secondEnd, firstEnd ) };
      crossP = firstBeg;
      return type;

    case SecondInsideFirst:
    {
      double beg = portionByPoint( firstBeg, firstEnd, secondBeg );
      double end = portionByPoint( firstBeg, firstEnd, secondEnd );
      if( beg < end )
      {
        portions = { beg, end, 0, 1 };
        crossP = secondBeg;
      }
      else
      {
        portions = { end, beg, 1, 0 };
        crossP = secondEnd;
      }
      return type;
    }

    case FrstInScndWithCommonBeg:
      portions = { 0, 1, 0, portionByPoint( secondBeg, secondEnd, firstEnd ) };
      crossP = firstBeg;
      return type;

    case FrstInScndWithCommonEnd:
      portions = { 0, 1, portionByPoint( secondBeg, secondEnd, firstBeg ), 1 };
      crossP = firstBeg;
      return type;

    case FrstInScndAndFrstBegIsScndEnd:
      portions = { 0, 1, 1, portionByPoint( secondBeg, secondEnd, firstEnd ) };
      crossP = firstBeg;
      return type;

    case FrstInScndAndFrstEndIsScndBeg:
      portions = { 0, 1, portionByPoint( secondBeg, secondEnd, firstBeg ), 0 };
      crossP = firstBeg;
      return type;

    case ScndInFrstWithCommonBeg:
      portions = { 0, portionByPoint( firstBeg, firstEnd, secondEnd ), 0, 1};
      crossP = firstBeg;
      return type;

    case ScndInFrstWithCommonEnd:
      portions = { portionByPoint( firstBeg, firstEnd, secondBeg ), 1, 0, 1};
      crossP = secondBeg;
      return type;

    case ScndInFrstAndFrstBegIsScndEnd:
      portions = { 0, portionByPoint( firstBeg, firstEnd, secondBeg ), 1, 0};
      crossP = firstBeg;
      return type;

    case ScndInFrstAndFrstEndIsScndBeg:
      portions = { portionByPoint( firstBeg, firstEnd, secondEnd ), 1, 1, 0};
      crossP = secondEnd;
      return type;

    case Coincide:
      portions = { 0, 1, 0, 1};
      crossP = firstBeg;
      return type;

    case Contradirectional:
      portions = { 0, 1, 1, 0};
      crossP = firstBeg;
      return type;

    case Intersection:
      IntersectorHelper<Point>::findSingleIntersection(
        firstBeg, firstEnd, secondBeg, secondEnd, crossP );
      portions = { portionByPoint( firstBeg, firstEnd, crossP ),
        portionByPoint( secondBeg, secondEnd, crossP ) };
      return type;
    default: // невозможный формальный выход
      return WithoutIntersection;
  }
}
//==============================================================================
/*!
 *\brief По соотношению, в котором точка делит внутренний интервал (заданный 
 *       соотношениями разбиения внешнего) определить, в каком соотношении 
 *       точка разделит внешний интервал
 *\param[in] innerBeg - соотношение для начала внутреннего интервала
 *\param[in] innerEnd - соотношение для конца внутреннего интервала
 *\param[in] innerPortion - соотношение, в котором точка делит внутренний интервал 
 *\return - поученное соотношение
 */
inline double generalizePortion(
  const double innerBeg, const double innerEnd, const double innerPortion )
{
  return innerBeg + ( innerEnd - innerBeg ) * innerPortion;
}
//==============================================================================
/*!
 *\brief Определить совпадающие точки приведенных отрезков (при наличии)
 *\details Сравниваются отрезки, у которых первая координата произвольна, а вторая
 *         изменяется в интервале [0, 1].
 *         Для компактности в функцию передаются только первые координаты отрезков.
 *         Если общая точка найдена, рассчитываются соотношения, в которых она 
 *         разбивает исходные отрезки
 *\param[in] firstBeg - начало первого отрезка (первая координата, вторая полагается равной 0)
 *\param[in] firstEnd - конец первого отрезка (первая координата, вторая полагается равной 1)
 *\param[in] secondBeg - начало второго отрезка (первая координата, вторая полагается равной 0)
 *\param[in] secondEnd - конец второго отрезка (первая координата, вторая полагается равной 1)
 *\param[out] portions - доля, которую отсекает найденная точка
 *\return - были ли найдены совпадающие участки
 */
bool haveEqPoints( const double& firstBeg, const double& firstEnd,
  const double& secondBeg, const double& secondEnd, std::vector<double>& portions );
//==============================================================================
/*!
 *\brief Определить близкие по горизонтали части приведенных отрезков (при наличии)
 *\details Сравниваются отрезки, у которых первая координата произвольна, а вторая
 *         изменяется в интервале [0, 1].
 *         Для компактности в функцию передаются только первые координаты отрезков.
 *         Ищутся части отрезков, расстояние между которыми по первой координате не
 *         превышает заданное.
 *         Если общая часть найдена, рассчитываются соотношения, в которых она
 *         разбивает исходные отрезки
 *\param[in] firstBeg - начало первого отрезка (первая координата, вторая полагается равной 0)
 *\param[in] firstEnd - конец первого отрезка (первая координата, вторая полагается равной 1)
 *\param[in] secondBeg - начало второго отрезка (первая координата, вторая полагается равной 0)
 *\param[in] secondEnd - конец второго отрезка (первая координата, вторая полагается равной 1)
 *\param[out] portions - доли, которые отсекают границы найденных совпадающих частей от 
 *                       исходных отрезков
 *\param[in] eps - допустимая разница 
 *\return - были ли найдены близкие участки
 */
bool haveHorizontallyCloseParts( const double& firstBeg, const double& firstEnd,
  const double& secondBeg, const double& secondEnd, std::vector<double>& portions,
  const double eps = 0 );
//==============================================================================
/*!
 *\brief Проверить, есть ли на рассматриваемых частях приведенных отрезков близкие
 *\details Сравниваются отрезки, у которых первая координата произвольна, а вторая
 *         изменяется в интервале [0, 1].
 *         Для компактности в функцию передаются только первые координаты отрезков.
 *         Оценивается близость по первой координате.
 *         Близкие части ищутся только внутри фрагментов, заданных portions. Если 
 *         portions пусты, то они так и останутся пустыми, а задача решаться не будет.
 *         По результатам оценки portions уточняются
 *\param[in] firstBeg - начало первого отрезка (первая координата, вторая полагается равной 0)
 *\param[in] firstEnd - конец первого отрезка (первая координата, вторая полагается равной 1)
 *\param[in] secondBeg - начало второго отрезка (первая координата, вторая полагается равной 0)
 *\param[in] secondEnd - конец второго отрезка (первая координата, вторая полагается равной 1)
 *\param[in|out] portions - доли, отсекаемые на заданных отрезках интервалом поиска,
 *                          а позднее найденными близкими частями

 *\return - нашлась ли хотя бы одна близкая точка
 */
bool stillHaveCloseParts( const double& firstBeg, const double& firstEnd,
  const double& secondBeg, const double& secondEnd, std::vector<double>& portions,
  const double eps = 0 );
//==============================================================================
/*!
 *\brief Найти значение, делящее одномерный интервал в заданном соотношении
 *\details Найти на AB точку C такую, что portion = |AC| / |AB|
 *\param[in] beg - начало интервала
 *\param[in] end - конец интервала
 *\param[in] portion - заданное соотношение, должен принадлежать [0, 1]
 *\return найденное значение
 *\throws GeometryFailure : PointNotInInterval
 */
double pointByPortion( const double beg, const double end,
  const double portion );
//==============================================================================
/*!
*\brief Определить тип пересечения и его положение с учетом времени и высоты
*\details Для простого пересечения вовзращается его относительная дальность на
*         первом и втором отрезках. Для наложения - относительные дальности
*         начала и конца наложения. Начало определяется по направлению первого.
*         Подразумевается, что у рассматриваемых отрезков есть хотя бы одна общая
*         временная точка.
*\param[in] firstBeg - начало первого отрезка
*\param[in] firstEnd - конец первого отрезка
*\param[in] secondBeg - начало второго отрезка
*\param[in] secondEnd - конец второго отрезка
*\param[out] portions - относительные дальности пересечения (при наличии)
*\param[out] crossBeg - первая общая точка (при наличии)
*\param[out] crossEnd - последняя общая точка (при наличии)
*\param[in] epsT - погрешность времени [c]
*                  прохождение разными КР одной точки с меньшим интервалом
*                  расценивается как столкновение
*\param[in] epsH - погрешность высоты [м]
*                  прохождение разными КР одной точки с меньшим вертикальным
*                  расстоянием расценивается как столкновение
*\return тип найденного пересечения (или его отсутствие)
*/
template <typename Point>
IntersectionType findIntersection(
  const WayPoint<Point>& firstBeg, const WayPoint<Point>& firstEnd,
  const WayPoint<Point>& secondBeg, const WayPoint<Point>& secondEnd,
  std::vector<double>& portions, 
  WayPoint<Point>& crossBeg, WayPoint<Point>& crossEnd,
  const double epsT = 0.01, const double epsH = 1 )
{
  // пересечение без учета времени и высоты (проекции траектории)
  Point crossP; 
  IntersectionType type = findIntersection( firstBeg.coord, firstEnd.coord,
    secondBeg.coord, secondEnd.coord, portions, crossP );

  if( !type ) // сами 2D отрезки не пересекаются
  {
    crossBeg = WayPoint<Point>( 0, Point( ), 0 );
    crossEnd = WayPoint<Point>( 0, Point( ), 0 );
    return WithoutIntersection;
  }

  if( portions.size( ) == 2 )
  {
    // сами отрезки пересекаются в одной точке
    double frstT = pointByPortion( firstBeg.t, firstEnd.t, portions[0] );
    double scndT = pointByPortion( secondBeg.t, secondEnd.t, portions[1] );
    double frstH = pointByPortion( firstBeg.h, firstEnd.h, portions[0] );
    double scndH = pointByPortion( secondBeg.h, secondEnd.h, portions[1] );
    // пересечение подойдет, если в нем время и высота окажутся близки
    if( BasicMath::isEqual( frstT, scndT, std::max( epsT, ACCURACY_DOUBLE ) )
      && BasicMath::isEqual( frstH, scndH, std::max( epsH, ACCURACY_DOUBLE ) ) )
    {
      crossBeg = crossEnd = WayPoint<Point>( frstT, crossP, frstH );
      return PointIntersection;
    }
    else
    {
      portions.clear( );
      crossBeg = WayPoint<Point>( 0, Point( ), 0 );
      crossEnd = WayPoint<Point>( 0, Point( ), 0 );
      return WithoutIntersection;
    }
  }

  // остались варианты частичного наложения 2D отрезков
  // сначала выделяем из них близкие по времени участки
  stillHaveCloseParts( 
    firstBeg.t, firstEnd.t, secondBeg.t, secondEnd.t, portions, epsT );
  // потом дополнительно сравниваем по высотам
  stillHaveCloseParts( 
    firstBeg.h, firstEnd.h, secondBeg.h, secondEnd.h, portions, epsH );

  if( !portions.size( ) )
  {
    crossBeg = WayPoint<Point>( 0, Point( ), 0 );
    crossEnd = WayPoint<Point>( 0, Point( ), 0 );
    return WithoutIntersection;
  }

  crossBeg = 
    WayPoint<Point>(
      pointByPortion( firstBeg.t, firstEnd.t, portions[0] ),
      IntersectorHelper<Point>::pointByPortion( firstBeg.coord, firstEnd.coord, portions[0] ),
      pointByPortion( firstBeg.h, firstEnd.h, portions[0] )
      );
  
  if( portions.size( ) == 2 )
  {
    crossEnd = crossBeg;
    return PointIntersection;
  }
  else
  {
    crossEnd =
      WayPoint<Point>(
        pointByPortion( firstBeg.t, firstEnd.t, portions[1] ),
        IntersectorHelper<Point>::pointByPortion( firstBeg.coord, firstEnd.coord, portions[1] ),
        pointByPortion( firstBeg.h, firstEnd.h, portions[1] )
        );
    return IntervalIntersection;
  }
}
//==============================================================================
/*!
 *\brief Присоединить новый интервал к упорядоченной последовательности
 *\detail Интервалы упорядочиваются и сравниваются по времени.
 *        Добавляемый интервал может пересекаться только с последним из заданных.
 *        При добавлении склеиваются интервалы, пересекающиеся по времени или 
 *        различающиеся в пределах epsT.
 *        Пространственные координаты точек при склейке не анализируются.
 *\param[in|out] collection - текущая последовательность непересекающихся интервалов
 *\param[in] newElement - добавляемый интервал
 *\param[in] epsT - максимальная допустимая погрешность по времени. 
 *                  При меньшей разнице интервалы склеиваются друг с другом.
 */
template <typename Point>
void appendWithGlue (
  std::vector<std::pair<WayPoint < Point >, WayPoint < Point >>>& collection,
  const std::pair<WayPoint < Point >, WayPoint < Point >>& newElement,
  const double epsT )
{
  if( !collection.size( ) )
  {
    collection = { newElement };
    return;
  }

  if( newElement.first.t > collection.back( ).second.t + epsT )
  {
    // если новый интервал существенно позже существующих, то просто добавляем его
    collection.push_back( newElement );
  }
  else // начало добавляемого интервала в целом попало в предыдущий
  {
    if( newElement.second.t > collection.back( ).second.t )
    {
      // интервалы накладываются. Требуется склейка
      collection.back( ).second = newElement.second;
    }
    // иначе новый интервал целиком лежит в предыдущем, никаких действий не требуется
  }
}
//==============================================================================
/*!
 *\brief Поиск пересечений двух траекторий
 *\details Ищутся точечные пересечения и первые точки наложений
 *\param[in] tr - первая траектория
 *\param[in] otherTr - вторая траектория
 *\param[in] otherDt - средний шаг по времени для второй траектории
 *           -1 - если шаг очень велик или сильно разнится
 *\param[in] epsT - погрешность времени [c]
 *                  прохождение разными КР одной точки с меньшим интервалом
 *                  расценивается как столкновение
 *\param[in] epsH - погрешность высоты [м]
 *                  прохождение разными КР одной точки с меньшим вертикальным
 *                  расстоянием расценивается как столкновение
 *\return - найденные точки пересечения, упорядоченные по первой траектории
 */
template <typename Point>
PrimaryEntities::MultiPoint <Point> findIntersections(
  const std::vector < WayPoint < Point > > tr,
  const std::vector < WayPoint < Point > > otherTr, 
  const double otherDt, const double epsT = 0.01, const double epsH = 1 )
{
  using namespace BasicMath;

  //TODO отсеивать совпадающие узлы (пропускать или кидать исключения)
  //     наверно отдельно совпадения по времени и по координатам

  //TODO бросать исключения или начать корректно обрабатывать траектории из <2 точек

  // возвращаемое значение: точки пересечений и начал наложений
  PrimaryEntities::MultiPoint <Point> retVal;
  // расширенное возвращаемое значение: полные интервалы наложений с учетом времени
  // Точеченым пересеечниям соответствуют интервалы с совпадающими границами
  std::vector<std::pair<WayPoint < Point >, WayPoint < Point >>> innerRetVal;

  size_t fI = 0; // этим  индексом ходим по первой траектории
  size_t sI = 0; // этим индексом ходим по второй траектории

  bool inFrstOverlay = false; // в tr остановились на пересечении / наложении
  bool inScndOverlay = false; // в otherTr остановились на пересечении / наложении

  size_t frstNum = tr.size( ); // число точек в первой траектории
  size_t scndNum = otherTr.size( ); // число точек во второй траектории

  bool exactTime = isNull( epsT );
  // TODO у траекторий из моделятора еще кучу узлов под начальный временной сдвиг можо опустить
  
  while( fI < frstNum - 1 )
  {
    while( sI < scndNum - 1 )
    {
      // проверяем, есть ли пересечение по времени
      if( otherTr[sI + 1].t + epsT < tr[fI].t ) // еще не дошли
      {
        if( otherDt > 0 ) // для регулярных можно сразу много узлов пропускать
          sI += std::max( 1, int(0.9 * ( tr[fI].t - otherTr[sI + 1].t ) / otherDt) );
        else
          sI++;
        continue;
      }

      if( otherTr[sI].t > tr[fI + 1].t + epsT ) // уже дальше (например, 1-я траектория стартовала раньше 2-ой)
        break;

      // проверяем стыки
      if ( exactTime && isEqual( otherTr[sI + 1].t,  tr[fI].t ) )
      {
        if( otherTr[sI + 1].coord == tr[fI].coord )
        {
          if( !inFrstOverlay )
            appendWithGlue( innerRetVal, { tr[fI], tr[fI] }, epsT );
          inFrstOverlay = true;
          inScndOverlay = true;
        }
        sI++;
        continue;
      }
      if( exactTime && isEqual( otherTr[sI].t, tr[fI + 1].t ) )
      {
        if( otherTr[sI].coord == tr[fI + 1].coord )
        {
          if( ! inScndOverlay ) 
            appendWithGlue( innerRetVal, { tr[fI+1], tr[fI+1] }, epsT );
          inFrstOverlay = true;
          inScndOverlay = true;
        }
        break;
      }

      // есть общий временной интервал, ищем пересечение
      std::vector<double> portions;
      WayPoint < Point > crossP1, crossP2; // сюда сохраняем место пересечения
      IntersectionType type = findIntersection(
        tr[fI], tr[fI + 1], otherTr[sI], otherTr[sI + 1], 
        portions, crossP1, crossP2, epsT, epsH );

      switch ( type )
      {
      case PointIntersection:
        // если нашли что-то новое - добавляем
        if ( ( ! ( inScndOverlay && BasicMath::isNull( portions[1] ) ) )
          && ( ! ( inFrstOverlay && BasicMath::isNull( portions[0] ) ) ) )
        { 
          appendWithGlue( innerRetVal, { crossP1, crossP2 }, epsT );
        }
        inScndOverlay = BasicMath::isEqual( portions[1], 1 );
        inFrstOverlay = BasicMath::isEqual( portions[0], 1 );
        break;
      case IntervalIntersection:
        if ( ( ! ( inScndOverlay && BasicMath::isNull( portions[2] ) ) )
          && ( ! ( inFrstOverlay && BasicMath::isNull( portions[0] ) ) ) )
        {
          appendWithGlue( innerRetVal, { crossP1, crossP2 }, epsT );
        }
        inScndOverlay = BasicMath::isEqual( portions[3], 1 );
        inFrstOverlay = BasicMath::isEqual( portions[1], 1 );
        break;
      default:
        break;
      }

      if( otherTr[sI + 1].t < tr[fI + 1].t + epsT )
        sI++; // переход по второй траектории
      else
        break;

    } // конец перебора второй траектории

    fI++;

    // из-за возможности больших временных допусков, некоторые отрезки 
    // имеет смысл рассмотреть повторно
    if ( sI < scndNum - 1 )
      while( ( otherTr[sI+1].t + epsT > tr[fI].t ) && ( sI ) )
        sI--;

  } // конец перебора первой траектории

  retVal.reserve( innerRetVal.size( ));
  for( const auto& interval : innerRetVal )
    retVal.emplace_back( interval.first.coord );

  return retVal;
}
//==============================================================================
} // ! AbstractOperations

#endif // ! ABSTRACT_OPERATIONS_LINEINTERSECTIONTOOLS_HPP