
#include "d.h"
#include "t.h"

I main(V) {
    S s = "abc: (11 + 12) * 10;";
    tns ts = tnsi(s, strl(s), _TGS);
    tnsrf(ts, qtfns(TGO, &tnopfn));
    tnsrf(ts, qtfns(TGV, &tnvfn));
    tnsrf(ts, qtfns(TGN, &tnnufn));
    tnsrf(ts, qtfns(TGG, &tngfn));
    tnsrf(ts, qtfns(TGE, &tnefn));
    tn t;
    while (tnsnt(ts, &t)) pf("%c\n", t.t);
    tnsf(ts);
    R 0; }
