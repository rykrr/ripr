#ifndef _RIPR_KEYW_
#define _RIPR_KEYW_

#define KEYW(w) w, (sizeof(w)/sizeof(char))-1

struct macro {
    char c;     // Character to expand
    
    char m[50]; // RIPR Macro
    int  l;     // RIPR Macro length
    
    char M[50]; // IPTABLES Macro
    int  L;     // IPTABLES Macro length
};

const struct macro macros[] = {
    
    {'a',   KEYW("append"), KEYW("-A")                      },
    {'r',   KEYW("remove"), KEYW("-D")                      },
    
    {'p',   KEYW("proto"),  KEYW("-p")                      },
    {'t',   KEYW("tcp"),    KEYW("tcp")                     },
    {'u',   KEYW("ucp"),    KEYW("ucp")                     },
    {'m',   KEYW("icmp"),   KEYW("icmp")                    },
    
    {'S',   KEYW("SRC"),    KEYW("-s")                      },
    {'D',   KEYW("DST"),    KEYW("-d")                      },
    {'s',   KEYW("sport"),  KEYW("--sport")                 },
    {'d',   KEYW("dport"),  KEYW("--dport")                 },
    
    {'I',   KEYW("INPUT"),  KEYW("INPUT")                   },
    {'O',   KEYW("OUTPUT"), KEYW("OUTPUT")                  },
    {'L',   KEYW("LOG"),    KEYW("LOG")                     },
    
    {'c',   KEYW("ct"),     KEYW("-m conntrack --ctstate")  },
    
    {'\a',  KEYW(""),       KEYW("iptables")                },
    {'\0',  KEYW("\0"),     KEYW("\0")                      },
};

struct macro key_expand(char);
void key_addsp(char[], int*);
void key_append(char[], const struct macro*, int*, int);
void key_parse(char[], const char[], int);

#endif
