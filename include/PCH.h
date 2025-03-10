#pragma once

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include <spdlog/sinks/basic_file_sink.h>

namespace logger = SKSE::log;
using namespace std::literals;

using FormID = RE::FormID;
using RefID = RE::FormID;
using Count = std::uint32_t;

using Remnants = std::map<std::pair<FormID, std::pair<std::uint8_t,FormID>>, Count>;

constexpr RefID player_refid = 20;