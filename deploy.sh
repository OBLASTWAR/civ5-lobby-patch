#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
INSTALL_DIR="/c/Program Files (x86)/Steam/steamapps/common/Sid Meier's Civilization V"
BUILT_DLL="$SCRIPT_DIR/build/steam_api.dll"
UNINSTALL=0

for arg in "$@"; do
    case "$arg" in
        --uninstall)        UNINSTALL=1 ;;
        --install-dir=*)    INSTALL_DIR="${arg#*=}" ;;
        *) echo "Unknown argument: $arg"; exit 1 ;;
    esac
done

ORIG_DLL="$INSTALL_DIR/steam_api_orig.dll"
ACTIVE_DLL="$INSTALL_DIR/steam_api.dll"

# ---- Uninstall --------------------------------------------------------------
if [[ $UNINSTALL -eq 1 ]]; then
    if [[ ! -f "$ORIG_DLL" ]]; then
        echo "Nothing to uninstall (steam_api_orig.dll not found)."
        exit 0
    fi
    rm "$ACTIVE_DLL"
    mv "$ORIG_DLL" "$ACTIVE_DLL"
    echo "Uninstalled. steam_api.dll restored."
    exit 0
fi

# ---- Build ------------------------------------------------------------------
echo "Building..."
cd "$SCRIPT_DIR"
cmd //c build.bat || { echo "Build failed, aborting."; exit 1; }

# ---- Sanity checks ----------------------------------------------------------
if [[ ! -d "$INSTALL_DIR" ]]; then
    echo "ERROR: Civ5 not found at: $INSTALL_DIR"
    echo "       Use --install-dir=/path/to/civ5 to override."
    exit 1
fi

# ---- Back up original (once) ------------------------------------------------
if [[ ! -f "$ORIG_DLL" ]]; then
    if [[ ! -f "$ACTIVE_DLL" ]]; then
        echo "ERROR: steam_api.dll not found in install dir — is Civ5 installed?"
        exit 1
    fi
    echo "Backing up steam_api.dll -> steam_api_orig.dll"
    mv "$ACTIVE_DLL" "$ORIG_DLL"
fi

# ---- Deploy -----------------------------------------------------------------
echo "Deploying to $INSTALL_DIR"
cp "$BUILT_DLL" "$ACTIVE_DLL"
echo "Done."
echo "Log will appear at: $INSTALL_DIR/civ5lobby_debug.log"
echo ""
echo "To uninstall: ./deploy.sh --uninstall"
