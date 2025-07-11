#pragma once
#include <shared_mutex>
#include <unordered_set>
#include "Utils.h"

enum RemnantSettingsType : uint8_t {
    kNone = 0,
    kOnRemoved = 1,
    kOnCrafted = 2,
    kTotal = 3
};

using Remnants = std::unordered_map<RE::TESBoundObject*, Count>;
using RemnantCatalyst = std::pair<FormID,RemnantSettingsType>;

struct RemnantsOwner {
	RemnantCatalyst catalyst;
    uint8_t bench_id;
    bool operator==(const RemnantsOwner& other) const {
        return catalyst == other.catalyst && bench_id == other.bench_id;
	}
};

struct RemnantsWithConditions {
    struct Conditions {
        std::vector<RE::BGSKeyword*> bench_keywords;
        bool Match(const RE::TESBoundObject* a_bench) const;
    };
    Remnants Get(const RE::TESBoundObject* a_bench);
    RemnantsWithConditions(const std::vector<RE::BGSKeyword*>& bench_keywords, const Remnants& remnants)
		: remnants(remnants), conditions(bench_keywords) {
	}
    RemnantsWithConditions() = default;

    void Print() const;

private:
    Remnants remnants;
	Conditions conditions;

};

inline void hash_combine(std::size_t& seed, std::size_t value) noexcept {
    // 0x9e3779b9 is the golden ratio, helps spread bits
    seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {

    template<>
    struct hash<RemnantCatalyst> {
        size_t operator()(const RemnantCatalyst& rc) const noexcept {
            size_t seed = 0;
            hash_combine(seed, std::hash<FormID>{}(rc.first));
            hash_combine(seed, std::hash<uint8_t>{}(rc.second));
            return seed;
        }
    };

    template<>
    struct hash<RemnantsOwner> {
        size_t operator()(const RemnantsOwner& ro) const noexcept {
            size_t seed = 0;
            hash_combine(seed, std::hash<RemnantCatalyst>{}(ro.catalyst));
            hash_combine(seed, std::hash<uint8_t>{}(ro.bench_id));
            return seed;
        }
    };
}

namespace PresetSettings {
    
    inline std::shared_mutex formGroups_mutex_;
    inline std::unordered_map<std::string, std::unordered_set<FormID>> formGroups;

	inline std::shared_mutex presets_mutex_;
	inline std::unordered_map<RemnantsOwner, RemnantsWithConditions> presets;


	std::vector<FormID> StringToFormIDs(const std::string& input);
    bool OwnerNodeHealthy(const YAML::Node& ownerNode);
	bool NodeExists(const YAML::Node& node, const std::string& key);
    void LoadFormGroups();
    void LoadPresets();
    void LoadSettings();

	template <typename T, typename U>
    std::vector<T> CollectFrom(const YAML::Node& node, const std::string& key);

    template <typename T>
    std::vector<T> CollectFrom(const YAML::Node& node, const std::string& key);


    template <>
    inline std::vector<FormID> CollectFrom<FormID,std::string>(const YAML::Node& node, const std::string& key) {
        auto res = std::vector<FormID>{};
        if (node[key].IsScalar()) {
			auto temp = StringToFormIDs(node[key].as<std::string>());
			res.insert(res.end(), temp.begin(), temp.end());
        }
		else {
			for (const auto& iterator_value : node[key]) {
				auto temp = StringToFormIDs(iterator_value.as<std::string>());
				res.insert(res.end(), temp.begin(), temp.end());
            }
        }
		return res;
    }

    template<typename T>
    std::vector<T> CollectFrom(const YAML::Node& node, const std::string& key)
    {
		auto res = std::vector<T>{};
        if (node[key].IsScalar()) {
			res.push_back(node[key].as<T>());
		}
		else {
			for (const auto& value : node[key]) {
				res.push_back(value.as<std::uint8_t>());
            }
        }
		return res;
    }

}
