# Android bring-up - 2026-10-15

Target: original PC speed.exe SHA-256 80774c2e5d619b4f120b48d4462896fd504c263399d203a238769cffde1d253c.

## Runtime and input

D3DX qsort comparators use cdecl. The host removes both callback arguments after each comparison; startup checks sorting, duplicates, preserved registers and stack balance. Android keyboard/mouse Acquire uses the default nonexclusive background cooperative level, as verified against real Windows DirectInput.

Most Wanted polls raw keyboard snapshots. Its GameFlow state 6 selects driving controls. Gas is Up, brake/reverse Down, handbrake Space, nitrous Left Alt, shifts Left Shift/Control, reset R, camera C. The menu X sends **Esc (DIK 0x01)**. The user confirmed gas and X work after installing the updated APK. Menu touches include a visible pointer.

All three PC ports distinguish physical gamepad sources and joystick devices with both X/Y axes from fingerprint sensors advertising only a Z axis. Connecting a controller hides touch controls; touching the screen reveals them. Repeated device enumeration no longer hides them again after that touch. Bluetooth hardware testing remains pending because no controller was available.

## Proportional rendering

The earlier camera/FOV and motion-blur experiments were removed after a white-screen/fisheye regression. The current adaptation changes engine dimensions and HUD coordinates while retaining the original camera and post-processing instructions. The user confirmed the corrected image on the S25.

Resolution returns use the launcher-selected render dimensions. The 720p preset preserves the display aspect ratio (1560x720 on a 2340x1080 display). HUD horizontal scale/center, frontend origins and mouse coordinates use the same aspect ratio, based on [ThirteenAG's WidescreenFix](https://github.com/ThirteenAG/WidescreenFixesPack/tree/master/source/NFSMostWanted.WidescreenFix). See THIRD_PARTY_NOTICES.md. The x86 ASI itself is not loaded on ARM64. The in-game selector still returns launcher-selected dimensions; separate selectable in-game modes are not implemented.

The independent depth-reference correction remains: MW copies its initial depth surface from 0x982A2C to descriptor 0x93DAC4 without AddRef and later releases both. The bridge retains the descriptor reference.

## Race crashes

A computed tail jump inside CRT memmove (0x7C4840, missed interior target 0x7C4AFC) is replaced with native memmove using the original cdecl ABI. tests/memmove_override.c checks 4,825 overlap/alignment/length and zero-length cases, guards, EAX and ESP.

The later null callback at 0x826749 traced to sound gain routine 0x81CDC6. At 0x81CE0E it compares a channel index with -1, then executes FUCOMPP before the integer JNE at 0x81CE1C. The shared lifter incorrectly treated FUCOMPP as an integer flag setter. An all-channel update then became a single-channel write at voice + 0x13C - 0x24 = voice + 0x118, corrupting the effects descriptor. The logged 0x432F4047 was a floating-point gain, not a callback address.

scripts/mw_lifter.py preserves integer flags for legacy x87 compares and FTST. FNSTSW still exposes the x87 result, and TEST/SAHF still explicitly transfer it to integer flags. FCOMI/FUCOMI variants keep their existing separate behavior. This correction is local to this project and does not edit the sibling pcrecomp checkout.

Both full generation and incremental relifting use the corrected lifter. To repair an existing local translation:

```powershell
python scripts/relift_hooks.py --x87-flags
```

Deleting hooks also removes their previously generated sites. Generated C remains local and ignored.

In a VS developer prompt, with capstone, pefile and Unicorn installed:

```powershell
python tests/sound_channel_flags.py
# Optional: reproduce the old lifter's known corruption too.
python tests/sound_channel_flags.py --prove-old-bug
```

The test compares the actual locally supplied sound routine with x86 execution over 16 channel/gain combinations, including -1, checking all general registers, stack cleanup, the complete voice object and surrounding guards. The old lift reproduces the overwritten descriptor; the corrected lift matches all cases.

## Online / LAN

The Android Winsock adapter now implements event creation/reset/set/close/waits and WSARecv/WSARecvFrom/WSAGetOverlappedResult, including pending scatter receives, event signaling, completion bytes/flags and cancellation on socket close. Pending operations are pumped nonblockingly during guest waits and scheduler yields.

A startup loopback regression exercises the actual HLE calls and their guest stack cleanup: timeout, event signal/reset, pending UDP scatter receive, incomplete/completed results and socket-close cancellation. This passed on the S25. Opening network menus reached a game lobby without the previous missing-WSAWaitForMultipleEvents crash.

The backend defaults to eahub.eu with EA-host DNS redirection. DNS resolution, backend TCP connection and a visible lobby do **not** prove successful community-server login or an Internet race. A real remote session remains unverified. Receive completion routines/APCs and overlapped ICMP receives return unsupported errors; the tested game path uses events.

## Final validation

Android builds for all three ports and the MW Windows host build pass. The
updated APKs install over the existing S25 apps without clearing their data.
The MW startup Winsock regression passes, including two independent events.
The updated game reaches a quick race, accelerates and progresses along the
track; nitrous sends DIK 0x38 and pause/back sends DIK 0x01. A brief driving
test did not reproduce the null sound callback. Sustained play and a complete
race remain to be tested. The game was left paused after the test.

No game files, generated translations or logs are included in source control.
Release APKs are published separately as GitHub release assets.


## Redmi Note 8 follow-up (2026-10-15)

Leaving Online reached speed.exe 0x7EC268, which calls the previously missing
kernel32!TerminateThread. The Android HLE now requests cooperative thread
termination, unwinds at scheduler/network-wait checkpoints, preserves the
requested exit code and signals thread completion through existing waits.
It does not forcibly cancel the host pthread. The device startup regression
passed blocked, running, suspended, self, already-completed, invalid-handle,
exit-code and guest ABI checks. The user confirmed that exiting Online works.
This confirmation does not establish that an Internet race is functional.

The launcher adds aspect-preserving 480p and 360p presets. On the Redmi's
2340x1080 display these render at 1040x480 and 780x360. The previous default
was 1756x810 (75%), independently of the game's low detail settings. Captured
3D menu samples at that default were about 13-16 FPS, with Present taking
34-53 ms. At 780x360, menu samples reached about 40-46 FPS; gameplay samples
varied around 17-39 FPS depending on the scene, with Present near 1 ms.
These are observed presentation rates across different scenes, not a controlled
race benchmark or a guarantee of sustained performance. CPU-side guest work
and render submission remain limiting factors after reducing GPU pixel load.

See [Audio and frame pacing follow-up](AUDIO_FRAME_PACING.md) for the subsequent
shared audio changes and outstanding device testing. The last audio revision
was compiled for review; it has not yet been validated on tester hardware.


The subsequent audio/frame-pacing release is documented in [AUDIO_FRAME_PACING.md](AUDIO_FRAME_PACING.md): ARM64 gameplay was exercised on the Redmi, both ABI APKs compile, and the final ARM64 releases install on the S25. Physical ARMv7 testing remains pending because only the ARM64-only S25 was connected at release time.
