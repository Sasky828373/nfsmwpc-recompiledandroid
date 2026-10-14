/* Most Wanted's function addresses are discovered independently.
 * Never inherit Underground overrides merely because the names match. */
#include "nfs_runtime.h"
nfs_override_fn nfs_override_lookup(uint32_t va) { (void)va; return 0; }
int nfs_override_count(void) { return 0; }
