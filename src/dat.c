#include "dat.h"

#include "pbg3.h"
#include "pbg4.h"

static uint8_t checkHeaders(const char magic[4]) {
    if(thIsPbg3Header(magic)) {
        return 6;
    }
    if(thIsPbg4Header(magic)) {
        return 7;
    }
    return 0;
}

static uint8_t guessVersion(FILE *file) {
    char magic[4];
    fread(magic, 1, 4, file);
    fseek(file, 0, SEEK_SET);
    return checkHeaders(magic);
}

uint8_t thOpenArchive(ThArchive *const archive, const char *const path) {
    FILE *file = fopen(path, "rb");
    if(file == NULL) {
        return 0;
    }
    const uint8_t version = guessVersion(file);
    switch(version) {
        case 6:
            thOpenPbg3ArchiveFromFile(archive, file);
            break;
        case 7:
            thOpenPbg4ArchiveFromFile(archive, file);
            break;
        default:
            break;
    }
    fclose(file);
    return version;
}

void *thGetEntry(ThArchive *const archive, const uint32_t entry_idx) {
    switch(archive->version) {
        case 6:
            return thGetPbg3Entry(archive, entry_idx);
            break;
        case 7:
            return thGetPbg4Entry(archive, entry_idx);
            break;
        default:
            return NULL;
            break;
    }
}
