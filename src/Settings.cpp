#include "Settings.h"
#include "Utils.h"
#include "CLibUtilsQTR/PresetHelpers/PresetHelpersTXT.hpp"
#include "CLibUtilsQTR/PresetHelpers/PresetHelpersYAML.hpp"

bool PresetSettings::OwnerNodeHealthy(const YAML::Node& ownerNode)
{
    static const std::array<std::string, 4> keys_o = { "owners","benches", "type", "remnants" };

    if (!std::ranges::all_of(keys_o, [&](const std::string& key) { return NodeExists(ownerNode, key); })) {
        logger::error("Not all required keys are present in owner node");
		return false;
    }
    if (const auto type = static_cast<RemnantSettingsType>(ownerNode["type"].as<uint8_t>()); type == RemnantSettingsType::kNone || type >= RemnantSettingsType::kTotal) {
		logger::error("Invalid type for owner node");
        return false;
	}

	auto& remnants_node = ownerNode["remnants"];
    static const std::array<std::string, 2> keys_r = { "FormEditorID","count"};
    for (auto& a_node : remnants_node) {
        if (!std::ranges::all_of(keys_r, [&](const std::string& key) { return NodeExists(a_node, key); })) {
            logger::error("Not all required keys are present in remnants node");
		    return false;
        }
        if (const Count count = a_node["count"].as<Count>(); count <= 0) {
		    logger::error("Count is not valid in remnants node");
			return false;
        }
	}
    return true;
}

bool PresetSettings::NodeExists(const YAML::Node& node, const std::string& key)
{
	return node[key] && !node[key].IsNull() && node[key].IsDefined();
}

void PresetSettings::LoadFormGroups()
{
	const auto folder_path = std::format("Data/SKSE/Plugins/{}", Utilities::mod_name) + "/formGroups";
    PresetHelpers::TXT_Helpers::GatherForms(folder_path);
}

void PresetSettings::LoadPresets()
{
    const auto folder_path = std::format("Data/SKSE/Plugins/{}", Utilities::mod_name) + "/presets";
    if (!std::filesystem::exists(folder_path)) {
        logger::error("Presets folder does not exist: {}", folder_path);
        return;
	}

    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
		if (!entry.is_regular_file() || entry.path().extension() != ".yml") continue;

        const auto filename = entry.path().string();
        YAML::Node config = YAML::LoadFile(filename);
        if (!config["ownerLists"]) {
            logger::trace("OwnerLists not found in {}", filename);
            continue;
		}

        for (const auto& ownerNode : config["ownerLists"]) {
            if (!OwnerNodeHealthy(ownerNode)) {
                logger::error("Owner node is not healthy in file: {}", filename);
                continue;
			}

			using namespace PresetHelpers::YAML_Helpers;
            for (const auto& owner_id : CollectFrom<FormID,std::string>(ownerNode,"owners")) {
				for (const auto& bench_id : CollectFrom<uint8_t>(ownerNode, "benches")) {

                    RemnantsOwner remnants_owner(
                        RemnantCatalyst(owner_id, static_cast<RemnantSettingsType>(ownerNode["type"].as<uint8_t>())),
                        bench_id);

                    // KWs
				    std::vector<RE::BGSKeyword*> bench_keywords;
					if (NodeExists(ownerNode, "bench_keywords")) {
						for (const auto& bench_keyword : CollectFrom<FormID,std::string>(ownerNode,"bench_keywords")) {
							if (auto a_kw = FormReader::GetFormByID<RE::BGSKeyword>(bench_keyword)) {
							    bench_keywords.push_back(a_kw);
							}
						}
					}
					// Remnants
				    Remnants remnants;
                    for (auto a_remnants_node : ownerNode["remnants"]) {
				        for (const auto a_formid : CollectFrom<FormID, std::string>(a_remnants_node, "FormEditorID")) {
                            remnants[FormReader::GetFormByID<RE::TESBoundObject>(a_formid)] = a_remnants_node["count"].as<Count>();
                        }
                    }

				    std::unique_lock lock(presets_mutex_);
                    presets[remnants_owner] = {bench_keywords,remnants};
                }
            }
        }
    }
}

void PresetSettings::LoadSettings()
{
	LoadFormGroups();
    LoadPresets();
}

bool RemnantsWithConditions::Conditions::Match(const RE::TESBoundObject* a_bench) const {
	if (bench_keywords.empty()) return true; // No keyword means no condition
	if (!a_bench) return false;
	if (a_bench->HasKeywordInArray(bench_keywords,false)) return true; // Owner has the keyword
    return false;
}

Remnants RemnantsWithConditions::Get(const RE::TESBoundObject* a_bench) {
	return conditions.Match(a_bench) ? remnants : Remnants{};
}

void RemnantsWithConditions::Print() const {
    logger::info("Bench Keywords:");
    for (const auto& kw : conditions.bench_keywords) {
        logger::info(" - {}", kw->GetName());
	}
    logger::info("Remnants:");
    for (const auto& [remnant, count] : remnants) {
        logger::info(" - {}: {}", remnant->GetName(), count);
	}
}
