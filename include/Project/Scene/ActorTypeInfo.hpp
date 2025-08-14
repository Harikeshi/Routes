#pragma once

#include <QString>
#include <map>

namespace Scene {
enum ActorType
{
    Zigzag,
    Shift,
    SectorSpiral,
    StraightTack,
    LinearPatrolling,
    OnStop
};

inline std::map<ActorType, QString>
    ActorTypeName{
        {ActorType::Zigzag, "Зигзаг"},
        {ActorType::Shift, "Сдвиг"},
        {ActorType::SectorSpiral, "Спираль"},
        {ActorType::StraightTack, "Гребенка"},
        {ActorType::LinearPatrolling, "Линейное"},
        {ActorType::OnStop, "На Стопе"},
    };
} // namespace Scene