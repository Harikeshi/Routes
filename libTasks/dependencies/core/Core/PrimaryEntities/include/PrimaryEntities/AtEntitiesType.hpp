#ifndef PRIMARY_ENTITIES_AT_ENTITIES_TYPE_HPP
#define PRIMARY_ENTITIES_AT_ENTITIES_TYPE_HPP
//==============================================================================
//! std
#include <typeinfo>
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
  /*
   *\brief Перечисление типов геометрий (сущностей)
   */
  enum EntityTypeEnum
  {
    UndefineEntity    = 0x0000, //!< Неопределённый тип
    GeometryEntity    = 0x0001, //!< Геометрический
    GeodesicEntity    = 0x0002, //!< Геодезический
    DottedEntity      = 0x0004, //!< Точечный
    LinearEntity      = 0x0008, //!< Линейный
    ArealEntity       = 0x0010, //!< Площадной
    Dim2DEntity       = 0x0020, //!< Двумерный
    Dim3DEntity       = 0x0040, //!< Трёхмерный
    SingleEntity      = 0x0080, //!< Одиночный
    CollectionEntity  = 0x0100  //!< Коллекция 
  };
//==============================================================================
  /*
   * \brief Анализ информации о типе геометрии (сущности)
   * \details На основе хэш-кода типа производится оценка соответствия геометрии 
   *          (сущности) к тому или иному типу
   * \param[in] typeEntity - информация о типе сущности
   * \return Набор флагов, соответствующих типам, к которым относится геометрия
   *         (сущность)
   */
  unsigned int analyseEntityType1(const std::type_info& typeEntity);
//==============================================================================
  /*
   * \brief Определение типа геометрии (сущности)
   * \details Функция предназначена для определения типа геометрии (сущности) 
   *          из состава Geodesic и Geometry Entities
   * \param[in] curEntity - геометрия (сущность), тип которой определяем
   * \return Набор флагов, соответствующих типам, к которым относится геометрия
   *         (сущность)
   */
  template <typename Entity>
  unsigned int determineEntityType1(const Entity& curEntity)
  {
    return analyseEntityType1(typeid(curEntity));
  }
//==============================================================================
  /*
   * \brief Проверка что тип геометрии (сущности) соответствует указанному
   * \details [Быстрая] Работает с кодами (флагами)
   * \param[in] entityType - код (флаг) геометрии (сущности)
   * \param[in] entityTypeFlag - код (флаг), наличие которого у геометрии 
   *                             (сущности) проверяется
   * \return true - геометрия (сущность) соответствует указанному типу
             false - геометрия (сущность) не соответствует указанному типу
   */
  inline bool isEntityType1(unsigned int entityType,
                           unsigned int entityTypeFlag)
  {
    return (entityType & entityTypeFlag) == entityTypeFlag;
  }
//==============================================================================
  /*
   * \brief Проверка что тип геометрии (сущности) соответствует указанному
   * \details [Медленная] Сначала получает код по типу геометрии (сущности) и 
   *                      только потом осуществляет проверку.
   * \param[in] curEntity - геометрия (сущность)
   * \param[in] entityTypeFlag - код (флаг), наличие которого у геометрии 
   *                             (сущности) проверяется
   * \return true - геометрия (сущность) соответствует указанному типу
             false - геометрия (сущность) не соответствует указанному типу
   */  
  template <typename Entity>
  bool isEntityType1(const Entity& curEntity, unsigned int entityTypeFlag)
  {
    return isEntityType1(determineEntityType1(curEntity), entityTypeFlag);
  }
//==============================================================================
} //! PrimaryEntities
//==============================================================================
#endif //! PRIMARY_ENTITIES_AT_ENTITIES_TYPE_HPP
