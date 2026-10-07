# Dungeons 2 Linux Patches

**Minecraft Dungeons II Linux / Proton fixes for Steam App ID 1912410.**

Unofficial community patches for running **Minecraft Dungeons II on Linux, SteamOS, Steam Deck, Flatpak Steam, Wine and Proton**. The first release targets **Minecraft Dungeons II 1.1.2.0** and fixes a reproducible `xgameruntime.dll` async task-queue crash discovered under Proton.

[![Latest release](https://img.shields.io/github/v/release/mixutin/Dungeons-2-Linux-Patches?label=latest%20patch)](https://github.com/mixutin/Dungeons-2-Linux-Patches/releases/latest)
[![Build](https://github.com/mixutin/Dungeons-2-Linux-Patches/actions/workflows/build.yml/badge.svg)](https://github.com/mixutin/Dungeons-2-Linux-Patches/actions/workflows/build.yml)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Steam App](https://img.shields.io/badge/Steam-1912410-1b2838)](https://store.steampowered.com/app/1912410/Minecraft_Dungeons_II/)

## Current supported game version

- **Game:** Minecraft Dungeons II
- **Version:** **1.1.2.0**
- **Steam App ID:** **1912410**
- **Hotfix date:** 2026-10-07
- **Known-good test environment:** Proton Experimental 11.0-20261001, Flatpak Steam, Ubuntu/Linux, AMD RADV

Mojang's 1.1.2.0 hotfix was published on October 7, 2026. This project is independent of Mojang, Microsoft, Xbox Game Studios, Valve and CodeWeavers.

## What this patch fixes

The Linux compatibility shim could keep a Microsoft/Xbox authentication request alive for roughly **240 seconds**. During that delay, the game could destroy or reuse the associated `XAsyncBlock` / `XTaskQueue`. When the shim later completed the async request it could dereference stale queue memory and crash.

Typical crash:

```text
Unhandled Exception: EXCEPTION_ACCESS_VIOLATION
xgameruntime.dll + 0x629e
complete_async()
```

The patch:

- retains an async operation's task queue until completion;
- prevents a closed queue handle from becoming a use-after-free target;
- stops Microsoft/Xbox authentication from blocking an async worker for four minutes;
- launches sign-in in the background and retries safely;
- respects `XDG_DATA_HOME`, which makes the helper usable inside Flatpak Steam;
- stages `xauth.py` in Flatpak Steam's writable data directory;
- hardens queue duplication/lifetime handling.

Before the crash, affected sessions could show **stale interaction prompts, UI elements that remained after the object disappeared, cursor/input fallback, multiplayer state oddities or level/world desync symptoms**. The verified fix is the async/runtime crash mechanism; individual gameplay or server bugs can have other causes.

## Download and install

The easiest method is the **latest GitHub Release**:

1. Download `Dungeons-2-Linux-Patches-1.1.2.0.zip`.
2. Extract it.
3. Close Minecraft Dungeons II completely.
4. Run:

```bash
chmod +x install.sh xauth.py
./install.sh
```

Then set the Minecraft Dungeons II Steam launch option to:

```text
WINEDLLOVERRIDES="xgameruntime=n" %command%
```

The installer copies the patched `xgameruntime.dll` to the game root, the shipping executable directory and the Proton prefix. It does **not** delete or modify your save files.

### Flatpak Steam

Flatpak Steam is detected automatically. The auth helper is staged under:

```text
~/.var/app/com.valvesoftware.Steam/data/dungeons2-compat/
```

If Microsoft sign-in is needed, complete the normal device-code login prompt. Token files are private local credentials: **never upload or share `tokens.txt`**.

## Build from source

On Debian/Ubuntu:

```bash
sudo apt install gcc-mingw-w64-x86-64-posix
x86_64-w64-mingw32-gcc-posix -shared -O2 -Wall -Wextra \
  -o src/xgameruntime.dll src/xgameruntime.c
./install.sh
```

GitHub Actions also builds the DLL on every push and pull request.

## Verification

The runtime has a normal async test plus a regression test for the queue-lifetime bug. The regression starts an async operation, closes the caller's queue handle immediately, then waits for completion. A correct build completes without an access violation.

Locally tested result:

```text
status=00000000 callback=1
```

## Troubleshooting keywords

This repository is intended to be discoverable for searches such as:

- Minecraft Dungeons II Linux fix
- Minecraft Dungeons 2 Proton crash
- Minecraft Dungeons II Steam Deck
- Minecraft Dungeons II xgameruntime.dll
- xgameruntime EXCEPTION_ACCESS_VIOLATION
- Minecraft Dungeons II cursor wrong on Linux
- Minecraft Dungeons II UI stuck Proton
- Minecraft Dungeons II multiplayer desync Linux
- Minecraft Dungeons II Flatpak Steam fix
- Steam App 1912410 Linux

If your crash stack is unrelated to `xgameruntime.dll`, open an issue with the crash context and Proton version rather than assuming this patch applies.

## Debug files

Useful paths:

```text
.../steamapps/compatdata/1912410/pfx/drive_c/xgr.log
.../AppData/Local/Dungeons2/Saved/Crashes/
```

Do not post token files, account IDs or private authentication data in issues.

## Upstream and attribution

This project is based on the MIT-licensed **Dungeons2_linux_fix** compatibility work by **Kubas556**. The original MIT copyright notice is preserved in [LICENSE](LICENSE).

This repository adds the queue-lifetime fix, non-blocking authentication behavior, Flatpak data-path handling, regression testing, release packaging and documentation.

## Version sources

- Official 1.1.2.0 hotfix: https://feedback.minecraft.net/hc/en-us/articles/49427995539981-Minecraft-Dungeons-II-1-1-2-0-Hotfix
- Steam store: https://store.steampowered.com/app/1912410/Minecraft_Dungeons_II/
- SteamDB app record: https://steamdb.info/app/1912410/

## Disclaimer

This is an unofficial community compatibility project. Minecraft, Minecraft Dungeons II, Microsoft, Xbox and their respective marks belong to their owners. No proprietary Microsoft Gaming Services DLL is redistributed; this repository builds an open-source compatibility implementation.
