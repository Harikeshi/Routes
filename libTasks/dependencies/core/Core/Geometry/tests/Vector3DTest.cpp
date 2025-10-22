#include <Geometry/Objects/Vector3D.hpp>

#include <Geometry/GeometryException.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
/*!
* \brief Тестирование конструктора класса Vector3D
*        по проекциям на оси и точке привязки
*/
TEST( Vector3DTest, Constructor )
{
  using namespace BasicMath;

  Vector3D vector;
  ASSERT_TRUE( isEqual( 0., vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( -1., 2.5, -4., Point3D( 3., -17.8, 9.1 ) );
  ASSERT_TRUE( isEqual( -1., vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 2.5, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( -4., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( 3., -17.8, 9.1 ), vector.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование конструктора класса Vector3D
*        по двум точкам
*/
TEST( Vector3DTest, ConstructorByPoints )
{
  using namespace BasicMath;

  Vector3D vector( Point3D( ), Point3D( 1.1, -2., 4.5 ) );
  ASSERT_TRUE( isEqual( 1.1, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -2., vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 4.5, vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  ASSERT_TRUE( isEqual( 3.5, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -6.3, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 8.2, vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), vector.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование конструктора класса Vector3D
*        от начала координат до точки
*/
TEST( Vector3DTest, ConstructorByPoint )
{
  using namespace BasicMath;

  Vector3D vector( Point3D( 1.1, -2., 4.5 ) );
  ASSERT_TRUE( isEqual( 1.1, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -2., vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 4.5, vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( -2.4, 4.3, -3.7 ) );
  ASSERT_TRUE( isEqual( -2.4, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 4.3, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( -3.7, vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование конструктора класса Vector3D
*        копирующий конструктор
*/
TEST( Vector3DTest, ConstructorCopied )
{
  using namespace BasicMath;

  Vector3D vector1( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  Vector3D vector2( vector1 );
  ASSERT_TRUE( isEqual( 3.5, vector2.getOX( ) ) );
  ASSERT_TRUE( isEqual( -6.3, vector2.getOY( ) ) );
  ASSERT_TRUE( isEqual( 8.2, vector2.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), vector2.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование lenght
*/
TEST( Vector3DTest, LenghtVector3D )
{
  using namespace BasicMath;

  Vector3D vector;
  ASSERT_TRUE( isEqual( 0., vector.length( ) ) );

  vector = Vector3D( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  ASSERT_TRUE( isEqual( sqrt( 3.5 * 3.5 + 6.3 * 6.3 + 8.2 * 8.2 ),
    vector.length( ) ) );
}
//==============================================================================
/*!
* \brief Тестирование /= (деление вектора на число)
*/
TEST( Vector3DTest, DivisionNumber )
{
  using namespace BasicMath;

  Vector3D vector;
  ASSERT_NO_THROW( vector /= 5. );
  ASSERT_TRUE( isEqual( 0., vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  ASSERT_THROW(vector /= 0., GeometryFailure);
  try
  {
    vector /= 0.;
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
    ASSERT_EQ( GeometryEnumFailure::DividingByZero, exception.getCode( ) );
  }

  ASSERT_NO_THROW( vector /= 5. );
  ASSERT_TRUE( isEqual( 0.7, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -1.26, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 1.64, vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), vector.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование operator / (деление вектора на число)
*/
TEST( Vector3DTest, OperatorDivisionNumber )
{
  using namespace BasicMath;

  Vector3D vector1;
  Vector3D vector2 = vector1 / 5.;
  ASSERT_TRUE( isEqual( 0., vector2.getOX( ) ) );
  ASSERT_TRUE( isEqual( 0., vector2.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector2.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector2.getAnchor( ) );

  vector1 = Vector3D( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  ASSERT_THROW(vector1 / 0., GeometryFailure);
  try
  {
    vector2 = vector1 / 0.;
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( GeometryEnumFailure::DividingByZero, exception.getCode( ) );
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
    ASSERT_EQ(
      "[Core::Geometry] <DividingByZero>: Попытка поделить вектор на ноль",
      std::string( exception.what( ) ) );
  }

  vector2 = vector1 / 5.;
  ASSERT_TRUE( isEqual( 0.7, vector2.getOX( ) ) );
  ASSERT_TRUE( isEqual( -1.26, vector2.getOY( ) ) );
  ASSERT_TRUE( isEqual( 1.64, vector2.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), vector2.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование *= (умножение вектора на число)
*/
TEST( Vector3DTest, MultiplicationNumber )
{
  using namespace BasicMath;

  Vector3D vector;
  vector *= 5.;
  ASSERT_TRUE( isEqual( 0., vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  vector *= 5.;
  ASSERT_TRUE( isEqual( 17.5, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -31.5, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 41., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), vector.getAnchor( ) );

  vector *= 0.;
  ASSERT_TRUE( isEqual( 0., vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), vector.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование operator * (умножение вектора на число)
*/
TEST( Vector3DTest, OperatorMultiplicationNumber )
{
  using namespace BasicMath;

  Vector3D vector1;
  Vector3D vector2 = vector1 * 5.;
  ASSERT_TRUE( isEqual( 0., vector2.getOX( ) ) );
  ASSERT_TRUE( isEqual( 0., vector2.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector2.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector2.getAnchor( ) );

  vector1 = Vector3D( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  vector2 = vector1 * 5.;
  ASSERT_TRUE( isEqual( 17.5, vector2.getOX( ) ) );
  ASSERT_TRUE( isEqual( -31.5, vector2.getOY( ) ) );
  ASSERT_TRUE( isEqual( 41., vector2.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), vector2.getAnchor( ) );

  vector2 = vector1 * 0.;
  ASSERT_TRUE( isEqual( 0., vector2.getOX( ) ) );
  ASSERT_TRUE( isEqual( 0., vector2.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector2.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), vector2.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование normalize
*        Приведение вектора к единичному
*/
TEST( Vector3DTest, NormalizeVector3D )
{
  using namespace BasicMath;

  Vector3D vector;
  ASSERT_THROW(vector.normalize(), GeometryFailure);
  try
  {
    vector.normalize( );
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
    ASSERT_EQ(
      GeometryEnumFailure::NullVectorIncorrectOperation,
      exception.getCode( ) );
  }

  vector = Vector3D( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  vector.normalize( );
  double lenght( sqrt( 3.5 * 3.5 + 6.3 * 6.3 + 8.2 * 8.2 ) );
  ASSERT_TRUE( isEqual( 3.5 / lenght, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -6.3 / lenght, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 8.2 / lenght, vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), vector.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование unit
*        Получение единичного вектора из текущего
*/
TEST( Vector3DTest, UnitVector3D )
{
  using namespace BasicMath;

  Vector3D vector;
  Vector3D unitVector;

  ASSERT_THROW(vector.unit(), GeometryFailure);
  try
  {
    unitVector = vector.unit( );
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( GeometryEnumFailure::DividingByZero, exception.getCode( ) );
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
    ASSERT_EQ(
      "[Core::Geometry] <DividingByZero>: Попытка поделить вектор на ноль",
      std::string( exception.what( ) ) );
  }

  vector = Vector3D( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  unitVector = vector.unit( );
  ASSERT_TRUE( isEqual( 3.5, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -6.3, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 8.2, vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), vector.getAnchor( ) );
  double lenght( sqrt( 3.5 * 3.5 + 6.3 * 6.3 + 8.2 * 8.2 ) );
  ASSERT_TRUE( isEqual( 3.5 / lenght, unitVector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -6.3 / lenght, unitVector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 8.2 / lenght, unitVector.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), unitVector.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование operator & для векторов
*        Скалярное произведение векторов
*/
TEST( Vector3DTest, ScalarProduct )
{
  using namespace BasicMath;

  Vector3D vector1;
  Vector3D vector2( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  ASSERT_TRUE( isEqual( 0., vector1 & vector2 ) );

  vector1 = Vector3D( Point3D( -1., 2.5, -4. ), Point3D( 3., -17.8, 9.1 ) );
  ASSERT_TRUE( isEqual( 4. * 3.5 + 20.3 * 6.3 + 13.1 * 8.2, vector1 & vector2 ) );
}
//==============================================================================
/*!
* \brief Тестирование operator * для векторов
*        Векторное произведение векторов
*/
TEST( Vector3DTest, CrossProduct )
{
  using namespace BasicMath;

  Vector3D vector1;
  Vector3D vector2( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  Vector3D vector3 = vector1 * vector2;
  ASSERT_TRUE( isEqual( 0., vector3.getOX( ) ) );
  ASSERT_TRUE( isEqual( 0., vector3.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector3.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector3.getAnchor( ) );

  vector1 = Vector3D( Point3D( -1., 2.5, -4. ), Point3D( 3., -17.8, 9.1 ) );
  vector3 = vector1 * vector2;
  ASSERT_TRUE( isEqual( -20.3 * 8.2 + 13.1 * 6.3, vector3.getOX( ) ) );
  ASSERT_TRUE( isEqual( 13.1 * 3.5 - 4. * 8.2, vector3.getOY( ) ) );
  ASSERT_TRUE( isEqual( 4 * -6.3 + 20.3 * 3.5, vector3.getOZ( ) ) );
  ASSERT_EQ( Point3D( -1., 2.5, -4. ), vector3.getAnchor( ) );

  vector3 = vector2 * vector1;
  ASSERT_TRUE( isEqual( -6.3 * 13.1 + 8.2 * 20.3, vector3.getOX( ) ) );
  ASSERT_TRUE( isEqual( 8.2 * 4. - 3.5 * 13.1, vector3.getOY( ) ) );
  ASSERT_TRUE( isEqual( 3.5 * -20.3 + 6.3 * 4., vector3.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), vector3.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование cosAtVector
*        Проверка нахождения косинуса угла
*/
TEST( Vector3DTest, CosAngle )
{
  using namespace BasicMath;

  Vector3D vector1 = Vector3D( Point3D( 0., 1., 1. ) );
  Vector3D vector2 = Vector3D( Point3D( 2., 2., 0. ) );
  ASSERT_TRUE( isEqual( 0.5, vector1.cosAtVector( vector2 ) ) );

  vector1 = Vector3D( Point3D( -1., 1., 1. ) );
  vector2 = Vector3D( Point3D( 2., 2., 0. ) );
  ASSERT_TRUE( isEqual( 0., vector1.cosAtVector( vector2 ) ) );

  vector1 = Vector3D( Point3D( 0., 0., 0. ) );
  vector2 = Vector3D( Point3D( 2., 2., 0. ) );
  ASSERT_TRUE( isEqual( 1., vector1.cosAtVector( vector2 ) ) );
}
//==============================================================================
/*!
* \brief Тестирование isParallel
*        Проверка параллельности векторов
*/
TEST( Vector3DTest, IsParallelVectors )
{
  Vector3D vector1;
  Vector3D vector2;
  ASSERT_THROW(vector1.isParallel(vector2), GeometryFailure);
  try
  {
    vector1.isParallel( vector2 );
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ(
      GeometryEnumFailure::NullVectorIncorrectOperation,
      exception.getCode( ) );
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
    ASSERT_EQ(
      "[Core::Geometry] <NullVectorIncorrectOperation>: "
      "Попытка провести с нулевым вектором операцию, "
      "которая для него не определена",
      std::string( exception.what( ) ) );
  }

  vector1 = Vector3D( Point3D( 3., 3., 3. ), Point3D( ) );
  vector2 = Vector3D( Point3D( 4., 5., 6. ), Point3D( 1., 2., 3. ) );
  ASSERT_TRUE( vector1.isParallel( vector2 ) );

  vector1 = Vector3D( Point3D( -3., -3., -3. ), Point3D( ) );
  ASSERT_TRUE( vector1.isParallel( vector2 ) );
}
//==============================================================================
/*!
* \brief Тестирование destination
*        Точка на конце вектора
*/
TEST( Vector3DTest, Destination )
{
  Vector3D vector;
  ASSERT_EQ( Point3D( ), vector.destination( ) );

  vector = Vector3D( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  ASSERT_EQ( Point3D( 1.1, -2., 4.5 ), vector.destination( ) );

  vector = Vector3D( -1., 2.5, -4., Point3D( 3., -17.8, 9.1 ) );
  ASSERT_EQ( Point3D( 2., -15.3, 5.1 ), vector.destination( ) );
}
//==============================================================================
/*!
* \brief Тестирование operator +
*        Сложение векторов
*/
TEST( Vector3DTest, AdditionVectors )
{
  using namespace BasicMath;

  Vector3D vector1;
  Vector3D vector2( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  Vector3D vector3 = vector1 + vector2;
  ASSERT_TRUE( isEqual( 3.5, vector3.getOX( ) ) );
  ASSERT_TRUE( isEqual( -6.3, vector3.getOY( ) ) );
  ASSERT_TRUE( isEqual( 8.2, vector3.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector3.getAnchor( ) );

  vector3 = vector2 + vector1;
  ASSERT_TRUE( isEqual( 3.5, vector3.getOX( ) ) );
  ASSERT_TRUE( isEqual( -6.3, vector3.getOY( ) ) );
  ASSERT_TRUE( isEqual( 8.2, vector3.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), vector3.getAnchor( ) );

  vector1 = Vector3D( Point3D( ), Point3D( 1.1, -2., 4.5 ) );
  vector3 = vector1 + vector2;
  ASSERT_TRUE( isEqual( 4.6, vector3.getOX( ) ) );
  ASSERT_TRUE( isEqual( -8.3, vector3.getOY( ) ) );
  ASSERT_TRUE( isEqual( 12.7, vector3.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector3.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование operator -
*        Вычитание векторов
*/
TEST( Vector3DTest, SubtractVectors )
{
  using namespace BasicMath;

  Vector3D vector1;
  Vector3D vector2( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  Vector3D vector3 = vector1 - vector2;
  ASSERT_TRUE( isEqual( -3.5, vector3.getOX( ) ) );
  ASSERT_TRUE( isEqual( 6.3, vector3.getOY( ) ) );
  ASSERT_TRUE( isEqual( -8.2, vector3.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector3.getAnchor( ) );

  vector3 = vector2 - vector1;
  ASSERT_TRUE( isEqual( 3.5, vector3.getOX( ) ) );
  ASSERT_TRUE( isEqual( -6.3, vector3.getOY( ) ) );
  ASSERT_TRUE( isEqual( 8.2, vector3.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), vector3.getAnchor( ) );

  vector1 = Vector3D( Point3D( ), Point3D( 1.1, -2., 4.5 ) );
  vector3 = vector1 - vector2;
  ASSERT_TRUE( isEqual( -2.4, vector3.getOX( ) ) );
  ASSERT_TRUE( isEqual( 4.3, vector3.getOY( ) ) );
  ASSERT_TRUE( isEqual( -3.7, vector3.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector3.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование rotateCorkscrewWithoutFulcrum
*        Вращение вектора вокруг оси без смены точки привязки
*/
TEST( Vector3DTest, RotateVectorWithoutFulcrum )
{
  using namespace BasicMath;

  Vector3D vector = Vector3D( Point3D( 5., 0., 0. ) );
  Vector3D axis = Vector3D( Point3D( 0., 0., 5. ) );
  vector.rotateCorkscrewWithoutFulcrum( axis, Degree( 112.5 ).toRadians( ) );
  ASSERT_TRUE( isEqual( -1.9134171618254485, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 4.6193976625564339, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( 5., 0., 0. ) );
  vector.rotateCorkscrewWithoutFulcrum( axis, Degree( -112.5 ).toRadians( ) );
  ASSERT_TRUE( isEqual( -1.9134171618254485, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -4.6193976625564339, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( 5., 0., 0. ) );
  vector.rotateCorkscrewWithoutFulcrum( axis, Degree( -247.5 ).toRadians( ) );
  ASSERT_TRUE( isEqual( -1.9134171618254485, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 4.6193976625564339, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( 5., 0., 0. ) );
  vector.rotateCorkscrewWithoutFulcrum( axis, Degree( 90. ).toRadians( ) );
  ASSERT_TRUE( isEqual( 0., vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 5., vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( 5., 0., 0. ) );
  vector.rotateCorkscrewWithoutFulcrum( axis, Degree( -90. ).toRadians( ) );
  ASSERT_TRUE( isEqual( 0., vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -5., vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( 5., 0., 0. ) );
  vector.rotateCorkscrewWithoutFulcrum( axis, Degree( 45. ).toRadians( ) );
  ASSERT_TRUE( isEqual( 3.5355339059327378, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 3.5355339059327378, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( 5., 0., 0. ) );
  vector.rotateCorkscrewWithoutFulcrum( axis, Degree( -45. ).toRadians( ) );
  ASSERT_TRUE( isEqual( 3.5355339059327378, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -3.5355339059327378, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( 5., 0., 0. ) );
  vector.rotateCorkscrewWithoutFulcrum( axis, Degree( 150. ).toRadians( ) );
  ASSERT_TRUE( isEqual( -4.3301270189221936, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 2.4999999999999991, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( 5., 0., 0. ) );
  vector.rotateCorkscrewWithoutFulcrum( axis, Degree( -150. ).toRadians( ) );
  ASSERT_TRUE( isEqual( -4.3301270189221936, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -2.4999999999999991, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( 5., -3., -7., Point3D( -2., 3., 8. ) );
  axis = Vector3D( -3., -5., 8., Point3D( 6., 8., -1. ) );
  vector.rotateCorkscrewWithoutFulcrum( axis, Degree( 112.5 ).toRadians( ) );
  ASSERT_TRUE( isEqual( 5.9631269313508639, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 6.8717669847563219, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( -0.46897303527072332, vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2., 3., 8. ), vector.getAnchor( ) );

  vector = Vector3D( 5., -3., -7., Point3D( -2., 3., 8. ) );
  vector.rotateCorkscrewWithoutFulcrum( axis, Degree( -112.5 ).toRadians( ) );
  ASSERT_TRUE( isEqual( -5.0493323440357392, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 3.3253817943775870, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( -6.8151360075274097, vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2., 3., 8. ), vector.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование rotateCorkscrew
*        Вращение вектора вокруг оси
*/
TEST( Vector3DTest, RotateVector )
{
  using namespace BasicMath;

  Vector3D vector = Vector3D( Point3D( 5., 0., 0. ) );
  Vector3D axis = Vector3D( Point3D( 0., 0., 5. ) );
  vector.rotateCorkscrew( axis, Degree( 90. ).toRadians( ) );
  ASSERT_TRUE( isEqual( 0., vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 5., vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( 5., 0., 0. ) );
  vector.rotateCorkscrew( axis, Degree( -90. ).toRadians( ) );
  ASSERT_TRUE( isEqual( 0., vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -5., vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( 5., 0., 0. ), Point3D( 5., 0., 5. ) );
  vector.rotateCorkscrew( axis, Degree( 90. ).toRadians( ) );
  ASSERT_TRUE( isEqual( 0., vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 5., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( 0., 5., 0. ), vector.getAnchor( ) );

  vector = Vector3D( Point3D( 5., 0., 0. ), Point3D( 5., 0., 5. ) );
  vector.rotateCorkscrew( axis, Degree( -90. ).toRadians( ) );
  ASSERT_TRUE( isEqual( 0., vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 0., vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 5., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( 0., -5., 0. ), vector.getAnchor( ) );

  vector = Vector3D( 5., -3., -7., Point3D( -2., 3., 8. ) );
  axis = Vector3D( -3., -5., 8., Point3D( 6., 8., -1. ) );
  vector.rotateCorkscrew( axis, Degree( 112.5 ).toRadians( ) );
  ASSERT_TRUE( isEqual( 5.9631269313508639, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 6.8717669847563219, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( -0.46897303527072332, vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( 3.4732655230105056,
    -2.0755959451527239,
    6.8802271054084869 ),
    vector.getAnchor( ) );

  vector = Vector3D( 5., -3., -7., Point3D( -2., 3., 8. ) );
  vector.rotateCorkscrew( axis, Degree( -112.5 ).toRadians( ) );
  ASSERT_TRUE( isEqual( -5.0493323440357392, vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( 3.3253817943775870, vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( -6.8151360075274097, vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( 4.4065247836364900,
    4.8305225834795529,
    11.546523408538405 ),
    vector.getAnchor( ) );
}
//==============================================================================
/*!
 * \brief Тестирование setAnchor
 *        Задание новой точки привязки
*/
TEST( Vector3DTest, SetVectorAnchor )
{
  using namespace BasicMath;

  Vector3D vector( Point3D( 1., -2., 3. ) );
  ASSERT_TRUE( isEqual( 1., vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -2., vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 3., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector.getAnchor( ) );

  vector.setAnchor( Point3D( -2., 3., -4. ) );
  ASSERT_TRUE( isEqual( 1., vector.getOX( ) ) );
  ASSERT_TRUE( isEqual( -2., vector.getOY( ) ) );
  ASSERT_TRUE( isEqual( 3., vector.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2., 3., -4. ), vector.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование operator ==
*        Покоординатное сравнение двух векторов
*/
TEST( Vector3DTest, VectorsEqual )
{
  Vector3D vector1( Point3D( 1., -2., 3 ) );
  Vector3D vector2( Point3D( 1., -2., 3 ) );
  ASSERT_TRUE( vector1 == vector2 );

  vector1 = Vector3D( Point3D( 1., 1., 1. ), Point3D( 2., -1., 4 ) );
  ASSERT_TRUE( vector1 == vector2 );

  vector1 = Vector3D( Point3D( 2., -1., 4 ), Point3D( 1., 1., 1. ) );
  ASSERT_FALSE( vector1 == vector2 );
}
//==============================================================================
} //! Tests
} //! Geometry
//==============================================================================