#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>

#include <Geodesic/GeodesicException.hpp>
#include <Geodesic/GeodesicCartesianTransition/Transition.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Correct.hpp>
//! Geometry
#include <Geometry/Objects/Vector3D.hpp>
//! GeographicLib
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/GeodesicLine.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
OrthodromyTools::OrthodromyTools(const Ellipsoid& ellipsoid 
                                                  /*= Ellipsoid(EL_WGS_84)*/)
: GeodesicLineTools()
, mGeodesic(std::make_shared<GeographicLib::Geodesic>(
                                                    ellipsoid.getMajorRadius(),
                                                    ellipsoid.getFlatenning()))
{
}
//==============================================================================
Orthodromy OrthodromyTools::createOrthodromy(const GeoPoint2D& startPoint, 
                                             const GeoPoint2D& finishPoint)
const
{
  //! Проверяем, равны ли начальная и конечная точки.
  if (startPoint == finishPoint)
    throw GeodesicFailure(GeodesicEnumFailure::OrthodromyThroughOnePoint);

  //! Создаем ортодромию 
  Orthodromy orthodromy;

  //! Задаём точки
  orthodromy.mStartPoint = startPoint;
  orthodromy.mFinishPoint = finishPoint;

  //! Широта точек должна быть в диапазоне [-Pi / 2; Pi / 2]
  //! Долгота точек должна быть в диапазоне[-Pi; Pi]
  orthodromy.mStartPoint.normalize(REGULAR);
  orthodromy.mFinishPoint.normalize(REGULAR);

  //! Координаты точки старта в градусах
  double startLat(orthodromy.mStartPoint.getLatitude().toDegrees().get());
  double startLon(orthodromy.mStartPoint.getLongitude().toDegrees().get());

  //! Вычисляем длины и азимуты
  double startAzimuth(0.);  //!< азимут от точки начала в градусах
  double finishAzimuth(0.); //!< азимут от точки завершения в градусах

  //! Проверка на равенство граничных точек
  if (! (startPoint == finishPoint))
  {
    //! Если точки не совпадают, то решаем обратную геодезическую задачу
    mGeodesic->Inverse(startLat, startLon,
                       orthodromy.mFinishPoint.getLatitude().toDegrees().get(),
                       orthodromy.mFinishPoint.getLongitude().toDegrees().get(),
                       orthodromy.mLength, startAzimuth, finishAzimuth);

  }

  //! Если точки совпадают, то азимуты из них задаются направлением на Север (0)
  //! Протяжённость приравнивается к 0 (по умолчанию из конструктора) 
 
  //! Переводим градусы в радианы
  orthodromy.mStartAzimuth.fromDegrees(Degree(startAzimuth));
  orthodromy.mFinishAzimuth.fromDegrees(Degree(finishAzimuth));

  //! Создаем линию
  orthodromy.mLine = std::make_shared<GeographicLib::GeodesicLine>(
                          mGeodesic->Line(startLat, startLon, startAzimuth));

  //! Возвращаем значение
  return orthodromy;
}
//==============================================================================
Orthodromy OrthodromyTools::createOrthodromy(
  const GeoPoint2D& startPoint, const Radian& azimuth, 
  double length /*= 50000.*/) const
{
  //! Создаем ортодромию 
  Orthodromy orthodromy;

  //! Задаём точку начала
  orthodromy.mStartPoint = startPoint;
  //! Задаём азимут
  orthodromy.mStartAzimuth = azimuth;
  //! Задаём протяжённость
  orthodromy.mLength = length;

  //! Широта точек должна быть в диапазоне [-Pi / 2; Pi / 2]
  //! Долгота точек должна быть в диапазоне[-Pi; Pi]
  orthodromy.mStartPoint.normalize(REGULAR);

  //! Инициализация координат точки завершения и азимута от неё в градусах
  double finishLat;
  double finishLon;
  double finishAzimuth;

  //! Решение прямой геодезической задачи 
  mGeodesic->Direct(orthodromy.mStartPoint.getLatitude().toDegrees().get(),
                    orthodromy.mStartPoint.getLongitude().toDegrees().get(),
                    orthodromy.mStartAzimuth.toDegrees().get(),
                    length, finishLat, finishLon, finishAzimuth);

  //! Переводим градусы в радианы
  orthodromy.mFinishPoint.set(Degree(finishLat), Degree(finishLon));
  orthodromy.mFinishPoint.normalize(REGULAR);
  orthodromy.mFinishAzimuth.fromDegrees(Degree(finishAzimuth));

  //! Создаем линию
  orthodromy.mLine = std::make_shared<GeographicLib::GeodesicLine>(
                      mGeodesic->Line(
                        orthodromy.mStartPoint.getLatitude().toDegrees().get(),
                        orthodromy.mStartPoint.getLongitude().toDegrees().get(),
                        azimuth.toDegrees().get()));

  //! Возвращаем значение
  return orthodromy;
}
//==============================================================================
double OrthodromyTools::distance(const GeoPoint2D& startPoint, 
                                 const GeoPoint2D& finishPoint) const
{
  //! Подготавливаем точки
  GeoPoint2D start = startPoint;
  GeoPoint2D finish = finishPoint;
  start.normalize(REGULAR);
  finish.normalize(REGULAR);

  //! Находим дальность: решение обратной геодезической задачи 
  double result;
  mGeodesic->Inverse(start.getLatitude().toDegrees().get(),
                     start.getLongitude().toDegrees().get(),
                     finish.getLatitude().toDegrees().get(),
                     finish.getLongitude().toDegrees().get(),
                     result);

  return result;
}
//==============================================================================
GeoPoint2D OrthodromyTools::pointPosition(const GeoPoint2D& point, 
                                          const Radian& azimuth, 
                                          double distance) const
{
  //! Подготавливаем точку отсчёта
  GeoPoint2D startPoint = point;
  startPoint.normalize(REGULAR);

  //! Инициализация координат искомой точки в градусах
  double lat;
  double lon;

  //! Находим нужную точку
  mGeodesic->Direct(startPoint.getLatitude().toDegrees().get(),
                    startPoint.getLongitude().toDegrees().get(),
                    azimuth.toDegrees().get(), distance, 
                    lat, lon);

  //! Формируем результат
  GeoPoint2D result = GeoPoint2D(Degree(lat), Degree(lon));
  result.normalize(REGULAR);

  return result;
}
//==============================================================================
GeoPoint2D OrthodromyTools::
pointPosition(
  const GeoPoint2D& startPoint, 
  const GeoPoint2D& finishPoint, 
  double distanceRatio /*= .5*/) const noexcept
{
  /*!
   * Если точки совпадают,
   * ортодромия между ними вырождается в единственную точку.
   */
  if (startPoint == finishPoint)
    return startPoint;

  //! Создание ортодромии
  Orthodromy ortodromy = createOrthodromy(startPoint, finishPoint);
  //! Вычисление искомой точки
  return ortodromy.pointPosition(distanceRatio * ortodromy.getLength());
}
//==============================================================================
GeoPolygon2D OrthodromyTools::circle(const GeoPoint2D& center, 
                                     const double radius, 
                                     const int numVerticies /*= 360*/) const
{
  //! Инициализация результирующего полигона
  GeoPolygon2D result;

  //! Если радиус меньше или равен 0, то полигон не собираем
  if (radius <= 0.)
    throw GeodesicFailure(GeodesicEnumFailure::CircleDoNotExist);

  //! Вычисление угла поворота для поиска следующей точки
  Radian angle(2 * M_PI / numVerticies);

  //! Последовательный обход по окружности с вычислением точек окружности
  for (int i = 0; i < numVerticies; i++)
    result.outer().emplace_back(pointPosition(center, angle * i, 
                                              radius));
  //! Замыкаем
  result.outer().push_back( result.outer( )[0] );

  //! Возвращаем результат
  return result;
}
//==============================================================================
GeoPolygon2D OrthodromyTools::circle(const GeoPoint2D& center, 
                                     const double radius, 
                                     const double segmentLength) 
