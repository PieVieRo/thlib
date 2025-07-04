#include "lzss.h"

#include <stdlib.h>

#include "bitstream.h"

#define LZSS_DICTIONARY_SIZE 0x2000
#define LZSS_OFFSET 13
#define LZSS_LENGTH 4
#define LZSS_MIN_MATCH 3

void *thDecompress(uint8_t *const input, const uint32_t uncompressed_size) {
    uint8_t dictionary[LZSS_DICTIONARY_SIZE] = { 0 };
    uint16_t dictionary_idx = 0;

    Bitstream bit_stream = BITSTREAM_INIT(input);

    uint8_t *const out = malloc(uncompressed_size);
    uint32_t out_idx = 0;
    if(!out) return NULL;
    while(out_idx < uncompressed_size) {
        uint8_t is_unencoded = thReadBits(&bit_stream, 1);
        if(is_unencoded) {
            const uint8_t read_byte = thReadBits(&bit_stream, 8);
            out[out_idx++] = read_byte;
            dictionary[dictionary_idx++] = read_byte;
            dictionary_idx %= LZSS_DICTIONARY_SIZE;
        } else {
            const uint16_t offset = thReadBits(&bit_stream, LZSS_OFFSET) - 1;
            const uint8_t length =
                thReadBits(&bit_stream, LZSS_LENGTH) + LZSS_MIN_MATCH;
            for(size_t idx = offset; idx < offset + length; ++idx) {
                const uint8_t read_byte =
                    dictionary[idx % LZSS_DICTIONARY_SIZE];
                out[out_idx++] = read_byte;
                dictionary[dictionary_idx++] = read_byte;
                dictionary_idx %= LZSS_DICTIONARY_SIZE;
            }
        }
    }
    return out;
}
