#include "pbg4.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lzss.h"

#define HEADER_SIZE 12 // 3 * 4 bytes

uint8_t thIsPbg4Header(const char magic[4]) {
    return !strncmp(magic, "PBG4", 4);
}

uint8_t thOpenPbg4ArchiveFromFile(ThArchive *const archive, FILE *file) {
    { // Check magic
        char magic[4];
        fread(magic, 1, 4, file);
        if(!thIsPbg4Header(magic)) {
            return 1;
        }
    }
    { // parse header
        fread(&archive->entries_count, sizeof(uint32_t), 1, file);
        fread(&archive->file_table_offset, sizeof(uint32_t), 1, file);
        fread(&archive->file_table_size, sizeof(uint32_t), 1, file);
    }
    { // read file table
        fseek(file, 0, SEEK_END);
        const uint32_t eof = ftell(file);
        fseek(file, archive->file_table_offset, SEEK_SET);
        const uint32_t compressed_ft_size = eof - ftell(file);

        uint8_t *compressed_ft = malloc(compressed_ft_size);
        fread(compressed_ft, 1, compressed_ft_size, file);

        uint8_t *const file_table =
            thDecompress(compressed_ft, archive->file_table_size);
        const uint8_t *ft_cursor = file_table;
        archive->entries = malloc(archive->entries_count * sizeof(ThEntry));
        for(uint32_t i = 0; i < archive->entries_count; i++) {
            ThEntry *const entry = &archive->entries[i];

            { // copy the name
                entry->name = malloc(MAX_FILENAME_SIZE);
                uint32_t idx = 0;
                while(ft_cursor[idx] != 0) {
                    entry->name[idx] = ft_cursor[idx];
                    idx++;
                    entry->name[idx] = '\0';
                }
                ft_cursor += idx + 1;
            }

            entry->offset = *(uint32_t *)ft_cursor;
            ft_cursor += sizeof(uint32_t);
            entry->uncompressed_size = *(uint32_t *)ft_cursor;
            ft_cursor += sizeof(uint32_t);
            entry->pbg4.unk = *(uint32_t *)ft_cursor;
            ft_cursor += sizeof(uint32_t);
        }
        free(compressed_ft);
        free(file_table);
    }
    { // calculate compressed size of entries and get the data
        for(uint32_t i = 0; i < archive->entries_count; i++) {
            ThEntry *const entry = &archive->entries[i];
            uint32_t next_offset;
            if(i + 1 == archive->entries_count) {
                next_offset = archive->file_table_offset;
            } else {
                next_offset = archive->entries[i + 1].offset;
            }
            entry->compressed_size = next_offset - entry->offset;

            fseek(file, entry->offset, SEEK_SET);
            entry->data = malloc(entry->compressed_size);
            fread(entry->data, 1, entry->compressed_size, file);
        }
    }

    archive->version = 7;
    return 0;
}

void *thGetPbg4Entry(ThArchive *const archive, const uint32_t entry_idx) {
    if(entry_idx >= archive->entries_count) {
        return NULL;
    }
    return thDecompress(archive->entries[entry_idx].data,
                        archive->entries[entry_idx].uncompressed_size);
}
