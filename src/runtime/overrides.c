/* Most Wanted's function addresses are discovered independently.
 * Never inherit Underground overrides merely because the names match. */
#include "nfs_runtime.h"
#include "recomp_types.h"
#include <string.h>
/* speed.exe 0x7C4840 is the CRT's cdecl memmove, verified by its overlap
 * check and forward/backward copy paths. Its negative-index jump tables
 * contain interior entries (e.g. 0x7C4AFC) missed by recursive discovery.
 * Implement the complete routine, retaining overlap and zero-length semantics. */
static void guest_memmove(void){
    uint32_t destination=MEM32(g_esp+4),source=MEM32(g_esp+8),size=MEM32(g_esp+12);
    if(size)memmove((void*)ADDR(destination),(const void*)ADDR(source),size);
    g_eax=destination;g_esp+=4; // cdecl: caller removes its three arguments
}
nfs_override_fn nfs_override_lookup(uint32_t va) { return va==0x007C4840u?guest_memmove:0; }
int nfs_override_count(void) { return 1; }
