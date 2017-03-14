/* Rapid ipTables Rules     */
/* Copyright 2017 Ryan Kerr */

#ifndef _RIPR_KEYS_
#define _RIPR_KEYS_

#define KEYW(w) w, (sizeof(w)/sizeof(char))-1

#ifndef ETHI
#define ETHI "eth0"
#endif

#ifndef ETHO
#define ETHO "eth1"
#endif

#define FWD "FORWARD"

struct macro {
    char c;     // Character to expand
    
    char m[50]; // RIPR Macro
    int  l;     // RIPR Macro length
    
    char M[50]; // IPTABLES Macro
    int  L;     // IPTABLES Macro length
    
    int  s;     // Add space?
};

extern const struct macro macros[];

struct macro key_expand(char);
void key_addsp(char[], int*);
void key_append(int, char[], const struct macro*, int*, int);
void key_parse(int, char[], const char[], int);

#endif
