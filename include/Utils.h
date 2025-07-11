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


    namespace Functions {

        namespace String {

            template <typename T>
            std::string join(const T& container, const std::string_view& delimiter) {
                std::ostringstream oss;
                auto iter = container.begin();

                if (iter != container.end()) {
                    oss << *iter;
                    ++iter;
                }

                for (; iter != container.end(); ++iter) {
                    oss << delimiter << *iter;
                }

                return oss.str();
            }

            std::string trim(const std::string& str);

            std::string toLowercase(const std::string& str);

            std::string replaceLineBreaksWithSpace(const std::string& input);

            bool includesString(const std::string& input, const std::vector<std::string>& strings);

            bool includesWord(const std::string& input, const std::vector<std::string>& strings);
        };

	}

};