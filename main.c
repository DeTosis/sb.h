#define SB_IMPLEMENTATION
#include "sb.h"

int main(void)
{
    /*
        SB Initialisation
    */
    string_builder sb = {0};
    // sb_new(string_builder*);
    sb_new(&sb);
    /*
        Library Usage:
    */
    // sb_append(string_builder*, const char*) dynamicly expands sb to fit provided string
    sb_append(&sb, "Hello World");

    printf("[%s]", sb.data);

    /*
        You are responsible for freeng sb, so do not forget it
    */
    sb_free(&sb);
    return 0;
}