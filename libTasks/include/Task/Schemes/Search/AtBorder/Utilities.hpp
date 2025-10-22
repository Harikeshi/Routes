#pragma once

#include <Task/Schemes/Search/AtBorder/Input.hpp>

/*! Core */
#include <Geometry/Objects/BorderedLine.hpp>

/*! std */
#include <string>
#include <vector>

namespace Schemes {
namespace Search {
namespace AtBorder {

using BorderedLine = Geometry::BorderedLine;

struct PatrollingParameters
{
    double patrollingLength; // длина участка патрулирования
    double lengthDifference; // полу разность длин участка патрулирования и обозреваемого участка

    PatrollingParameters() = default;
    PatrollingParameters(double patrollingLength, double lengthDifference)
        : patrollingLength{patrollingLength}, lengthDifference{lengthDifference}
    {
    }
};

/// @brief Расчёт коэффициента для расчёта восьмёрки используемый в calculateLambdas
double calculateAParameter(double mParameter);

/// @brief Расчёт коэффициентов используемых в расчёте обзора и длина хода поисковика
PatrollingParameters relativePatrollingParameters(double mParameter, const std::string& flag);

/// @brief Расчёт длин обзора и хода поисковика
/// @note версия функции для перехода к разным БЭК в ТГ
/// \return first
PatrollingParameters patrollingParameters(double searchVel, double targetVel, double detectionDist, double rCirculation, const std::string& flag);

/*!
 * \brief calculatePositions
 * \param input[in]
 */
BorderedLine positionsAtBorder(const BorderedLine& border, double lengthDifference);

} // namespace AtBorder
} // namespace Search
} // namespace Schemes
