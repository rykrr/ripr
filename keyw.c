#include "keyw.h"

struct macro key_expand(char c) {
    
    for(int i=0; macros[i].c; i++)
        if(macros[i].c == c)
            return macros[i];
    return (struct macro){'\0'," ",0," ",0};
}

inline void key_addsp(char out[], int *len) {
    
    out[(*len)++] = ' ';
}

void key_append(char out[], const struct macro *add, int *len, int asp) {
    
    if(asp) key_addsp(out, len);
    
    for(int i=0; i<add->L; i++)
        out[(*len)++] = add->M[i];
}

void key_parse(char out[], const char raw[], const int len) {
    
    int front = 1;
    int length = 0;
    struct macro m = key_expand('\a');
    
    key_append(out, &m, &length, 0);
    
    for(int i=0; i<len; i++) {
        
        if((m=key_expand(raw[i])).c=='\0') {
            
            if(front) {
                key_addsp(out, &length);
                front = 0;
            }
            
            out[length++] = raw[i];
        }
        else {
            front = 1;
            key_append(out, &m, &length, 1);
        }
    }
    
    out[length++] = '\0';
}
