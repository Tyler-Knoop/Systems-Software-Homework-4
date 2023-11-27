#include <limits.h>
#include <stdlib.h>
#include "ast.h"
#include "bof.h"
#include "code.h"
#include "gen_code.h"
#include "literal_table.h"
#include "machine_types.h"
#include "regname.h"
#include "utilities.h"

#define STACK_SPACE 4096

void gen_code_initialize()
{
    literal_table_initialize();
}

void gen_code_program(BOFFILE bf, block_t prog)
{
    code_seq main_cs = gen_code_block(prog);
    
    //NOT FINISHED, do not know what to do here
}

code_seq gen_code_block(block_t blk)
{
    code_seq ret = gen_code_const_decls(blk.const_decls);
    ret = code_seq_concat(ret, gen_code_var_decls(blk.var_decls));
    ret = code_seq_concat(ret, code_seq_proc_decls(blk.proc_decls));
    ret = code_seq_concat(ret, gen_code_stmt(blk.stmt));
    return ret;
}

code_seq gen_code_const_decls(const_decls_t cds)
{
    code_seq ret = code_seq_empty();
    const_decl_t* cdp = cds.const_decls;
    while (cdp != NULL)
    {
        ret = code_seq_concat(ret, gen_code_var_decl(*cdp));
        cdp = cdp->next;
    }
    return ret;
}

code_seq gen_code_const_decl(const_decl_t cd)
{
    return gen_code_const_defs(cd.const_defs);
}

code_seq gen_code_const_defs(const_defs_t cdfs)
{
    code_seq ret = code_seq_empty();
    const_def_t* cdf = cdfs.const_defs;
    while (cdf != NULL)
    {
        ret = code_seq_concat(ret, gen_code_const_def(*cdf));
        cdf = cdf->next;
    }
}

code_seq gen_code_const_def(const_def_t cdf)
{
    code_seq ret = code_seq_singleton(cd.ident);
    ret = code_seq_concat(ret, gen_code_number(cd.number));
    return ret;
}

code_seq gen_code_var_decls(var_decls_t vds)
{
    code_seq ret = code_seq_empty();
    var_decl_t* vdp = vds.var_decls;
    while (vdp != NULL)
    {
        ret = code_seq_concat(ret, gen_code_var_decl(*vdp));
        vdp = vdp->next;
    }
    return ret;
}

code_seq gen_code_var_decl(var_decl_t vd)
{
    return gen_code_idents(vd.idents);
}

code_seq gen_code_idents(idents_t idents)
{
    code_seq ret = code_seq_empty();
    ident_t* idp = idents.idents;
    while (idp != NULL)
    {
        code_seq alloc_and_init = code_seq_singleton(code_addi(SP, SP, - BYTES_PER_WORD));
        ret = code_seq_concat(ret, alloc_and_init);
        idp = idp->next;
    }
    return ret;
}

void gen_code_proc_decls(proc_decls_t pds)
{
}

void gen_code_proc_decl(proc_decl_t pd)
{
}

code_seq gen_code_stmt(stmt_t stmt)
{
    switch (stmt.stmt_kind)
    {
    case assign_stmt:
        return gen_code_assign_stmt(stmt.data.assign_stmt);
        break;
    case call_stmt:
        return gen_code_call_stmt(stmt.data.call_stmt);
        break;
    case begin_stmt:
        return gen_code_begin_stmt(stmt.data.begin_stmt);
        break;
    case if_stmt:
        return gen_code_if_stmt(stmt.data.if_stmt);
        break;
    case while_stmt:
        return gen_code_while_stmt(stmt.data.while_stmt);
        break;
    case read_stmt:
        return gen_code_read_stmt(stmt.data.read_stmt);
        break;
    case write_stmt:
        return gen_code_write_stmt(stmt.data.write_stmt);
        break;
    case skip_stmt:
        return gen_code_skip_stmt(stmt.data.skip_stmt);
        break;
    default:
        bail_with_error("Unknown stmt_kind (%d) in gen_code_stmt", stmt.stmt_kind);
        break;
    }

    return code_seq_empty();
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
    code_seq ret = code_seq_empty();
    stmt_t *sp = stmts.stmts;
    while (sp != NULL) 
    {
        ret = code_seq_concat(ret, gen_code_stmt(*sp));
        sp = sp->next;
    }
    return ret;
}

code_seq gen_code_if_stmt(if_stmt_t stmt)
{
    // put truth value of stmt.expr in $v0
    code_seq ret = gen_code_expr(stmt.expr);
    ret = code_seq_concat(ret, code_pop_stack_into_reg(V0));
    code_seq cbody = gen_code_stmt(*(stmt.then_stmt));
    int cbody_len = code_seq_size(cbody);
    
    // skip over body if $v0 contains false
    ret = code_seq_add_to_end(ret, code_beq(V0, 0, cbody_len));
    return code_seq_concat(ret, cbody);
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
    switch (cond.cond_kind)
    {
    case ck_odd:
        return gen_code_odd_condition(cond.data.odd_cond);
        break;
    case ck_rel:
        return gen_code_rel_op_condition(cond.data.rel_op_cond);
        break;
    default:
        bail_with_error("Unknown cond_kind (%d) in gen_code_condition", cond.cond_kind);
        break;
    }

    return code_seq_empty();
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
    switch (exp.expr_kind)
    {
    case expr_ident:
        return gen_code_ident(exp.data.ident);
        break;
    case expr_bin:
        return gen_code_binary_op_expr(exp.data.binary);
        break;
    case expr_number:
        return gen_code_number(exp.data.number);
        break;
    default:
        bail_with_error("Unexpected expr_kind (%d) in gen_code_expr", exp.expr_kind);
        break;
    }

    return code_seq_empty();
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