#include "pbg3.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "bitstream.h"
#include "lzss.h"

#define MAX_HEADER_SIZE 9 // 2 integers * 4 bytes + 0.5 byte

static uint32_t readVariableInt(Bitstream *const stream) {
    const uint8_t variant = thReadBits(stream, 2);
    return thReadBits(stream, (variant + 1) * 8);
}

uint8_t thIsPbg3Header(const char magic[4]) {
    return !strncmp(magic, "PBG3", 4);
}

uint8_t thOpenPbg3ArchiveFromFile(ThArchive *const archive, FILE *file) {
    { // Check magic
        char magic[4];
        fread(magic, 1, 4, file);
        if(!thIsPbg3Header(magic)) {
            return 1;
        }
    }
    { // Parse header
        uint8_t header[MAX_HEADER_SIZE];
        fread(header, 1, sizeof(header), file);
        Bitstream header_stream = BITSTREAM_INIT(header);
        archive->entries_count = readVariableInt(&header_stream);
        archive->file_table_offset = readVariableInt(&header_stream);

        fseek(file, 0, SEEK_END);
        const uint32_t eof = ftell(file);
        fseek(file, archive->file_table_offset, SEEK_SET);
        archive->file_table_size = eof - ftell(file);
    }
    { // Read file table
        archive->entries = malloc(sizeof(ThEntry) * archive->entries_count);
        uint8_t *const file_table = malloc(archive->file_table_size);
        fread(file_table, 1, archive->file_table_size, file);
        Bitstream ft_stream = BITSTREAM_INIT(file_table);
        for(uint32_t i = 0; i < archive->entries_count; i++) {
            ThEntry *const entry = &archive->entries[i];
            entry->pbg3.unk_1 = readVariableInt(&ft_stream);
            entry->pbg3.unk_2 = readVariableInt(&ft_stream);
            entry->pbg3.checksum = readVariableInt(&ft_stream);

            entry->offset = readVariableInt(&ft_stream);
            entry->uncompressed_size = readVariableInt(&ft_stream);

            entry->name = malloc(MAX_FILENAME_SIZE);
            thReadString(&ft_stream, entry->name, MAX_FILENAME_SIZE);
        }
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

    archive->version = 6;
    return 0;
}

void *thGetPbg3Entry(ThArchive *const archive, const uint32_t entry_idx) {
    if(entry_idx >= archive->entries_count) {
        return NULL;
    }
    return thDecompress(archive->entries[entry_idx].data,
                        archive->entries[entry_idx].uncompressed_size);
}
