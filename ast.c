/* $Id: ast.c,v 1.13 2023/11/27 20:13:36 leavens Exp $ */
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "utilities.h"
#include "ast.h"
#include "id_use.h"
#include "lexer.h"

// apparently strdup is not declared in <string.h>
extern char *strdup(const char *s);

// Return the file location from an AST
file_location *ast_file_loc(AST t) {
    return t.generic.file_loc;
}

// Return an AST for a block which contains the given ASTs.
block_t ast_block(const_decls_t const_decls, var_decls_t var_decls,
		  proc_decls_t proc_decls, stmt_t stmt)
{
    block_t ret;
    ret.file_loc = const_decls.file_loc;
    ret.const_decls = const_decls;
    ret.var_decls = var_decls;
    ret.proc_decls = proc_decls;
    ret.stmt = stmt;
    return ret;
}

// Return an AST for an empty const decls
extern const_decls_t ast_const_decls_empty(empty_t empty)
{
    const_decls_t ret;
    ret.file_loc = empty.file_loc;
    ret.const_decls = NULL;
    return ret;
}


// Return an AST for the const decls
const_decls_t ast_const_decls(const_decls_t const_decls,
			      const_decl_t const_decl)
{
    const_decls_t ret = const_decls;
    // make a copy of const_decl on the heap
    const_decl_t *p = (const_decl_t *) malloc(sizeof(const_decl_t));
    if (p == NULL) {
	bail_with_error("Cannot allocate space for %s!", "const_decl_t");
    }
    *p = const_decl;
    p->next = NULL;
    const_decl_t *last = ast_last_list_elem(ret.const_decls);
    if (last == NULL) {
	ret.const_decls = p;
    } else {
	last->next = p;
    }
    return ret;
}



// Return an AST for a const_decl
const_decl_t ast_const_decl(const_defs_t const_defs)
{
    const_decl_t ret;
    ret.file_loc = const_defs.file_loc;
    ret.const_defs = const_defs;
    ret.next = NULL;
    return ret;
}

// Return an AST for const_defs
extern const_defs_t ast_const_defs_singleton(const_def_t const_def)
{
    const_defs_t ret;
    ret.file_loc = const_def.file_loc;
    const_def_t *p = (const_def_t *) malloc(sizeof(const_def_t));
    if (p == NULL) {							
	bail_with_error("Unable to allocate space for a %s!", "const_def_t"); 
    }		    
    *p = const_def;		
    p->next = NULL;    
    ret.const_defs = p;							
    return ret;
}

// Return an AST for const_defs
extern const_defs_t ast_const_defs(const_defs_t const_defs,
				   const_def_t const_def)
{
    const_defs_t ret = const_defs;
    // make a copy of const_def on the heap
    const_def_t *p = (const_def_t *) malloc(sizeof(const_def_t));
    if (p == NULL) {
	bail_with_error("Cannot allocate space for %s!", "const_def_t");
    }
    *p = const_def;
    p->next = NULL;
    const_def_t *last = ast_last_list_elem(ret.const_defs);
    if (last == NULL) {
	ret.const_defs = p;
    } else {
	last->next = p;
    }
    return ret;
}

// Return an AST for a const-def
const_def_t ast_const_def(ident_t ident, number_t number)
{
    const_def_t ret;
    ret.file_loc = file_location_copy(ident.file_loc);
    assert((ret.file_loc)->filename != NULL);
    ret.next = NULL;
    ret.ident = ident;
    ret.number = number;
    return ret;
}


// Return an AST for varDecls that are empty
var_decls_t ast_var_decls_empty(empty_t empty)
{
    var_decls_t ret;
    ret.file_loc = empty.file_loc;
    ret.var_decls = NULL;
    return ret;
}

