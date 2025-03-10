#include "Events.h"
#include "Logger.h"

void OnMessage(SKSE::MessagingInterface::Message* message) {
    if (message->type == SKSE::MessagingInterface::kDataLoaded) {
        // Start
        auto* eventSink = ourEventSink::GetSingleton();
        auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventSourceHolder->AddEventSink<RE::TESContainerChangedEvent>(eventSink);
        eventSourceHolder->AddEventSink<RE::TESFurnitureEvent>(eventSink);
    }
    if (message->type == SKSE::MessagingInterface::kNewGame || message->type == SKSE::MessagingInterface::kPostLoadGame) {
        // Post-load
    }
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {

    SetupLog();
    logger::info("Plugin loaded");
    SKSE::Init(skse);
    SKSE::GetMessagingInterface()->RegisterListener(OnMessage);
    return true;
}