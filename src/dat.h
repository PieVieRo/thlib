#if !defined(DAT_H)
#define DAT_H

#include <stdint.h>

#define MAX_FILENAME_SIZE 256

typedef struct {
    char *name;      // name of the entry
    uint8_t *data;   // pointer to compressed data
    uint32_t offset; // absolute offset to the data in the archive
    uint32_t uncompressed_size;
    uint32_t compressed_size;

    union { // version-specific data
        struct {
            uint32_t unk_1;
            uint32_t unk_2;
            uint32_t checksum;
        } pbg3;

        struct {
            uint32_t unk;
        } pbg4;
    };
} ThEntry;

typedef struct {
    ThEntry *entries;
    uint32_t entries_count;
    uint32_t file_table_offset; // absolute offset to the file table
    uint32_t file_table_size;   // size of the uncompressed file table

    uint8_t version; // version of the game
} ThArchive;

uint8_t thOpenArchive(ThArchive *archive, const char *const path);
void *thGetEntry(ThArchive *archive, const uint32_t entry_idx);

#endif // !defined(DAT_H)
