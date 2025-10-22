#ifndef GEOMETRY_SECTOR_HPP
#define GEOMETRY_SECTOR_HPP
//==============================================================================
//! MathTools
#include <MathTools/AngularUnits/Degree.hpp>
#include <MathTools/AngularUnits/Radian.hpp>
#include <MathTools/BasicMath/Range.hpp>
#include <MathTools/BasicMath/Compare.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
/*!
* \brief Сектор на плоскости. Задается граничными углами в диапазоне [0,2pi]. 
* \details Начальный и конечный углы могут совпадать. В этом случае ширина 
*          сектора - 0. Для формирования полного сектора следует задавать
*          углы, разница между которыми составляет период.
*          Если после нормализации начальный угол больше конечного, сектор 
*          переходит через разрыв периодов.
*/
class Sector
{
public:
//==============================================================================
  /*!
  * \brief Конструктор класса через два параметра (угла) в радианах (Radian)
  * \details Углы, выходящие за пределы диапазона, нормализуются.
  * \param[in] begAngle - начальный угол [рад]
  * \param[in] endAngle - конечный угол [рад]
  */
  Sector( const Radian& begAngle = Radian( 0. ),
    const Radian& endAngle = Radian( 0. ) );
//==============================================================================
  /*!
  * \brief Конструктор класса через два параметра (угла) в градусах (Degree)
  * \details Углы, выходящие за пределы диапазона, нормализуются.
  * \param[in] begAngle - начальный угол [градусы]
  * \param[in] endAngle - конечный угол [градусы]
  */
  Sector( const Degree& begAngle, const Degree& endAngle );
//==============================================================================
  /*!
  * \brief Копирующий конструктор класса
  * \param[in] sector - копируемый сектор
  */
  Sector( const Sector& sector )
    : mSector( sector.mSector )
  {
  }
//==============================================================================
  /*!
  * \brief Получение граничных углов
  * \return Граничные углы: first - начальный [рад]
  *                         second - конечный [рад]
  */
  inline std::pair<Radian, Radian> get( ) const
  {
    return mSector;
  }
//==============================================================================
  /*!
  * \brief Получение граничных углов
  * \return Граничные углы: first - начальный [градусы]
  *                         second - конечный [градусы]
  */
  inline std::pair<Degree, Degree> getDegree( ) const
  {
    return std::make_pair ( mSector.first.toDegrees(), mSector.second.toDegrees() );
  }
//==============================================================================
  /*!
  * \brief Получение начального угла
  * \return начальный угол [рад]
  */
  inline Radian getBegAngle( ) const
  {
    return mSector.first;
  }
//==============================================================================
  /*!
  * \brief Получение конечного угла
  * \return конечный угол [рад]
  */
  inline Radian getEndAngle( ) const
  {
    return mSector.second;
  }
//==============================================================================
  /*!
  * \brief Задание новых граничных углов
  * \param[in] begAngle - начальный угол [рад]
  * \param[in] endAngle - конечный угол [рад]
  */
  inline void set( const Radian& begAngle, const Radian& endAngle )
  {
    mSector.first = begAngle;
    mSector.second = endAngle;
  }
//==============================================================================
  /*!
  * \brief Задание новых граничных углов
  * \param[in] begAngle - начальный угол [градусы]
  * \param[in] endAngle - конечный угол [градусы]
  */
  inline void set( const Degree& begAngle, const Degree& endAngle )
  {
    mSector.first = begAngle.toRadians();
    mSector.second = endAngle.toRadians();
  }
//==============================================================================
  /*!
  * \brief Сравнение двух секторов на равенство.
  * \param[in] sector - сектор, с которым сравниваем текущий
  * \return true  - равны
  *         false - не равны
  */
  inline bool operator == ( const Sector& sector ) const
  {
    if( BasicMath::isEqual( mSector.first, sector.mSector.first )
      && BasicMath::isEqual( mSector.second, sector.mSector.second ) )
      return true;
    return false;
  }
//==============================================================================
private:
  //==============================================================================
  std::pair<Radian, Radian> mSector; //!< углы, задающие границы сектора
  //==============================================================================
};
//==============================================================================
} //! Geometry
//==============================================================================
#endif //! GEOMETRY_SECTOR_HPP
