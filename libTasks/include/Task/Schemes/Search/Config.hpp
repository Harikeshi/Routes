#pragma once

namespace Schemes {
namespace Search {

const double TURN_ANGLE_MIN_DEG = 30.; // в градусах
const double TURN_ANGLE_MAX_DEG = 80.; // в градусах
const double MAX_SIZE = 1e6;           // измерения района не могут превышать 1000 км
//const double TRAVERSA_MIN = 1.;        // в милях, @todo рассчитывать по формуле через "псишку"
//const double TACK_DIST_MIN = 1.;       // в милях
const double PROJECTION_SHIFT = 0.001;
const double LAST_TACK_SPLIT = 20;
const double TIME_ACCURRACY = 0.001;
const double TRAVERSA_MIN = 500;     // минимальное траверзное расстояние до границы, метры
const double TACK_DIST_MIN = 1000;   // минимальная длина галса, метры
const double TACK_DIST_LEFT = 3000;  // левая граница случайной длины галса, метры
const double MAX_DELAY_TIME = 18000; // секунды
const double NONCONVEXIVITY_RATIO = 0.85;
const double MIN_RELATIVE_ENTRANCE_LENGTH = 0.01;

const double MIN_BORDER_CLOSING_PROBABILITY = 0.99; // требуемая вероятность обнаружения при поиске на рубеже

} // namespace Search
} // namespace Schemes
