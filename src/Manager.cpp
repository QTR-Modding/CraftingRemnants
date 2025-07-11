#include "Manager.h"

Remnants Manager::GetRemnants(const RemnantsOwner& a_owner, const RE::TESFurniture* a_bench)
{
	std::shared_lock lock(PresetSettings::presets_mutex_);
	if (PresetSettings::presets.contains(a_owner)) {
		return PresetSettings::presets.at(a_owner).Get(a_bench);
	}
	return Remnants{};
}

void Manager::ProcessRemnants(const Remnants& remnants)
{
    if (remnants.empty()) {
        return;
    }
    const auto player = RE::PlayerCharacter::GetSingleton();
	for (const auto& [remnant, count] : remnants) {
        if (count == 0) continue;
        player->AddObjectToContainer(remnant, nullptr, count, nullptr);
	}
}
