# string_builder.h

Simple extra-small header-only library for more convenient work with strings in C.

## Usage

1. Copy [`sb.h`](https://github.com/DeTosis/sb.h/blob/master/sb.h) file into your project.
2.  In **one** source file define `SB_IMPLEMENTATION` before including it:
```c
#define SB_IMPLEMENTATION
#include "sb.h"
```
3. In other files you could include it normally:
```c
#include "sb.h"
```

You can find usage examples and explanations in [`sb.c`](https://github.com/DeTosis/sb.h/blob/master/sb.c)

> This library folows the single-header style popularized by [stb](https://github.com/nothings/stb).