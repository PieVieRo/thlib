#if !defined(PBG4_H)
#define PBG4_H

#include <stdint.h>
#include <stdio.h>

#include "dat.h"

uint8_t thIsPbg4Header(const char magic[4]);

uint8_t thOpenPbg4ArchiveFromFile(ThArchive *const archive, FILE *file);
void *thGetPbg4Entry(ThArchive *const archive, const uint32_t entry_idx);

#endif // !defined(PBG4_H)
