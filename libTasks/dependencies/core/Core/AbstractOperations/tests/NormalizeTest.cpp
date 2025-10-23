#include <AbstractOperations/Normalize.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace AbstractOperations 
{
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование нормализации геометрий
 * в географической системе координат.
 */
TEST( NormalizeTest, Normalize )
{
  //! Множество точек.

  GeoMultiPoint2D multiPoint{ GeoPoint2D( Degree( 0. ), Degree( -20. ) ) };

  normalize( multiPoint, NORMAL );
  ASSERT_EQ( 1, multiPoint.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0. ), Degree( 340. ) ), multiPoint[0] );

  //! Ломаная линия.

  PrimaryEntities::Linestring<GeoPoint3D> linestring{
    GeoPoint3D( Degree( -478.089 ), Degree( -965.764 ), 263.219f ),
    GeoPoint3D( Degree( 160.196 ), Degree( 249.704 ), 41.73f ),
    GeoPoint3D( Degree( 107.924 ), Degree( 754.557 ), 822.415f ),
    GeoPoint3D( Degree( -356.93 ), Degree( 18.506 ), 367.128f )
  };

  normalize( linestring, NORMAL );
  ASSERT_EQ( 4, linestring.size( ) );
  ASSERT_EQ(
    GeoPoint3D( Degree( -478.089 ), Degree( 114.236 ), 263.219f ),
    linestring[0] );
  ASSERT_EQ(
    GeoPoint3D( Degree( 160.196 ), Degree( 249.704 ), 41.73f ),
    linestring[1] );
  ASSERT_EQ(
    GeoPoint3D( Degree( 107.924 ), Degree( 34.557 ), 822.415f ),
    linestring[2] );
  ASSERT_EQ(
    GeoPoint3D( Degree( -356.93 ), Degree( 18.506 ), 367.128f ),
    linestring[3] );

  //! Множество ломаных линий.

  GeoMultiLinestring2D multiLinestring{
    {
      GeoPoint2D( Degree( -786.187 ), Degree( 385.39 ) ),
      GeoPoint2D( Degree( -709.849 ), Degree( -429.374 ) )
    },
    {
      GeoPoint2D( Degree( -165.409 ), Degree( -367.026 ) ),
      GeoPoint2D( Degree( -61.694 ), Degree( -956.624 ) )
    }
  };

  normalize( multiLinestring, NORMAL );
  ASSERT_EQ( 2, multiLinestring.size( ) );
  ASSERT_EQ( 2, multiLinestring[0].size( ) );
  ASSERT_EQ( 2, multiLinestring[1].size( ) );
  ASSERT_EQ(
    GeoPoint2D( Degree( -786.187 ), Degree( 25.39 ) ),
    multiLinestring[0][0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -709.849 ), Degree( 290.626 ) ),
    multiLinestring[0][1] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -165.409 ), Degree( 352.974 ) ),
    multiLinestring[1][0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -61.694 ), Degree( 123.376 ) ),
    multiLinestring[1][1] );

  //! Прямоугольник, стороны которого параллельны координатным осям.

  GeoBox3D box(
    GeoPoint3D(Degree(-18.019), Degree(461.671), 784.404f),
    GeoPoint3D(Degree(791.845), Degree(-459.201), 578.728f));

  normalize( box, REGULAR );
  ASSERT_EQ(GeoPoint3D( Degree( -18.019 ), Degree( 101.671 ), 784.404f ),
            box.min_corner( ) );
  ASSERT_EQ( GeoPoint3D( Degree( 791.845 ), Degree( -99.201 ), 578.728f ),
             box.max_corner( ) );

  //! Кольцо.

  PrimaryEntities::Ring<GeoPoint3D> ring{
    GeoPoint3D( Degree( -376.108 ), Degree( -512.682 ), -196.821f ),
    GeoPoint3D( Degree( -231.475 ), Degree( -921.585 ), -439.974f ),
    GeoPoint3D( Degree( 483.836 ), Degree( -627.204 ), -877.427f ),
    GeoPoint3D( Degree( -534.798 ), Degree( -774.534 ), 160.749f )
  };

  normalize( ring, NORMAL );
  ASSERT_EQ( 4, ring.size( ) );
  ASSERT_EQ(
    GeoPoint3D( Degree( -376.108 ), Degree( 207.318 ), -196.821f ), ring[0] );
  ASSERT_EQ(
    GeoPoint3D( Degree( -231.475 ), Degree( 158.415 ), -439.974f ), ring[1] );
  ASSERT_EQ(
    GeoPoint3D( Degree( 483.836 ), Degree( 92.796 ), -877.427f ), ring[2] );
  ASSERT_EQ(
    GeoPoint3D( Degree( -534.798 ), Degree( 305.466 ), 160.749f ), ring[3] );

  //! Многоугольник.

  GeoPolygon2D polygon;
  polygon.outer( ) = {
    GeoPoint2D( Degree( -429.518 ), Degree( -623.707 ) ),
    GeoPoint2D( Degree( 318.055 ), Degree( -62.679 ) ),
    GeoPoint2D( Degree( -123.694 ), Degree( -347.654 ) ),
    GeoPoint2D( Degree( -871.746 ), Degree( -610.26 ) )
  };
  polygon.inners( ) = {
    {
      GeoPoint2D( Degree( 513.546 ), Degree( -702.728 ) ),
      GeoPoint2D( Degree( 287.016 ), Degree( 111.098 ) ),
      GeoPoint2D( Degree( 489.639 ), Degree( 877.927 ) ),
      GeoPoint2D( Degree( 207.778 ), Degree( 395.419 ) )
    }
  };

  normalize( polygon, NORMAL );
  ASSERT_EQ( 4, polygon.outer( ).size( ) );
  ASSERT_EQ( 1, polygon.inners( ).size( ) );
  ASSERT_EQ( 4, polygon.inners( )[0].size( ) );
  ASSERT_EQ(
    GeoPoint2D( Degree( -429.518 ), Degree( 96.293 ) ), polygon.outer( )[0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 318.055 ), Degree( 297.321 ) ), polygon.outer( )[1] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -123.694 ), Degree( 12.346 ) ), polygon.outer( )[2] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -871.746 ), Degree( 109.74 ) ),
    polygon.outer( )[3] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 513.546 ), Degree( 17.272 ) ),
    polygon.inners( )[0][0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 287.016 ), Degree( 111.098 ) ),
    polygon.inners( )[0][1] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 489.639 ), Degree( 157.927 ) ),
    polygon.inners( )[0][2] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 207.778 ), Degree( 35.419 ) ),
    polygon.inners( )[0][3] );

  //! Множество многоугольников.

  GeoMultiPolygon2D multiPolygon;
  multiPolygon.resize( 2 );
  multiPolygon[0].outer( ) = {
    GeoPoint2D( Degree( -983.217 ), Degree( 2.546 ) ),
    GeoPoint2D( Degree( -258.959 ), Degree( 323.517 ) ),
    GeoPoint2D( Degree( 189.356 ), Degree( -161.948 ) ),
    GeoPoint2D( Degree( 532.94 ), Degree( 282.632 ) )
  };
  multiPolygon[0].inners( ) = {
    {
      GeoPoint2D( Degree( 29.311 ), Degree( -681.272 ) ),
      GeoPoint2D( Degree( 537.551 ), Degree( 495.468 ) ),
      GeoPoint2D( Degree( 548.204 ), Degree( -661.424 ) ),
      GeoPoint2D( Degree( -103.419 ), Degree( 272.797 ) )
    }
  };
  multiPolygon[1].outer( ) = {
    GeoPoint2D( Degree( 249.595 ), Degree( -390.78 ) ),
    GeoPoint2D( Degree( 791.965 ), Degree( -997.636 ) ),
    GeoPoint2D( Degree( -328.811 ), Degree( -237.394 ) ),
    GeoPoint2D( Degree( 390.275 ), Degree( -192.951 ) )
  };
  multiPolygon[1].inners( ) = {
    {
      GeoPoint2D( Degree( 79.735 ), Degree( 388.764 ) ),
      GeoPoint2D( Degree( -691.815 ), Degree( -241.971 ) ),
      GeoPoint2D( Degree( 9.046 ), Degree( 649.768 ) ),
      GeoPoint2D( Degree( 209.036 ), Degree( -385.632 ) )
    }
  };

  normalize( multiPolygon, NORMAL );
  ASSERT_EQ( 2, multiPolygon.size( ) );
  ASSERT_EQ( 4, multiPolygon[0].outer( ).size( ) );
  ASSERT_EQ( 1, multiPolygon[0].inners( ).size( ) );
  ASSERT_EQ( 4, multiPolygon[0].inners( )[0].size( ) );
  ASSERT_EQ( 4, multiPolygon[1].outer( ).size( ) );
  ASSERT_EQ( 1, multiPolygon[1].inners( ).size( ) );
  ASSERT_EQ( 4, multiPolygon[1].inners( )[0].size( ) );
  ASSERT_EQ(
    GeoPoint2D( Degree( -983.217 ), Degree( 2.546 ) ),
    multiPolygon[0].outer( )[0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -258.959 ), Degree( 323.517 ) ),
    multiPolygon[0].outer( )[1] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 189.356 ), Degree( 198.052 ) ),
    multiPolygon[0].outer( )[2] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 532.94 ), Degree( 282.632 ) ),
    multiPolygon[0].outer( )[3] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 29.311 ), Degree( 38.728 ) ),
    multiPolygon[0].inners( )[0][0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 537.551 ), Degree( 135.468 ) ),
    multiPolygon[0].inners( )[0][1] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 548.204 ), Degree( 58.576 ) ),
    multiPolygon[0].inners( )[0][2] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -103.419 ), Degree( 272.797 ) ),
    multiPolygon[0].inners( )[0][3] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 249.595 ), Degree( 329.22 ) ),
    multiPolygon[1].outer( )[0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 791.965 ), Degree( 82.364 ) ),
    multiPolygon[1].outer( )[1] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -328.811 ), Degree( 122.606 ) ),
    multiPolygon[1].outer( )[2] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 390.275 ), Degree( 167.049 ) ),
    multiPolygon[1].outer( )[3] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 79.735 ), Degree( 28.764 ) ),
    multiPolygon[1].inners( )[0][0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -691.815 ), Degree( 118.029 ) ),
    multiPolygon[1].inners( )[0][1] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 9.046 ), Degree( 289.768 ) ),
    multiPolygon[1].inners( )[0][2] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 209.036 ), Degree( 334.368 ) ),
    multiPolygon[1].inners( )[0][3] );
}
//==============================================================================
/*!
 * \brief Тестирование приведения координат точек геометрии
 * в географической системе координат в диапазон [0, 2π] по долготе.
 */
