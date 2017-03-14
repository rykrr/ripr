#include "keys.h"

const struct macro macros[] = {
    
    {'a',   KEYW("append"), KEYW("-A"),                     1   },
//    {'r',   KEYW("remove"), KEYW("-D"),                     1   },
    
    {'P',   KEYW("policy"), KEYW("-P"),                     1   },
    
    {'p',   KEYW("proto"),  KEYW("-p"),                     1   },
    {'t',   KEYW("tcp"),    KEYW("tcp"),                    1   },
    {'u',   KEYW("ucp"),    KEYW("ucp"),                    1   },
    {'m',   KEYW("icmp"),   KEYW("icmp"),                   1   },
    
    {'S',   KEYW("src"),    KEYW("-s"),                     1   },
    {'D',   KEYW("dst"),    KEYW("-d"),                     1   },
    {'s',   KEYW("sport"),  KEYW("--sport"),                1   },
    {'d',   KEYW("dport"),  KEYW("--dport"),                1   },
    
    {'#',   KEYW("if"),     KEYW("-i"),                     1   },
    {'@',   KEYW("of"),     KEYW("-o"),                     1   },
    
    {']',   KEYW(ETHI),     KEYW(ETHI),                     1   },
    {'[',   KEYW(ETHO),     KEYW(ETHO),                     1   },
    
    {'}',   KEYW("W>L"),    KEYW("-i "ETHI" -o "ETHO),      1   },
    {'{',   KEYW("L>W"),    KEYW("-i "ETHO" -o "ETHI),      1   },
    
    {'I',   KEYW("IN"),     KEYW("INPUT"),                  1   },
    {'O',   KEYW("OUT"),    KEYW("OUTPUT"),                 1   },
    {'L',   KEYW("LOG"),    KEYW("LOG"),                    1   },
    {'A',   KEYW("ACCEPT"), KEYW("ACCEPT"),                 1   },
    {'v',   KEYW("DROP"),   KEYW("DROP"),                   1   },
    {'>',   KEYW("FWDI"),   KEYW(FWD" -i "ETHI" -o "ETHO),  1   },
    {'<',   KEYW("FWDO"),   KEYW(FWD" -i "ETHO" -o "ETHI),  1   },
    {'_',   KEYW("FWD"),    KEYW(FWD),                      1   },
    
    {'f',   KEYW("tflag"),  KEYW("--tcp-flags"),            1   },
    
    {'Y',   KEYW("SYN"),    KEYW("SYN"),                    0   },
    {'y',   KEYW("syn"),    KEYW("--syn"),                  1   },
    {'K',   KEYW("ACK"),    KEYW("ACK"),                    0   },
    {'R',   KEYW("RST"),    KEYW("RST"),                    0   },
    {'F',   KEYW("FIN"),    KEYW("FIN"),                    0   },
    {'V',   KEYW("ALL"),    KEYW("ALL"),                    0   },
    {'N',   KEYW("NONE"),   KEYW("NONE"),                   0   },
    
    {'g',   KEYW("frag"),   KEYW("-f"),                     1   },
    
    {'r',   KEYW("REL"),    KEYW("RELATED"),                0   },
    {'i',   KEYW("INV"),    KEYW("INVALID"),                0   },
    {'e',   KEYW("EST"),    KEYW("ESTABLISHED"),            0   },
    {'n',   KEYW("NEW"),    KEYW("NEW"),                    0   },
    
    {';',   KEYW("to"),     KEYW("-j"),                     1   },
    {',',   KEYW(","),      KEYW(","),                      0   },
    
    {'c',   KEYW("ct"),     KEYW("-m conntrack --ctstate"), 1   },
    
    {' ',   KEYW(" "),      KEYW(" "),                      0   },
    {'\a',  KEYW(""),       KEYW("iptables"),               0   },
    {'\0',  KEYW("\0"),     KEYW("\0"),                     0   },
};

struct macro key_expand(char c) {
    
    for(int i=0; macros[i].c; i++)
        if(macros[i].c == c)
            return macros[i];
    return (struct macro){'\0'," ",0," ",0,0};
}

inline void key_addsp(char out[], int *len) {
    
    out[(*len)++] = ' ';
}

void key_append(int ncmode, char out[], const struct macro *add, int *len, int asp) {
    
    const char *macro = (ncmode? (add->m) : (add->M));
    int length = (ncmode?add->l:add->L);
    
    if(asp) key_addsp(out, len);
    
    for(int i=0; i<length; i++)
        out[(*len)++] = macro[i];
}

void key_parse(int ncmode, char out[], const char raw[], const int len) {
    
    int front = 1;
    int length = 0;
    struct macro m = key_expand('\a');
    
    if(!ncmode)
        key_append(ncmode, out, &m, &length, 0);
    
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
            key_append(ncmode, out, &m, &length, m.s);
        }
    }
    
    out[length++] = '\0';
}
