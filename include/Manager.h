#pragma once
#include "Settings.h"
#include "ClibUtil/singleton.hpp"

class Manager : public clib_util::singleton::ISingleton<Manager>{
public:
    static Remnants GetRemnants(const RemnantsOwner& a_owner, const RE::TESFurniture* a_bench);
    static void ProcessRemnants(const Remnants& remnants);

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
};