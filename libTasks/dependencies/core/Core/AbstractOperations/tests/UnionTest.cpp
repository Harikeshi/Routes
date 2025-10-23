#include <AbstractOperations/Algorithms/Union.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
#include <PrimaryEntities/Equalities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
//! Тесты для проверки правильности работы функции объединения (union)
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
class UnionTest : public ::testing::Test
{
//==============================================================================
protected: 
//==============================================================================
  void SetUp()
  {
    //! Точечные - Dotted
    point1 = Point2D(1., 1.);
    point2 = Point2D(2., 2.);

    //! Линейный - Linear
    line1 = { { 1., 0. },{ 1., 2. },{ 2., 2. } };
    line2 = { { 2., 2. },{ 2., 3. },{ 3., 3. } };

    //! Площадные - Areal
    box = Box2D(Point2D(0., 0.), Point2D(2., 2.));
    ring = { { 1., 1. },{ 1., 3. },{ 3., 3. },{ 3., 1. },{ 1., 1. } };
    incorrectRing = { { 1., 1. },{ 2., 2. },{ 2., 1. },{ 1., 2. },{ 1., 1. } };
    polygon.outer() = ring;

    polygon2 = { { { 0., 0. },{ 0., 2. },{ 2., 2. },{ 2., 0. }, { 0., 0. } } };

    mpolygonResult = { { { { 1., 2. },{ 1., 3. },{ 3., 3. },{ 3., 1. },
                           { 2., 1. },{ 2., 0. },{ 0., 0. },{ 0., 2. },
                           { 1., 2. } } } };
  }
//==============================================================================
  void TearDown()
  {

  }
//==============================================================================
  //! Точечные - Dotted
  Point2D             point1;
  Point2D             point2;
  MultiPoint2D        multiPoints;

  //! Линейный - Linear
  Linestring2D      line1;
  Linestring2D      line2;
  MultiLinestring2D multiline;

