#include "c_types.h"
#include "rom.h"
#include <xtensa/corebits.h>

#define EXCCAUSE_LOAD_STORE_ERROR	3	/* Load Store Error */

void load_non_32_wide_handler (struct exception_frame *ef, uint32_t cause);
