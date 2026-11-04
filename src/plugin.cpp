#include "Events.h"
#include "CLibUtilsQTR/Logging.hpp"

void OnMessage(SKSE::MessagingInterface::Message* message) {
    if (message->type == SKSE::MessagingInterface::kDataLoaded) {
        // Start

        PresetSettings::LoadSettings();

        auto* eventSink = EventSink::GetSingleton();
        auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventSourceHolder->AddEventSink<RE::TESContainerChangedEvent>(eventSink);
        RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(eventSink);
        auto itemcraftedsource = RE::ItemCrafted::GetEventSource();
        itemcraftedsource->AddEventSink(eventSink);
    }
    if (message->type == SKSE::MessagingInterface::kNewGame || message->type == SKSE::MessagingInterface::kPostLoadGame) {
        // Post-load
    }
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {

    clib_utilsQTR::SetupLog();
    logger::info("Plugin loaded");
    SKSE::Init(skse, false);
    SKSE::GetMessagingInterface()->RegisterListener(OnMessage);
    return true;
}