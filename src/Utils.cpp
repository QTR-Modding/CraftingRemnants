#include "Utils.h"
#include "CLibUtilsQTR/FormReader.hpp"

int Utilities::MsgBoxesNotifs::Windows::Po3ErrMsg() {
    MessageBoxA(nullptr, po3_err_msgbox.c_str(), "Error", MB_OK | MB_ICONERROR);
    return 1;
}

bool Utilities::IsPo3Installed() { return std::filesystem::exists(po3path);}

std::string Utilities::Functions::String::trim(const std::string& str) {
    // Find the first non-whitespace character from the beginning
    const size_t start = str.find_first_not_of(" \t\n\r");

    // If the string is all whitespace, return an empty string
    if (start == std::string::npos) return "";

    // Find the last non-whitespace character from the end
    const size_t end = str.find_last_not_of(" \t\n\r");

    // Return the substring containing the trimmed characters
    return str.substr(start, end - start + 1);
}
std::string Utilities::Functions::String::toLowercase(const std::string& str) {
    std::string result = str;
    std::ranges::transform(result, result.begin(),
                           [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}
std::string Utilities::Functions::String::replaceLineBreaksWithSpace(const std::string& input) {
    std::string result = input;
    std::ranges::replace(result, '\n', ' ');
    return result;
}
bool Utilities::Functions::String::includesString(const std::string& input, const std::vector<std::string>& strings) {
    const std::string lowerInput = toLowercase(input);

    for (const auto& str : strings) {
        std::string lowerStr = str;
        std::ranges::transform(lowerStr, lowerStr.begin(),
                               [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (lowerInput.find(lowerStr) != std::string::npos) {
            return true;  // The input string includes one of the strings
        }
    }
    return false;  // None of the strings in 'strings' were found in the input string
}
bool Utilities::Functions::String::includesWord(const std::string& input, const std::vector<std::string>& strings) {
    std::string lowerInput = toLowercase(input);
    lowerInput = replaceLineBreaksWithSpace(lowerInput);
    lowerInput = trim(lowerInput);
    lowerInput = " " + lowerInput + " ";  // Add spaces to the beginning and end of the string

    for (const auto& str : strings) {
        std::string lowerStr = str;
        lowerStr = trim(lowerStr);
        lowerStr = " " + lowerStr + " ";  // Add spaces to the beginning and end of the string
        std::ranges::transform(lowerStr, lowerStr.begin(),
                               [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

        // logger::trace("lowerInput: {} lowerStr: {}", lowerInput, lowerStr);

        if (lowerInput.find(lowerStr) != std::string::npos) {
            return true;  // The input string includes one of the strings
        }
    }
    return false;  // None of the strings in 'strings' were found in the input string
};
