/* Original MW camera, HUD and resolution behavior restored after the
 * experimental wide-display hooks caused a regression. No ASI is loaded. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "nfs_widescreen.h"

void nfs_widescreen_init(const char *game_root) { (void)game_root; }
void nfs_widescreen_set_display_aspect(float aspect) { (void)aspect; }
void nfs_widescreen_set_resolution(unsigned width, unsigned height) { (void)width; (void)height; }
int nfs_widescreen_hud_entries(void) { return 0; }
void nfs_widescreen_set_minimap_offset(float dy) { (void)dy; }
void nfs_widescreen_set_frame_rate(unsigned fps) { (void)fps; }
void nfs_widescreen_apply_frame_rate(void) {}
void nfs_frontend_cursor_units(int on) { (void)on; }
float nfs_widescreen_hud_pos_x(void) { return 320.f; }
void recomp_hook(uint32_t va) {
    fprintf(stderr, "Unexpected disabled MW render hook at 0x%08X\n", (unsigned)va);
    abort();
}
