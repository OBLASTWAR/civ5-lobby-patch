#include "matchmaking_hook.h"

// ISteamMatchmaking vtable indices (ISteamMatchmaking008/009, Steamworks ~2013)
//   0  GetFavoriteGameCount
//   1  GetFavoriteGame
//   2  AddFavoriteGame
//   3  RemoveFavoriteGame
//   4  RequestLobbyList                    <-- we hook this
//   5  AddRequestLobbyListStringFilter
//   6  AddRequestLobbyListNumericalFilter
//   7  AddRequestLobbyListNearValueFilter
//   8  AddRequestLobbyListFilterSlotsAvailable
//   9  AddRequestLobbyListDistanceFilter   <-- we call this inside our hook
//  10  AddRequestLobbyListResultCountFilter
//  11  GetLobbyByIndex ...
static const int VTIDX_RequestLobbyList              = 4;
static const int VTIDX_AddRequestLobbyListDistFilter = 9;

// k_ELobbyDistanceFilterWorldwide = 3  (from Steamworks SDK isteammatchmaking.h)
static const int DISTANCE_WORLDWIDE = 3;

// Calling convention note:
//   ISteamMatchmaking virtual functions use __thiscall on 32-bit MSVC.
//   To implement a __thiscall hook as a free function, we use __fastcall
//   with a dummy second parameter that receives the EDX register
//   (ignored by __thiscall, but required by __fastcall's signature).

typedef unsigned __int64 (__thiscall *FnRequestLobbyList)   (void* pThis);
typedef void              (__thiscall *FnAddDistanceFilter)  (void* pThis, int eFilter);

static FnRequestLobbyList g_origRequestLobbyList = nullptr;
static void*              g_pMatchmaking         = nullptr;

// Replacement for ISteamMatchmaking::RequestLobbyList().
// Injects k_ELobbyDistanceFilterWorldwide before forwarding the call so that
// whatever regional filter the engine set is overridden.
static unsigned __int64 __fastcall HookedRequestLobbyList(void* pThis, void* /*edx_unused*/)
{
    void** vtable = *(void***)pThis;

    // vtable[9] is still the original AddRequestLobbyListDistanceFilter
    FnAddDistanceFilter addDist = (FnAddDistanceFilter)vtable[VTIDX_AddRequestLobbyListDistFilter];
    addDist(pThis, DISTANCE_WORLDWIDE);

    return g_origRequestLobbyList(pThis);
}

void InitMatchmakingHook(HMODULE hOrigDll)
{
    typedef void* (*SteamMatchmakingFn)();
    SteamMatchmakingFn realFn =
        (SteamMatchmakingFn)GetProcAddress(hOrigDll, "SteamMatchmaking");
    if (!realFn) return;

    g_pMatchmaking = realFn();
    if (!g_pMatchmaking) return;

    void** vtable = *(void***)g_pMatchmaking;

    // Save original before patching
    g_origRequestLobbyList = (FnRequestLobbyList)vtable[VTIDX_RequestLobbyList];

    // vtable lives in a read-only page; make it writable long enough to patch
    DWORD oldProtect;
    VirtualProtect(&vtable[VTIDX_RequestLobbyList], sizeof(void*),
                   PAGE_EXECUTE_READWRITE, &oldProtect);
    vtable[VTIDX_RequestLobbyList] = (void*)HookedRequestLobbyList;
    VirtualProtect(&vtable[VTIDX_RequestLobbyList], sizeof(void*),
                   oldProtect, &oldProtect);
}

void* GetHookedSteamMatchmaking()
{
    return g_pMatchmaking;
}