  //! Площадные - Areal
  Box2D               box;
  Ring2D              ring;
  Ring2D              incorrectRing;
  Polygon2D           polygon;
  Polygon2D           polygon2;
  MultiPolygon2D      mpolygon;
  MultiPolygon2D      mpolygonResult;
};
//==============================================================================
TEST_F(UnionTest, Union)
{
  //unionOf работает только с площадными геометриями
  
  //! Объединение точек
  ASSERT_THROW(unionOf(point1, point2, multiPoints),
               AbstractOperationsFailure);
  try
  {
    unionOf(point1, point2, multiPoints);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IrrelevantGeometryType, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }

  //! Объединение линий
  ASSERT_THROW(unionOf(line1, line2, multiline),
               AbstractOperationsFailure);
  try
  {
    unionOf(line1, line2, multiline);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IrrelevantGeometryType, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }

  //! Объединение площадных объектов
  unionOf(box, ring, mpolygon);
  ASSERT_EQ(mpolygonResult.size(), mpolygon.size());
  try
  {
    for (size_t i = 0; i < mpolygonResult.size(); i++)
    {
      // Сравниванием размеры внешних колец
      ASSERT_EQ(mpolygonResult.at(i).outer().size(),
        mpolygon.at(i).outer().size());

      // Сравниваем точки внешних колец
      ASSERT_TRUE(areEqualUpToRotate(mpolygonResult.at(i).outer(),
        mpolygon.at(i).outer()));

      // Сравниванием количество внутренних колец
      ASSERT_EQ(mpolygonResult.at(i).inners().size(),
        mpolygon.at(i).inners().size());

      for (size_t j = 0; j < mpolygonResult.at(i).inners().size(); j++)
      {
        // Сравниваем размеры внутренних колец
        ASSERT_EQ(mpolygonResult.at(i).inners().at(j).size(),
          mpolygon.at(i).inners().at(j).size());

        // Сравниваем точки внутренних колец
        ASSERT_TRUE(areEqualUpToRotate(mpolygonResult.at(i).inners().at(j),
          mpolygon.at(i).inners().at(j)));
      }
    }
  }
  catch (...)
  {
    //! если что-то пошло не так (например: выход за пределы массива), то тесты 
    //! не проходятся, сообщаем заведомым провалом
    ASSERT_TRUE(false);
  }

  unionOf(box, polygon, mpolygon);
  ASSERT_EQ(mpolygonResult.size(), mpolygon.size());
  try
  {
    for (size_t i = 0; i < mpolygonResult.size(); i++)
    {
      // Сравниванием размеры внешних колец
      ASSERT_EQ(mpolygonResult.at(i).outer().size(),
                mpolygon.at(i).outer().size());

      // Сравниваем точки внешних колец
      ASSERT_TRUE(areEqualUpToRotate(mpolygonResult.at(i).outer(),
                                     mpolygon.at(i).outer()));

      // Сравниванием количество внутренних колец
      ASSERT_EQ(mpolygonResult.at(i).inners().size(),
                mpolygon.at(i).inners().size());

      for (size_t j = 0; j < mpolygonResult.at(i).inners().size(); j++)
      {
        // Сравниваем размеры внутренних колец
        ASSERT_EQ(mpolygonResult.at(i).inners().at(j).size(),
                  mpolygon.at(i).inners().at(j).size());

        // Сравниваем точки внутренних колец
        ASSERT_TRUE(areEqualUpToRotate(mpolygonResult.at(i).inners().at(j),
                                       mpolygon.at(i).inners().at(j)));
      }
    }
  }
  catch (...)
  {
    //! если что-то пошло не так (например: выход за пределы массива),
    //! то тесты не проходятся, сообщаем заведомым провалом
    ASSERT_TRUE(false);
  }

  polygon.clear();
  unionOf(polygon, mpolygon, mpolygon);
  ASSERT_EQ(mpolygonResult.size(), mpolygon.size());
  try
  {
    for (size_t i = 0; i < mpolygonResult.size(); i++)
    {
      // Сравниванием размеры внешних колец
      ASSERT_EQ(mpolygonResult.at(i).outer().size(),
                mpolygon.at(i).outer().size());

      // Сравниваем точки внешних колец
      ASSERT_TRUE(areEqualUpToRotate(mpolygonResult.at(i).outer(),
                                     mpolygon.at(i).outer()));

      // Сравниванием количество внутренних колец
      ASSERT_EQ(mpolygonResult.at(i).inners().size(),
                mpolygon.at(i).inners().size());

      for (size_t j = 0; j < mpolygonResult.at(i).inners().size(); j++)
      {
        // Сравниваем размеры внутренних колец
        ASSERT_EQ(mpolygonResult.at(i).inners().at(j).size(),
                  mpolygon.at(i).inners().at(j).size());

        // Сравниваем точки внутренних колец
        ASSERT_TRUE(areEqualUpToRotate(mpolygonResult.at(i).inners().at(j),
                                       mpolygon.at(i).inners().at(j)));
      }
    }
  }
  catch (...)
  {
    //! если что-то пошло не так (например: выход за пределы массива),
    //! то тесты не проходятся, сообщаем заведомым провалом
    ASSERT_TRUE(false);
  }

  //! Некорректная геометрия
  ASSERT_THROW(unionOf(box, incorrectRing, mpolygon),
               AbstractOperationsFailure);
  try
  {
    unionOf(box, incorrectRing, mpolygon);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IncorrectGeometry, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }
}
//==============================================================================
TEST_F(UnionTest, UnionWithCorrect)
{
  ring = { { 1., 1. },{ 3., 1. },{ 3., 1. },{ 3., 3. },{ 1., 3. } };

  //! Объединение площадных объектов
  unionOf(box, ring, mpolygon, true);

  ASSERT_EQ(mpolygonResult.size(), mpolygon.size());
  try
  {
    for (size_t i = 0; i < mpolygonResult.size(); i++)
    {
      // Сравниванием размеры внешних колец
      ASSERT_EQ(mpolygonResult.at(i).outer().size(),
        mpolygon.at(i).outer().size());

      // Сравниваем точки внешних колец
      ASSERT_TRUE(areEqualUpToRotate(mpolygonResult.at(i).outer(),
        mpolygon.at(i).outer()));

      // Сравниванием количество внутренних колец
      ASSERT_EQ(mpolygonResult.at(i).inners().size(),
        mpolygon.at(i).inners().size());

      for (size_t j = 0; j < mpolygonResult.at(i).inners().size(); j++)
      {
        // Сравниваем размеры внутренних колец
        ASSERT_EQ(mpolygonResult.at(i).inners().at(j).size(),
          mpolygon.at(i).inners().at(j).size());

        // Сравниваем точки внутренних колец
        ASSERT_TRUE(areEqualUpToRotate(mpolygonResult.at(i).inners().at(j),
          mpolygon.at(i).inners().at(j)));
      }
    }
  }
  catch (...)
  {
    //! если что-то пошло не так (например: выход за пределы массива), то тесты 
    //! не проходятся, сообщаем заведомым провалом
    ASSERT_TRUE(false);
  }
}
//==============================================================================
TEST_F(UnionTest, UnionInternal)
{
  mpolygon = { { { { 0., 0. },{ 0., 2. },{ 2., 2. },{ 2., 0. },{ 0., 0. } } },
               { { { 1., 1. },{ 1., 3. },{ 3., 3. },{ 3., 1. },{ 1., 1. } } },
               { { { 2., 2. },{ 2., 4. },{ 4., 4. },{ 4., 2. },{ 2., 2. } } } };

  unionInternal(mpolygon);
  ASSERT_EQ(1, mpolygon.size());
  ASSERT_EQ(13, mpolygon.at(0).outer().size());
  ASSERT_EQ(Point2D(1., 2.), mpolygon.at(0).outer().at(0));
  ASSERT_EQ(Point2D(1., 3.), mpolygon.at(0).outer().at(1));
  ASSERT_EQ(Point2D(2., 3.), mpolygon.at(0).outer().at(2));
  ASSERT_EQ(Point2D(2., 4.), mpolygon.at(0).outer().at(3));
  ASSERT_EQ(Point2D(4., 4.), mpolygon.at(0).outer().at(4));
  ASSERT_EQ(Point2D(4., 2.), mpolygon.at(0).outer().at(5));
  ASSERT_EQ(Point2D(3., 2.), mpolygon.at(0).outer().at(6));
  ASSERT_EQ(Point2D(3., 1.), mpolygon.at(0).outer().at(7));
  ASSERT_EQ(Point2D(2., 1.), mpolygon.at(0).outer().at(8));
  ASSERT_EQ(Point2D(2., 0.), mpolygon.at(0).outer().at(9));
  ASSERT_EQ(Point2D(0., 0.), mpolygon.at(0).outer().at(10));
  ASSERT_EQ(Point2D(0., 2.), mpolygon.at(0).outer().at(11));
  ASSERT_EQ(Point2D(1., 2.), mpolygon.at(0).outer().at(12));

  mpolygon = { { { { 0., 0. },{ 0., 2. },{ 2., 2. },{ 2., 0. },{ 0., 0. } } },
               { { { 1., 1. },{ 1., 3. },{ 3., 3. },{ 3., 1. } } },
               { { { 2., 2. },{ 2., 4. },{ 4., 4. },{ 4., 2. },{ 2., 2. } } } };

  //! Некорректная геометрия
  ASSERT_THROW(unionInternal(mpolygon), AbstractOperationsFailure);
  try
  {
    unionInternal(mpolygon);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(ExceptionTools::TransitFailure, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
    ASSERT_TRUE(
      std::string(failure.what()).find(std::string("IncorrectGeometry"))
      < std::string(failure.what()).size());
  }

  //! Недопустимая геометрия для операции
  ASSERT_THROW(unionInternal(multiline), AbstractOperationsFailure);
  try
  {
    unionInternal(multiline);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(InvalidGeometryType, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }
}
//==============================================================================
TEST_F(UnionTest, UnionInternalWithCorrect)
{
  mpolygon = { { { { 0., 0. },{ 0., 2. },{ 2., 2. },{ 2., 0. },{ 0., 0. } } },
               { { { 1., 1. },{ 1., 3. },{ 3., 3. },{ 3., 1. } } },
               { { { 2., 2. },{ 2., 4. },{ 4., 4. },{ 4., 2. },{ 2., 2. } } } };
  unionInternal(mpolygon, true);
  ASSERT_EQ(1, mpolygon.size());
  ASSERT_EQ(13, mpolygon.at(0).outer().size());
  ASSERT_EQ(Point2D(1., 2.), mpolygon.at(0).outer().at(0));
  ASSERT_EQ(Point2D(1., 3.), mpolygon.at(0).outer().at(1));
  ASSERT_EQ(Point2D(2., 3.), mpolygon.at(0).outer().at(2));
  ASSERT_EQ(Point2D(2., 4.), mpolygon.at(0).outer().at(3));
  ASSERT_EQ(Point2D(4., 4.), mpolygon.at(0).outer().at(4));
  ASSERT_EQ(Point2D(4., 2.), mpolygon.at(0).outer().at(5));
  ASSERT_EQ(Point2D(3., 2.), mpolygon.at(0).outer().at(6));
  ASSERT_EQ(Point2D(3., 1.), mpolygon.at(0).outer().at(7));
  ASSERT_EQ(Point2D(2., 1.), mpolygon.at(0).outer().at(8));
  ASSERT_EQ(Point2D(2., 0.), mpolygon.at(0).outer().at(9));
  ASSERT_EQ(Point2D(0., 0.), mpolygon.at(0).outer().at(10));
  ASSERT_EQ(Point2D(0., 2.), mpolygon.at(0).outer().at(11));
  ASSERT_EQ(Point2D(1., 2.), mpolygon.at(0).outer().at(12));
}
//==============================================================================
TEST_F(UnionTest, UnionPolygons)
{
  std::vector<Polygon2D> polygons = { polygon2, polygon };
  
  MultiPolygon2D result = unionPolygons(polygons);

  ASSERT_EQ(mpolygonResult.size(), result.size());
  ASSERT_EQ(mpolygonResult.at(0).outer().size(), result.at(0).outer().size());
  ASSERT_EQ(mpolygonResult.at(0).inners().size(),
            result.at(0).inners().size());

  ASSERT_TRUE(areEqualUpToRotate(mpolygonResult.at(0).outer(),
                                 result.at(0).outer()));
}
//==============================================================================
TEST_F(UnionTest, UnionAll)
{
  MultiPolygon2D result = { polygon2, polygon };

  unionAll(result);

  ASSERT_EQ(mpolygonResult.size(), result.size());
  ASSERT_EQ(mpolygonResult.at(0).outer().size(), result.at(0).outer().size());
  ASSERT_EQ(mpolygonResult.at(0).inners().size(),
            result.at(0).inners().size());

  ASSERT_TRUE(areEqualUpToRotate(mpolygonResult.at(0).outer(),
                                 result.at(0).outer()));

  // Пустые полигоны добавлены не будут
  result.push_back(Polygon2D());

  unionAll(result);

  ASSERT_EQ(mpolygonResult.size(), result.size());
  ASSERT_EQ(mpolygonResult.at(0).outer().size(), result.at(0).outer().size());
  ASSERT_EQ(mpolygonResult.at(0).inners().size(),
            result.at(0).inners().size());

  ASSERT_TRUE(areEqualUpToRotate(mpolygonResult.at(0).outer(),
    result.at(0).outer()));
}
//==============================================================================
} // ! Tests
} // ! AbstractOperations
