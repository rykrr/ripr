#include "ripr.h"

struct cmd      cmdline;
struct cmdlist  cmds;

const struct cmd defaults[] = {
    
    /* POLICY RULES */
    {RULE("PIv")                },  // Drop all inbound
    {RULE("POv")                },  // Drop all outbound
    {RULE("P_v")                },  // Drop all forwarding
    
    /* INPUT OUTPUT RULES */
    {RULE("aIpm;v")             },  // Drop icmp packets
    {RULE("aIptd22;A")          },  // Allow SSH to server
    {RULE("aIptd3141;A")        },  // Allow SSH to server
    {RULE("aIc E,R;A")          },  // Allow related traffic in
    {RULE("aOc E,R;A")          },  // Allow related traffic out
    {RULE("aIc i;v")            },  // Drop invalid packets
    
    /* FORWARD RULES */
    {RULE("a>pm;v")             },  // Drop icmp packets
    
    {RULE("a>pt!yc n;v")        },  // Drop New Non-SYN Packets
    {RULE("a>g;v")              },  // Drop fragmented packets
    
    {RULE("a>ptf V V;v")        },  // Drop xmas packets
    {RULE("a>ptf V N;v")        },  // Drop none packets
    
    {RULE("a>c i;v")            },  // Drop invalid packets
    
    {RULE("a>c r,e;A")          },  // Allow EST/REL traffic in
    {RULE("a<c r,e;A")          },  // Allow EST/REL traffic out
    
    {RULE("a>ptd80c n,e;A")     },  // Allow HTTP
    {RULE("a<pts80c e;A")       },  // Allow HTTP
    {RULE("a>ptd443c n,e;A")    },  // Allow HTTPS
    {RULE("a<pts443c e;A")      },  // Allow HTTPS
    
    {RULE("\0")                 },  // END RULESET
};

void ncui_loop() {
    char c=0;
    
    while((c=getch())) {
        if(c == KEY_RESIZE || c == -102) {
            endwin();
            ncui_init();
        }
        
        if(scr.m) {
            switch(c) {
                case 10:
                case 13:
                    ncui_list_add(cmdline);
                    
                    for(int i=0; i<64; i++)
                        cmdline.c[i] = 0;
                    cmdline.len = 0;
                    scr.m = 0;
                    ncui_clear_row(scr.x-1);
                    break;
                    
                case 'Q':
                case 27:
                    for(int i=0; i<64; i++)
                        cmdline.c[i] = 0;
                    cmdline.len = 0;
                    scr.m = 0;
                    ncui_print_cmd();
                    mvprintw(scr.x-1, 0, "  ");
                    break;
                    
                case 7:
                    if(0<cmdline.len)
                        cmdline.c[cmdline.len--] = 0;
                    else
                        cmdline.c[0] = 0;
                    ncui_print_cmd();
                    break;
                    
                default:
                    cmdline.c[cmdline.len++] = c;
                    ncui_print_cmd();
            }
        }
        else {
            switch(c) {
                case 'J':
                    ncui_list_swap(list_scr.s, list_scr.s+1);
                case 'j':
                    ncui_list_dn();
                    break;
                case 'K':
                    ncui_list_swap(list_scr.s, list_scr.s-1);
                case 'k':
                    ncui_list_up();
                    break;
                case 'd':
                    ncui_list_del(list_scr.s);
                    break;
                case ';':
                    scr.m=1;
                    mvprintw(scr.x-1, 0, ">> ");
                    break;
                case 'Q':
                case 'q':
                    ncui_end();
                    break;
                case 's':
                    cmds_save(0);
                    break;
                case 'r':
                    ripr_run();
                    break;
                case 'e':
                    ncui_list_edit();
                    break;
            }
        }
    }
}

void ncui_list_add(struct cmd c) {
    cmds.c[cmds.len++] = c;
    ncui_list_print();
}

void ncui_list_swap(int a, int b) {
    
    if(0 <= a && a < cmds.len && 0 <= b && b < cmds.len) {
        struct cmd c = cmds.c[a];
        cmds.c[a] = cmds.c[b];
        cmds.c[b] = c;
    }
    
    ncui_list_print();
}

void ncui_list_del(int sel) {
    if(cmds.len) {
        for(int i=sel; i<cmds.len; i++) {
            for(int j=0; j<64; j++) {
                cmds.c[i].c[j] = cmds.c[i+1].c[j];
                cmds.c[i].len  = cmds.c[i+1].len;
            }
        }
        cmds.len--;
    }
    ncui_list_up();
    ncui_list_print();
}

void ncui_list_up() {
    
    if(0<list_scr.s) {
        list_scr.s--;
        if(list_scr.s<list_scr.o && list_scr.o)
            list_scr.o--;
        ncui_list_print();
    }
}

void ncui_list_dn() {
    
    if(list_scr.s+1<cmds.len) {
        list_scr.s++;
        if(list_scr.s+1>list_scr.x+list_scr.o)
            list_scr.o++;
        ncui_list_print();
    }
}

