
#pragma once
#include "Utils.h"

struct Preset {

    Remnants items;  // rest items

    [[nodiscard]] bool IsHealthy() const { return !init_failed; }

    [[nodiscard]] bool CheckIntegrity();

private:
    bool init_failed = false;

    bool InitFailed() {
        init_failed = true;
        return false;
    }
};

using CustomPresets = std::map<std::vector<std::string>, Preset>;

Preset parsePreset_(const YAML::Node& config);
CustomPresets parsePresets();