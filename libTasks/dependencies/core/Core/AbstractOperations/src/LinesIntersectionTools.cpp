#include <AbstractOperations/LinesIntersectionTools.hpp>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
//==============================================================================
bool haveEqPoints( const double& firstBeg, const double& firstEnd,
  const double& secondBeg, const double& secondEnd, std::vector<double>& portions )
{
  portions.clear( );

  // считаем горизонтальные расстояния между концами отрезков
  double difBeg( firstBeg - secondBeg );
  double difEnd( firstEnd - secondEnd );

  bool eqBeg( BasicMath::isNull( difBeg ) );
  bool eqFin( BasicMath::isNull( difEnd ) );

  if( eqBeg )
  {
    if( eqFin )
    {
      // полное наложение отрезков
      portions = { 0, 1, 0, 1 };
      return true;
    }
    else
    {
      // совпадают начала
      portions = { 0, 0 };
      return true;
    }
  }
  else
  {
    if( eqFin )
    {
      // совпадают окончания
      portions = { 1, 1 };
      return true;
    }
    else
    {
      // если разницы одного знака, то отрезки не пересекаются
      if( difBeg * difEnd > 0 )
        return false;

      // иначе отрезки пересекаются, но не в граничных точках
      double y = ( +secondBeg - firstBeg )
        / ( firstEnd - firstBeg - secondEnd + secondBeg );
      portions = { y, y };
      return true;
    }
  }
}

//==============================================================================
bool haveHorizontallyCloseParts( const double& firstBeg, const double& firstEnd,
  const double& scndBeg, const double& scndEnd, std::vector<double>& portions,
  const double eps /*= 0*/ )
{
  if( BasicMath::isNull( eps ) )
    return haveEqPoints( firstBeg, firstEnd, scndBeg, scndEnd, portions );

  portions.clear( );

  // считаем горизонтальные расстояния между концами отрезков
  double difBeg( firstBeg - scndBeg );
  double difEnd( firstEnd - scndEnd );

  // являются ли начала или окончания отрезков достаточно близкими
  bool begsAreNear( fabs( difBeg ) <= eps );
  bool endsAreNear( fabs( difEnd ) <= eps );

  // если отрезки близки (или совпадают) с обеих сторон, то они близки все целиком
  if( begsAreNear && endsAreNear )
  {
    portions = { 0, 1, 0, 1 };
    return true;
  }

  // если отрезки не пересекаются и вершины не близки, то и близких частей нет
  if( ( difBeg * difEnd > 0 ) && ( !begsAreNear ) && ( !endsAreNear ) )
    return false;

  // во всех остальных случаях есть близкие части, но это не отрезки целиком

  // внутри отрезка x(y) = x2 * y + x1 * ( 1 - y )
  // горизонтальное расстояние оказывается равным eps на уровне: 
  // y = { +- eps - xf1 + xs1 } / { xf2 - xf1 - xs2 + xs1}
  // 0 в знаменателе м.б. только у параллельных отрезков, они отсеиваются выше

  // ищем подходящие уровни
  double y1 = ( eps - firstBeg + scndBeg ) / ( firstEnd - firstBeg - scndEnd + scndBeg );
  double y2 = ( -eps - firstBeg + scndBeg ) / ( firstEnd - firstBeg - scndEnd + scndBeg );
  // если один из корней лежит за пределами рассматриваемого интервала, то нужен другой
  if( ( y1 < 0 ) || ( y1 > 1 ) )
  {
    y1 = y2;
  }
  else if( ( y2 < 0 ) || ( y2 > 1 ) )
  {
    y2 = y1;
  }
  double yMax = std::max( y1, y2 );
  double yMin = std::min( y1, y2 );
  // если оба корня подошли, то выбор зависит от положения искомых частей
  if( begsAreNear )
  {
    portions = { 0, yMax, 0, yMax };
  }
  else if( endsAreNear )
  {
    portions = { yMin, 1, yMin, 1 };
  }
  else
  {
    // если попали сюда, то отрезки пересекаются внутри интервала
    portions = { yMin, yMax, yMin, yMax };
  }
    return true;
}
//==============================================================================
bool stillHaveCloseParts( const double& firstBeg, const double& firstEnd,
  const double& secondBeg, const double& secondEnd, std::vector<double>& portions,
  const double eps /*= 0*/ )
{
  // если нет рассматриваемых участков, то и делать ничего не нужно
  if( !portions.size( ) )
    return false;

  // если проверяется только одна точка, то просто сравниваем значения
  if( portions.size( ) == 2 )
  {
    double frstX = pointByPortion( firstBeg, firstEnd, portions[0] );
    double scndX = pointByPortion( secondBeg, secondEnd, portions[1] );
    if( BasicMath::isEqual( frstX, scndX, std::max(eps, ACCURACY_DOUBLE) ) )
      return true;
    else
    {
      portions.clear( );
      return false;
    }
  }

  /* если проверяются протяженные отрезки, вводим локальные координаты (x, s):
  * x - первая координата, без изменений
  * s - в начале общего участка - 0, в конце - 1
  */
  double frstStX = pointByPortion( firstBeg, firstEnd, portions[0] );
  double frstFinX = pointByPortion( firstBeg, firstEnd, portions[1] );
  double scndStX = pointByPortion( secondBeg, secondEnd, portions[2] );
  double scndFinX = pointByPortion( secondBeg, secondEnd, portions[3] );

  std::vector<double> innerPortions;
  bool retVal = haveHorizontallyCloseParts(
    frstStX, frstFinX, scndStX, scndFinX, innerPortions, eps );

  if( !innerPortions.size( ) )
  {
    portions.clear( );
    return false;
  }

  // пересчитываем доли относительно исходных отрезков
  if( innerPortions.size( ) == 2 )
  {
    innerPortions[0] = generalizePortion( portions[0], portions[1], innerPortions[0] );
    innerPortions[1] = generalizePortion( portions[2], portions[3], innerPortions[1] );
  }
  else
  {
    innerPortions[0] = generalizePortion( portions[0], portions[1], innerPortions[0] );
    innerPortions[1] = generalizePortion( portions[0], portions[1], innerPortions[1] );
    innerPortions[2] = generalizePortion( portions[2], portions[3], innerPortions[2] );
    innerPortions[3] = generalizePortion( portions[2], portions[3], innerPortions[3] );
  }

  portions = innerPortions;
  return true;
}
//==============================================================================
double pointByPortion( const double beg, const double end,
  const double portion )
{
  if( ( portion < 0 ) || ( portion > 1 ) )
    throw AbstractOperationsFailure(
      PointNotInInterval, "", "pointByPortion<double>" );

  return ( 1 - portion ) * beg + portion * end;
}
//==============================================================================
} // ! AbstractOperations
