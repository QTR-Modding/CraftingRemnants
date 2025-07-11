#include "Utils.h"

int Utilities::MsgBoxesNotifs::Windows::Po3ErrMsg() {
    MessageBoxA(nullptr, po3_err_msgbox.c_str(), "Error", MB_OK | MB_ICONERROR);
    return 1;
}

bool Utilities::IsPo3Installed() { return std::filesystem::exists(po3path);}
