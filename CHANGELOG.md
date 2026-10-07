# Changelog

## v1.1.2.0-linux-fix.1 — 2026-10-07

First public Linux/Proton patch release for Minecraft Dungeons II 1.1.2.0.

### Fixed

- `xgameruntime.dll` async queue lifetime crash in `complete_async()`.
- Queue use-after-free risk when the game closes a task queue while an async operation is still pending.
- 240-second blocking Microsoft/Xbox token path that could complete into stale async state.
- Flatpak Steam auth-helper path by honoring `XDG_DATA_HOME`.
- Queue duplication/reference handling.

### Added

- Queue-lifetime regression test.
- Reproducible GitHub Actions DLL build.
- Flatpak Steam installer support.
- Search-friendly Linux/Proton troubleshooting documentation.
