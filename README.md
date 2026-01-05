# Static Ring Buffer

A lightweight and quick circular buffer implementation designed for embedded systems.

## Features

* Written in C (C89), compatible with ``size_t`` for size data types
* Platform independent default code - with restrictions for smaller CPU architectures
* FIFO (First In First Out) buffer implementation
* No dynamic memory allocation, data is storaged in static array
* Uses optimized memory copy instead of loops to read/write data from/to memory
* Suitable for DMA transfers from and to memory with zero-copy overhead between buffer and application memory
* Supports data peek, skip for read and advance for write
* User friendly Apache-2.0 license

## Contribute

Fresh contributions are always welcome. Simple instructions to proceed:

1. Fork Github repository
2. Follow [MISRA C:2012](https://582328.fs1.hubspotusercontent-na1.net/hubfs/582328/GrammaTech-Files/MISRA_C-2012_7.3.pdf) already used in the project
3. Create a pull request to develop branch with new features or bug fixes

Alternatively you may:

1. Report a bug
2. Ask for a feature request
