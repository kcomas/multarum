
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
    TGL, // List
    TGF, // Function
    TGE, // End Statement
    _TGS // Size of group
} TG; // token group

td st {
    TG g; // the tokens group
    C t; // the token value
} TN; // token

td st {
    TG g;
    I (*fn)(C);
} TFS; // functions that group a token

#define qtfns(g, f) (TFS) { g, f }

td st _TNS {
    ST i, bl, fnsi, fnsl; // the index in the buffer, buffer len, registerd fns, total fns
    S b; // the buffer to tokenize
    TFS fns[];
} *TNS; // token state

TNS tnsi(S b, ST bl, ST fnsl); // init token state

V tnsf(TNS s); // token state free

I tnsnt(TNS s, TN *t); // get next token return 0 on done

I tnsrf(TNS s, TFS f); // register token fn

I tnopfn(C t); // token operator fn

I tnvfn(C t); // token variable fn

I tnnufn(C t); // token number fn

I tngfn(C t); // token grouping

I tnlfn(C t); // token list

I tnffn(C t); // token fn

I tnefn(C t); // token end statement

#endif
