#if !defined(THLIB_H)
#define THLIB_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char *name;      // name of the entry
    uint8_t *data;   // pointer to compressed data
    uint32_t offset; // absolute offset to the data in the archive
    uint32_t uncompressed_size;
    uint32_t compressed_size;

    union {
        struct {
            uint32_t unk_1;
            uint32_t unk_2;
            uint32_t checksum;
        } pbg3;
    };
} ThEntry;

typedef struct {
    ThEntry *entries;
    uint32_t entries_count;
    uint32_t file_table_offset; // absolute offset to the file table
    uint32_t file_table_size;   // size of the uncompressed file table

    uint8_t version; // version of the game
} ThArchive;

// =========================
// VERSION GENERIC FUNCTIONS
// =========================

// Takes an archive struct to fill out, and the path to the archive
// returns the detected file version
uint8_t thOpenArchive(ThArchive *archive, const char *const path);

// Returns the decompressed entry at the specified index
void *thGetEntry(ThArchive *archive, const uint32_t entry_idx);

#ifdef __cplusplus
}
#endif

#endif // !defined(THLIB_H)
