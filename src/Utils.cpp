#include "Utils.h"
#include "CLibUtilsQTR/FormReader.hpp"

RE::TESForm* Utilities::FunctionsSkyrim::GetFormByID(const RE::FormID id, const std::string& editor_id) {
    if (!editor_id.empty()) {
        auto* form = RE::TESForm::LookupByEditorID(editor_id);
        if (form) return form;
    }
    auto form = RE::TESForm::LookupByID(id);
    if (form) return form;
    return nullptr;
}

FormID Utilities::FunctionsSkyrim::GetFormEditorIDFromString(const std::string& formEditorId)
{
    static std::string delimiter = "~";
	const auto plugin_and_localid = FormReader::split(formEditorId, delimiter);
	if (plugin_and_localid.size() == 2) {
		const auto& plugin_name = plugin_and_localid[1];
		const auto local_id = FormReader::GetFormIDFromString(plugin_and_localid[0]);
		const auto formid = FormReader::GetForm(plugin_name.c_str(), local_id);
		if (const auto form = RE::TESForm::LookupByID(formid)) return form->GetFormID();
	}

    if (Functions::isValidHexWithLength7or8(formEditorId.c_str())) {
        int form_id_;
        std::stringstream ss;
        ss << std::hex << formEditorId;
        ss >> form_id_;
        if (const auto temp_form = GetFormByID(form_id_, "")) return temp_form->GetFormID();
        logger::warn("Formid is null for editorid {}", formEditorId);
        return 0;
    }
    if (formEditorId.empty()) return 0;
    if (!IsPo3Installed()) {
        logger::error("Po3 is not installed.");
        MsgBoxesNotifs::Windows::Po3ErrMsg();
        return 0;
    }
    if (const auto temp_form = GetFormByID(0, formEditorId)) return temp_form->GetFormID();
    return 0;
}

bool Utilities::Functions::isValidHexWithLength7or8(const char* input) { 
    std::string inputStr(input);

    if (inputStr.substr(0, 2) == "0x") {
        // Remove "0x" from the beginning of the string
        inputStr = inputStr.substr(2);
    }

    std::regex hexRegex("^[0-9A-Fa-f]{7,8}$");  // Allow 7 to 8 characters
    bool isValid = std::regex_match(inputStr, hexRegex);
    return isValid;
}

int Utilities::MsgBoxesNotifs::Windows::Po3ErrMsg() {
    MessageBoxA(nullptr, po3_err_msgbox.c_str(), "Error", MB_OK | MB_ICONERROR);
    return 1;
}

bool Utilities::IsPo3Installed() { return std::filesystem::exists(po3path);}

std::string Utilities::Functions::String::trim(const std::string& str) {
    // Find the first non-whitespace character from the beginning
    size_t start = str.find_first_not_of(" \t\n\r");

    // If the string is all whitespace, return an empty string
    if (start == std::string::npos) return "";

    // Find the last non-whitespace character from the end
    size_t end = str.find_last_not_of(" \t\n\r");

    // Return the substring containing the trimmed characters
    return str.substr(start, end - start + 1);
}
std::string Utilities::Functions::String::toLowercase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}
std::string Utilities::Functions::String::replaceLineBreaksWithSpace(const std::string& input) {
    std::string result = input;
    std::replace(result.begin(), result.end(), '\n', ' ');
    return result;
}
bool Utilities::Functions::String::includesString(const std::string& input, const std::vector<std::string>& strings) {
    std::string lowerInput = toLowercase(input);

    for (const auto& str : strings) {
        std::string lowerStr = str;
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
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
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

        // logger::trace("lowerInput: {} lowerStr: {}", lowerInput, lowerStr);

        if (lowerInput.find(lowerStr) != std::string::npos) {
            return true;  // The input string includes one of the strings
        }
    }
    return false;  // None of the strings in 'strings' were found in the input string
};
