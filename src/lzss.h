#if !defined(LZSS_H)
#define LZSS_H

#include <stdint.h>

void *thDecompress(uint8_t *data, const uint32_t uncompressed_size);

#endif // !defined(LZSS_H)