TEST( NormalizeTest, NormalizeIfNeed )
{
  //! Множество точек (нормализация оказалась не нужна).

  GeoMultiPoint2D multiPoint{ GeoPoint2D( Degree( 0. ), Degree( -20. ) ) };

  normalizeIfNeed( multiPoint );
  ASSERT_EQ( 1, multiPoint.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0. ), Degree( -20. ) ), multiPoint[0] );

  //! Ломаная линия (нормализация оказалась не нужна).

  PrimaryEntities::Linestring<GeoPoint3D> linestring{
    GeoPoint3D( Degree( -478.089 ), Degree( -965.764 ), 263.219f ),
    GeoPoint3D( Degree( 160.196 ), Degree( 249.704 ), 41.73f ),
    GeoPoint3D( Degree( 107.924 ), Degree( 754.557 ), 822.415f ),
    GeoPoint3D( Degree( -356.93 ), Degree( 18.506 ), 367.128f )
  };

  normalizeIfNeed( linestring );
  ASSERT_EQ( 4, linestring.size( ) );
  ASSERT_EQ(
    GeoPoint3D( Degree( -478.089 ), Degree( -965.764 ), 263.219f ),
    linestring[0] );
  ASSERT_EQ(
    GeoPoint3D( Degree( 160.196 ), Degree( 249.704 ), 41.73f ),
    linestring[1] );
  ASSERT_EQ(
    GeoPoint3D( Degree( 107.924 ), Degree( 754.557 ), 822.415f ),
    linestring[2] );
  ASSERT_EQ(
    GeoPoint3D( Degree( -356.93 ), Degree( 18.506 ), 367.128f ),
    linestring[3] );

  //! Множество ломаных линий (нормализация оказалась нужна).

  GeoMultiLinestring2D multiLinestring{
    {
      GeoPoint2D( Degree( -786.187 ), Degree( 385.39 ) ),
      GeoPoint2D( Degree( -709.849 ), Degree( -429.374 ) )
    },
    {
      GeoPoint2D( Degree( -165.409 ), Degree( -367.026 ) ),
      GeoPoint2D( Degree( -61.694 ), Degree( -956.624 ) )
    }
  };

  normalizeIfNeed( multiLinestring );
  ASSERT_EQ( 2, multiLinestring.size( ) );
  ASSERT_EQ( 2, multiLinestring[0].size( ) );
  ASSERT_EQ( 2, multiLinestring[1].size( ) );
  ASSERT_EQ(
    GeoPoint2D( Degree( -786.187 ), Degree( 25.39 ) ),
    multiLinestring[0][0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -709.849 ), Degree( 290.626 ) ),
    multiLinestring[0][1] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -165.409 ), Degree( 352.974 ) ),
    multiLinestring[1][0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -61.694 ), Degree( 123.376 ) ),
    multiLinestring[1][1] );

  /*!
   * Прямоугольник, стороны которого параллельны координатным осям
   * (нормализация оказалась нужна).
   */

  GeoBox3D box(
    GeoPoint3D(Degree(-18.019), Degree(461.671), 784.404f),
    GeoPoint3D(Degree(791.845), Degree(-459.201), 578.728f));

  normalizeIfNeed( box );
  ASSERT_EQ( GeoPoint3D( Degree( -18.019 ), Degree( 101.671 ), 784.404f ),
             box.min_corner( ) );
  ASSERT_EQ( GeoPoint3D( Degree( 791.845 ), Degree( 260.799 ), 578.728f ),
             box.max_corner( ) );

  //! Кольцо (нормализация оказалась нужна).

  PrimaryEntities::Ring<GeoPoint3D> ring{
    GeoPoint3D( Degree( -376.108 ), Degree( -512.682 ), -196.821f ),
    GeoPoint3D( Degree( -231.475 ), Degree( -921.585 ), -439.974f ),
    GeoPoint3D( Degree( 483.836 ), Degree( -627.204 ), -877.427f ),
    GeoPoint3D( Degree( -534.798 ), Degree( -774.534 ), 160.749f )
  };

  normalizeIfNeed( ring );
  ASSERT_EQ( 4, ring.size( ) );
  ASSERT_EQ(
    GeoPoint3D( Degree( -376.108 ), Degree( 207.318 ), -196.821f ), ring[0] );
  ASSERT_EQ(
    GeoPoint3D( Degree( -231.475 ), Degree( 158.415 ), -439.974f ), ring[1] );
  ASSERT_EQ(
    GeoPoint3D( Degree( 483.836 ), Degree( 92.796 ), -877.427f ), ring[2] );
  ASSERT_EQ(
    GeoPoint3D( Degree( -534.798 ), Degree( 305.466 ), 160.749f ), ring[3] );

  //! Многоугольник (нормализация не нужна).

  GeoPolygon2D polygon;
  polygon.outer( ) = {
    GeoPoint2D( Degree( -429.518 ), Degree( -623.707 ) ),
    GeoPoint2D( Degree( 318.055 ), Degree( -62.679 ) ),
    GeoPoint2D( Degree( -123.694 ), Degree( -347.654 ) ),
    GeoPoint2D( Degree( -871.746 ), Degree( -610.26 ) )
  };
  polygon.inners( ) = {
    {
      GeoPoint2D( Degree( 513.546 ), Degree( -702.728 ) ),
      GeoPoint2D( Degree( 287.016 ), Degree( 111.098 ) ),
      GeoPoint2D( Degree( 489.639 ), Degree( 877.927 ) ),
      GeoPoint2D( Degree( 207.778 ), Degree( 395.419 ) )
    }
  };

  normalizeIfNeed( polygon );
  ASSERT_EQ( 4, polygon.outer( ).size( ) );
  ASSERT_EQ( 1, polygon.inners( ).size( ) );
  ASSERT_EQ( 4, polygon.inners( )[0].size( ) );
  ASSERT_EQ(
    GeoPoint2D( Degree( -429.518 ), Degree( -623.707 ) ),
    polygon.outer( )[0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 318.055 ), Degree( -62.679 ) ),
    polygon.outer( )[1] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -123.694 ), Degree( -347.654 ) ),
    polygon.outer( )[2] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -871.746 ), Degree( -610.26 ) ),
    polygon.outer( )[3] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 513.546 ), Degree( -702.728 ) ),
    polygon.inners( )[0][0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 287.016 ), Degree( 111.098 ) ),
    polygon.inners( )[0][1] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 489.639 ), Degree( 877.927 ) ),
    polygon.inners( )[0][2] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 207.778 ), Degree( 395.419 ) ),
    polygon.inners( )[0][3] );

  //! Множество многоугольников (нормализация нужна).

  GeoMultiPolygon2D multiPolygon;
  multiPolygon.resize( 2 );
  multiPolygon[0].outer( ) = {
    GeoPoint2D( Degree( -983.217 ), Degree( 2.546 ) ),
    GeoPoint2D( Degree( -258.959 ), Degree( 323.517 ) ),
    GeoPoint2D( Degree( 189.356 ), Degree( -161.948 ) ),
    GeoPoint2D( Degree( 532.94 ), Degree( 282.632 ) )
  };
  multiPolygon[0].inners( ) = {
    {
      GeoPoint2D( Degree( 29.311 ), Degree( -681.272 ) ),
      GeoPoint2D( Degree( 537.551 ), Degree( 495.468 ) ),
      GeoPoint2D( Degree( 548.204 ), Degree( -661.424 ) ),
      GeoPoint2D( Degree( -103.419 ), Degree( 272.797 ) )
    }
  };
  multiPolygon[1].outer( ) = {
    GeoPoint2D( Degree( 249.595 ), Degree( -390.78 ) ),
    GeoPoint2D( Degree( 791.965 ), Degree( -997.636 ) ),
    GeoPoint2D( Degree( -328.811 ), Degree( -237.394 ) ),
    GeoPoint2D( Degree( 390.275 ), Degree( -192.951 ) )
  };
  multiPolygon[1].inners( ) = {
    {
      GeoPoint2D( Degree( 79.735 ), Degree( 388.764 ) ),
      GeoPoint2D( Degree( -691.815 ), Degree( -241.971 ) ),
      GeoPoint2D( Degree( 9.046 ), Degree( 649.768 ) ),
      GeoPoint2D( Degree( 209.036 ), Degree( -385.632 ) )
    }
  };

  normalizeIfNeed( multiPolygon );
  ASSERT_EQ( 2, multiPolygon.size( ) );
  ASSERT_EQ( 4, multiPolygon[0].outer( ).size( ) );
  ASSERT_EQ( 1, multiPolygon[0].inners( ).size( ) );
  ASSERT_EQ( 4, multiPolygon[0].inners( )[0].size( ) );
  ASSERT_EQ( 4, multiPolygon[1].outer( ).size( ) );
  ASSERT_EQ( 1, multiPolygon[1].inners( ).size( ) );
  ASSERT_EQ( 4, multiPolygon[1].inners( )[0].size( ) );
  ASSERT_EQ(
    GeoPoint2D( Degree( -983.217 ), Degree( 2.546 ) ),
    multiPolygon[0].outer( )[0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -258.959 ), Degree( 323.517 ) ),
    multiPolygon[0].outer( )[1] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 189.356 ), Degree( 198.052 ) ),
    multiPolygon[0].outer( )[2] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 532.94 ), Degree( 282.632 ) ),
    multiPolygon[0].outer( )[3] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 29.311 ), Degree( 38.728 ) ),
    multiPolygon[0].inners( )[0][0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 537.551 ), Degree( 135.468 ) ),
    multiPolygon[0].inners( )[0][1] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 548.204 ), Degree( 58.576 ) ),
    multiPolygon[0].inners( )[0][2] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -103.419 ), Degree( 272.797 ) ),
    multiPolygon[0].inners( )[0][3] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 249.595 ), Degree( 329.22 ) ),
    multiPolygon[1].outer( )[0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 791.965 ), Degree( 82.364 ) ),
    multiPolygon[1].outer( )[1] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -328.811 ), Degree( 122.606 ) ),
    multiPolygon[1].outer( )[2] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 390.275 ), Degree( 167.049 ) ),
    multiPolygon[1].outer( )[3] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 79.735 ), Degree( 28.764 ) ),
    multiPolygon[1].inners( )[0][0] );
  ASSERT_EQ(
    GeoPoint2D( Degree( -691.815 ), Degree( 118.029 ) ),
    multiPolygon[1].inners( )[0][1] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 9.046 ), Degree( 289.768 ) ),
    multiPolygon[1].inners( )[0][2] );
  ASSERT_EQ(
    GeoPoint2D( Degree( 209.036 ), Degree( 334.368 ) ),
    multiPolygon[1].inners( )[0][3] );
}
//==============================================================================
} //! Tests
} //! AbstractOperations
//==============================================================================
