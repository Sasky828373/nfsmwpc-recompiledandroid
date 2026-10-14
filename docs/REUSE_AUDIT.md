# Auditoría de reutilización — 2026-10-14

Se inspeccionaron los archivos reales, informes, código generado y logs de Underground 1 y 2. No se modificaron esos proyectos.

| Componente | Evidencia y decisión para Most Wanted PC |
|---|---|
| pcrecomp | Usar el checkout parcheado existente. Conserva SIMD, flags, x87, despacho estricto, callbacks y bloqueo de máquina. No aplicar direcciones de otro juego. |
| Driver de lifting | Adaptado desde Underground 1: reconstruye bloques y recupera semillas que perdió el descubrimiento. Configuración independiente. |
| Semillas de datos | Herramienta de Underground 1 reutilizada. Primer resultado: 14.881 semillas; añadir raíces comprobadas en crashes cuando la heurística no reconozca un constructor. |
| Normalización | El ejecutable también conserva reubicaciones sin directorio PE. Se generalizó la validación de la cadena, sin asumir los RVA ni el número de entradas de SPEED2. Solo se escribe una copia en `work`. |
| Registros, memoria e imports en Windows | Reutilizar `pcrecomp/runtime/native32`, `recomp32` e infraestructura de callbacks. El host requiere MSVC x86; no es todavía el runtime ARM64. |
| Host y crash report | Adaptador local: nombre, directorio, rango de imagen, entry y límites de código propios. |
| Shims Win32 y recursos PE | Reutilizar código de Underground 2 en el build, con identidad de logs propia. Validar con las importaciones y las ejecuciones de Most Wanted. |
| Observadores D3D9 y profiler | Reutilizar fuentes de Underground 2 desde CMake. Variables de entorno convertidas a `NFSMW_*` en archivos locales del build. |
| DirectInput | Partir del observador de Underground 1 que ya eliminó lecturas de direcciones de SPEED2. Los IDs de acciones deben medirse en Most Wanted. |
| Widescreen y FPS | No reutilizar parches de direcciones. El adaptador inicial del observador D3D9 no escribe geometría ni timing de Underground. Pendiente implementar el perfil propio. |
| DLL y bancos de audio | `server.dll` existe en la copia; no asumir el perfil de DLL de SPEED2 ni reutilizar los bloques ABK de Underground 1. Analizar si se ejecutan. |
| Android / DXVK | Reutilizar el backend preparado y puente COM D3D9 cuando Windows tenga evidencia suficiente. Revisar especialmente shaders, vida de recursos, memoria y nuevos imports. |
| Launcher y controles | Reutilizar componentes después de aislar paquete, carpeta de datos, perfiles e IDs de entradas. No producir un APK que anuncie gameplay antes de verificarlo. |

## Estado real de las referencias

Underground 2: el código generado local registra 27.742 funciones, cero fallos de lifting y 2.910 sitios pendientes (principalmente 3DNow, desactivado por CPUID). Los informes de Windows documentan CRT, vídeo, menú, coche y carrera de demostración; el informe Android de controles documenta carga de Resort Loop y confirmación del usuario. Esto no equivale a una prueba nueva de toda la campaña.

Underground 1: el estado local del lifting registra 22.795 funciones, cero fallos de lifting y 3.293 sitios pendientes. El log `logs/run_ws.txt` registra más de 4.000 Present correctos. `logs/android_build.log` termina en BUILD SUCCESSFUL. El README documenta menús y perfil; las carreras no están verificadas allí. Se inspeccionó también el runtime Android adaptado, sus puentes, banco de audio y CMake; hay constantes específicas que requieren un perfil diferente.

## NFS Evolved Core

La primera implementación comparte herramientas y fuentes genéricas por referencia de build. Se mantienen locales la configuración, raíces forzadas, host y diagnóstico con direcciones propias. Una biblioteca independiente se decidirá después de demostrar el comportamiento de Most Wanted, conservando builds independientes.
