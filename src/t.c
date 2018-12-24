
#include "t.h"

TNS tnsi(S b, ST bl, ST fnsl) {
    TNS s = al(sz(st _TNS) + fnsl * sz(TFS));
    s->g = TGI;
    s->ri = 0;
    s->rl = 20;
    s->r = al(s->rl);
    s->bl = bl;
    s->b = b;
    s->fnsl = fnsl;
    s->i = 0;
    R s; }

V tnsf(TNS s) {
    free(s); }

I tnsnt(TNS s, TN *t) {
    while (s->i < s->bl) { for (ST i = 0; i < s->fnsi; i++) if (s->fns[i].fn(s->b[s->i])) {
            t->g = s->fns[i].g;
            t->t = s->b[s->i++];
            R 1; }
    s->i++; } R 0; }

static void rp(TNS s, TNG *tg) {
    for (ST i = 0; i < s->ri; i++) putchar(s->r[i]);
    pf("\n"); }
    // for (ST i = 0; I < s->fnsi; i++) if (s->fns[i].g == s->g) R s->fns[i].gfn(s->r, s->ri, tg); }

I tnsntg(TNS s, TNG *tg) {
    TN t;
    while (tnsnt(s, &t)) {
        if (s->g == TGI) {
            s->g = t.g;
            s->r[s->ri++] = t.t;
            cont; }
        if (s->g != t.g) {
            // parse r
            rp(s, tg);
            s->g = t.g;
            s->ri = 0;
            s->r[s->ri++] = t.t;
            R 1; }
        s->r[s->ri++] = t.t;
        if (s->ri == s->rl) bk; }
    rp(s, tg);
    R 0; }

I tnsrf(TNS s, TFS fs) {
    if (s->fnsi == s->fnsl) R 0;
    s->fns[s->fnsi++] = fs;
    R 1; }

I tnopfn(C t) {
    stat ct S o = "+-*/%:";
    for (S i = o; *i != '\0'; i++) if (t == *i) R 1;
    R 0; }

I tnvfn(C t) {
    R (t >= 'a' && t <= 'z') || (t >= 'A' && t <= 'Z'); }

I tnnufn(C t) {
    R t >= '0' && t <= '9'; }

I tngfn(C t) {
    R t == '(' || t == ')'; }

I tnlfn(C t) {
    R t == '[' || t == ']'; }

I tnffn(C t) {
    R t == '{' || t == '}'; }

I tnefn(C t) {
    R t == ';'; }
