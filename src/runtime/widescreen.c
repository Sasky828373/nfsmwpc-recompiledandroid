/*
 * widescreen.c - Most Wanted wide-display support for the Android runtime:
 * NOT PORTED YET. Keeps the interface the runtime calls with no effect; the
 * port of ThirteenAG's NFSMostWanted.WidescreenFix (sites located in this
 * speed.exe by the mod's patterns) replaces it. A RECOMP_HOOK reaching here
 * stops loudly: config/hooks.txt lists none yet.
 * The Windows host uses video_adapter.c instead.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "nfs_widescreen.h"
#ifdef __ANDROID__
#include <android/log.h>
#endif

void nfs_widescreen_init(const char *game_root) { (void)game_root; }
void nfs_widescreen_set_display_aspect(float aspect) { (void)aspect; }
void nfs_widescreen_set_resolution(unsigned width, unsigned height) { (void)width; (void)height; }
int nfs_widescreen_hud_entries(void) { return 0; }
void nfs_widescreen_set_minimap_offset(float dy) { (void)dy; }
void nfs_widescreen_set_frame_rate(unsigned fps) { (void)fps; }
void nfs_widescreen_apply_frame_rate(void) {}
void nfs_frontend_cursor_units(int on) { (void)on; }
float nfs_widescreen_hud_pos_x(void) { return 320.0f; }

void recomp_hook(uint32_t va) {
    char why[96];
    snprintf(why, sizeof why, "RECOMP_HOOK at 0x%08X but no Most Wanted hook is implemented", (unsigned)va);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_FATAL, "NFSMW", "%s", why);
#endif
    fprintf(stderr, "%s\n", why);
    abort();
}
