/* $Id: scope_check.c,v 1.16 2023/11/28 11:50:16 leavens Exp leavens $ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scope_check.h"
#include "id_attrs.h"
#include "file_location.h"
#include "ast.h"
#include "utilities.h"
#include "symtab.h"
#include "scope_check.h"

// Build the symbol table for the given program AST
// and Check the given program AST for duplicate declarations
// or uses of identifiers that were not declared
block_t scope_check_program(block_t prog)
{
    return scope_check_block(prog);
}

// build the symbol table and check the declarations in blk
block_t scope_check_block(block_t blk)
{
    symtab_enter_scope();
    scope_check_constDecls(blk.const_decls);
    scope_check_varDecls(blk.var_decls);
    blk.proc_decls = scope_check_procDecls(blk.proc_decls);
    blk.stmt = scope_check_stmt(blk.stmt);
    symtab_leave_scope();
    return blk;
}

// build the symbol table and check the declarations in cds
void scope_check_constDecls(const_decls_t cds)
{
    const_decl_t *dcl = cds.const_decls;
    while (dcl != NULL) {
	scope_check_constDecl(*dcl);
	dcl = dcl->next;
    }
}

// build the symbol table and check the const declaration cd
// and add its const-defs to the current scope's symbol table
// or produce an error if these names have already been declared
void scope_check_constDecl(const_decl_t cd)
{
    scope_check_constDefs(cd.const_defs);
}

// build the symbol table and check the const definitions cdfs
// and add the declared names to the current scope's symbol table
// or produce an error if these names have already been declared
void scope_check_constDefs(const_defs_t cdfs)
{
    const_def_t *cdf = cdfs.const_defs;
    while (cdf != NULL) {
	scope_check_constDef(*cdf);
	cdf = cdf->next;
    }
}

// Put the given name, which is to be declared with kind k,
// and has its declaration at the given file location (floc),
// into the current scope's symbol table at the offset scope_size().
static void add_ident_to_scope(const char *name, id_kind k, file_location floc)
{
    id_use *idu = symtab_lookup(name);
    if (idu != NULL && idu->levelsOutward == 0) {
	bail_with_prog_error(floc,
			     "%s \"%s\" is already declared as a %s",
			     id_attrs_id_kind_string(k),
			     name,
			     id_attrs_id_kind_string(id_use_get_attrs(idu)->kind));
    } else {
	symtab_insert(name,
		      id_attrs_create(floc, k, symtab_scope_loc_count()));
    }
}

// build the symbol table and check the const definition cdf
// and add the declared name to the current scope's symbol table
// or produce an error if this name has already been declared
void scope_check_constDef(const_def_t cdf)
{
    add_ident_to_scope(cdf.ident.name, constant_idk, *(cdf.file_loc));
}

// build the symbol table and check the declarations in vds
void scope_check_varDecls(var_decls_t vds)
{
    var_decl_t *vdp = vds.var_decls;
    while (vdp != NULL) {
	scope_check_varDecl(*vdp);
	vdp = vdp->next;
    }
}

// check the var declaration vd
// and add the declared names to the current scope's symbol table
// or produce an error if the names have already been declared
void scope_check_varDecl(var_decl_t vd)
{
    scope_check_varIdents(vd.idents);
}

// check the identifiers from a var declaration, as given in ids
// and add the declared names to the current scope's symbol table
// or produce an error if the names have already been declared
void scope_check_varIdents(idents_t ids)
{
    ident_t *idp = ids.idents;
    while (idp != NULL) {
	add_ident_to_scope(idp->name, variable_idk, *(idp->file_loc));
	idp = idp->next;
    }
}

// build the symbol table and check the declarations in pds
// Return the modified AST with id_use pointers
proc_decls_t scope_check_procDecls(proc_decls_t pds)
{
    proc_decl_t *pdp = pds.proc_decls;
    while (pdp != NULL) {
	scope_check_procDecl(*pdp);
	pdp = pdp->next;
    }
    return pds;
}

// check the procedure declaration pd
// and add it to the current scope's symbol table
// or produce an error if its name has already been declared
// Return the modified AST with id_use pointers
proc_decl_t scope_check_procDecl(proc_decl_t pd)
{
    // add name to scope first, so that the procedure can be recursive
    add_ident_to_scope(pd.name, procedure_idk, *(pd.file_loc));
    *(pd.block) = scope_check_block(*(pd.block));
    return pd;
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
stmt_t scope_check_stmt(stmt_t stmt)
{
    switch (stmt.stmt_kind) {
    case assign_stmt:
	stmt.data.assign_stmt
	    = scope_check_assignStmt(stmt.data.assign_stmt);
	break;
    case call_stmt:
	stmt.data.call_stmt
	    = scope_check_callStmt(stmt.data.call_stmt);
	break;
    case begin_stmt:
	stmt.data.begin_stmt
	    = scope_check_beginStmt(stmt.data.begin_stmt);
	break;
    case if_stmt:
	stmt.data.if_stmt
	    = scope_check_ifStmt(stmt.data.if_stmt);
	break;
    case while_stmt:
	stmt.data.while_stmt
	    = scope_check_whileStmt(stmt.data.while_stmt);
	break;
    case read_stmt:
	stmt.data.read_stmt
	    = scope_check_readStmt(stmt.data.read_stmt);
	break;
    case write_stmt:
	stmt.data.write_stmt
	    = scope_check_writeStmt(stmt.data.write_stmt);
	break;
    case skip_stmt:
	stmt.data.skip_stmt
	    = scope_check_skipStmt(stmt.data.skip_stmt);
	break;
    default:
	bail_with_error("Unknown stmt_kind (%d) in scope_check_stmt!",
			stmt.stmt_kind);
	break;
    }
    return stmt;
}

// Check the statement to make sure that
// all idenfifiers referenced in it have been declared
// and that the name assigned to is a variable
// (and if one of these does not hold, then produce an error).
// Return the modified AST with id_use pointers
assign_stmt_t scope_check_assignStmt(assign_stmt_t stmt)
{
    const char *name = stmt.name;
    stmt.idu
	= scope_check_ident_declared(*(stmt.file_loc),
				     name);
    assert(stmt.idu != NULL);  // since would bail if not declared
    id_kind k = id_use_get_attrs(stmt.idu)->kind;
    if (k != variable_idk) {
	bail_with_prog_error(*(stmt.file_loc),
			     "Cannot assign to %s, as it is a %s!",
			     stmt.name,
			     id_attrs_id_kind_string(k));
    }
    *(stmt.expr) = scope_check_expr(*(stmt.expr));
    return stmt;
}

// check the statement to make sure that
// the procedure being called has been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
call_stmt_t scope_check_callStmt(call_stmt_t stmt)
{
    const char *name = stmt.name;
    stmt.idu
	= scope_check_ident_declared(*(stmt.file_loc),
				     name);
    assert(stmt.idu != NULL);  // since would bail if not declared
    return stmt;
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
begin_stmt_t scope_check_beginStmt(begin_stmt_t stmt)
{
    stmt.stmts = scope_check_stmts(stmt.stmts);
    return stmt;
}

// check all the statement in stmts to make sure that
// all idenfifiers referenced in them have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
stmts_t scope_check_stmts(stmts_t stmts)
{
    stmt_t *sp = stmts.stmts;
    while (sp != NULL) {
	*sp = scope_check_stmt(*sp);
	sp = sp->next;
    }
    return stmts;
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
if_stmt_t scope_check_ifStmt(if_stmt_t stmt)
{
    stmt.condition = scope_check_condition(stmt.condition);
    *(stmt.then_stmt) = scope_check_stmt(*(stmt.then_stmt));
    *(stmt.else_stmt) = scope_check_stmt(*(stmt.else_stmt));
    return stmt;
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
while_stmt_t scope_check_whileStmt(while_stmt_t stmt)
{
    stmt.condition = scope_check_condition(stmt.condition);
    *(stmt.body) = scope_check_stmt(*(stmt.body));
    return stmt;
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
read_stmt_t scope_check_readStmt(read_stmt_t stmt)
{
    const char *name = stmt.name;
    stmt.idu
	= scope_check_ident_declared(*(stmt.file_loc),
				     name);
    assert(stmt.idu != NULL);  // since would bail if not declared
    return stmt;
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
write_stmt_t scope_check_writeStmt(write_stmt_t stmt)
{
    stmt.expr = scope_check_expr(stmt.expr);
    return stmt;
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
skip_stmt_t scope_check_skipStmt(skip_stmt_t stmt)
{
    // nothing to do!
    return stmt;
}

// check the condition to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
condition_t scope_check_condition(condition_t cond)
{
    switch (cond.cond_kind){
    case ck_odd:
	cond.data.odd_cond
	    = scope_check_odd_condition(cond.data.odd_cond);
	break;
    case ck_rel:
	cond.data.rel_op_cond
	    = scope_check_rel_op_condition(cond.data.rel_op_cond);
	break;
    default:
	bail_with_error("Unexpected type_tag (%d) in scope_check_cond!",
			cond.cond_kind);
	break;
    }
    return cond;
}

// check the condition to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
odd_condition_t scope_check_odd_condition(odd_condition_t cond)
{
    cond.expr = scope_check_expr(cond.expr);
    return cond;
}

// check the condition to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
rel_op_condition_t scope_check_rel_op_condition(rel_op_condition_t cond)
{
    cond.expr1 = scope_check_expr(cond.expr1);
    cond.expr2 = scope_check_expr(cond.expr2);
    return cond;
}

// check the expresion to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
expr_t scope_check_expr(expr_t exp)
{
    switch (exp.expr_kind) {
    case expr_ident:
	exp.data.ident
	    = scope_check_ident_expr(exp.data.ident);
	break;
    case expr_bin:
	scope_check_binary_op_expr(exp.data.binary);
	break;
    case expr_number:
	// no identifiers are possible in this case, so just return
	break;
    default:
	bail_with_error("Unexpected expr_kind_e (%d) in scope_check_expr!",
			exp.expr_kind);
	break;
    }
    return exp;
}

// check the expression (exp) to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
binary_op_expr_t scope_check_binary_op_expr(binary_op_expr_t exp)
{
    *(exp.expr1) = scope_check_expr(*(exp.expr1));
    *(exp.expr2) = scope_check_expr(*(exp.expr2));
    return exp;
}

// check the identifier (id) to make sure that
// all it has been declared (if not, then produce an error)
// Return the modified AST with id_use pointers
ident_t scope_check_ident_expr(ident_t id)
{
    id.idu
	= scope_check_ident_declared(*(id.file_loc),
				     id.name);
    return id;
}

// check that name has been declared,
// if so, then return an id_use for it
// otherwise, produce an error 
id_use *scope_check_ident_declared(
         file_location floc,
         const char *name)
{
    id_use *ret = symtab_lookup(name);
    if (ret == NULL) {
	bail_with_prog_error(floc,
				"identifier \"%s\" is not declared!",
				name);
    }
    assert(id_use_get_attrs(ret) != NULL);
    return ret;
}
