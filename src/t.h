
#ifndef T_H
#define T_H

#include "d.h"

/**
 * User registers tokens into the token state and provides a group
 */

td en {
    TGO, // Operator
    TGV, // Variable
    TGN, // Number
    TGG, // Grouping
    TGE, // End Statement
    _TGS // Size of group
} tg; // token group

td st {
    tg g; // the tokens group
    C t; // the token value
} tn; // token

td st {
    tg g;
    I (*fn)(C);
} tfs; // functions that group a token

#define qtfns(g, f) (tfs) { g, f }

td st _tns {
    ST i, bl, fnsi, fnsl; // the index in the buffer, buffer len, registerd fns, total fns
    S b; // the buffer to tokenize
    tfs fns[];
} *tns; // token state

tns tnsi(S b, ST bl, ST fnsl); // init token state

V tnsf(tns s); // token state free

I tnsnt(tns s, tn *t); // get next token return 0 on done

I tnsrf(tns s, tfs f); // register token fn

I tnopfn(C t); // token operator fn

I tnvfn(C t); // token variable fn

I tnnufn(C t); // token number fn

I tngfn(C t); // token grouping

I tnefn(C t); // token end statement

#endif
