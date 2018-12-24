
#ifndef T_H
#define T_H

#include "d.h"

/**
 * User registers tokens into the token state and provides a group
 */

td en {
    TGI, // Invalid
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
    C t; // the token char
} TN; // token

td en {
    ADD,
    SUB,
    MUL,
    DIV,
    REM,
    GET
} OP;

td st {
    TG g; // the tokens group
    un {
     J j;
     S s;
     OP op;
    } v; // token value
} TNG; // grouped token

td st {
    TG g;
    I (*fn)(C);
    // I (*gfn)(S, ST, TNG*); // grouping function
} TFS; // functions that group a token

#define qtfns(g, fa) (TFS) { g, fa }

td st _TNS {
    ST i, bl, fnsi, fnsl; // the index in the buffer, buffer len, registerd fns, total fns
    TG g; // the last grouping
    ST ri, rl; // resevrer index, reserve length
    S r; // reserve for holding token parts
    S b; // the buffer to tokenize
    TFS fns[];
} *TNS; // token state

TNS tnsi(S b, ST bl, ST fnsl); // init token state

V tnsf(TNS s); // token state free

I tnsnt(TNS s, TN *t); // get next token return 0 on done

I tnsntg(TNS s, TNG *t); // get the next grouped token

I tnsrf(TNS s, TFS f); // register token fn

I tnopfn(C t); // token operator fn

I tnopgn(S r, ST ri, TNG *tg); // group token operators

I tnvfn(C t); // token variable fn

I tnnufn(C t); // token number fn

I tngfn(C t); // token grouping

I tnlfn(C t); // token list

I tnffn(C t); // token fn

I tnefn(C t); // token end statement

#endif
