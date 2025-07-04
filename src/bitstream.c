#include "bitstream.h"

static void getNextByte(Bitstream *const reader) {
    reader->current_byte++;
    reader->mask = 0x80;
}

static uint8_t testBit(Bitstream const *const reader) {
    return (*reader->current_byte & reader->mask) != 0;
}

uint32_t thReadBits(Bitstream *const stream, uint32_t bits) {
    uint32_t result = 0;
    while(bits--) {
        if(!stream->mask) getNextByte(stream);
        result <<= 1;
        result |= testBit(stream);
        stream->mask >>= 1;
    }
    return result;
}
uint8_t thReadString(Bitstream *const stream, char *const out,
                     const uint32_t max) {
    for(uint32_t idx = 0; idx < max; ++idx) {
        out[idx] = thReadBits(stream, 8);
        if(out[idx] == 0) return 0;
    }
    return 1;
}
