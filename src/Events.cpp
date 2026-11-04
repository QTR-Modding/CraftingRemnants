#include "Events.h"
#include "Utils.h"

RE::BSEventNotifyControl EventSink::ProcessEvent(const RE::TESContainerChangedEvent* event,
                                                    RE::BSTEventSource<RE::TESContainerChangedEvent>*) {
    
    if (!event) return RE::BSEventNotifyControl::kContinue;
    if (event->oldContainer!=player_refid) return RE::BSEventNotifyControl::kContinue;

    Manager::ProcessRemnants({event->baseObj, RemnantSettingsType::kOnRemoved}, bench, event->itemCount);

    return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl EventSink::ProcessEvent(const RE::MenuOpenCloseEvent* event,
                                                    RE::BSTEventSource<RE::MenuOpenCloseEvent>*) {
    
    if (!event) return RE::BSEventNotifyControl::kContinue;
    if (event->menuName != RE::CraftingMenu::MENU_NAME) return RE::BSEventNotifyControl::kContinue;

    bench = event->opening ? Utilities::GetOccupiedFurnitureBase(RE::PlayerCharacter::GetSingleton()) : nullptr;

#ifndef NDEBUG
    if (event->opening) Manager::Print();
#endif
    
    return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl EventSink::ProcessEvent(const RE::ItemCrafted::Event* a_event, RE::BSTEventSource<RE::ItemCrafted::Event>*)
{
    if (!a_event || !a_event->item) {
        return RE::BSEventNotifyControl::kContinue;
    }

    Manager::ProcessRemnants({a_event->item->GetFormID(), RemnantSettingsType::kOnCrafted}, bench);
    return RE::BSEventNotifyControl::kContinue;
}
