#define SB_IMPLEMENTATION
#include "sb.h"

int main(void)
{
    string_builder sb = {0};
    // void sb_new(string_builder* sb);
    sb_new(&sb);

    // void sb_append(string_builder* sb, const char* value);
    sb_append(&sb, "Hello World!\n");
    printf("%s", sb.data); //Hello World\n

    //char sb_at(string_builder* sb, size_t pos)
    printf("%c\n", sb_at(&sb, 2)); //l

    // const char* sb_take(string_builder* sb, size_t at, size_t count)
    printf("%s\n", sb_take(&sb, 6, 5)); //World

    // void sb_clear(string_builder* sb);
    sb_clear(&sb);
    sb_append(&sb, "foo bar baz\n");
    printf("%s", sb.data); //foo bar baz\n

    // int sb_length(string_builder* sb);
    printf("%d\n", sb_length(&sb)); //12

    string_builder src = {0};
    sb_new(&src);
    sb_append(&src, "foo bar baz ?!\n");
    printf("%s", src.data); //foo bar baz ?!

    // void sb_realloc(string_builder* sb, size_t size)
    sb_realloc(&src, 5); //foo bar baz ?! -> foo b

    // void sb_copy(string_builder* dest, string_builder* src)
    sb_copy(&sb, &src);
    printf("%s\n", sb.data); //foo b

    // void sb_free(string_builder* sb)
    sb_free(&sb);
    return 0;
}