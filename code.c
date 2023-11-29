/* $Id: code.c,v 1.23 2023/11/28 03:48:42 leavens Exp $ */
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "utilities.h"
#include "code.h"
#include "regname.h"

// Return a fresh code struct, with next pointer NULL
// containing the given instruction instr.
// If there is not enough space, bail with an error,
// so this will never return NULL.
static code *code_create(bin_instr_t instr)
{
    code *ret = (code *)malloc(sizeof(code));
    if (ret == NULL) {
	bail_with_error("Not enough space to allocate a code struct!");
    }
    ret->next = NULL;
    ret->instr = instr;
    return ret;
}

// return a fresh register-type instruction
static code *create_reg_instr(reg_num_type rs, reg_num_type rt, reg_num_type rd,
			     shift_type h, func_type func)
{
    reg_instr_t ri;
    ri.op = REG_O;
    ri.rs = rs;
    ri.rt = rt;
    ri.rd = rd;
    ri.shift = h;
    ri.func = func;
    bin_instr_t bi;
    bi.reg = ri;
    return code_create(bi);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register rd
code *code_add(reg_num_type rs, reg_num_type rt, reg_num_type rd)
{
    return create_reg_instr(rs, rt, rd, 0, ADD_F);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register rd
code *code_sub(reg_num_type rs, reg_num_type rt, reg_num_type rd)
{
    return create_reg_instr(rs, rt, rd, 0, SUB_F);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: registers lo, hi
code *code_mul(reg_num_type rs, reg_num_type rt)
{
    return create_reg_instr(rs, rt, 0, 0, MUL_F);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: registers lo, hi
code *code_div(reg_num_type rs, reg_num_type rt)
{
    return create_reg_instr(rs, rt, 0, 0, DIV_F);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register rd
code *code_mfhi(reg_num_type rd)
{
    return create_reg_instr(0, 0, rd, 0, MFHI_F);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register rd
code *code_mflo(reg_num_type rd)
{
    return create_reg_instr(0, 0, rd, 0, MFLO_F);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register rd
code *code_and(reg_num_type rs, reg_num_type rt, reg_num_type rd)
{
    return create_reg_instr(rs, rt, rd, 0, AND_F);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register rd
code *code_bor(reg_num_type rs, reg_num_type rt, reg_num_type rd)
{
    return create_reg_instr(rs, rt, rd, 0, BOR_F);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
code *code_nor(reg_num_type rs, reg_num_type rt, reg_num_type rd)
{
    return create_reg_instr(rs, rt, rd, 0, NOR_F);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register rd
code *code_xor(reg_num_type rs, reg_num_type rt, reg_num_type rd)
{
    return create_reg_instr(rs, rt, rd, 0, XOR_F);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register rd
code *code_sll(reg_num_type rt, reg_num_type rd, shift_type h)
{
    return create_reg_instr(0, rt, rd, h, SLL_F);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register rd
code *code_srl(reg_num_type rt, reg_num_type rd, shift_type h)
{
    return create_reg_instr(0, rt, rd, h, SRL_F);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: PC
code *code_jr(reg_num_type rs)
{
    return create_reg_instr(rs, 0, 0, 0, JR_F);
}

// each system call has its own function (below)

// return a fresh immediate-type instruction
static code *create_immed_instr(op_code op, reg_num_type rs,
				reg_num_type rt, immediate_type immed)
{
    immed_instr_t ii;
    ii.op = op;
    ii.rs = rs;
    ii.rt = rt;
    ii.immed = immed;  // this is extended by the VM, don't do that here!
    bin_instr_t bi;
    bi.immed = ii;
    return code_create(bi);
}


// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: rt
code *code_addi(reg_num_type rs, reg_num_type rt, immediate_type i)
{
    return create_immed_instr(ADDI_O, rs, rt, i);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: rt
code *code_andi(reg_num_type rs, reg_num_type rt, immediate_type i)
{
    return create_immed_instr(ANDI_O, rs, rt, i);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: rt
code *code_bori(reg_num_type rs, reg_num_type rt, immediate_type i)
{
    return create_immed_instr(BORI_O, rs, rt, i);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: rt
code *code_xori(reg_num_type rs, reg_num_type rt, immediate_type i)
{
    return create_immed_instr(XORI_O, rs, rt, i);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: PC
code *code_beq(reg_num_type rs, reg_num_type rt, immediate_type o)
{
    return create_immed_instr(BEQ_O, rs, rt, o);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: PC
code *code_bgez(reg_num_type rs, immediate_type o)
{
    return create_immed_instr(BGEZ_O, rs, 1, o);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: PC
code *code_bgtz(reg_num_type rs, immediate_type o)
{
    return create_immed_instr(BGTZ_O, rs, 0, o);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: PC
code *code_blez(reg_num_type rs, immediate_type o)
{
    return create_immed_instr(BLEZ_O, rs, 0, o);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: PC
code *code_bltz(reg_num_type rs, immediate_type o)
{
    return create_immed_instr(BLTZ_O, rs, 0, o);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: PC
code *code_bne(reg_num_type rs, reg_num_type rt, immediate_type o)
{
    return create_immed_instr(BNE_O, rs, rt, o);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register rt
code *code_lbu(reg_num_type rb, reg_num_type rt, immediate_type o)
{
    return create_immed_instr(LBU_O, rb, rt, o);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register rt
code *code_lw(reg_num_type rb, reg_num_type rt, immediate_type o)
{
    return create_immed_instr(LW_O, rb, rt, o);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: memory[GPR.ints[rb]+formOffset(o)]
code *code_sb(reg_num_type rb, reg_num_type rt, immediate_type o)
{
    return create_immed_instr(SB_O, rb, rt, o);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: memory[GPR.ints[rb]+formOffset(o)]
code *code_sw(reg_num_type rb, reg_num_type rt, immediate_type o)
{
    return create_immed_instr(SW_O, rb, rt, o);
}

// return a fresh jump-type instruction
static code *create_jump_instr(op_code op, address_type a)
{
    jump_instr_t ji;
    ji.op = op;
    ji.addr = a;
    bin_instr_t bi;
    bi.jump = ji;
    return code_create(bi);
}


// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: PC
code *code_jmp(address_type a)
{
    return create_jump_instr(JMP_O, a);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register $ra, PC
code *code_jal(address_type a)
{
    code *ret = create_jump_instr(JAL_O, a);
    return ret;
}


// return a fresh syscall-type instruction
static code *create_syscall_instr(syscall_type t)
{
    syscall_instr_t si;
    si.op = REG_O;
    si.code = t;
    si.func = SYSCALL_F;
    bin_instr_t bi;
    bi.syscall = si;
    return code_create(bi);
}


// Create and return a fresh instruction
// with the given mnemonic and parameters
code *code_exit()
{
    return create_syscall_instr(exit_sc);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register $v0
code *code_pstr()
{
    return create_syscall_instr(print_str_sc);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register $v0
code *code_pint()
{
    return create_syscall_instr(print_int_sc);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register $v0
code *code_pch()
{
    return create_syscall_instr(print_char_sc);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
// Modifies when executed: register $v0
code *code_rch()
{
    return create_syscall_instr(read_char_sc);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
code *code_stra()
{
    return create_syscall_instr(start_tracing_sc);
}

// Create and return a fresh instruction
// with the given mnemonic and parameters
code *code_notr()
{
    return create_syscall_instr(stop_tracing_sc);
}


// ==== Code Sequence manipulation functions below ====

// Return an empty code_seq
code_seq code_seq_empty()
{
    return NULL;
}

// Return a code_seq containing just the given code
code_seq code_seq_singleton(code *c)
{
    return c;
}


// Is seq empty?
bool code_seq_is_empty(code_seq seq)
{
    return seq == NULL;
}

// Requires: !code_seq_is_empty(seq)
// Return the first element of the given code sequence, seq
code *code_seq_first(code_seq seq)
{
    return seq;
}

// Requires: !code_seq_is_empty(seq)
// Return the rest of the given sequence, seq
code_seq code_seq_rest(code_seq seq)
{
    return seq->next;
}

// Return the size (number of instructions/words) in seq
unsigned int code_seq_size(code_seq seq)
{
    unsigned int ret = 0;
    while (!code_seq_is_empty(seq)) {
	ret++;
	seq = code_seq_rest(seq);
    }
    return ret;
}

// Requires: !code_seq_is_empty(seq)
// Return the last element in the given sequence
code *code_seq_last_elem(code_seq seq)
{
    code *ret = seq;
    while (!code_seq_is_empty(seq)) {
	ret = seq;
	seq = code_seq_rest(seq);
    }
    return ret;
}

// Requires: c != NULL
// Add the given code *c to the end of the seq
// and return the seq, which has been modified if it was not empty
// Caution: be sure to assign the result,
// as any modifications may not have any effect, since seq is passed by value
code_seq code_seq_add_to_end(code_seq seq, code *c)
{
    if (code_seq_is_empty(seq)) {
	return code_seq_singleton(c);
    }
    // assert(!code_seq_is_empty(seq));
    code *last = code_seq_last_elem(seq);	
    last->next = c;
    c->next = NULL;
    return seq;
}

// Concatenate the given code sequences in order first s1 then s2
// This may modify the sequence s1 if both s1 and s2 are not empty
code_seq code_seq_concat(code_seq s1, code_seq s2)
{
    if (code_seq_is_empty(s1)) {
	return s2;
    } else if (code_seq_is_empty(s2)) {
	return s1;
    } else {
	code *last = code_seq_last_elem(s1);
	last->next = s2;
	return s1;
    }
}

// Return a code sequence that load the static link that is STATIC_LINK_OFFSET
// from (i.e., lower than) the address contained in register rb,
// and place it in register rt.
// Modifies only register rt.
code_seq code_load_static_link(reg_num_type rb, reg_num_type rt)
{
    code_seq ret = code_seq_singleton(code_lw(rb, rt, STATIC_LINK_OFFSET));
    return ret;
}

// Requires: reg != FP
// Modifies only: register reg
// Return a code sequence that will put the address that corresponds to the
// frame pointer for the given number of scopes outward in register reg
code_seq code_compute_fp(reg_num_type reg, unsigned int levelsOut)
{
    assert(reg != FP);
    code_seq ret;
    ret = code_seq_singleton(code_add(0, FP, reg));
    while (levelsOut > 0) {
	ret = code_seq_concat(ret,
			      code_load_static_link(reg, reg));
	levelsOut--;
    }
    return ret;
}

// Allocate space on the runtime stack and copy the contents of register reg
// into the newly allocated space on top of the stack.
// Modifies SP and memory.words[SP-BYTES_PER_WORD] when executed
code_seq code_push_reg_on_stack(reg_num_type reg)
{
    code_seq ret = code_allocate_stack_space(BYTES_PER_WORD);
    ret = code_seq_add_to_end(ret, code_sw(SP, reg, 0));
    return ret;
}


// Copy the value from the top of the runtime stack into register reg
// and then deallocate the top of the stack.
// Modifies SP and GPR.words[reg] when executed
code_seq code_pop_stack_into_reg(reg_num_type reg)
{
    code_seq ret = code_seq_singleton(code_lw(SP, reg, 0));
    ret = code_seq_concat(ret, code_deallocate_stack_space(BYTES_PER_WORD));
    return ret;
}

// Requires: bytes >= 0 && (bytes % BYTES_PER_WORD) == 0
// Allocate the given number of bytes on the runtime stack
// Modifies when executed: SP register
// At the end of exeuction, SP holds the (byte) address
// of the last allocated word. (So, if the call allocates only one word,
// then SP addresses that word.)
code_seq code_allocate_stack_space(immediate_type bytes)
{
    assert(bytes >= 0);
    assert(bytes % BYTES_PER_WORD == 0);
    code_seq ret = code_seq_singleton(code_addi(SP, SP, -bytes));
    return ret;
}

// Requires: bytes >= 0 && (bytes % BYTES_PER_WORD) == 0
// Deallocate the given number of bytes on the runtime stack
// Modifies when executed: SP register
code_seq code_deallocate_stack_space(immediate_type bytes)
{
    assert(bytes >= 0);
    assert(bytes % BYTES_PER_WORD == 0);
    code_seq ret = code_seq_singleton(code_addi(SP, SP, bytes));
    return ret;
}

#define MINIMAL_STACK_ALLOC_IN_WORDS 12
#define MINIMAL_STACK_ALLOC_BYTES (BYTES_PER_WORD*MINIMAL_STACK_ALLOC_IN_WORDS)

// Set up the runtime stack for a procedure,
// where the static link is found in register $a0.
// Modifies when executed, the SP register, the FP register,
// and memory from SP to SP - MINIMAL_STACK_ALLOC_BYTES
// (inclusive)
code_seq code_save_registers_for_AR()
{
    // assume that SP is pointing to the lowest local storage already allocated
    code_seq ret;
    // push stack_pointer at word index 0 - 1 from current SP
    ret = code_seq_singleton(code_sw(SP, SP, -1));
    // push the frame pointer (dynamic link) at word index -2
    ret = code_seq_add_to_end(ret, code_sw(SP, FP, -2));
    // save SP into FP register so FP points to the base of the AR
    ret = code_seq_add_to_end(ret, code_add(0, SP, FP));
    // allocate the space on the stack, by subtracting from SP
    ret = code_seq_add_to_end(ret, code_addi(SP, SP, - MINIMAL_STACK_ALLOC_BYTES));
    // push the static link at word index -3
    ret = code_seq_add_to_end(ret, code_sw(FP, A0, -3));
    // push the return address at word index 3
    ret = code_seq_add_to_end(ret, code_sw(FP, RA, -4));
    // save the registers $s0 to $s7 (inclusive)
    int idx = -5;
    for (int rn = S0; rn <= S7; rn++) {
	ret = code_seq_add_to_end(ret, code_sw(FP, rn, idx--));
    }
    return ret;
}

// Finish using the runtime stack, just before exiting a procedure.
// This restores the saved registers, but not $a0.
// Modifies when executed, the SP register, the FP register,
// and registers from $s0 to $s7 (inclusive),
// restoring their saved contents from memory
// (as saved by code_start_AR)
code_seq code_restore_registers_from_AR()
{
    code_seq ret;
    // restore the RA register
    ret = code_seq_singleton(code_lw(FP, RA, -4));
    // restore the registers $s0 to $s7 (inclusive)
    int idx = -5;
    for (int rn = S0; rn <= S7; rn++) {
	ret = code_seq_add_to_end(ret, code_lw(FP, rn, idx--));
    }
    // deallocate the space on the stack, by restoring the SP
    ret = code_seq_add_to_end(ret, code_lw(FP, SP, -1));
    // restore the old FP
    ret = code_seq_add_to_end(ret, code_lw(FP, FP, -2));
    // the old static link is not restored
    return ret;
}

// Requires: out is open for writing
// print the instructions in the code_seq to out
// in assembly language format
void code_seq_debug_print(FILE *out, code_seq seq)
{
    while(!code_seq_is_empty(seq)) {
	fprintf(out, "%s\n",
		instruction_assembly_form(code_seq_first(seq)->instr));
	seq = code_seq_rest(seq);
    }
}
