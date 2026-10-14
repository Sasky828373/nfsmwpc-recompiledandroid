# Android bring-up ? 2026-10-14

Target: original PC speed.exe SHA-256 80774c2e5d619b4f120b48d4462896fd504c263399d203a238769cffde1d253c.

## Runtime and input

D3DX qsort comparators use cdecl. The host now removes the two callback arguments after each comparison. A startup regression checks sorting, duplicate values, register preservation and stack balance through the guest ABI.

Windows DirectInput keyboard/mouse devices allow Acquire with their default nonexclusive background cooperative level; `tests/directinput_defaults.c` exercises the real Windows API. Android now supplies that default. Absolute mouse clicks no longer depend on an Underground-specific mouse-object address. Virtual window activation sends focus messages.

A joystick source alone is insufficient to identify a controller: Redmi's uinput-fpc fingerprint sensor exposes only Z. Joystick devices require both X and Y ranges; true gamepad sources remain supported.

Most Wanted uses raw keyboard snapshots instead of Underground driving action maps. The touch layout observes MW GameFlow at 0x925E90 (state 6), sampled on the guest thread. Nitrous maps to Left Alt, handbrake to Space, manual shifts to Left Shift/Control. A visible touch pointer is drawn in menus.

## Rendering

The launcher previously advertised render modes but the engine still selected its original 640x480 preset. Six verified return sites in GetRacingResolution now return the launcher dimensions before the engine allocates its backbuffer and scene targets. Device logs confirm 1756x810 on Redmi Note 8 at 75% resolution. A device rebuild exposed a second owner of the initial depth surface: MW copies 0x982A2C into descriptor 0x93DAC4 without AddRef, then releases both. The bridge retains that verified descriptor reference instead of accepting stale COM handles.

Native camera hooks adapt ThirteenAG's MIT WidescreenFix Rendering module. HUD quads preserve centered 4:3 proportions and touch coordinates apply the inverse mapping. This does not yet reproduce every upstream HUD anchor or ASI feature.

Android disables the original motion-blur option at its verified comparison site. Windows validation without a launcher resolution keeps the original behavior. Regenerate affected functions with `python scripts/relift_hooks.py`; a full lift also reads config/hooks.txt.

Reference: https://github.com/ThirteenAG/WidescreenFixesPack/tree/master/source/NFSMostWanted.WidescreenFix

## Validation and limits

Android assembleDebug and Windows host builds pass. On-device qsort ABI checks pass; intro videos and profile/menu navigation work. The user reached playable gameplay before these additional quality/control corrections. Those corrections require further race verification. A transition test after the depth-reference correction still reported a frame stall in lifted D3DX (0xC06D2D); the later launch presented again. Sustained transitions and the reported fisheye appearance remain under investigation.

The existing Internet backend defaults to eahub.eu, with EA-host DNS redirection and Android Internet permission. DNS resolves, but this is not evidence of successful PC Most Wanted login or an Internet race. Community server compatibility, credentials and a real session remain unverified.

No game files, generated translations, logs or APK are included in this repository.
