# Android bring-up ? 2026-10-14

Target: original PC speed.exe SHA-256 80774c2e5d619b4f120b48d4462896fd504c263399d203a238769cffde1d253c.

## Runtime and input

D3DX qsort comparators use cdecl. The host now removes the two callback arguments after each comparison. A startup regression checks sorting, duplicate values, register preservation and stack balance through the guest ABI.

Windows DirectInput keyboard/mouse devices allow Acquire with their default nonexclusive background cooperative level; `tests/directinput_defaults.c` exercises the real Windows API. Android now supplies that default. Absolute mouse clicks no longer depend on an Underground-specific mouse-object address. Virtual window activation sends focus messages.

A joystick source alone is insufficient to identify a controller: Redmi's uinput-fpc fingerprint sensor exposes only Z. Joystick devices require both X and Y ranges; true gamepad sources remain supported.

Most Wanted uses raw keyboard snapshots instead of Underground driving action maps. The touch layout observes MW GameFlow at 0x925E90 (state 6), sampled on the guest thread. Nitrous maps to Left Alt, handbrake to Space, manual shifts to Left Shift/Control. A visible touch pointer is drawn in menus.

## Rendering recovery

The experimental resolution, camera, HUD and motion-blur hooks introduced a reported white-screen and fisheye regression. All thirteen sites are now removed from config/hooks.txt and from the generated functions. The original engine projection and 640x480 startup preset are restored. Launcher resolution modes are still advertised; forcing arbitrary engine dimensions is disabled pending complete transition validation.

`python scripts/relift_hooks.py` now also discovers previously generated hook sites, so deleting a configured hook actually removes it from local generated code. A fresh full lift uses the empty hook configuration.

The independent depth-reference correction remains: MW copies the initial depth surface from 0x982A2C to descriptor 0x93DAC4 without AddRef, then releases both. The bridge retains the descriptor reference instead of accepting stale COM handles.

Widescreen implementation reference for future work: https://github.com/ThirteenAG/WidescreenFixesPack/tree/master/source/NFSMostWanted.WidescreenFix

## Validation and limits

Android assembleDebug and Windows host builds pass after rollback. On-device qsort ABI checks pass. The recovery APK created its original 640x480 backbuffer, loaded the saved profile paulo, reached the main menu and opened Options without the previous white-screen during this test. The user previously reached gameplay; sustained race transitions after recovery remain to be checked. The touch-control fixes are retained.

The existing Internet backend defaults to eahub.eu, with EA-host DNS redirection and Android Internet permission. DNS resolves, but this is not evidence of successful PC Most Wanted login or an Internet race. Community server compatibility, credentials and a real session remain unverified.

No game files, generated translations, logs or APK are included in this repository.
