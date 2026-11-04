#pragma once

namespace Utilities {

    const auto mod_name = static_cast<std::string>(SKSE::PluginDeclaration::GetSingleton()->GetName());

    RE::TESFurniture* GetOccupiedFurnitureBase(RE::Actor* a_actor);
};