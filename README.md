# Need for Speed Most Wanted (2005, PC) — Recompiled

**English** · [Español](#español)

Static recompilation of the **PC version** of *Need for Speed: Most Wanted* (2005, `speed.exe`, x86) to native code, first as a Windows validation host and then for **Android ARM64**. The goal is the third game of the *NFS Android Evolved* family, after [Underground 2](https://github.com/codepdbh/nfsu2-recompiled) and [Underground 1](https://github.com/codepdbh/nfsu1-recompiled).

This is **not** an emulator and **not** a port of the Xbox 360 version: every function of the original PC executable is translated ahead of time from x86 to C with [pcrecomp](https://github.com/sp00nznet/pcrecomp) (patched), then compiled by MSVC (Windows) or the Android NDK (ARM64).

> No game files are included. You need your own copy of the PC game. Generated C code, analysis copies and logs are produced locally and are never committed.

### Status

| Milestone | State |
|---|---|
| MW-M0 Executable analysis (`speed.exe`, SHA-256 `80774c2e…253c`) | ✅ Direct3D 9 + D3DX effects, DirectInput 8, DirectSound, relocations recovered |
| MW-M1 Function catalog | ✅ 41,262 functions (data, switch-table, immediate and forced seeds) |
| MW-M2 C generation | ✅ 0 lift failures |
| MW-M3 Windows host build and link | ✅ |
| MW-M4/M5 CRT, memory, original entry point | ✅ |
| MW-M6/M7 Win32, Direct3D 9, first frame | ✅ D3DX effects loaded from the game's own resources |
| MW-M8/M9 Intro, menus, world | 🔧 3D world with traffic renders (≈1,450 draws/frame), 75 s+ without crashes; menus and input in progress |
| MW-M10+ Racing, police, saving | ⏳ |
| Android ARM64 APK | Builds and runs: cinematics, menu input and user-confirmed gameplay; race quality/control validation ongoing |

Android runtime fixes and current verification limits: [bring-up notes](docs/ANDROID_BRINGUP.md).

### Layout

```
config/    reference executable, forced seeds, hooks, overrides (Most Wanted only)
scripts/   binary analysis, seed discovery, lift driver, Windows build/test
src/       Windows host, runtime adapters, Most Wanted shims
docs/      binary report and reuse audit of Underground 1/2
android/   Android app (in progress)
work/      (local only) analysis copy, catalog, logs
```

Shared infrastructure (Win32 shims, D3D9 observers, logging) is reused from the Underground 2 project at build time without modifying it.

### Building (Windows)

Requirements: Visual Studio with MSVC x86, Python 3 with `capstone` and `pefile`, the patched `pcrecomp` and `nfsu2-recompiled` checked out next to this folder, and the game in `../Need for Speed Most Wanted 2005`.

```powershell
python scripts/analyze_binary.py            # writes work/speed.analysis.exe and work/binary.json
python scripts/data_seeds.py ...            # see docs/BINARY_REPORT.md for the full sequence
python scripts/run_lift.py catalog work/speed.analysis.exe work/seeds_code.json work/catalog.json
python scripts/run_lift.py lift work/speed.analysis.exe work/catalog.json src/recomp/gen
powershell -File scripts/build_windows.ps1
powershell -File scripts/test_windows.ps1   # windowed test run
```

### License and credits

Code in this repository: see each file. pcrecomp by sp00nznet. *Need for Speed* is a trademark of Electronic Arts; this project is not affiliated with EA.

---

## Español

Recompilación estática de la **versión de PC** de *Need for Speed: Most Wanted* (2005, `speed.exe`, x86) a código nativo: primero un host de validación en Windows y después **Android ARM64**. Es el tercer juego de la familia *NFS Android Evolved*, después de [Underground 2](https://github.com/codepdbh/nfsu2-recompiled) y [Underground 1](https://github.com/codepdbh/nfsu1-recompiled).

**No** es un emulador ni un port de la versión de Xbox 360: cada función del ejecutable original de PC se traduce de antemano de x86 a C con [pcrecomp](https://github.com/sp00nznet/pcrecomp) (modificado) y después se compila con MSVC (Windows) o con el NDK de Android (ARM64).

> No se incluyen archivos del juego. Necesitas tu propia copia del juego de PC. El código C generado, las copias de análisis y los registros se producen localmente y nunca se suben.

### Estado

| Hito | Estado |
|---|---|
| MW-M0 Análisis del ejecutable (`speed.exe`, SHA-256 `80774c2e…253c`) | ✅ Direct3D 9 + efectos D3DX, DirectInput 8, DirectSound, reubicaciones recuperadas |
| MW-M1 Catálogo de funciones | ✅ 41.262 funciones (semillas de datos, tablas switch, inmediatos y forzadas) |
| MW-M2 Generación de C | ✅ 0 fallos de lifting |
| MW-M3 Compilación y enlace del host de Windows | ✅ |
| MW-M4/M5 CRT, memoria, punto de entrada original | ✅ |
| MW-M6/M7 Win32, Direct3D 9, primer frame | ✅ efectos D3DX cargados desde los recursos del propio juego |
| MW-M8/M9 Intro, menús, mundo | 🔧 el mundo 3D con tráfico se dibuja (≈1.450 draws por frame), más de 75 s sin cierres; menús y controles en curso |
| MW-M10+ Carreras, policía, guardado | ⏳ |
| APK de Android ARM64 | ⏳ |

### Compilación (Windows)

Requisitos: Visual Studio con MSVC x86, Python 3 con `capstone` y `pefile`, `pcrecomp` (modificado) y `nfsu2-recompiled` junto a esta carpeta, y el juego en `../Need for Speed Most Wanted 2005`. Los pasos están en la sección en inglés.

### Licencia y créditos

Código de este repositorio: ver cada archivo. pcrecomp por sp00nznet. *Need for Speed* es una marca de Electronic Arts; este proyecto no está afiliado a EA.
