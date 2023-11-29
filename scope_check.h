/* $Id: scope_check.h,v 1.6 2023/11/28 11:50:16 leavens Exp leavens $ */
#ifndef _SCOPE_CHECK_H
#define _SCOPE_CHECK_H
#include "ast.h"

// Build the symbol table for the given program AST
// and check the given program AST for duplicate declarations
// or uses of identifiers that were not declared
extern block_t scope_check_program(block_t prog);

// build the symbol table and check the declarations in blk
extern block_t scope_check_block(block_t blk);

// build the symbol table and check the declarations in cds
extern void scope_check_constDecls(const_decls_t cds);

// build the symbol table and check the const declaration cd
// and add its const-defs to the current scope's symbol table
// or produce an error if these names have already been declared
extern void scope_check_constDecl(const_decl_t cd);

// build the symbol table and check the const definitions cdfs
// and add the declared names to the current scope's symbol table
// or produce an error if these names have already been declared
extern void scope_check_constDefs(const_defs_t cdfs);

// build the symbol table and check the const definition cdf
// and add the declared name to the current scope's symbol table
// or produce an error if its name has already been declared
extern void scope_check_constDef(const_def_t cdf);

// build the symbol table and check the declarations in vds
extern void scope_check_varDecls(var_decls_t vds);

// check the var declaration vd
// and add the declared names to the current scope's symbol table
// or produce an error if the names have already been declared
extern void scope_check_varDecl(var_decl_t vd);

// check the identifiers from a var declaration, as given in ids
// and add the declared names to the current scope's symbol table
// or produce an error if the names have already been declared
extern void scope_check_varIdents(idents_t ids);

// build the symbol table and check the declarations in pds
// Return the modified AST with id_use pointers
extern proc_decls_t scope_check_procDecls(proc_decls_t pds);

// check the procedure declaration pd
// and add it to the current scope's symbol table
// or produce an error if its name has already been declared
// Return the modified AST with id_use pointers
extern proc_decl_t scope_check_procDecl(proc_decl_t pd);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern stmt_t scope_check_stmt(stmt_t stmt);

// Check the statement to make sure that
// all idenfifiers referenced in it have been declared
// and that the name assigned to is a variable
// (and if one of these does not hold, then produce an error).
// Return the modified AST with id_use pointers
extern assign_stmt_t scope_check_assignStmt(assign_stmt_t stmt);

// check the statement to make sure that
// the procedure being called has been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern call_stmt_t scope_check_callStmt(call_stmt_t stmt);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern begin_stmt_t scope_check_beginStmt(begin_stmt_t stmt);

// check all the statement in stmts to make sure that
// all idenfifiers referenced in them have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern stmts_t scope_check_stmts(stmts_t stmts);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern if_stmt_t scope_check_ifStmt(if_stmt_t stmt);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern while_stmt_t scope_check_whileStmt(while_stmt_t stmt);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern read_stmt_t scope_check_readStmt(read_stmt_t stmt);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern write_stmt_t scope_check_writeStmt(write_stmt_t stmt);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern skip_stmt_t scope_check_skipStmt(skip_stmt_t stmt);

// check the condition to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern condition_t scope_check_condition(condition_t cond);

// check the condition to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern odd_condition_t scope_check_odd_condition(odd_condition_t cond);

// check the condition to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern rel_op_condition_t scope_check_rel_op_condition(rel_op_condition_t cond);

// check the expresion to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern expr_t scope_check_expr(expr_t exp);

// check that the given name has been declared,
// if not, then produce an error using the file_location (floc) given.
// Return the modified AST with id_use pointers
extern ident_t scope_check_ident(file_location floc, const char *name);

// check the expression (exp) to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern binary_op_expr_t scope_check_binary_op_expr(binary_op_expr_t exp);


// check the identifier (id) to make sure that
// all it has been declared (if not, then produce an error)
// Return the modified AST with id_use pointers
extern ident_t scope_check_ident_expr(ident_t id);

// check that the given name has been declared,
// if so, then return an id_use (containing the attributes) for that name,
// otherwise, produce an error using the file_location (floc) given.
// Return the modified AST with id_use pointers
extern id_use *scope_check_ident_declared(file_location floc, const char *name);

#endif
