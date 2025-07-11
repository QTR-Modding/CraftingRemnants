#include "Events.h"

RE::BSEventNotifyControl EventSink::ProcessEvent(const RE::TESContainerChangedEvent* event,
                                                    RE::BSTEventSource<RE::TESContainerChangedEvent>*) {
    
    if (!event) return RE::BSEventNotifyControl::kContinue;
    if (bench.second == 0) return RE::BSEventNotifyControl::kContinue;
    if (event->oldContainer!=player_refid) return RE::BSEventNotifyControl::kContinue;

	constexpr RemnantSettingsType remnant_type = RemnantSettingsType::kOnRemoved;

    const RemnantCatalyst a_catalyst(event->baseObj,remnant_type);
	const RemnantsOwner remnants_owner(a_catalyst,bench.second);

    const auto remnants = Manager::GetRemnants(remnants_owner,bench.first);
    Manager::ProcessRemnants(remnants);

    return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl EventSink::ProcessEvent(const RE::TESFurnitureEvent* event,
                                                    RE::BSTEventSource<RE::TESFurnitureEvent>*) {
    
    if (!event) return RE::BSEventNotifyControl::kContinue;
    if (!event->actor) return RE::BSEventNotifyControl::kContinue;
    if (!event->actor->IsPlayerRef()) return RE::BSEventNotifyControl::kContinue;
    if (bench.second!=0 && event->type == RE::TESFurnitureEvent::FurnitureEventType::kEnter) {
        return RE::BSEventNotifyControl::kContinue;
    }
    if (bench.second == 0 && event->type != RE::TESFurnitureEvent::FurnitureEventType::kEnter) {
        return RE::BSEventNotifyControl::kContinue;
    }
    if (bench.second != 0 && event->type == RE::TESFurnitureEvent::FurnitureEventType::kExit) {
        OnBenchExit();
		return RE::BSEventNotifyControl::kContinue;
	}

    if (!event->targetFurniture) return RE::BSEventNotifyControl::kContinue;
    
    const auto furn_base = event->targetFurniture->GetBaseObject();
    if (!furn_base) return RE::BSEventNotifyControl::kContinue;
    if (furn_base->formType.underlying() != 40) return RE::BSEventNotifyControl::kContinue;

    const auto temp_bench = furn_base->As<RE::TESFurniture>();
    if (!temp_bench) return RE::BSEventNotifyControl::kContinue;
    const auto bench_no = static_cast<std::uint8_t>(temp_bench->workBenchData.benchType.get());
    
    OnBenchEnter(temp_bench,bench_no);
    
    return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl EventSink::ProcessEvent(const RE::ItemCrafted::Event* a_event, RE::BSTEventSource<RE::ItemCrafted::Event>*)
{
    if (!a_event || !a_event->item) {
        return RE::BSEventNotifyControl::kContinue;
    }

	constexpr RemnantSettingsType remnant_type = RemnantSettingsType::kOnCrafted;

    const RemnantCatalyst a_catalyst(a_event->item->GetFormID(),remnant_type);
	const RemnantsOwner remnants_owner(a_catalyst,bench.second);

    const auto remnants = Manager::GetRemnants(remnants_owner,bench.first);
    Manager::ProcessRemnants(remnants);
    return RE::BSEventNotifyControl::kContinue;
}

void EventSink::OnBenchEnter(RE::TESFurniture* a_bench, const std::uint8_t bench_no) {
    if (bench_no == 0) return;
	bench = {a_bench,bench_no};
}

void EventSink::OnBenchExit() {
	bench = { nullptr,0 };
}
