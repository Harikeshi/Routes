#ifndef MATH_TOOLS_BASIC_MATH_LENGTH_OPERATIONS_HPP
#define MATH_TOOLS_BASIC_MATH_LENGTH_OPERATIONS_HPP
//==============================================================================
namespace BasicMath
{
  /*
   * \brief Определение параметров равномерного разбиения общей длины на 
   *        сегменты, чья длина не должна превышать заданную. 
   * \details Если общая длина или максимальная длина сегмента не превышают 0, 
   *          данные параметры некорректны и разбивать не надо.
   *          Если общая длина не превосходит максимальную длину сегмента, то
   *          разбивать не надо, потому что это уже и так сделано.
   * \param[in] totalLength - общая длина (должна быть больше 0)
   * \param[in] maxSegmentLegth - максимальная длина сегмента 
   *                              (должна быть больше 0)
   * \param[out] avgSegmentLength - усреднённая длина сегмента для равномерного 
   *                                разбиения общей длины
   * \param[out] numSegments - количество сегментов, на которые разбивается общая
   *                          длина
   * \return Признак необходимости в разбиении:
   *         true  - разбивать надо;
   *         false - разбивать не надо.
  */
  bool avgSegmentLength(const double totalLength, 
                        const double maxSegmentLegth,
                        double& avgSegmentLength,
                        int& numSegments);
}
//==============================================================================
#endif //! MATH_TOOLS_BASIC_MATH_LENGTH_OPERATIONS_HPP
