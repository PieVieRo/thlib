Compression
===
In every game since Touhou 6 files are compressed using the LZSS algorithm.
The parameters for the algorithm have never changed. The dictionary size is 8192 (0x2000) bytes and
the minimum match length is 3 bytes. The keys use 13 bits for the offset and 4 bits for the length of the match.
Important to note is that to calculate the match length you need to length value in the key to the minimum.

Encryption 
===
PBGZ introduced encryption into the format. To decrypt encrypted data a function we need 6 values. A key, a value
to increment the key by, size of a chunk, the max amount of bytes to decrypt and the data itself along with its size.

The function first, decrements the data size value by its remainder with chunk size if the remainder is smaller than `chunk size / 4`.
It also decrements the data size by 1 as well if the data size is odd (before the first subtraction).  
The limit is rounded **up** to the nearest multiple of chunk size.  
Then, in every chunk, it takes the first half of the input and XORs it with the key, putting the results in reverse order
every other byte, counting from the end.
Next, it does the same with the other half. So, for example, an 8 byte chunk (1 2 3 4 5 6 7 8) would result in 
the following order (8 4 7 3 6 2 5 1).
Then it increments the key by the specified value and goes through another chunk until it reaches the specified size or the limit.
When the function finishes it copies the chopped off bytes into the output unchanged.

PBG3
===
Used by Touhou 6
### Header
The values within the header aren't byte-aligned
1. "PBG3" without '\0'
2. Number of entries as a variable integer
3. Absolute byte offset to the file table a variable integer
### File table
1. Two unknown variable integers (the game doesn't even use them afaik)
2. Checksum as a variable integer
3. Absolute byte offset to compressed data
4. Size of the ***uncompressed*** data
5. Null terminated string - name of the archive (hardcoded to be 256 characters max including null)
### Files
Files are byte-aligned.
Checksum is the total of the compressed bytes
### Variable integer format
- 2 bits dictate the variant
- formula for length - (variant+1) bytes long
- stored in big-endian format
- examples:
  - `00110011 00` - 1 byte long (variant `00`) integer of value 204 (`11001100`)
  - `01000111 01110110 10001011 1101` - 2 integers. 
  First one is 2 bytes long (variant `01`) of value 7642 (`00011101 11011010`).
  Second one is 1 byte long (variant `00`) of value 189 (`10111101`).

PBG4
===
Used by Touhou 7
### Header
1. "PBG4" without '\0'
2. Number of entries as an unsigned 32 bit integer
3. Absolute offset to the compressed file table as an unsigned 32 bit integer
4. Size of the uncompressed file table as an unsigned 32 bit integer
### File table
LZSS compressed, same as file compression.
1. Name of the file
2. Absolute offset to the entry as an unsigned 32 bit integer
3. Size of the uncompressed entry as an unsigned 32 bit integer
4. Another 32 bit integer (unused, seems to always be zero)
### Files
Files are byte aligned  

PBGZ
===
Used by Touhou 8 and Touhou 9
### Header
The header (without magic) is encrypted with block size 0xC, XOR key 0x1B, increment 0x37, limit 0x400
1. "PBGZ" without '\0'
2. Number of entries (+ 123456) as an unsigned 32 bit integer
3. Absolute offset (+ 345678) to the compressed file table as an unsigned 32 bit integer
4. Size (+567891) of the uncompressed file table as an unsigned 32 bit integer
### File Table
File table is compressed and then encrypted with
block size 0x80, XOR key 0x3E, increment 0x9B and limit 0x400
1. Name of the file
2. Absolute offset to the entry as an unsigned 32 bit integer
3. Size of the uncompressed entry as an unsigned 32 bit integer
4. Another 32 bit integer 
### Files
Files are encrypted with values different for each type and *then* compressed.
1. "edz" without '\0'
2. Type specified as a character
  - 'M' for .msg files
  - 'T' for .txt files
  - 'A' for .anm files
  - 'J' for .jpg files
  - 'E' for .ecl files
  - 'W' for .wav files
  - '-' for any other types
  - '\*' for no type (supported by the game, but unused)
3. Encrypted data

| type \ values | XOR key | XOR increment | block size |  limit |
|--------------:|:--------|:--------------|:-----------|:-------|
|          .msg | 0x1B    | 0x37          | 0x40       | 0x2000 |
|          .txt | 0x51    | 0xE9          | 0x40       | 0x3000 |
|          .anm | 0xC1    | 0x51          | 0x1400     | 0x2000 |
|          .jpg | 0x03    | 0x19          | 0x1400     | 0x7800 |
|          .ecl | 0xAB    | 0xCD          | 0x200      | 0x1000 |
|          .wav | 0x12    | 0x34          | 0x400      | 0x2800 |
|         other | 0x35    | 0x97          | 0x80       | 0x2800 |
|          null | 0x99    | 0x37          | 0x400      | 0x1000 |
