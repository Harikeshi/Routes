#ifndef SAMPLES_FOR_TESTS_BASE_GEO_FIGURES_HPP
#define SAMPLES_FOR_TESTS_BASE_GEO_FIGURES_HPP
//==============================================================================
//! PrimaryEntities
#include <PrimaryEntities/GeodesicEntities.hpp>
//==============================================================================
namespace SamplesForTests
{
//==============================================================================
  /*
   *\brief Построить квадрат с центром в данной точке и длиной сторон в градусах
   *\param[in] center - центр квадрата (географическая точка)
   *\param[in] size - длина стороны квадрата [градусы]
   *\return - построенный квадрат
  */
  GeoPolygon2D buildGSquare(const GeoPoint2D& center, const Degree size);
//==============================================================================
  /*
   *\brief Построить прямоугольник с координатой Юго-Западного угла
   * прямогольника и с размерами сторон, заданными в градусах
   *\param[in] southWestPoint - координата точки (географическая точка)
   *\param[in] latSize - длина стороны прямоугольника по широте [градусы]
   *\param[in] lonSize - длина стороны прямоугольника по долготе [градусы]
   *\return - построенный прямоугольник
  */ 
  GeoPolygon2D buildGeoRectangle(const GeoPoint2D& southWestPoint,
                                 const Degree latSize,
                                 const Degree lonSize);
//==============================================================================
  /*
   *\brief Построить прямоугольник с координатой Юго-Западного угла 
   * прямоугольника и с размерами принимаемыми в double
   *\param[in] southWestPoint - координата точки
   *\param[in] latSize - длина стороны прямоугольника по широте [градусы]
   *\param[in] lonSize - длина стороны прямоугольника по долготе [градусы]
   *\retutn - построенный прямоугольник
  */
  GeoPolygon2D buildGeoRectangle(const GeoPoint2D& southWestPoint,
                                 const double latSize,
                                 const double lonSize);
//==============================================================================
  /*
   *\brief Построить прямоугольник с дыркой
   *\param[in] SouthWestPoint - Юго-Западный угол прямоугольника
   *\param[in] HoleSouthWestPoint - Юго-Западный угол дырки
   *\param[in] latSize - длина стороны прямоугольника по широте
   *\param[in] lonSize - длина стороны прямоугольника по долготе
   *\param[in] holeLatSize - длина стороны дырки по широте
   *\param[in] holeLonSize - длина стороны дырки по долготе
   *\return - построенная фигура
  */
  GeoPolygon2D buildGeoHoleRectangle(const GeoPoint2D& southWestPoint,
                                     const GeoPoint2D& holeSouthWestPoint,
                                     const Degree latSize,
                                     const Degree lonSize,
                                     const Degree holelatSize,
                                     const Degree holelonSize);
//==============================================================================
  /*
   *\brief Построить бантик (некорректная фигура)
   *с Юго-Западной координатой точки на входе
   *\param[in] southWestPoint - координата Юго-Западной точки
   *\param[in] latSize - длина стороны фигуры по широте
   *\param[in] lonSize - длина стороны фигуры по долготе
   *\return - построенный бантик
  */
  GeoPolygon2D buildBow(const GeoPoint2D& southWestPoint,
                        const Degree latSize,
                        const Degree lonSize);
//==============================================================================
  /*
   *\brief Построить фигуру с самокасанием
   *\param[in] southWestPoint - координата Юго-Западной точки
   *\param[in] latSize - длина стороны фигуры по широте
   *\param[in] lonSize - длина стороны фигуры по долготе
   *\return - построенная фигура
  */
  GeoPolygon2D buildPost(const GeoPoint2D& southWestPoint,
                         const Degree latSize,
                         const Degree lonSize);
//==============================================================================
  /*
   *\brief Добавление в полигон прямоугольной дырки с Юго-Западной координатой 
   * угла и сторонами, передающимися как double,
   * считающимися заданными в градусах
   *\param[in|out] polygon - исходный многоугольник
   *\param[in] holePoint - координата Юго-Западного угла дырки
   *\param[in] holeLatSize - длина стороны дырки по широте
   *\param[in] holeLonSize - длина стороны дырки по долготе
   *\return - построенная фигура с дыркой
  */
  void addHole(GeoPolygon2D& polygon,
               const GeoPoint2D& holePoint,
               const double holeLatSize,
               const double holeLonSize);
//==============================================================================
} //! SamplesForTests
//==============================================================================
#endif //! SAMPLES_FOR_TESTS_BASE_GEO_FIGURES_HPP