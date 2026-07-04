#include <windows.h>
#include "hooks/hooks.hpp"
#include "../shared/config.hpp"

static DWORD WINAPI cheat_thread(LPVOID) {
    // TODO: resolve interfaces (IClientEntityList, IEngineClient, etc.)
    // TODO: install hooks
    // TODO: main loop (CreateMove override, PaintTraverse overlay, etc.)
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        CreateThread(nullptr, 0, cheat_thread, nullptr, 0, nullptr);
    }
    return TRUE;
}
