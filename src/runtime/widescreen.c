/* Native camera adaptation of ThirteenAG's NFSMostWanted.WidescreenFix
 * Rendering.ixx (MIT), with centered 4:3 HUD geometry on a wide surface.
 * Sites verified in speed.exe SHA-256 80774c2e...253c. No x86 ASI is loaded. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "recomp_types.h"
#include "nfs_widescreen.h"
#ifdef __ANDROID__
#include <android/log.h>
#endif

void nfs_widescreen_init(const char *game_root) { (void)game_root; }
static float display_aspect;
static unsigned render_width, render_height;
static double camera_scale[3]={1.0,0.5,1.0};
void nfs_widescreen_set_display_aspect(float aspect) { display_aspect=aspect>1.34f&&aspect<8.f?aspect:0.f; }
void nfs_widescreen_set_resolution(unsigned width, unsigned height) { render_width=width; render_height=height; }
int nfs_widescreen_hud_entries(void) { return 0; }
void nfs_widescreen_set_minimap_offset(float dy) { (void)dy; }
void nfs_widescreen_set_frame_rate(unsigned fps) { (void)fps; }
void nfs_widescreen_apply_frame_rate(void) {}
void nfs_frontend_cursor_units(int on) { (void)on; }
float nfs_widescreen_hud_pos_x(void) { return display_aspect?240.f*display_aspect:320.f; }

void recomp_hook(uint32_t va) {
    /* Native equivalent of WidescreenFix's DisableMotionBlur branch.
     * Disable the option before the original comparison, retaining flags. */
    if(va==0x006DF1CCu){ if(render_width)MEM32(0x009017DCu)=g_ebx; return; }
    /* GetRacingResolution's six original return sites. Change the engine's
     * dimensions before it allocates scene/post-processing targets, rather
     * than enlarging only the D3D backbuffer. Stack: return, width*, height*. */
    switch(va){
    case 0x006C27F9u:case 0x006C2810u:case 0x006C2827u:
    case 0x006C283Eu:case 0x006C2855u:case 0x006C286Cu:
        if(render_width&&render_height){
            MEM32(MEM32(g_esp+4))=render_width;
            MEM32(MEM32(g_esp+8))=render_height;
        }
        return;
    }
    if(!display_aspect)return;
    switch(va){
    case 0x006CF4F6u:
        if(g_ecx==1||g_ecx==4){camera_scale[0]=(4.0/3.0)/display_aspect/1.047485948;camera_scale[1]=0.43434;camera_scale[2]=1.21;}
        else{camera_scale[0]=1.0;camera_scale[1]=0.5;camera_scale[2]=1.0;}
        g_st[0]=g_ecx==3?0.4:1.0;return;
    case 0x006CF56Cu:g_st[0]*=camera_scale[0];return;
    case 0x006CF57Eu:g_st[0]*=camera_scale[1]/0.5;return;
    case 0x006CF5E2u:g_st[0]*=camera_scale[2];return;
    case 0x006D12CCu:case 0x006D1316u:g_st[0]*=(4.0/3.0)/display_aspect;return;
    }
    char why[96];
    snprintf(why, sizeof why, "RECOMP_HOOK at 0x%08X but no Most Wanted hook is implemented", (unsigned)va);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_FATAL, "NFSMW", "%s", why);
#endif
    fprintf(stderr, "%s\n", why);
    abort();
}
