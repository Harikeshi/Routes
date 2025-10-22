#pragma once

/**
 * @brief Перечисление SearchScheme, содержит все известные схемы
 *
 */
enum SearchScheme
{
    /**
     * @brief В районе
     *
     */
    Zigzag,             // Зигзаг
    Shift,              // Сдвиг
    DeterminedShift,    // Закономерный Сдвиг
    StraightTackRegion, // Патрулирование в районе на прямом галсе
                        //    _Zigzag,         //!

    // по вызову
    /**
     * @brief По вызову
     *
     */
    SectorSpiral,     // спираль в секторе
    StraightTackCall, // На прямом галсе (гребенка)
    // на рубеже
    /**
     * @brief На рубеже
     *
     */
    LinearPatrolling, // Линейное патрулирование
    CrossPatrolling,  // Перекрестное патрулирование
    OnStop,           // На стопе

    /**
     * @brief Восстановление
     *
     */
    PetljaK,
    ProryvK
};

/**
 * @brief Перечисление SearchType, содержит все известные типы
 *
 */
enum SearchType
{
    InRegion, // в районе
    ByCall,   // по вызову
    AtBorder, // на рубеже
    Restoring
};