void ncui_list_print() {
    
    for(int i=0; i<list_scr.x; i++)
        ncui_clear_row(i+scr.o);
    
    for(int i=0; i<list_scr.x && i+list_scr.o<cmds.len; i++) {
        char out[512];
        key_parse(0, out, cmds.c[i+list_scr.o].c, cmds.c[i+list_scr.o].len);
        
        if(i==list_scr.s-list_scr.o)
            attron(A_BOLD|A_REVERSE);
        
        ncui_clear_row(scr.o+i);
        mvprintw(scr.o+i, 0, "%s", out);
        
        attroff(A_BOLD|A_REVERSE);
    }
}

void ncui_print_cmd() {
    
    ncui_clear_row(scr.x-1);
    mvprintw(scr.x-1, 0, ">>");
    
    if(cmdline.len) {
        char out[512];
        key_parse(1, out, cmdline.c, cmdline.len);
        cmdline.c[cmdline.len] = 0;
        mvprintw(scr.x-1, 2, "%s", out);
    }
}

void ncui_clear_row(int row) {
    for(int i=0; i<scr.y; i++)
        mvprintw(row, i, " ");
}

void ncui_init() {
    
    initscr();
    nonl();
    noecho();
    cbreak();
    
    keypad(stdscr, TRUE);
    curs_set(0);
    
    getmaxyx(stdscr, scr.x, scr.y);
    scr.o = 3;
    scr.m = 0;
    list_scr.x = scr.x-5;
    
    if(scr.x < 16 || scr.y < 8) {
        endwin();
        printf("Screen too small. Exiting.\n");
        cmds_save(1);
        exit(1);
    }
    
    if(has_colors()) {
        start_color();
    }
    
    for(int i=0; i<scr.x; i++)
        ncui_clear_row(i);
    
    attron(A_UNDERLINE);
    for(int i=0; i<scr.y; i++) {
        mvprintw(1, i, " ");
        mvprintw(scr.x-2, i, " ");
    }
    attroff(A_UNDERLINE);
    attron(A_BOLD);
    mvprintw(0, 0, "Simple Rapid ipTables Rules");
    mvprintw(0, scr.y-9, "Ryan Kerr");
    attroff(A_BOLD);
    ncui_list_print();
}

void ncui_end() {
    mvprintw(scr.x-1, 0, "Are you sure? (y/n)");
    char x = getch();
    
    if(x == 'Y' || x == 'y') {
        ncui_clear_row(scr.x-1);
        mvprintw(scr.x-1, 0, "Save? (y/n)");
        
        x = getch();
        
        if(x == 'Y' || x == 'y')
            cmds_save(0);
        else
            cmds_save(1);
        
        endwin();
        exit(0);
    }
    
    ncui_clear_row(scr.x-1);
}

void ncui_list_edit() {
    if(list_scr.s && !cmdline.len) {
        for(int i=0; i<cmds.c[list_scr.s].len; i++)
            cmdline.c[cmdline.len++] = cmds.c[list_scr.s].c[i];
        ncui_print_cmd();
        scr.m=1;
    }
}

void cmds_save(int bck) {
    
    FILE *f;
    
    if(bck)
        f = fopen("backup.ripr", "w");
    else
        f = fopen("ruleset.ripr", "w");
    
    if(f) {
        fprintf(f, "%c\n", '\f');
        for(int i=0; i<cmds.len; i++) {
            for(int j=0; j<cmds.c[i].len; j++)
                fprintf(f, "%c", cmds.c[i].c[j]);
            fprintf(f, "$");
        }
        
        fclose(f);
    }
}

void cmds_load(char path[256]) {
    
    FILE *f = fopen(path, "r");
    
    if(f) {
        int run = 0;
        char c;
        
        while(run != -1 && fscanf(f, "%c", &c) != EOF) {
            
            if(1<run) {
                if(c == '$') {
                    cmds.len++;
                }
                else {
                    int l = cmds.c[cmds.len].len++;
                    cmds.c[cmds.len].c[l] = c;
                }
            }
            else {
                if(!run && c == '\f')
                    run++;
                else if(!run)
                    run=-1;
                else
                    run++;
            }
        }
        
        fclose(f);
    }
}

void ripr_run() {
    
    system("iptables -F");
    system("iptables -X");
    
    for(int i=0; i<cmds.len; i++) {
        char out[256];
        key_parse(0, out, cmds.c[i].c, cmds.c[i].len);
        
        int res = system(out);
    }
}

void populate() {
    for(int i=0; defaults[i].c[0] != '\0'; i++) {
        for(int j=0; j<defaults[i].len; j++)
            cmds.c[i].c[cmds.c[i].len++] = defaults[i].c[j];
        cmds.len++;
    }
}

int main(int argc, char *argv[]) {
    if(3==argc) {
        if(argv[1][0] == 'r') {
            cmds_load(argv[2]);
            ripr_run();
        }
        else if(argv[1][0] == 'f') {
            cmds_load(argv[2]);
            ncui_init();
            ncui_loop();
        }
    }
    else {
        printf("hi");
        populate(cmds);
        ncui_init();
        ncui_loop();
    }
}