// Return an AST varDecls that have a var_decl
var_decls_t ast_var_decls(var_decls_t var_decls, var_decl_t var_decl)
{
    var_decls_t ret = var_decls;
    // make a copy of var_decl on the heap
    var_decl_t *p = (var_decl_t *) malloc(sizeof(var_decl_t));
    if (p == NULL) {
	bail_with_error("Cannot allocate space for %s!", "var_decl_t");
    }
    *p = var_decl;
    p->next = NULL;
    var_decl_t *last = ast_last_list_elem(ret.var_decls);
    if (last == NULL) {
	ret.var_decls = p;
    } else {
	last->next = p;
    }
    return ret;
}

// Return an AST for a var_decl
var_decl_t ast_var_decl(idents_t idents)
{
    var_decl_t ret;
    ret.file_loc = idents.file_loc;
    ret.next = NULL;
    ret.idents = idents;
    return ret;
}

// Return an AST made for one ident
extern idents_t ast_idents_singleton(ident_t ident)
{
    idents_t ret;
    ret.file_loc = ident.file_loc;
    // make a copy of ident on the heap
    ident_t *p = (ident_t *) malloc(sizeof(ident_t));	
    if (p == NULL) {							
	bail_with_error("Unable to allocate space for a %s!", "ident_t"); 
    }		    
    *p = ident;		
    p->next = NULL;    
    ret.idents = p;						
    return ret;
}

// Return an AST made for idents
extern idents_t ast_idents(idents_t idents, ident_t ident)
{
    idents_t ret = idents;
    // make a copy of ident on the heap
    ident_t *p = (ident_t *) malloc(sizeof(ident_t));
    if (p == NULL) {
	bail_with_error("Cannot allocate space for %s!", "ident_t");
    }
    *p = ident;
    p->next = NULL;
    ident_t *last = ast_last_list_elem(ret.idents);
    if (last == NULL) {
	ret.idents = p;
    } else {
	last->next = p;
    }
    return ret;
}

// Return an AST for proc_decls
proc_decls_t ast_proc_decls_empty(empty_t empty)
{
    proc_decls_t ret;
    ret.file_loc = empty.file_loc;
    ret.proc_decls = NULL;
    return ret;
}

// Return an AST for proc_decls
proc_decls_t ast_proc_decls(proc_decls_t proc_decls,
			    proc_decl_t proc_decl)
{
    proc_decls_t ret = proc_decls;
    // make a copy of proc_decl on the heap
    proc_decl_t *p = (proc_decl_t *) malloc(sizeof(proc_decl_t));	
    if (p == NULL) {							
	bail_with_error("Unable to allocate space for a %s!", "proc_decl_t"); 
    }		    
    *p = proc_decl;		
    p->next = NULL;    
    proc_decl_t *last = ast_last_list_elem(ret.proc_decls);
    if (last == NULL) {
	ret.proc_decls = p;
    } else {
	last->next = p;
    }
    return ret;
}

// Return an AST for a proc_decl
proc_decl_t ast_proc_decl(ident_t ident, block_t block)
{
    proc_decl_t ret;
    ret.file_loc = file_location_copy(ident.file_loc);
    ret.next = NULL;
    ret.name = ident.name;
    block_t *p = (block_t *) malloc(sizeof(block_t));
    if (p == NULL) {
	bail_with_error("Unable to allocate space for a %s!", "block_t");
    }
    *p = block;
    ret.block = p;
    return ret;
}

// Return an AST for a skip statement
skip_stmt_t ast_skip_stmt(file_location *file_loc) {
    skip_stmt_t ret;
    ret.file_loc = file_loc;
    return ret;
}

// Return an AST for a write statement
write_stmt_t ast_write_stmt(expr_t expr) {
    write_stmt_t ret;
    ret.file_loc = expr.file_loc;
    ret.expr = expr;
    return ret;
}

// Return an AST for a read statement
read_stmt_t ast_read_stmt(ident_t ident) {
    read_stmt_t ret;
    ret.file_loc = file_location_copy(ident.file_loc);
    ret.name = ident.name;
    return ret;
}

