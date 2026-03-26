#include <windows.h>
#include "matchmaking_hook.h"

// Exported to satisfy the g_pSteamClientGameServer data import.
// This is a game-server-side symbol; the client game never reads it.
extern "C" __declspec(dllexport) void* g_pSteamClientGameServer = nullptr;

// Forward all other steam_api exports directly to the original DLL.
// #pragma comment(linker) is more reliable than .def forwarding in modern MSVC.
#pragma comment(linker, "/export:GetHSteamPipe=steam_api_orig.GetHSteamPipe")
#pragma comment(linker, "/export:GetHSteamUser=steam_api_orig.GetHSteamUser")
#pragma comment(linker, "/export:Steam_GetHSteamUserCurrent=steam_api_orig.Steam_GetHSteamUserCurrent")
#pragma comment(linker, "/export:Steam_RegisterInterfaceFuncs=steam_api_orig.Steam_RegisterInterfaceFuncs")
#pragma comment(linker, "/export:Steam_RunCallbacks=steam_api_orig.Steam_RunCallbacks")
#pragma comment(linker, "/export:SteamAPI_GetHSteamPipe=steam_api_orig.SteamAPI_GetHSteamPipe")
#pragma comment(linker, "/export:SteamAPI_GetHSteamUser=steam_api_orig.SteamAPI_GetHSteamUser")
#pragma comment(linker, "/export:SteamAPI_GetSteamInstallPath=steam_api_orig.SteamAPI_GetSteamInstallPath")
#pragma comment(linker, "/export:SteamAPI_Init=steam_api_orig.SteamAPI_Init")
#pragma comment(linker, "/export:SteamAPI_InitSafe=steam_api_orig.SteamAPI_InitSafe")
#pragma comment(linker, "/export:SteamAPI_IsSteamRunning=steam_api_orig.SteamAPI_IsSteamRunning")
#pragma comment(linker, "/export:SteamAPI_RegisterCallback=steam_api_orig.SteamAPI_RegisterCallback")
#pragma comment(linker, "/export:SteamAPI_RegisterCallResult=steam_api_orig.SteamAPI_RegisterCallResult")
#pragma comment(linker, "/export:SteamAPI_RestartAppIfNecessary=steam_api_orig.SteamAPI_RestartAppIfNecessary")
#pragma comment(linker, "/export:SteamAPI_RunCallbacks=steam_api_orig.SteamAPI_RunCallbacks")
#pragma comment(linker, "/export:SteamAPI_SetBreakpadAppID=steam_api_orig.SteamAPI_SetBreakpadAppID")
#pragma comment(linker, "/export:SteamAPI_SetMiniDumpComment=steam_api_orig.SteamAPI_SetMiniDumpComment")
#pragma comment(linker, "/export:SteamAPI_SetTryCatchCallbacks=steam_api_orig.SteamAPI_SetTryCatchCallbacks")
#pragma comment(linker, "/export:SteamAPI_Shutdown=steam_api_orig.SteamAPI_Shutdown")
#pragma comment(linker, "/export:SteamAPI_UnregisterCallback=steam_api_orig.SteamAPI_UnregisterCallback")
#pragma comment(linker, "/export:SteamAPI_UnregisterCallResult=steam_api_orig.SteamAPI_UnregisterCallResult")
#pragma comment(linker, "/export:SteamAPI_UseBreakpadCrashHandler=steam_api_orig.SteamAPI_UseBreakpadCrashHandler")
#pragma comment(linker, "/export:SteamAPI_WriteMiniDump=steam_api_orig.SteamAPI_WriteMiniDump")
#pragma comment(linker, "/export:SteamApps=steam_api_orig.SteamApps")
#pragma comment(linker, "/export:SteamClient=steam_api_orig.SteamClient")
#pragma comment(linker, "/export:SteamContentServer=steam_api_orig.SteamContentServer")
#pragma comment(linker, "/export:SteamContentServer_Init=steam_api_orig.SteamContentServer_Init")
#pragma comment(linker, "/export:SteamContentServer_RunCallbacks=steam_api_orig.SteamContentServer_RunCallbacks")
#pragma comment(linker, "/export:SteamContentServer_Shutdown=steam_api_orig.SteamContentServer_Shutdown")
#pragma comment(linker, "/export:SteamContentServerUtils=steam_api_orig.SteamContentServerUtils")
#pragma comment(linker, "/export:SteamFriends=steam_api_orig.SteamFriends")
#pragma comment(linker, "/export:SteamGameServer=steam_api_orig.SteamGameServer")
#pragma comment(linker, "/export:SteamGameServer_BSecure=steam_api_orig.SteamGameServer_BSecure")
#pragma comment(linker, "/export:SteamGameServer_GetHSteamPipe=steam_api_orig.SteamGameServer_GetHSteamPipe")
#pragma comment(linker, "/export:SteamGameServer_GetHSteamUser=steam_api_orig.SteamGameServer_GetHSteamUser")
#pragma comment(linker, "/export:SteamGameServer_GetIPCCallCount=steam_api_orig.SteamGameServer_GetIPCCallCount")
#pragma comment(linker, "/export:SteamGameServer_GetSteamID=steam_api_orig.SteamGameServer_GetSteamID")
#pragma comment(linker, "/export:SteamGameServer_Init=steam_api_orig.SteamGameServer_Init")
#pragma comment(linker, "/export:SteamGameServer_InitSafe=steam_api_orig.SteamGameServer_InitSafe")
#pragma comment(linker, "/export:SteamGameServer_RunCallbacks=steam_api_orig.SteamGameServer_RunCallbacks")
#pragma comment(linker, "/export:SteamGameServer_Shutdown=steam_api_orig.SteamGameServer_Shutdown")
#pragma comment(linker, "/export:SteamGameServerApps=steam_api_orig.SteamGameServerApps")
#pragma comment(linker, "/export:SteamGameServerHTTP=steam_api_orig.SteamGameServerHTTP")
#pragma comment(linker, "/export:SteamGameServerNetworking=steam_api_orig.SteamGameServerNetworking")
#pragma comment(linker, "/export:SteamGameServerStats=steam_api_orig.SteamGameServerStats")
#pragma comment(linker, "/export:SteamGameServerUtils=steam_api_orig.SteamGameServerUtils")
#pragma comment(linker, "/export:SteamHTTP=steam_api_orig.SteamHTTP")
#pragma comment(linker, "/export:SteamMatchmakingServers=steam_api_orig.SteamMatchmakingServers")
#pragma comment(linker, "/export:SteamNetworking=steam_api_orig.SteamNetworking")
#pragma comment(linker, "/export:SteamRemoteStorage=steam_api_orig.SteamRemoteStorage")
#pragma comment(linker, "/export:SteamScreenshots=steam_api_orig.SteamScreenshots")
#pragma comment(linker, "/export:SteamUser=steam_api_orig.SteamUser")
#pragma comment(linker, "/export:SteamUserStats=steam_api_orig.SteamUserStats")
#pragma comment(linker, "/export:SteamUtils=steam_api_orig.SteamUtils")

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
