#include "OrthographicInner.hpp"
//==============================================================================
//! Helper
#include "Helper/EllipsoidInner.hpp"
//! Transition
#include <Geodesic/GeodesicCartesianTransition/Transition.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! Geometry
#include <Geometry/Objects/Basis3D.hpp>
#include <Geometry/Operations/AtVectors.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
OrthographicInner::OrthographicInner( 
  const std::shared_ptr<Ellipsoid>& ellipsoid 
                                    /*= std::make_shared<Ellipsoid>(EL_WGS_84)*/,
  const GeoPoint3D& center /*= GeoPoint3D(Radian(0.), Radian(0.), 0.f)*/, 
  const Radian& azimuth /*= Radian(0.)*/)
: mEllipsoid(ellipsoid)
, mTransition(nullptr)
, mBasis(std::make_shared<Geometry::Basis3D>())
, mCenter(center)
, mAzimuth(azimuth)
{
  //! Если нет эллипсоида, то собираем эллипсоид WGS-84
  if (!mEllipsoid)
    mEllipsoid = std::make_shared<Ellipsoid>(EL_WGS_84);
      
  //! Собираем перепроецировщик
  mTransition = std::make_shared<Transition>(mEllipsoid);

  //! Инициализируем параметры проекции, заданием точки проекции
  setProjectionPoint(mCenter, mAzimuth);
}
//==============================================================================
void OrthographicInner::setProjectionPoint(
  const GeoPoint3D center, const Radian azimuth /*= Radian(0.)*/)
{
  using namespace BasicMath;
  using namespace Geometry;

  //! Орт k направляем от начала координат глобального базиса к центру проекции.
  //! Саму плоскость проекции располагаем перпендикулярно полученному орту k (не 
  //! касательно к поверхности).
  //! Плоскость проекции проходит через начало координат глобального базиса
  //! Проецирующие лучи параллельны орту k.
  //! Орты i, j соответствуют плоским координатам x, y в проекции с учетом ее 
  //! азимута.
  //! Локальный базис всегда правый.

  mCenter = center;
  mAzimuth = azimuth;

  //! Собираем локальный базис со следующими условиями:
  //! - базис должен быть правым
  //! - орт k должен быть направлен из центра эллипсоида к центру проекции

  //! Собираем орт k локального базиса
  Vector3D vK(mTransition->fromGeodesicToCartesian(mCenter));
  vK.normalize();

  //! Орт z глобального базиса
  Vector3D vZ(0., 0., 1.);

  //! Собираем орты i и j локального базиса
  Vector3D vI;
  Vector3D vJ;
  if (isNull(vK.getOX()) && isNull(vK.getOY()))
  {
    //! Если центр в (0, 0), то используем орты глобального базиса
    //! Центр проекции расположен на одном из полюсов (z и k коллинеарны)
    vI = Vector3D(1., 0., 0.);
    vJ = Vector3D(0., 1., 0.);

    //! Определяем направление орта j
    if (scalarProduct(vK, vZ) < 0)
      vJ *= -1.;
  }
  else
  {
    //! Если центр не в (0, 0), то собираем через векторное произведение
    vI = crossProduct(vZ, vK);
    vJ = crossProduct(vK, vI);
    vI.normalize();
    vJ.normalize();
  }

  if (!isNull(mAzimuth))
  {
    //! Если задан ненулевой азимут (азимут оси OY СК проекции не направлен на 
    //! север), то дополнительно поворачиваем оси (орты i и j)
    //! (Задаём направление оси OY для проекции с центорм на полюсе:
    //!  +90° - для северного полюса; -90° - для южного полюса)
    vI.rotateCorkscrew(vK, -mAzimuth);
    vJ = crossProduct(vK, vI);
  }

  //! Обновляем базис
  mBasis->set(vI, vJ, vK, Point3D());

  //! Вычисляем вспомогательные параметры проекции для перевода в 
  //! геодезические координаты
  if (!isNull(vK.getOZ()))
  {
    //! Если есть проекция орта K на ось OZ, то вычисляем основные
    mTgZKxz = vK.getOX() / vK.getOZ();
    mTgZKxy = vK.getOY() / vK.getOZ();
    mTgZK = mTgZKxz * mTgZKxz + mTgZKxy * mTgZKxy 
          + mEllipsoid->mParams->aaDEVbb;
  }
  else if (!isNull(vK.getOX()))
  {
    //! Если есть проекция орта K на ось OX, то вычисляем резервные
    mTgYX = vK.getOY() / vK.getOX();
    mCosYX = 1. + mTgYX * mTgYX;
  }
}
//==============================================================================
bool OrthographicInner::setEllipsoid(const std::shared_ptr<Ellipsoid>& ellipsoid)
{
  if (!ellipsoid)  
    return false; //!< Нет эллипсоида - не перестраиваем

  mEllipsoid = ellipsoid;

  //! Меняем перепроецировщих
  mTransition.reset();
  mTransition = std::make_shared<Transition>(mEllipsoid);

  //! Инициализируем параметры проекции, заданием точки проекции
  setProjectionPoint(mCenter, mAzimuth);

  //! Перестроение проекции завершено
  return true;
}
//==============================================================================
Point3D OrthographicInner::toProjection(const GeoPoint3D& geoPoint) const
{
  //! Преобразуем геодезические координаты в декартовы глобального базиса.
  //! Переводим точку из глобального базиса в локальный.
  //! ( B, L, H -> X, Y, Z -> i, j, k )
  return mBasis->fromGlobal(mTransition->fromGeodesicToCartesian(geoPoint));
}
//==============================================================================
bool OrthographicInner::fromProjection(const Point3D& point, 
                                       GeoPoint3D& visibleGeoPoint, 
                                       GeoPoint3D& invisibleGeoPoint,
                                       const bool hasOnlyVisible /*= true*/)
