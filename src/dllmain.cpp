#include <windows.h>
#include "matchmaking_hook.h"

// Exported to satisfy the g_pSteamClientGameServer data import.
// This is a game-server-side symbol; the client game never reads it.
extern "C" __declspec(dllexport) void* g_pSteamClientGameServer = nullptr;

static HMODULE g_hOrigDll = nullptr;

// The one export we intercept. All other steam_api exports are forwarded
// directly to steam_api_orig.dll via the .def file.
extern "C" __declspec(dllexport) void* __cdecl SteamMatchmaking()
{
    // Lazy init: patch the vtable on first call (Steam must be running by now)
    static bool s_patched = false;
    if (!s_patched && g_hOrigDll) {
        s_patched = true;
        InitMatchmakingHook(g_hOrigDll);
    }
    return GetHookedSteamMatchmaking();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID /*reserved*/)
{
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        // Load the real steam_api alongside us; forwarded exports resolve through it
        g_hOrigDll = LoadLibraryA("steam_api_orig.dll");
    }
    else if (reason == DLL_PROCESS_DETACH) {
        if (g_hOrigDll) {
            FreeLibrary(g_hOrigDll);
            g_hOrigDll = nullptr;
        }
    }
    return TRUE;
}