// Return an immediate data holding an address
while_stmt_t ast_while_stmt(condition_t condition, stmt_t body) {
    while_stmt_t ret;
    ret.file_loc = condition.file_loc;
    ret.condition = condition;
    stmt_t *p = (stmt_t *) malloc(sizeof(stmt_t));
    if (p == NULL) {
	bail_with_error("Unable to allocate space for a %s!", "stmt_t"); 
    }
    *p = body;		
    ret.body = p;					
    return ret;
}

// Return an AST for an instruction
// with the given information
if_stmt_t ast_if_stmt(condition_t condition, stmt_t then_stmt,
		      stmt_t else_stmt)
{
    if_stmt_t ret;
    ret.file_loc = condition.file_loc;
    ret.condition = condition;
    // copy then_stmt to the heap
    stmt_t *p = (stmt_t *) malloc(sizeof(stmt_t));			
    if (p == NULL) {							
	bail_with_error("Unable to allocate space for a %s!", "stmt_t"); 
    }									
    *p = then_stmt;	
    ret.then_stmt = p;						
    // copy else_stmt to the heap
    p = (stmt_t *) malloc(sizeof(stmt_t));	
    if (p == NULL) {							
	bail_with_error("Unable to allocate space for a %s!", "stmt_t"); 
    }		    
    *p = else_stmt;		
    ret.else_stmt = p;						
    return ret;
}

// Return an AST for a begin statement
// containing the given list of statements
begin_stmt_t ast_begin_stmt(stmts_t stmts)
{
    begin_stmt_t ret;
    ret.file_loc = stmts.file_loc;
    ret.stmts = stmts;
    return ret;
}

// Return an AST for a call statment
 call_stmt_t ast_call_stmt(ident_t ident)
{
    call_stmt_t ret;
    ret.file_loc = file_location_copy(ident.file_loc);
    ret.name = ident.name;
    return ret;
}

// Return an AST for an assignment statement
assign_stmt_t ast_assign_stmt(ident_t ident, expr_t expr)
{
    assign_stmt_t ret;
    ret.file_loc = file_location_copy(ident.file_loc);
    ret.name = ident.name;
    assert(ret.name != NULL);
    expr_t *p = (expr_t *) malloc(sizeof(expr_t));
    if (p == NULL) {
	bail_with_error("Unable to allocate space for a %s!", "expr_t");
    }
    *p = expr;
    ret.expr = p;
    assert(ret.expr != NULL);
    return ret;
}

// Return an AST for the list of statements 
stmts_t ast_stmts_singleton(stmt_t stmt) {
    // debug_print("Entering ast_stmts_singleton\n");
    stmts_t ret;
    ret.file_loc = stmt.file_loc;
    stmt.next = NULL;
    // copy stmt to the heap
    stmt_t *p = (stmt_t *) malloc(sizeof(stmt_t));	
    if (p == NULL) {							
	bail_with_error("Unable to allocate space for a %s!", "stmt_t"); 
    }		    
    *p = stmt;
    p->next = NULL;
    // there will be no statments in front of stmt in the list
    ret.stmts = p;					
    return ret;
}

// Return an AST for the list of statements 
stmts_t ast_stmts(stmts_t stmts, stmt_t stmt) {
    // debug_print("Entering ast_stmts...\n");
    stmts_t ret = stmts;
    // copy stmt to the heap
    stmt_t *s = (stmt_t *) malloc(sizeof(stmt_t));
    if (s == NULL) {
	bail_with_error("Cannot allocate space for %s!", "stmt_t");
    }
    *s = stmt;
    s->next = NULL;
    stmt_t *last = ast_last_list_elem(ret.stmts);
    assert(last != NULL); // because there are no empty lists of stmts
    last->next = s;
    return ret;
}

// Return an AST for the given statment
stmt_t ast_stmt_assign(assign_stmt_t s)
{
    stmt_t ret;
    ret.file_loc = s.file_loc;
    ret.next = NULL;
    ret.stmt_kind = assign_stmt;
    ret.data.assign_stmt = s;
    return ret;
}

