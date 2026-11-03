#pragma once
#include "Manager.h"
#include "ClibUtil/singleton.hpp"

class EventSink final :public RE::BSTEventSink<RE::TESContainerChangedEvent>,
                       public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
                       public RE::BSTEventSink<RE::ItemCrafted::Event>,
                       public clib_util::singleton::ISingleton<EventSink>
{
    RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent* event,
                                          RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;

    RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* event,
                                          RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;

    RE::BSEventNotifyControl ProcessEvent(const RE::ItemCrafted::Event* a_event,
                                             RE::BSTEventSource<RE::ItemCrafted::Event>*) override;

    RE::TESFurniture* bench = nullptr;
};
