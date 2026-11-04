#include "Manager.h"

Remnants Manager::GetRemnants(const RemnantsOwner& a_owner, const RE::TESFurniture* a_bench)
{
	std::shared_lock lock(PresetSettings::presets_mutex_);
	if (const auto preset = PresetSettings::presets.find(a_owner); preset != PresetSettings::presets.end()) {
		return preset->second.Get(a_bench);
	}
	return Remnants{};
}

void Manager::ProcessRemnants(const RemnantCatalyst& a_catalyst, const RE::TESFurniture* a_bench, Count a_quantity)
{
    if (!a_bench) return;
    const auto bench_type = a_bench->workBenchData.benchType.get();
    if (bench_type == RE::TESFurniture::WorkBenchData::BenchType::kNone) return;
    const auto remnants = GetRemnants({a_catalyst, static_cast<std::uint8_t>(bench_type)}, a_bench);
    if (remnants.empty()) {
        return;
    }
    const auto player = RE::PlayerCharacter::GetSingleton();
	for (const auto& [remnant, count] : remnants) {
        if (count == 0 || a_quantity == 0) continue;
        player->AddObjectToContainer(remnant, nullptr, count * a_quantity, nullptr);
	}
}
