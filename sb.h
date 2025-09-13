
/*
    This code is not production ready, so use it on
    your own risk. 
*/
#ifndef SB_H
#define SB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCALE_FACTOR (1.5f)
#define MINIMAL_SIZE (16)

typedef struct
{
    char* data;
    size_t size;
} string_builder;

void sb_new(string_builder* sb);
void sb_free(string_builder* sb);

void sb_append(string_builder* sb, const char* value);

// *** SB_IMPLEMENTATION ***
#ifdef SB_IMPLEMENTATION

void sb_new(string_builder* sb)
{
    if (sb->data || sb->size != 0)
    {
        free(sb->data);
        sb->data = NULL;
        sb->size = 0;
    }

    sb->data = malloc(MINIMAL_SIZE);
    if (!sb->data)
    {
        printf(" [ ERROR ] string_builder->data malloc failed");
        return;
    }
    sb->size = MINIMAL_SIZE;
    sb->data[0] = '\0';
}
void sb_free(string_builder* sb)
{
    if (!sb) return;

    free(sb->data);
    sb->data = NULL;
    sb->size = 0;
}

void sb_append(string_builder* sb, const char* value)
{
    size_t clen = strlen(sb->data);
    size_t len = strlen(value);

    if (clen + len + 1 > sb->size)
    {
        sb->size = (size_t)((clen + len + 1) * SCALE_FACTOR);
        char* temp = realloc(sb->data, sb->size);

        if (!temp)
        {
            printf("[ ERROR ] sb_append 'temp' reallocation failed");
            sb_free(sb);
            return;
        }
        sb->data = temp;
    }
    memcpy(sb->data + clen, value, len);
    sb->data[clen + len] = '\0';
}

#endif //SB_IMPLEMENTATION

#endif //SB_H

/*
Code is guarded by MIT license
===============================================
MIT License

Copyright (c) 2025 Nevostruev Alexander

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
===============================================
*/