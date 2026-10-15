/* Resolution and HUD adaptation from ThirteenAG's MIT WidescreenFix.
 * Update the original HUD constants and centers, not the x87 camera stack. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recomp_types.h"
#include "nfs_widescreen.h"
static unsigned render_width,render_height;
static float display_aspect;
static float hud_center=320.f;
static void put_float(uint32_t address,float value){uint32_t bits;memcpy(&bits,&value,sizeof(value));MEM32(address)=bits;}
static void update_hud(void){
    float aspect=display_aspect>0.f?display_aspect:4.f/3.f;
    hud_center=240.f*aspect;
    put_float(0x008AF9A4u,1.f/hud_center);
    put_float(0x00894B40u,hud_center);
}

void nfs_widescreen_init(const char *game_root) { (void)game_root; }
void nfs_widescreen_set_display_aspect(float aspect) { display_aspect=aspect>1.f&&aspect<8.f?aspect:0.f; }
void nfs_widescreen_set_resolution(unsigned width, unsigned height) { render_width=width;render_height=height; }
int nfs_widescreen_hud_entries(void) { return 0; }
void nfs_widescreen_set_minimap_offset(float dy) { (void)dy; }
void nfs_widescreen_set_frame_rate(unsigned fps) { (void)fps; }
void nfs_widescreen_apply_frame_rate(void) {}
void nfs_frontend_cursor_units(int on) { (void)on; }
float nfs_widescreen_hud_pos_x(void) { return hud_center; }
void recomp_hook(uint32_t va) {
    switch(va){
    case 0x006C27F9u:case 0x006C2810u:case 0x006C2827u:
    case 0x006C283Eu:case 0x006C2855u:case 0x006C286Cu:
        if(render_width&&render_height){
            MEM32(MEM32(g_esp+4))=render_width;
            MEM32(MEM32(g_esp+8))=render_height;
        }
        update_hud();
        return;
    /* After each original mov [esp+offset],320, before its following Y write. */
    case 0x0056FED8u:put_float(g_esp+0x19c,hud_center);return;
    case 0x00584EFAu:put_float(g_esp+0x90,hud_center);return;
    case 0x00599E84u:put_float(g_esp+0x84,hud_center);return;
    case 0x0059A12Bu:put_float(g_esp+0xc4,hud_center);return;
    case 0x0059A5B6u:case 0x0059A849u:put_float(g_esp+0x94,hud_center);return;
    case 0x005A44D0u:put_float(g_esp+0x64,hud_center);return;
    /* Original GetCursorPos conversion returns 0..640. HUD coordinates now
     * span 0..2*center; change the result before the game computes deltas. */
    case 0x006C9C5Cu:MEM32(g_esi+4)=(uint32_t)((float)(int32_t)MEM32(g_esi+4)*(hud_center/320.f));return;
    }
    fprintf(stderr, "Unexpected disabled MW render hook at 0x%08X\n", (unsigned)va);
    abort();
}
