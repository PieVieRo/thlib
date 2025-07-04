ThLib
---
Library (**HEAVILY** W.I.P.) for working with Touhou files, written in C17
(will probably bump it down in the future).

This library focuses on readability and was made to document various Touhou file formats (primarily .dat archives)

Current Progress
---
- [ ] .dat files
  - [ ] Original (Touhou 1-2)
  - [ ] Legacy (Touhou 3-5)
  - [x] PBG3 (Touhou 6)
  - [x] PBG4 (Touhou 7)
  - [ ] PBGZ (Touhou 8-9)
  - [ ] Modern (Touhou 9.5-20)
  - [ ] Tasofro Legacy (7.5, Twilight Frontier's fangames)
  - [ ] Tasofro Modern (10.5-12.3)
- [ ] .anm files
- [ ] .ecl files
- [ ] .msg files
- [ ] .std files

Major thanks to
---
ZUN for making Touhou :)
[thtk](https://github.com/thpatch/thtk) for making this possible
[th06 decomp](https://github.com/happyhavoc/th06) for having a simpler PBG3 implementation
[th08 decomp](https://github.com/happyhavoc/th08) for having a simpler PBGZ implementation
[This file](https://github.com/GovanifY/PyTouhou/blob/master/pytouhou/utils/lzss.pyx) for making me finally grok LZSS decompression
[this vid](https://www.youtube.com/watch?v=08f5Dav72aE) for teaching me how to create libraries in CMake