// Return an AST for the given statment
stmt_t ast_stmt_call(call_stmt_t s)
{
    stmt_t ret;
    ret.file_loc = s.file_loc;
    ret.next = NULL;
    ret.stmt_kind = call_stmt;
    ret.data.call_stmt = s;
    return ret;
}

// Return an AST for the given statment
stmt_t ast_stmt_begin(begin_stmt_t s)
{
    stmt_t ret;
    ret.file_loc = s.file_loc;
    ret.next = NULL;
    ret.stmt_kind = begin_stmt;
    ret.data.begin_stmt = s;
    return ret;
}

// Return an AST for the given statment
stmt_t ast_stmt_if(if_stmt_t s)
{
    stmt_t ret;
    ret.file_loc = s.file_loc;
    ret.next = NULL;
    ret.stmt_kind = if_stmt;
    ret.data.if_stmt = s;
    return ret;
}

// Return an AST for the given statment
stmt_t ast_stmt_while(while_stmt_t s)
{
    stmt_t ret;
    ret.file_loc = s.file_loc;
    ret.next = NULL;
    ret.stmt_kind = while_stmt;
    ret.data.while_stmt = s;
    return ret;
}

// Return an AST for the given statment
stmt_t ast_stmt_read(read_stmt_t s)
{
    stmt_t ret;
    ret.file_loc = s.file_loc;
    ret.next = NULL;
    ret.stmt_kind = read_stmt;
    ret.data.read_stmt = s;
    return ret;
}

// Return an AST for the given statment
stmt_t ast_stmt_write(write_stmt_t s)
{
    // debug_print("Entering ast_stmt_write...\n");
    stmt_t ret;
    ret.file_loc = s.file_loc;
    ret.next = NULL;
    ret.stmt_kind = write_stmt;
    ret.data.write_stmt = s;
    return ret;
}

// Return an AST for the given statment
stmt_t ast_stmt_skip(skip_stmt_t s)
{
    stmt_t ret;
    ret.file_loc = s.file_loc;
    ret.stmt_kind = skip_stmt;
    ret.next = NULL;
    return ret;
}

// Return an AST for an odd condition
odd_condition_t ast_odd_condition(expr_t expr)
{
    odd_condition_t ret;
    ret.file_loc = expr.file_loc;
    ret.expr = expr;
    return ret;
}

// Return an AST for an initializer with the given value
rel_op_condition_t ast_rel_op_condition(expr_t expr1, token_t rel_op,
					expr_t expr2)
{
    rel_op_condition_t ret;
    ret.file_loc = expr1.file_loc;
    ret.expr1 = expr1;
    ret.rel_op = rel_op;
    ret.expr2 = expr2;
    return ret;
}

// Return an AST for an odd condition
condition_t ast_condition_odd(odd_condition_t odd_cond)
{
    condition_t ret;
    ret.file_loc = odd_cond.file_loc;
    ret.cond_kind = ck_odd;
    ret.data.odd_cond = odd_cond;
    return ret;
}

// Return an AST for a relational condition
condition_t ast_condition_rel(rel_op_condition_t rel_op_cond)
{
    condition_t ret;
    ret.file_loc = rel_op_cond.file_loc;
    ret.cond_kind = ck_rel;
    ret.data.rel_op_cond = rel_op_cond;
    return ret;
}

// Return an AST for an odd condition
binary_op_expr_t ast_binary_op_expr(expr_t expr1, token_t arith_op,
				    expr_t expr2)
{
    binary_op_expr_t ret;
    ret.file_loc = expr1.file_loc;

    expr_t *p = (expr_t *) malloc(sizeof(expr_t));
    if (p == NULL) {
	bail_with_error("Unable to allocate space for a %s!", "expr_t");
    }
    *p = expr1;
    ret.expr1 = p;

    ret.arith_op = arith_op;
    
    p = (expr_t *) malloc(sizeof(expr_t));
    if (p == NULL) {
	bail_with_error("Unable to allocate space for a %s!", "expr_t");
    }
    *p = expr2;
    ret.expr2 = p;

    return ret;
}

