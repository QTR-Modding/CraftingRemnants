#pragma once
#include "Settings.h"

class Manager {

    CustomPresets presets;
    std::map<FormID, Remnants> sources;
    std::set<std::uint8_t> benches;

    Remnants MakeSource(const FormID item, const Remnants& remnants);

public:
    Manager() { Init(); };

    static Manager* GetSingleton() {
        static Manager singleton;
        return &singleton;
    }

    [[nodiscard]] bool IsBenchSupported(const std::uint8_t bench) const;

    Remnants GetRemnants(FormID used_item_formid);

    void Init();
};