#include <limits.h>
#include <stdlib.h>
#include "ast.h"
#include "bof.h"
#include "code.h"
#include "gen_code.h"
#include "id_attrs.h"
#include "id_use.h"
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

    int vars_len_in_bytes = (code_seq_size(main_cs) / 2) * BYTES_PER_WORD;
    main_cs = code_seq_concat(main_cs, code_save_registers_for_AR());
    main_cs = code_seq_concat(main_cs, gen_code_stmt(prog.stmt));
    main_cs = code_seq_concat(main_cs, code_restore_registers_from_AR());
    main_cs = code_seq_concat(main_cs, code_deallocate_stack_space(vars_len_in_bytes));
    main_cs = code_seq_add_to_end(main_cs, code_exit());

    BOFHeader bfh;
    bfh.text_start_address = 0;
    bfh.text_length = code_seq_size(main_cs) * BYTES_PER_WORD;
    int dsa = MAX(bfh.text_length, 1024);
    bfh.data_start_address = dsa;
    bfh.data_length = literal_table_size() * BYTES_PER_WORD;
    int sba = dsa + bfh.data_start_address + bfh.data_length + STACK_SPACE;
    bfh.stack_bottom_addr = sba;

    bof_write_header(bf, bfh);

    while (!code_seq_is_empty(main_cs))
    {
        bin_instr_t inst = (code_seq_first(main_cs))->instr;
        instruction_write_bin_instr(bf, inst);
        main_cs = code_seq_rest(main_cs);
    }

    literal_table_start_iteration();
    while (literal_table_iteration_has_next())
    {
        word_type w = literal_table_iteration_next();
        bof_write_word(bf, w);
    }
    literal_table_end_iteration();

    bof_close(bf);
}

code_seq gen_code_block(block_t blk)
{
    code_seq ret = gen_code_const_decls(blk.const_decls);
    ret = code_seq_concat(ret, gen_code_var_decls(blk.var_decls));
    gen_code_proc_decls(blk.proc_decls);
    ret = code_seq_concat(ret, gen_code_stmt(blk.stmt));
    return ret;
}

code_seq gen_code_const_decls(const_decls_t cds)
{
    code_seq ret = code_seq_empty();
    const_decl_t *cdp = cds.const_decls;
    while (cdp != NULL)
    {
        ret = code_seq_concat(ret, gen_code_const_decl(*cdp));
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
    const_def_t *cdf = cdfs.const_defs;
    while (cdf != NULL)
    {
        ret = code_seq_concat(ret, gen_code_const_def(*cdf));
        cdf = cdf->next;
    }
}

code_seq gen_code_const_def(const_def_t cdf)
{
    code_seq ret = gen_code_ident(cdf.ident);
    ret = code_seq_concat(ret, gen_code_number(cdf.number));
    return ret;
}

code_seq gen_code_var_decls(var_decls_t vds)
{
    code_seq ret = code_seq_empty();
    var_decl_t *vdp = vds.var_decls;
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
    ident_t *idp = idents.idents;
    while (idp != NULL)
    {
        ret = code_seq_concat(ret, gen_code_ident(*idp));
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
    code_seq ret = gen_code_expr(*(stmt.expr));
    assert(stmt.idu != NULL);
    assert(id_use_get_attrs(stmt.idu) != NULL);
    ret = code_seq_concat(ret, code_pop_stack_into_reg(V0));
    ret = code_seq_concat(ret, code_compute_fp(T9, stmt.idu->levelsOutward));
    unsigned int offset_count = id_use_get_attrs(stmt.idu)->offset_count;
    assert(offset_count <= USHRT_MAX);
    ret = code_seq_add_to_end(ret, code_sw(T9, V0, offset_count));
    return ret;
}

code_seq gen_code_call_stmt(call_stmt_t stmt)
{
}

code_seq gen_code_begin_stmt(begin_stmt_t stmt)
{
    return gen_code_stmts(stmt.stmts);
}

code_seq gen_code_stmts(stmts_t stmts)
{
    code_seq ret = code_seq_empty();
    stmt_t *stmt = stmts.stmts;
    while (stmt != NULL)
    {
        ret = code_seq_concat(ret, gen_code_stmt(*stmt));
        stmt = stmt->next;
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
    unsigned int offset = stmt.idu->attrs->offset_count;
    code_seq ret = code_seq_singleton(code_rch());
    ret = code_seq_add_to_end(ret, code_lw(T9, FP, offset));
    ret = code_seq_add_to_end(ret, code_sw(T9, V0, offset));
    return ret;
}

code_seq gen_code_write_stmt(write_stmt_t stmt)
{
    code_seq ret = gen_code_expr(stmt.expr);
    ret = code_seq_concat(ret, code_pop_stack_into_reg(A0));
    return code_seq_add_to_end(ret, code_pint());
}

code_seq gen_code_skip_stmt(skip_stmt_t stmt)
{
    return code_seq_singleton(code_srl(AT, AT, 0));
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
    /*
    assert(id.idu != NULL);
    code_seq ret = code_compute_fp(T9, id.idu->levelsOutward);
    assert(id_use_get_attrs(id.idu) != NULL);
    unsigned int offset_count = id_use_get_attrs(id.idu)->offset_count;
    assert(offset_count <= USHRT_MAX);
    type_exp_e typ = id_use_get_attrs(id.idu)->type;
    ret = code_seq_add_to_end(ret, code_lw(T9, V0, offset_count));
    return code_seq_concat(ret, code_push_reg_on_stack(V0, typ));
    */
}

code_seq gen_code_number(number_t num)
{
    unsigned int offset = literal_table_lookup(num.text, num.value);
    code_seq ret = code_lw(GP, V0, offset);
    ret = code_seq_concat(ret, code_push_reg_on_stack(V0));
    return ret;
}