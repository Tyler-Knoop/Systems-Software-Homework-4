#include "ast.h"
#include "code.h"
#include "gen_code.h"
#include "literal_table.h"
#include "regname.h"

void gen_code_initialize()
{
    literal_table_initialize();
}

void gen_code_program(BOFFILE bf, block_t prog)
{
}

code_seq gen_code_block(block_t blk)
{
}

code_seq gen_code_const_decls(const_decls_t cds)
{
}

code_seq gen_code_const_decl(const_decl_t cd)
{
}

code_seq gen_code_const_defs(const_defs_t cdfs)
{
}

code_seq gen_code_const_def(const_def_t cdf)
{
}

code_seq gen_code_var_decls(var_decls_t vds)
{
}

code_seq gen_code_var_decl(var_decl_t vd)
{
}

code_seq gen_code_idents(idents_t idents)
{
}

code_seq gen_code_proc_decls(proc_decls_t pds)
{
}

code_seq gen_code_proc_decl(proc_decl_t pd)
{
}

code_seq gen_code_stmt(stmt_t stmt)
{
}

code_seq gen_code_assign_stmt(assign_stmt_t stmt)
{
}

code_seq gen_code_call_stmt(call_stmt_t stmt)
{
}

code_seq gen_code_begin_stmt(begin_stmt_t stmt)
{
}

code_seq gen_code_stmts(stmts_t stmts)
{
}

code_seq gen_code_if_stmt(if_stmt_t stmt)
{
}

code_seq gen_code_while_stmt(while_stmt_t stmt)
{
}

code_seq gen_code_read_stmt(read_stmt_t stmt)
{
}

code_seq gen_code_write_stmt(write_stmt_t stmt)
{
    code_seq ret = gen_code_expr(stmt.expr);
    ret = code_seq_concat(ret, code_pop_stack_into_reg(A0));
    return code_seq_add_to_end(ret, code_pint());
}

code_seq gen_code_skip_stmt(skip_stmt_t stmt)
{
}

code_seq gen_code_condition(condition_t cond)
{
}

code_seq gen_code_odd_condition(odd_condition_t cond)
{
}

code_seq gen_code_rel_op_condition(rel_op_condition_t cond)
{
}

code_seq gen_code_rel_op(token_t rel_op)
{
}

code_seq gen_code_expr(expr_t exp)
{
}

code_seq gen_code_binary_op_expr(binary_op_expr_t exp)
{
}
code_seq gen_code_arith_op(token_t arith_op)
{
}

code_seq gen_code_ident(ident_t id)
{
}

code_seq gen_code_number(number_t num)
{
}