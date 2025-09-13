#define SB_IMPLEMENTATION
#include "sb.h"

int main(void)
{
    string_builder sb = {0};
    // void sb_new(string_builder* sb);
    sb_new(&sb);

    // void sb_append(string_builder* sb, const char* value);
    sb_append(&sb, "Hello World\n");
    printf("%s", sb.data); //Hello World\n

    //char sb_at(string_builder* sb, size_t pos)
    printf("%c\n", sb_at(&sb, 2)); //l

    // const char* sb_take(string_builder* sb, size_t at, size_t count)
    printf("%s\n", sb_take(&sb, 6, 5)); //World

    // void sb_free(string_builder* sb)
    sb_free(&sb);
    return 0;
}