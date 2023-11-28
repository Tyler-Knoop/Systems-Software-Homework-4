/* $Id: gen_code.h,v 1.12 2023/11/27 00:36:33 leavens Exp $ */
#ifndef _GEN_CODE_H
#define _GEN_CODE_H
#include <stdio.h>
#include "ast.h"
#include "bof.h"
#include "instruction.h"
#include "code.h"


// Initialize the code generator
extern void gen_code_initialize();

// Requires: bf if open for writing in binary
// Generate code for prog into bf
extern void gen_code_program(BOFFILE bf, block_t prog);

// Requires: bf if open for writing in binary
// Generate code for the given AST
extern code_seq gen_code_block(block_t blk);

// Generate code for the const-decls, cds
// There are 3 instructions generated for each identifier declared
// (one to allocate space and two to initialize that space)
extern code_seq gen_code_const_decls(const_decls_t cds);

// Generate code for the const-decl, cd
extern code_seq gen_code_const_decl(const_decl_t cd);

// Generate code for the const-defs, cdfs
extern code_seq gen_code_const_defs(const_defs_t cdfs);

// Generate code for the const-def, cdf
extern code_seq gen_code_const_def(const_def_t cdf);

// Generate code for the var_decls_t vds to out
// There are 2 instructions generated for each identifier declared
// (one to allocate space and another to initialize that space)
extern code_seq gen_code_var_decls(var_decls_t vds);

// Generate code for a single <var-decl>, vd,
// There are 2 instructions generated for each identifier declared
// (one to allocate space and another to initialize that space)
extern code_seq gen_code_var_decl(var_decl_t vd);

// Generate code for the identififers in idents
// in reverse order (so the first declared are allocated last).
// There are 2 instructions generated for each identifier declared
// (one to allocate space and another to initialize that space)
extern code_seq gen_code_idents(idents_t idents);

// (Stub for:) Generate code for the procedure declarations
extern void gen_code_proc_decls(proc_decls_t pds);

// (Stub for:) Generate code for a procedure declaration
extern void gen_code_proc_decl(proc_decl_t pd);

// Generate code for stmt
extern code_seq gen_code_stmt(stmt_t stmt);

// Generate code for stmt
extern code_seq gen_code_assign_stmt(assign_stmt_t stmt);

// Generate code for stmt
extern code_seq gen_code_call_stmt(call_stmt_t stmt);

// Generate code for stmt
extern code_seq gen_code_begin_stmt(begin_stmt_t stmt);

// Generate code for the list of statments given by stmts
extern code_seq gen_code_stmts(stmts_t stmts);

// Generate code for the if-statment given by stmt
extern code_seq gen_code_if_stmt(if_stmt_t stmt);

// Generate code for the if-statment given by stmt
extern code_seq gen_code_while_stmt(while_stmt_t stmt);

// Generate code for the read statment given by stmt
extern code_seq gen_code_read_stmt(read_stmt_t stmt);

// Generate code for the write statment given by stmt.
extern code_seq gen_code_write_stmt(write_stmt_t stmt);

// Generate code for the skip statment, stmt
extern code_seq gen_code_skip_stmt(skip_stmt_t stmt);

// Generate code for cond, putting its truth value
// on top of the runtime stack
// and using V0 and AT as temporary registers
// May modify HI,LO when executed
extern code_seq gen_code_condition(condition_t cond);

// Generate code for cond, putting its truth value
// on top of the runtime stack
// and using V0 and AT as temporary registers
// Modifies SP, HI,LO when executed
extern code_seq gen_code_odd_condition(odd_condition_t cond);

// Generate code for cond, putting its truth value
// on top of the runtime stack
// and using V0 and AT as temporary registers
// May also modify SP, HI,LO when executed
extern code_seq gen_code_rel_op_condition(rel_op_condition_t cond);

// Generate code for the rel_op
// applied to 2nd from top and top of the stack,
// putting the result on top of the stack in their place,
// and using V0 and AT as temporary registers
// May also modify SP, HI,LO when executed
extern code_seq gen_code_rel_op(token_t rel_op);

// Generate code for the expression exp
// putting the result on top of the stack,
// and using V0 and AT as temporary registers
// May also modify SP, HI,LO when executed
extern code_seq gen_code_expr(expr_t exp);

// Generate code for the expression exp
// putting the result on top of the stack,
// and using V0 and AT as temporary registers
// May also modify SP, HI,LO when executed
extern code_seq gen_code_binary_op_expr(binary_op_expr_t exp);

// Generate code to apply arith_op to the
// 2nd from top and top of the stack,
// putting the result on top of the stack in their place,
// and using V0 and AT as temporary registers
// May also modify SP, HI,LO when executed
extern code_seq gen_code_arith_op(token_t arith_op);

// Generate code to put the value of the given identifier
// on top of the stack
// Modifies T9, V0, and SP when executed
extern code_seq gen_code_ident(ident_t id);

// Generate code to put the given number on top of the stack
extern code_seq gen_code_number(number_t num);

#endif
