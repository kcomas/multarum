
#include "t.h"

tns tnsi(S b, ST bl, ST fnsl) {
    tns s = al(sz(st _tns) + fnsl * sz(tfs));
    s->bl = bl;
    s->b = b;
    s->fnsl = fnsl;
    s->i = 0;
    R s; }

V tnsf(tns s) {
    free(s); }

I tnsnt(tns s, tn *t) {
    while (s->i < s->bl) { for (ST i = 0; i < s->fnsi; i++) if (s->fns[i].fn(s->b[s->i])) {
            t->g = s->fns[i].g;
            t->t = s->b[s->i++];
            R 1; }
    s->i++; } R 0; }

I tnsrf(tns s, tfs fs) {
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

I tnefn(C t) {
    R t == ';'; }
