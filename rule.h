#ifndef _RIPR_RULES_
#define _RIPR_RULES_

typedef uint unsigned int;
typedef schr char[128];

struct ruleset {
    
    uint index;
    
    uint table : 4; // Table    [ 0: Filter | 1: NAT    | 2: Mangle ]
    uint chain : 4; // Chain    [ 0: IN     | 1: OUT    | 2: FWD    ]
    unit tname : 4; // Target   [ 0: DROP   | 1: ACCEPT | 2: LOG    ]
    
    uint proto : 4; // Protocol [ 0: NULL   | 1: TCP    | 2: UDP    ]
                    //          [ 3: ICMP   | 4: SCTP   | 5: ESP    ]
    
    uint cstat : 4; // ctstate  [ 0: NULL   | 1: NEW    | 2: EST    ]
                    //          [ 3: REL    | 4: INV    |           ]
    
    uint tflag : 4; // tcpflag  [ 0: NULL   | 1: ACK    | 2: SYN    ]
    
    schr inter;     // Interfaces
    
    schr srcip;     // Source IP Address
    schr dstip;     // Destination IP Address
};

struct policy {
    
    uint table : 4; // Table    [ 0: Filter | 1: NAT    | 2: Mangle ]
    uint chain : 4; // Chain    [ 0: IN     | 1: OUT    | 2: FWD    ]
    unit tname : 4; // Target   [ 0: DROP   | 1: ACCEPT | 2: LOG    ]
};

#endif
