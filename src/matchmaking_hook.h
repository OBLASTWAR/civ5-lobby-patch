#pragma once
#include <windows.h>

void  InitMatchmakingHook(HMODULE hOrigDll);
void* GetHookedSteamMatchmaking();
