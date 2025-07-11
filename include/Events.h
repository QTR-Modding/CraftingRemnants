#pragma once
#include "Manager.h"
#include "ClibUtil/singleton.hpp"

class EventSink final :public RE::BSTEventSink<RE::TESContainerChangedEvent>,
                       public RE::BSTEventSink<RE::TESFurnitureEvent>,
                       public RE::BSTEventSink<RE::ItemCrafted::Event>,
                       public clib_util::singleton::ISingleton<EventSink>
{
    RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent* event,
                                          RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;

    RE::BSEventNotifyControl ProcessEvent(const RE::TESFurnitureEvent* event,
                                          RE::BSTEventSource<RE::TESFurnitureEvent>*) override;

    RE::BSEventNotifyControl ProcessEvent(const RE::ItemCrafted::Event* a_event,
                                             RE::BSTEventSource<RE::ItemCrafted::Event>*) override;

    void OnBenchEnter(RE::TESFurniture* a_bench, std::uint8_t bench_no);
    void OnBenchExit();

    std::pair<RE::TESFurniture*,std::uint8_t> bench = {nullptr,0};
};
						