#include <stdio.h>
#include "keyw.h"

int main() {
    struct macro a = key_expand('a');
    printf("First: %s\n", a.m);
    
    char out[512];
    char in[] = "aIpts22S192.168.1.1";
    char inx[] = "aIpts22S192.168.1.1\0";
    
    printf("In:  %s\n", inx);
    key_parse(out, in, sizeof(in)/sizeof(char));
    printf("Out: %s\n", out);
}
