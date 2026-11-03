#include "Utils.h"

RE::TESFurniture* Utilities::GetOccupiedFurnitureBase(RE::Actor* a_actor) {
    const auto furniture_ref = a_actor->GetOccupiedFurniture().get();
    if (!furniture_ref) return nullptr;
    const auto furn_base = furniture_ref->GetBaseObject();
    return furn_base ? furn_base->As<RE::TESFurniture>() : nullptr;
}