const
{
  using namespace BasicMath;

  bool result(false); //!< Инициализация флага успешности перевода

  Point3D globalPoint = mBasis->toGlobal(point);

  //! Пусть (.)B - точка на проекции
  //!       (.)C - точка на поверхности эллипсоида, которую ищем

  if (!isNull(mBasis->getK().getOZ()))
  {
    //! Если есть проекция орта k локального базиса на ось OZ
    //! Орт k не лежит в плоскости экватора.
    //! (.)M - точка пересечения (ВС) и плоскости экватора OXY, zM = 0

    //! (.)C лежит на эллипсоиде => x^2 / a^2 + y^2 / a^2 + z^2 / b^2 = 1
    //! (.)C лежит на прямой (BM) => 
    //! (z - zM) / (zB - zM) = (x - xM) / (xB - xM) = (y - yM) / (yB - yM)
    //! Если все подставить, то для z получаем квадратное уравнение:
    //! z^2 + 2 * b * z + c = 0
    //! Формула дискримината для данного уравнения:
    //! discr = 4 * b^2 - 4 * c 
    //! Аналогично формула для вычисления корней уравнения: -b +/- sqrt(discr4)

    //! Координаты точки M в глобальном базисе
    double xM(globalPoint.getX() - mTgZKxz * globalPoint.getZ());
    double yM(globalPoint.getY() - mTgZKxy * globalPoint.getZ());

    //! Ищем коэффициены b и c для квадратного уравнения
    double b((mTgZKxz * xM + mTgZKxy * yM) / mTgZK);
    double c((xM * xM + yM * yM - mEllipsoid->mParams->aa) / mTgZK);

    //! Ищем дискриминант квадратного уравнения делёный на 4
    double discr4(b * b - c);

    if (discr4 >= 0.)
    {
      //! Решение есть только при неотрицательном дискриминанте
      result = true;

      //! Ищем квадратный корень для дискриминанта 
      double sqrtDiscr(sqrt(discr4));

      //! Используем первый корень уравнения для поиска искомой точки в 
      //! глобальном базисе
      double zCurPoint(-b - sqrtDiscr);         //!< z координата искомой точки
      Point3D curPoint(mTgZKxz * zCurPoint + xM,//!< x координата искомой точки
                       mTgZKxy * zCurPoint + yM,//!< y координата искомой точки
                       zCurPoint);

      //! Определение видимой точки проекции 
      if (mBasis->fromGlobal(curPoint).getZ() > 0)
      {
        //! Если координата z в локальном базисе положительная, то точка видимая
        visibleGeoPoint = mTransition->fromCartesianToGeodesic(curPoint);
        
        if (!hasOnlyVisible)
        {
          //! Вычисление невидимой точки через второй корень
          zCurPoint = -b + sqrtDiscr;
          curPoint.set(mTgZKxz * zCurPoint + xM, 
                       mTgZKxy * zCurPoint + yM, 
                       zCurPoint);
          invisibleGeoPoint = mTransition->fromCartesianToGeodesic(curPoint);
        }
      }
      else
      {
        //! Иначе - точка невидимая
        if (!hasOnlyVisible)
          invisibleGeoPoint = mTransition->fromCartesianToGeodesic(curPoint);
        
        //! Вычисление видимой точки через второй корень
        zCurPoint = -b + sqrtDiscr;
        curPoint.set(mTgZKxz * zCurPoint + xM,
                     mTgZKxy * zCurPoint + yM,
                     zCurPoint);
        visibleGeoPoint = mTransition->fromCartesianToGeodesic(curPoint);
      }
    }
  }
  else if (!isNull(mBasis->getK().getOX()))
  {
    //! Если есть проекция орта k локального базиса на ось OX
    //! Орт k лежит в плоскости экватора, но долгота не +/-(Pi / 2).
    //! (.)M - точка пересечения (ВС) и плоскости OZY, xM = 0
    //! Ищем точку, лежащую на эллипсоиде, а также в плоскости z = zM и 
    //! на прямой (BM) 

    //! Координаты точки M в глобальном базисе
    double yM(globalPoint.getY() - mTgYX * globalPoint.getX());
    double zM(globalPoint.getZ());

    //! Ищем коэффициены b и c для квадратного уравнения
    double b(mTgYX * yM / mCosYX);
    double c((yM * yM + zM * zM * mEllipsoid->mParams->aaDEVbb 
              - mEllipsoid->mParams->aa) / mCosYX);

    //! Ищем дискриминант квадратного уравнения делёный на 4
    double discr4(b * b - c);

    if (discr4 >= 0.)
    {
      //! Решение есть только при положительном дискриминанте
      result = true;

      //! Ищем квадратный корень для дискриминанта 
      double sqrtDiscr(sqrt(discr4));

      //! Используем первый корень уравнения для поиска искомой точки в 
      //! глобальном базисе
      double xCurPoint(-b - sqrtDiscr);       //!< x координата искомой точки  
      Point3D curPoint(xCurPoint,             
                       mTgYX * xCurPoint + yM,//!< y координата искомой точки
                       zM);                   //!< z координата искомой точки
      
      //! Определение видимой точки проекции 
      if (mBasis->fromGlobal(curPoint).getZ() > 0)
      {
        //! Если координата z в локальном базисе положительная, то точка видимая
        visibleGeoPoint = mTransition->fromCartesianToGeodesic(curPoint);

        if (!hasOnlyVisible)
        {
          //! Вычисление невидимой точки через второй корень
          xCurPoint = -b + sqrtDiscr;
          curPoint.set(xCurPoint, mTgYX * xCurPoint + yM, zM);
          invisibleGeoPoint = mTransition->fromCartesianToGeodesic(curPoint);
        }
      }
      else
      {
        //! Иначе - точка невидимая
        if (!hasOnlyVisible)
          invisibleGeoPoint = mTransition->fromCartesianToGeodesic(curPoint);

        //! Вычисление видимой точки через второй корень
        xCurPoint = -b + sqrtDiscr;
        curPoint.set(xCurPoint, mTgYX * xCurPoint + yM, zM);
        visibleGeoPoint = mTransition->fromCartesianToGeodesic(curPoint);
      }
    }
  }
  else if (!isNull(mBasis->getK().getOY()))
  {
    //! Если есть проекция орта k локального базиса на ось OY
    //! Орт k лежит в плоскости экватора и долгота +/-(Pi / 2).
    //! Искомая точка лежит на эллипсоиде, на высоте z.
    //! Сечение эллипсоида на этой высоте - окружность, радиуса R, где
    //! R = a^2 - a^2 / b^2 * z^2
    //! При известном x для окружности y = +/- sqrt ( R * R - x * x ).
    //! При этом положительные y соответствуют видимым точкам, 
    //! а отрицательные - невидимым

    //! Координаты точки M в глобальном базисе
    double xM(globalPoint.getX());
    double zM(globalPoint.getZ());

    //! Ищем дискриминант квадратного уравнения делёный на 4
    double discr4(mEllipsoid->mParams->aa 
                  - mEllipsoid->mParams->aaDEVbb * zM * zM - xM * xM);

    if (discr4 >= 0.)
    {
      //! Решение есть только при положительном дискриминанте
      result = true;
      
      double yCurPoint(sqrt(discr4)); //!< y координата искомой точки  
      Point3D curPoint(xM,            //!< x координата искомой точки
                       yCurPoint, 
                       zM);           //!< z координата искомой точки
    
      //! Определение видимой точки проекции 
      if (mBasis->fromGlobal(curPoint).getZ() > 0)
      {
        //! Если координата z в локальном базисе положительная, то точка видимая
        visibleGeoPoint = mTransition->fromCartesianToGeodesic(curPoint);

        if (!hasOnlyVisible)
        {
          //! Вычисление невидимой точки через второй корень
          curPoint.set(xM, -yCurPoint, zM);
          invisibleGeoPoint = mTransition->fromCartesianToGeodesic(curPoint);
        }
      }
      else
      {
        //! Иначе - точка невидимая
        if (!hasOnlyVisible)
          invisibleGeoPoint = mTransition->fromCartesianToGeodesic(curPoint);

        //! Вычисление видимой точки через второй корень
        curPoint.set(xM, -yCurPoint, zM);
        visibleGeoPoint = mTransition->fromCartesianToGeodesic(curPoint);
      }
    }
  }

  //! Возвращаем флаг уcпешности перевода
  //! Если была подана точка за пределами проекции эллипсоида, то вернётся false
  return result;
}
//==============================================================================
} //! Geodesic
//==============================================================================
