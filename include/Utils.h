#pragma once
#include <iostream>
#include <string>

namespace Utilities {

    const auto mod_name = static_cast<std::string>(SKSE::PluginDeclaration::GetSingleton()->GetName());

    constexpr auto po3path = "Data/SKSE/Plugins/po3_Tweaks.dll";
    bool IsPo3Installed();
    const auto po3_err_msgbox = std::format(
        "{}: If you are trying to use Editor IDs, but you must have powerofthree's Tweaks "
        "installed. See mod page for further instructions.",
        mod_name);

    namespace MsgBoxesNotifs {
        namespace Windows {

            int Po3ErrMsg();
        }
    };
};