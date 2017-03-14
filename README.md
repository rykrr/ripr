# RIPR: (Simple) Rapid ipTables Rules
A simple utility for quickly writing, ordering, and executing ipTable rules in a compact manner.
This code was written for use in QNS tournament. While it is extremely simple and does not do
rule checking or dynamic command syntax, it could be used for simple firewalls.

RIPR comes with a few pre-configured rules used mainly for forwarding specific traffic.

Compiled using: gcc -lncurses -oripr key.c ripr.c

Usage:
ripr [ r: run-only | f: load-file ] [ path ]
If left blank, the UI will load the default preset.