const
{
  //! Инициализация результирующего полигона
  GeoPolygon2D result;

  //! Если радиус неположителен, полигон не собираем.
  if (radius <= 0.)
    throw GeodesicFailure(GeodesicEnumFailure::CircleDoNotExist);
  /*!
   * Если расстояние между соседними точками контура неположительно
   * или радиус не превосходит половины расстояния между соседними 
   * вершинами контура, то полигон не собираем.
   */
  if ((segmentLength <= 0.) || (radius <= segmentLength / 2.))
    throw GeodesicFailure(
            GeodesicEnumFailure::IncorrectSegmentLengthForEllipseApproximation);

  //! Находим количество точек в контуре

  //! Собираем переводчик из геодезических координат в декартовы для текущего
  //! эллипсоида   
  Transition trasition(std::make_shared<Ellipsoid>(getMajorRadius(), 
                                                   getFlatenning()));

  //! Ищем длину радиус-вектора (от центра эллипсоида до точки, задающей центр
  //! окружности на поверхности эллипсоида) 
  double lenVR(
    Geometry::Vector3D(
      trasition.fromGeodesicToCartesian(
        GeoPoint3D(center, 0.f))).length());

  //! Ищем угол поворота, соответсвующий требуемой длине дуги между двумя 
  //! соседними вершинами контура
  Radian angle(segmentLength / (lenVR * sin(radius / lenVR)));

  //! Находим количество вершин для формирования равноудалённых вершин контура
  int numVerticies = int(std::ceil(M_PI * 2 / angle.get()));

  //! Формируем конутр окружности
  return circle(center, radius, numVerticies);
}
//==============================================================================
GeoPolygon2D OrthodromyTools::ellipse(const GeoPoint2D& firstFocus, 
                                      const GeoPoint2D& secondFocus, 
                                      const double majorRadius, 
                                      const double segmentLength /*= 10000.*/, 
                                      const double accuracy /*= 0.1*/)
{
  //! Инициализация результирующего полигона
  GeoPolygon2D result;

  //! Если длина большей полуоси неположительна, не строим эллипс.
  if (majorRadius <= 0.)
    throw GeodesicFailure(GeodesicEnumFailure::EllipseDoNotExist);

  /*!
   * Если расстояния между соседними вершинами 
   * контура меньше или равны 0, то контур не строим.
   */
  if (segmentLength <= 0.)
    throw GeodesicFailure(
            GeodesicEnumFailure::IncorrectSegmentLengthForEllipseApproximation);

  //! Если точность неположительна, численный метод может никогда не сойтись.
  if (accuracy <= std::numeric_limits<double>::epsilon())
    throw GeodesicFailure(GeodesicEnumFailure::NonPositiveAccuracy);
  
  //! Если расстояние между фокусами = 0, то они совпадают, поэтому строим
  //! окружность
  if (BasicMath::isNull(distance(firstFocus, secondFocus)))
    return circle(firstFocus, majorRadius, segmentLength);

  //! Максимальное суммарное расстояние между фокусами через любую вершину 
  //! контура (длина больей оси) 
  double lenMax(2. * majorRadius);
  
  //! Строим ортодромию от второго фокуса до первого
  Orthodromy ortodromy = createOrthodromy(secondFocus, firstFocus);

  //! Если макcимальное расстояние не превосходит расстояние между фокусными 
  //! точками, то контур не строим
  if (lenMax <= ortodromy.getLength())
    throw GeodesicFailure(GeodesicEnumFailure::EllipseDoNotExist);

  //! Вычисляем начальное расстояние от первого фокуса до вершины 
  //! (по остаточному принципу)
  double len((lenMax - ortodromy.getLength()) / 2.);

  //! Инициализируем переменные для поиска вершин
  Radian azimuth(0.); //!< угол перехода к следующей вершине
  GeoPoint2D vertex;  //!< координаты вершины
  double lenSum;      //!< суммарное расстояние между фокусами через вершину

  //! Собираем контур пока не пройдем круг (2 * Pi)  
  do 
  {
    //! Поиск координат вершины до достижения требуемой точности
    do 
    {
      //! Находим уточнённое положение вершины относительно первого фокуса
      vertex = pointPosition(firstFocus, 
                             Radian(ortodromy.getAzimuthAtFinish() + azimuth),
                             len);

      //! Суммарное расстояние между фокусами через вершину
      lenSum = len + distance(vertex, secondFocus);

      //! Корректируем расстояние от первого фокуса
      len *= lenMax / lenSum;

    } while (fabs(lenSum - lenMax) > accuracy);
    
    //! Переходим к следующему азимуту
    azimuth += Radian(segmentLength / len);

    //! Добавляем вершину в контур
    result.outer().emplace_back(vertex);

  } while (azimuth < Radian(2 * M_PI));

  //! Корректируем контур (замыкание и направление обхода)
  AbstractOperations::correct(result);

  //! Возвращаем результат
  return result;
}
//==============================================================================
GeoPolygon2D OrthodromyTools::circleRectangle(const GeoPoint2D& center, 
                                              const double radius, 
                                              const double buffer /*= 0.*/)
{
  /* TODO Требуется несколько переработать функцию или создать рядом продвинутый аналог
  Текущие проблемы:
  * Заужает коробку по долготам. Чем круг располагается ближе к полюсам, 
    тем больше и заметнее искажение. Для круга радиусом 100 км на широте 70 градусов
    требуется дополнительно 90 метров буфера, чтобы влезть в зауженную коробку целиком
  * Не может растягиваться в -180, 180 при накрытии окружностью полюса  
  Еще есть филослфский вопрос, не следует ли возвращать box, который потом гарантированно легко 
  конвертируетя в полигон
  */

  //! Если радиус не превышает 0, то такая окружность существовать не может
  if (radius <= 0.)
    throw GeodesicFailure(CircleDoNotExist);

  //! Если величина дополнительного отступа отрицательна, готовим исключение
  if (buffer < 0.)
    throw GeodesicFailure(NegativeDistanceLikeValue);

  double fullRadius(radius + buffer);

  //! Максимальная широта
  GeoPoint2D upPoint = pointPosition(center, Radian(0.), fullRadius);
  //! Максимальная долгота
  GeoPoint2D rightPoint = pointPosition(center, Radian(M_PI_2), fullRadius);
  //! Минимальная широта
  GeoPoint2D downPoint = pointPosition(center, Radian(M_PI), fullRadius);
  //! Минимальная долгота
  GeoPoint2D leftPoint = pointPosition(center, Radian(3 * M_PI_2), fullRadius);

  //! Собираем полигон
  GeoPolygon2D result = { { GeoPoint2D(upPoint.getLatitude(), 
                                       leftPoint.getLongitude()),
                            GeoPoint2D(upPoint.getLatitude(), 
                                       rightPoint.getLongitude()),
                            GeoPoint2D(downPoint.getLatitude(),
                                       rightPoint.getLongitude()), 
                            GeoPoint2D(downPoint.getLatitude(),
                                       leftPoint.getLongitude()) } };
  result.outer().push_back(result.outer().at(0));

  //! Возвращаем результат
  return result;
}
//==============================================================================
GeoPolygon2D OrthodromyTools::
ellipseRectangle(const GeoPoint2D& firstFocus,
                 const GeoPoint2D& secondFocus,
                 const double majorRadius,
                 const double buffer /*= 0.*/,
                 const double accuracy /*= 0.1*/)
{
  //! Если точки фокуса совпадают, то это уже окружность
  if (firstFocus == secondFocus)
    return circleRectangle(firstFocus, majorRadius, buffer);

  //! Если точность неположительна, экстренно покидаем тело функции.
  if (accuracy <= ACCURACY_DOUBLE)
    throw GeodesicFailure(GeodesicEnumFailure::NonPositiveAccuracy);

  /*!
   * Если величина дополнительного отступа отрицательна,
   * также экстренно завершаем работу.
   */
  if (buffer < 0.)
    throw GeodesicFailure(GeodesicEnumFailure::NegativeDistanceLikeValue);

  //!< Удвоенная длина большой полуоси эллипса.
  const double doubleMajorRadius(majorRadius * 2.);

  //! Поиск длины малой полуоси эллипса.

  //! Поиск точек, которые являются краями большой оси эллипса.

  //!< Ортодромия, проходящая через фокусы эллипса.
  const Orthodromy majorAxis(createOrthodromy(firstFocus, secondFocus));
  //!< Фокусное расстояние эллипса.
  const double     focusDistance(majorAxis.getLength());

  /*!
   * Если фокусное расстояние равно удвоенной длине большой полуоси,
   * эллипс вырождается в отрезок.
   * Если фокусное расстояние больше удвоенной длины большой полуоси,
   * действительного эллипса не существует.
   * Обе эти ситуации мы считаем исключительными.
   */
  if (doubleMajorRadius <= focusDistance)
    throw GeodesicFailure(GeodesicEnumFailure::EllipseDoNotExist);


  /*!<
   * Азимут края большой оси эллипса,
   * получаемого при перемещении по ортодромии,
   * проведённой через фокусы эллипса,
   * в положительном направлении.
   */
  Radian majorAxisPositiveEdgeAzimuth;
  /*!<
   * Край большой оси эллипса,
   * достигающийся при перемещении по ортодромии,
   * проведённой через фокусы эллипса,
   * в положительном направлении.
   */
  const GeoPoint2D majorAxisPositiveEdge(
    majorAxis.pointPosition(
      (doubleMajorRadius + focusDistance) / 2.,
      majorAxisPositiveEdgeAzimuth));

  /*!<
   * Азимут края большой оси эллипса,
   * получаемого при перемещении по ортодромии,
   * проведённой через фокусы эллипса,
   * в отрицательном направлении.
   */
  Radian majorAxisNegativeEdgeAzimuth;
  /*!<
   * Край большой оси эллипса,
   * достигающийся при перемещении по ортодромии,
   * проведённой через фокусы эллипса,
   * в отрицательном направлении.
   */
  const GeoPoint2D majorAxisNegativeEdge(
    majorAxis.pointPosition(
      (focusDistance - doubleMajorRadius) / 2.,
      majorAxisNegativeEdgeAzimuth));

  //! Поиск точек, которые являются краями малой оси эллипса.
  /*!
   * TODO: Найти обоснование корректности этого кода
   * и добавить описания ко всем неописанным переменным.
   */

  const Orthodromy lineAD(
    createOrthodromy(
      majorAxisNegativeEdge,
      majorAxisNegativeEdgeAzimuth + Degree(270.).toRadians()));

  const Orthodromy lineBC(
    createOrthodromy(
      majorAxisPositiveEdge,
      majorAxisPositiveEdgeAzimuth - Degree(90.).toRadians()));

  double h1(
    std::sqrt(
      doubleMajorRadius * doubleMajorRadius - focusDistance * focusDistance)
    / 2.);
  double m(0.);
  double k(1.);
  while (std::fabs(m - doubleMajorRadius / 2.) > accuracy)
  {
    if (m != 0.)
    {
      k = (doubleMajorRadius / 2.) / m;
      h1 *= k;
    }

    const GeoPoint2D a(lineAD.pointPosition(h1));
    const GeoPoint2D b(lineBC.pointPosition(h1));

    const Orthodromy lineAB(createOrthodromy(a, b));
    const double     L_AB(lineAB.getLength());

    const GeoPoint2D centerAB(lineAB.pointPosition(L_AB / 2.0));

    m = createOrthodromy(firstFocus, centerAB).getLength();
  }

  double h2(h1);
  m = 0.;
  k = 1.;
  while (std::fabs(m - doubleMajorRadius / 2.0) > accuracy)
  {
    if (m != 0)
    {
      k = (doubleMajorRadius / 2.) / m;
      h2 *= k;
    }

    const GeoPoint2D c(lineBC.pointPosition(-h2));
    const GeoPoint2D d(lineAD.pointPosition(-h2));

    const Orthodromy lineDC(createOrthodromy(d, c));
    const double L_DC(lineDC.getLength());

    const GeoPoint2D centerDC = lineDC.pointPosition(L_DC / 2.);

    m = createOrthodromy(firstFocus, centerDC).getLength();
  }

  //!< Длина малой полуоси эллипса (в метрах).
  const double ellpB((h1 + h2) / 2.);
  GeoPolygon2D result;   //!< Искомый прямоугольник.
  result.outer().resize(5);
  result.outer()[0] = lineAD.pointPosition(ellpB + buffer);
  result.outer()[1] = lineBC.pointPosition(ellpB + buffer);
  result.outer()[2] = lineBC.pointPosition(-(ellpB + buffer));
  result.outer()[3] = lineAD.pointPosition(-(ellpB + buffer));
  result.outer()[4] = result.outer()[0];
  return result;
}
//==============================================================================
GeoMultiPoint2D
OrthodromyTools::collapsePoints(
  const GeoMultiPoint2D& points,
  const double minimalDistance) const noexcept
{
  GeoMultiPoint2D result;

  std::size_t pointsSize(points.size());
  std::size_t resultSize(0);
  for (std::size_t i(0); i < pointsSize; ++i)
  {
    bool isNeedToRemain(true);
    for (std::size_t j(0); j < resultSize; ++j)
    {
      if (points[i] == result[j])
      {
        isNeedToRemain = false;
        break;
      }
      else if (
        createOrthodromy(points[i], result[j]).getLength() < minimalDistance)
      {
        isNeedToRemain = false;
        break;
      }
    }
    if (isNeedToRemain)
    {
      result.emplace_back(points[i]);
      ++resultSize;
    }
  }

  return result;
}
//==============================================================================
double OrthodromyTools::getMajorRadius() const
{
  return mGeodesic->MajorRadius();
}
//==============================================================================
double OrthodromyTools::getFlatenning() const
{
  return mGeodesic->Flattening();
}
//==============================================================================
std::shared_ptr<Inner::GeodesicLine> 
OrthodromyTools::createLine(const GeoPoint2D& startPoint,
                            const GeoPoint2D& finishPoint) const
{
  return std::make_shared<Orthodromy>(createOrthodromy(startPoint, 
                                                       finishPoint));
}
//==============================================================================
} //! Geodesic
//==============================================================================
