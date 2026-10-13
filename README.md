# civ5-lobby-patch

Allows you to see public lobbies regardless of geography.

Civilization V asks Steam for the multiplayer lobby list with a regional
distance filter, so lobbies hosted far away never show up. This patch makes
every lobby list request worldwide.

## How it works

A proxy `steam_api.dll` in the game's root folder (next to `CivilizationV.exe`).
The game's own DLL is renamed to `steam_api_orig.dll`, and the proxy forwards
every call to it. The only change: when the game calls
`ISteamMatchmaking::RequestLobbyList`, the proxy first sets the distance filter
to `k_ELobbyDistanceFilterWorldwide`.

Nothing else changes. Game rules, saves and multiplayer sync are untouched, so
you can play with people who don't have the patch.

Works on Windows and on Linux under Proton.

## Install

- **[Civ V Mod Installer](https://github.com/OBLASTWAR/civ5-mod-installer):** NETWORK MODS → LOBBY PATCH → INSTALL.
- **Manually:** in the game folder, rename `steam_api.dll` to
  `steam_api_orig.dll`, then copy `steam_api.dll` from the
  [latest release](https://github.com/OBLASTWAR/civ5-lobby-patch/releases/latest) into it.

## Uninstall

- **[Civ V Mod Installer](https://github.com/OBLASTWAR/civ5-mod-installer):** LOBBY PATCH → UNINSTALL.
- **Manually:** delete `steam_api.dll` and rename `steam_api_orig.dll` back to
  `steam_api.dll`. Steam's "Verify integrity of game files" also restores it.
