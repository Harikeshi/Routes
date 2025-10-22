#ifndef GEOMETRY_OPERATIONS_AT_VECTORS_HPP
#define GEOMETRY_OPERATIONS_AT_VECTORS_HPP
//==============================================================================
#include <Geometry/Objects/Vector3D.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
/*!
 * \brief Векторное произведение двух векторов
 * \param[in] vector1 - первый вектор
 * \param[in] vector2 - второй вектор
 * \return Векторное произведение двух векторов [вектор]
*/
inline Vector3D crossProduct(const Vector3D& vector1, const Vector3D& vector2)
{
  return vector1 * vector2;
}
//==============================================================================
/*!
 * \brief Скалярное произведение двух векторов
 * \param[in] vector1 - первый вектор
 * \param[in] vector2 - второй вектор
 * \return Скалярное произведение двух векторов [скаляр, он же число] 
*/
inline double scalarProduct(const Vector3D& vector1, const Vector3D& vector2)
{
  return vector1 & vector2;
}
//==============================================================================
/*!
 * \brief Нахождение косинуса угла между векторами
 * \param[in] vector1 - первый вектор
 * \param[in] vector2 - второй вектор
 * \return Косинус угла.
*/
inline double cosAngle(const Vector3D& vector1, const Vector3D& vector2)
{
  return vector1.cosAtVector(vector2);
}
//==============================================================================
} //! Geometry
//==============================================================================
#endif //! GEOMETRY_OPERATIONS_AT_VECTORS_HPP