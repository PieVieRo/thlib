#if !defined(BITSTREAM_H)
#define BITSTREAM_H

#include <stdint.h>

typedef struct {
    uint8_t *data;
    uint8_t *current_byte;
    uint8_t mask;
} Bitstream;

#define BITSTREAM_INIT(source)                                              \
    (Bitstream){ .data = (source), .current_byte = (source), .mask = 0x80 }

uint32_t thReadBits(Bitstream *const stream, uint32_t bits);
uint8_t thReadString(Bitstream *const stream, char *const out,
                     const uint32_t max);

#endif // !defined(BITSTREAM_H)
