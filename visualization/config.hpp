#pragma once

#include <QtMath>

constexpr inline auto DEG_TO_RAD = M_PI / 180.0;
constexpr inline auto RAD_TO_DEG = 180 / M_PI;
constexpr inline auto KNOT_TO_MPS = 0.514444; // 1 узел = 0.514444 м/с

constexpr inline auto EPS = 1e-9;
constexpr inline auto TAU = M_PI * 2.0;

constexpr inline size_t sampling_factor = 10; // Коэффициент дискретизации
const auto max_distance = 5000;               //! Максимальная длина отрезков для нахождения пересечения

constexpr inline size_t split_factor = 10; // Количество разбиений.

constexpr inline auto acceleration = 0.015; // Коэффициент ускорения 0.005-0.015
constexpr inline auto deceleration = 0.015; // Коэффициент замедления

constexpr inline auto L = 60.0;    // Длина судна, м
constexpr inline auto Beam = 15.0; // Ширина судна, м
constexpr inline auto Draft = 10;  // Осадка, м
constexpr inline auto Ar = 0.1;    // Площадь руля, м2
constexpr inline auto delta = 35.; // угол отклонения руля в радианах, до 35°
constexpr inline auto K = 2.;      // коэффициент маневренности (от 1.5 до 2.5)

// Угловая скорость при радиусе циркуляции 100-250 -> 0.015 - 0.04
constexpr inline auto omega = 0.02;          // Угловая скорость, не является константой, рад/с
constexpr inline auto velocity = 5.0;        // Средняя допустимая скорость судна, используется для поворотов >= 90.
constexpr inline auto turn_velocity = 4.5;   // Минимальная скорость, для поворотов < 90
constexpr inline auto u_turn_velocity = 4.5; // Скорость разворота.