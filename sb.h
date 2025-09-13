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


/*
    ********************************
          SB CORE MANIPULATIONS
    ********************************
*/
/*
    desc:
        allocates new string_builder dynamicly with base size
        of 'MINIMAL_SIZE'
        
    input:
        string_builder* sb - reference to a sstring_builder

    NOTES:
        * Allocation of sb is contiguous so it is better to not use 
        this library for satrings larger then ~ 300mb

        * If sb is already initialised all data will be lost 
        when second initialization would take place, data is
        cleared at the initialisation state;
*/
void sb_new(string_builder* sb);
/*
    desc:
        free the data of the string_builder's buffer
        essentialy clearing it.

    input:
        string_builder* sb - reference to a string_builder

    NOTES:
        * You are responsible for freeng string_builder
        Usage after free is only single-thread safe. 
 
        * If you want only to clear string_builder, better to call
        sb_new(string_builder*), because data would be freed and
        reallocated with 'MINIMAL_SIZE', it is irrelevent to create a
        new object or manually free string_builder.
*/
void sb_free(string_builder* sb);
/*
    desc:
        safly allocates buffer of desired size for a string_builder

    input: 
        string_builder* sb - reference to a string_builder
        size_t size in bytes to allocate

    NOTES:
        * 'sb_realloc()' reallocates existing buffer, if any, so contents would
        not be lost, due to this fact it could not be used as 'sb_new()' in
        case of clearing a buffer

*/
void sb_realloc(string_builder* sb, size_t size);

/*
    ********************************
        SB CONTENT MANIPULATIONS
    ********************************
*/
/*
    desc:
        appends string_builder's buffer at the end with cstring provided. 
        buffer is expanded dynamicly with a 'SCALE_FACTOR'

    input:
        string_builder* sb - reference to a string_builder

    NOTES: 
        * makes a copy for a stored string_builder string and a copy
        for `const char*` string provided, so it is quite inefficient
        on a large text scale, can result in cash losses, memory allocation failures
        or memory fragmentation, be aware.
*/
void sb_append(string_builder* sb, const char* value);
/*
    desc:
        clears string_builder's buffer, allocates a new string_builder
        if it is not initialised before clearing

    input:
        string_builder* sb - reference to a string_builder

    NOTES:
        * if string_builder is not initialised, sb_clear would
        initialize it with 'MINIMAL_SIZE'

        * sb_clear is just a wrapper of sb_new() for clearer readability
*/
void sb_clear(string_builder* sb);
/*
    desc:
        safly copies contents from one string_builder to another 

    input: 
        string_builder* dest - reference to a string_builder
            in wich data would be copied
        string_builder* src - reference to a string_builder
            from wich data would be copied

*/
void sb_copy(string_builder* dest, string_builder* src);

/*
    ********************************
        SB CONTENT ACCESSING
    ********************************
*/
/*
    desc:
        safly returns a character from a string_builder's buffer

    input: 
        string_builder* sb - reference to a string_builder
        size_t pos - pos of a character that should be returned

    return:
        character, in case of an error `underflow | overflow` returns '\0'
*/
char sb_at(string_builder* sb, size_t pos);
/*
    desc:
        safly takes a part of a string_buiilder's buffer
    
    input: 
        string_builder* sb - reference to a string_builder
        size_t at - start point in the string_builder's buffer
        size_t count - length of the string to take  

    return:
        cstyle string, in case of an error 
        `underflow | overflow | bad allocation etc..` 
        returns "" - emty string 
*/
const char* sb_take(string_builder* sb, size_t at, size_t count);
/*
    desc:
        safly gets length of a stored data     
    
    input: 
        string_builder* sb - reference to a string_builder

    return:
        int size of string_builder's buffer - 
        size of stored string, if string_builder is not
        initialised or data is not properly allocated 
        returns -1 

*/
int sb_length(string_builder* sb);

// *** SB_IMPLEMENTATION ***
#ifdef SB_IMPLEMENTATION

void sb_new(string_builder* sb)
{
    free(sb->data);
    sb->data = NULL;
    sb->size = 0;

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
void sb_realloc(string_builder* sb, size_t size)
{
    if (!sb->data || sb->size == 0 || size == 0)
    {
        sb_new(sb);
        return;
    }

    char* temp = realloc(sb->data, size);
    if (!temp)
    {
        printf("[ ERROR ] 'sb_realloc' 'temp' allocation failed");
        return;
    }

    temp[size] = '\0';
    sb->size = size;
    sb->data = temp;
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
void sb_clear(string_builder* sb) { sb_new(sb); }
void sb_copy(string_builder* dest, string_builder* src)
{
    if (!src->data || src->size == 0)
    {
        sb_new(src);
        sb_new(dest);
        return;
    }

    sb_new(dest);
    sb_realloc(dest, src->size);
    memcpy(dest->data, src->data, src->size);
}

char sb_at(string_builder* sb, size_t pos)
{
    if (sb->size < pos || sb->size == 0) return '\0';
    return sb->data[pos];
}
const char* sb_take(string_builder* sb, size_t at, size_t count)
{
    if (sb_at(sb, at + count) == '\0') return "";

    char* result = malloc(count + 1);
    if (!result)
    {
        printf("[ ERROR ] 'sb_take' 'result' allocation failed");
        return "";
    }

    for (int i = 0; i < count; i++)
    {
        result[i] = sb->data[at + i];
    }
    result[count + 1] = '\0';
    return result;
}
int sb_length(string_builder* sb)
{
    if (!sb->data || sb->size == 0 || sb->size < strlen(sb->data))
        return -1;
    else
        return strlen(sb->data);
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