// Return an expression AST for a binary operation expresion
expr_t ast_expr_binary_op(binary_op_expr_t e)
{
    expr_t ret;
    ret.file_loc = e.file_loc;
    ret.expr_kind = expr_bin;
    ret.data.binary = e;
    return ret;
}

// Return an expression AST for an signed number
expr_t ast_expr_negated_number(token_t sign, number_t number)
{
    char buf[BUFSIZ];
    expr_t ret;
    ret.file_loc = file_location_copy(sign.file_loc);
    ret.expr_kind = expr_number;
    ret.data.number = number;
    // add the minus sign to the text, so it gets in the literal table properly
    strcat(buf, "-");
    strncat(buf, number.text, BUFSIZ-1);
    ret.data.number.text = strdup(buf);
    // negate the value
    ret.data.number.value = - ret.data.number.value;
    return ret;
}

// Return an expression AST for an signed number
expr_t ast_expr_pos_number(token_t sign, number_t number)
{
    expr_t ret;
    ret.file_loc = file_location_copy(sign.file_loc);
    ret.expr_kind = expr_number;
    ret.data.number = number;
    return ret;
}

// Return an AST for the given token
token_t ast_token(file_location *file_loc, const char *text, int code)
{
    token_t ret;
    ret.file_loc = file_loc;
    ret.text = text;
    ret.code = code;
    return ret;
}

// Return an AST for a (signed) number with the given value
number_t ast_number(token_t sgn, word_type value)
{
    number_t ret;
    ret.file_loc = file_location_copy(sgn.file_loc);
    ret.value = value;
    return ret;
}

// Return an AST for an identifier
ident_t ast_ident(file_location *file_loc, const char *name)
{
    ident_t ret;
    ret.file_loc = file_loc;
    ret.name = name;
    return ret;
}

// Return an AST for an expression that's a binary expression
expr_t ast_expr_binary_op_expr(binary_op_expr_t e)
{
    expr_t ret;
    ret.file_loc = e.file_loc;
    ret.expr_kind = expr_bin;
    ret.data.binary = e;
    return ret;
}

// Return an AST for an expression that's an identifier
expr_t ast_expr_ident(ident_t e)
{
    expr_t ret;
    ret.file_loc = e.file_loc;
    ret.expr_kind = expr_ident;
    ret.data.ident = e;
    return ret;
}

// Return an AST for an expression that's a number
expr_t ast_expr_number(number_t e)
{
    expr_t ret;
    ret.file_loc = e.file_loc;
    ret.expr_kind = expr_number;
    ret.data.number = e;
    return ret;
}

// Return an AST for empty found in the given file location
empty_t ast_empty(file_location *file_loc)
{
    empty_t ret;
    ret.file_loc = file_loc;
    return ret;
}

// Requires: lst is a pointer to a non-circular 
//           linked list with next pointers
//           as in generic_t
// Return a pointer to the last element in lst.
// This only returns NULL if lst == NULL.
void *ast_last_list_elem(void *lst)
{
    // debug_print("Entering ast_last_list_elem\n");
    if (lst == NULL) {
	return lst;
    }
    // assert(lst != NULL);
    void *prev = NULL;
    while (lst != NULL) {
	prev = lst;
	lst = ((generic_t *)lst)->next;
    }
    // here lst == NULL;
    return prev;
}

// Requires: lst is a pointer to a non-circular 
//           linked list with next pointers
//           as in generic_t
// Return the number of elements in the linked list lst
int ast_list_length(void *lst)
{
    int ret = 0;
    generic_t *p = (generic_t *) lst;
    while (p != NULL) {
	p = p->next;
	ret++;
    }
    return ret;
}

// Requires: lst is a pointer to a non-circular 
//           linked list with next pointers
//           as in generic_t
// Is lst empty?
bool ast_list_is_empty(void *lst)
{
    return lst == NULL;
}
