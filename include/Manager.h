#pragma once
#include "Settings.h"

class Manager {
public:
    static void ProcessRemnants(const RemnantCatalyst& a_catalyst, const RE::TESFurniture* a_bench);

#ifndef NDEBUG
    static void Print() {
        std::shared_lock lock(PresetSettings::presets_mutex_);
	    logger::info("Current Remnants Presets:");
	    for (const auto& [owner, remnant_conditions] : PresetSettings::presets) {
		    logger::info("Owner: Catalyst: {:x}, settings type {},Bench ID: {}", owner.catalyst.first,
                         owner.catalyst.second, owner.bench_id);
		    remnant_conditions.Print();
	    }
    }
#endif
private:
    static Remnants GetRemnants(const RemnantsOwner& a_owner, const RE::TESFurniture* a_bench);
};
