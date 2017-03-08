#ifndef _RIPR_NCUI_
#define _RIPR_NCUI_

#include <ncurses.h>
#include "rule.h"
#include "keyw.h"

struct {
    int x;
    int y;
} screen_dim;

struct {
    struct ruleset rules[128];
    int len;
    
    int x; // Height
    int y; // Width
    int s; // Selected X
    int o; // Offset x from top
} scroll_scr;

struct {
    struct ruleset rule;
    struct macro macro[64];
    
    char raw_str[64];
    int key_len;
    int raw_len;
} cmd;

#endif
