#ifndef _RIPR_MAIN_
#define _RIPR_MAIN_

#define RULE(r) r, sizeof(r)/sizeof(char)-1

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "keys.h"

struct {
    int x;
    int y;
    int m;
    int o;
} scr;

struct {
    int len;
    
    int x; // Height
    int s; // Selected X
    int o; // Offset x from top
} list_scr;

struct cmd {
    char c[64];
    int len;
};

extern struct cmd cmdline;

struct cmdlist {
    struct cmd c[256];
    int len;
};

extern struct cmdlist cmds;
extern const struct cmd defaults[32];

void ncui_loop();
void ncui_init();
void ncui_end();

void ncui_list_print();
void ncui_list_swap(int, int);
void ncui_list_del(int);
void ncui_list_add(struct cmd);
void ncui_list_up();
void ncui_list_dn();

void ncui_clear_row(int);
void ncui_print_cmd();

void cmds_save(int);
void cmds_load(char[]);
void populate();
void ripr_run();

#endif
