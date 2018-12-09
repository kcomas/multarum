
#ifndef MT_REPL_H
#define MT_REPL_H

#include "file.h"
#include "ctx.h"
#include "ast.h"
#include "cgen.h"
#include "vm.h"

typedef struct {
    mt_ast_sym_table* sym_table;
    mt_ctx ctx;
    mt_vm vm;
} mt_repl;

void mt_repl_run(int argc, char** argv);

#endif
