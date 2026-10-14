# Referencia del ejecutable — MW-M0

Fecha de análisis: 2026-10-14. Copia local `Need for Speed Most Wanted 2005/speed.exe`, 6.029.312 bytes.

SHA-256: `80774c2e5d619b4f120b48d4462896fd504c263399d203a238769cffde1d253c`.

PE32 x86 (`Machine=0x014c`); base `0x00400000`, entrada `0x007c4040`, SizeOfImage `0x00678e4e`. No se encontró recurso de versión ni directorio TLS. La versión/parche y región no se atribuyen por el nombre de la carpeta: esta referencia se identifica por su hash. Los datos locales incluyen vídeos en español e inglés; no establece por sí solo la región del ejecutable.

| Sección | RVA | Tamaño virtual | Tamaño en disco |
|---|---:|---:|---:|
| .text | 0x1000 | 0x48e2a5 | 4.780.032 |
| .rdata | 0x490000 | 0x5971d | 368.640 |
| .data | 0x4ea000 | 0xdce10 | 147.456 |
| .rsrc | 0x5c7000 | 0x7064e | 462.848 |
| Sin nombre | 0x638000 | 0x40e4e | 266.240 |

La entrada decodifica un prólogo CRT normal con registro SEH vía `fs:[0]`. La sección de código tiene entropía 6,514. No se observa un stub de descompresión en la entrada; estas observaciones no prueban la ausencia de toda protección.

La sección sin nombre conserva una cadena de reubicaciones validada: 240.976 bytes y 114.755 entradas HIGHLOW. El directorio original estaba vacío. `scripts/analyze_binary.py` lo restaura únicamente en `work/speed.analysis.exe`; SHA-256 de esa copia: `53e21e6ad5b4243b29d776fc33b7d84b805be24bb176f9716c1888646966d8de`.

300 importaciones de 14 DLL: KERNEL32 (157), d3d9 (1), d3dx9_26 (13), DINPUT8 (1), USER32 (41), ADVAPI32 (6), SHFOLDER (1), SHELL32 (1), DSOUND (2), WINMM (24), GDI32 (12), TAPI32 (9), NETAPI32 (1), WS2_32 (31). El JSON completo de símbolos y encabezados se genera en `work/binary.json`.

Gráficos: `Direct3DCreate9` y operaciones D3DX de matrices, efectos desde recursos y pool de efectos. Audio: DirectSound y WinMM. Entrada: DirectInput 8. El runtime debe conservar callbacks, sincronización, hilos y SEH. La carpeta contiene además `server.dll`; su carga y código se validarán si la ejecución los necesita.

Primer lifting: 41.261 entradas, 138 unidades C, 5.556.158 líneas, cero fallos de generación y 72 sitios pendientes, entre ellos `lock xadd` y `pmuludq`. Un catálogo heurístico puede incluir alias, datos mal interpretados y entradas no alcanzadas; estas cifras no expresan cobertura funcional.

Primer host: compila y enlaza, enlaza las 300 importaciones sin pendientes y ejecuta la entrada original. Se detuvo durante la inicialización de vectores en `0x007c90a0`, al llamar al constructor `0x00505f80` que la heurística no catalogó. El ensamblador y el crash respaldan su raíz explícita en `config/forced_seeds.json`; no se omite su ejecución.
