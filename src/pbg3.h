#if !defined(PBG3_H)
#define PBG3_H

#include <stdint.h>
#include <stdio.h>

#include "dat.h"

uint8_t thIsPbg3Header(const char magic[4]);

uint8_t thOpenPbg3ArchiveFromFile(ThArchive *const archive, FILE *file);
void *thGetPbg3Entry(ThArchive *const archive, const uint32_t entry_idx);

#endif // !defined(PBG3_H)
