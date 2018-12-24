
#include "d.h"
#include "t.h"

I main(V) {
    S s = "fn: { [x;y] (x + y) * 2 }; fn[23;43] - 10;";
    TNS ts = tnsi(s, strl(s), _TGS);
    tnsrf(ts, qtfns(TGO, &tnopfn));
    tnsrf(ts, qtfns(TGV, &tnvfn));
    tnsrf(ts, qtfns(TGN, &tnnufn));
    tnsrf(ts, qtfns(TGG, &tngfn));
    tnsrf(ts, qtfns(TGL, &tnlfn));
    tnsrf(ts, qtfns(TGF, &tnffn));
    tnsrf(ts, qtfns(TGE, &tnefn));
    TN t;
    while (tnsnt(ts, &t)) pf("%c\n", t.t);
    tnsf(ts);
    R 0; }
