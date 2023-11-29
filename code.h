/* $Id: code.h,v 1.19 2023/11/28 11:50:16 leavens Exp leavens $ */
#ifndef _CODE_H
#define _CODE_H
#include <stdbool.h>
#include "machine_types.h"
#include "instruction.h"

typedef struct code_s code;
// code sequences
typedef code *code_seq;

// SRM assembly language instructions (that can be in linked lists)
typedef struct code_s {
    code *next;
    bin_instr_t instr;
} code;

// Code creation functions below

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_add(reg_num_type rs, reg_num_type rt, reg_num_type rd);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_sub(reg_num_type rs, reg_num_type rt, reg_num_type rd);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_mul(reg_num_type rs, reg_num_type rt);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_div(reg_num_type rs, reg_num_type rt);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_mfhi(reg_num_type rd);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_mflo(reg_num_type rd);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_and(reg_num_type rs, reg_num_type rt, reg_num_type rd);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_bor(reg_num_type rs, reg_num_type rt, reg_num_type rd);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_nor(reg_num_type rs, reg_num_type rt, reg_num_type rd);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_xor(reg_num_type rs, reg_num_type rt, reg_num_type rd);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_sll(reg_num_type rt, reg_num_type rd, shift_type h);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_srl(reg_num_type rt, reg_num_type rd, shift_type h);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_jr(reg_num_type rs);

// each system call has its own function (below)

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_addi(reg_num_type rs, reg_num_type rt, immediate_type i);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_andi(reg_num_type rs, reg_num_type rt, immediate_type i);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_bori(reg_num_type rs, reg_num_type rt, immediate_type i);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_xori(reg_num_type rs, reg_num_type rt, immediate_type i);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_beq(reg_num_type rs, reg_num_type rt, immediate_type o);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_bgez(reg_num_type rs, immediate_type o);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_bgtz(reg_num_type rs, immediate_type o);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_blez(reg_num_type rs, immediate_type o);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_bltz(reg_num_type rs, immediate_type o);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_bne(reg_num_type rs, reg_num_type rt, immediate_type o);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_lbu(reg_num_type rb, reg_num_type rt, immediate_type o);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_lw(reg_num_type rb, reg_num_type rt, immediate_type o);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_sb(reg_num_type rb, reg_num_type rt, immediate_type o);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_sw(reg_num_type rb, reg_num_type rt, immediate_type o);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_jmp(address_type a);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_jal(address_type a);

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_exit();

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_pstr();

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_pint();

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_pch();

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_rch();

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_stra();

// Create and return a fresh instruction
// with the given mnemonic and parameters
extern code *code_notr();


// ==== Code Sequence manipulation functions below ====

// Return an empty code_seq
extern code_seq code_seq_empty();

// Return a code_seq containing just the given code
extern code_seq code_seq_singleton(code *c);

// Is seq empty?
extern bool code_seq_is_empty(code_seq seq);

// Requires: !code_seq_is_empty(seq)
// Return the first element of the given code sequence, seq
extern code *code_seq_first(code_seq seq);

// Requires: !code_seq_is_empty(seq)
// Return the rest of the given sequence, seq
extern code_seq code_seq_rest(code_seq seq);

// Return the size (number of instructions/words) in seq
extern unsigned int code_seq_size(code_seq seq);

// Requires: !code_seq_is_empty(seq)
// Return the last element in the given sequence
extern code *code_seq_last(code_seq seq);

// Requires: c != NULL
// Add the given code *c to the end of the seq
// and return the seq, which has been modified if it was not empty
// Caution: be sure to assign the result,
// as any modifications may not have any effect, since seq is passed by value
extern code_seq code_seq_add_to_end(code_seq seq, code *c);

// Concatenate the given code sequences in order first s1 then s2
// This may modify the sequence s1 if both s1 and s2 are not empty
extern code_seq code_seq_concat(code_seq s1, code_seq s2);

// === Some convenience functions that may help with code generation follow===

// Offset from the FP where the static link is found in an AR
#define STATIC_LINK_OFFSET (-3)

// Return a code sequence that load the static link that is STATIC_LINK_OFFSET
// from (i.e., lower than) the address contained in register rb,
// and place it in register rt.
// Modifies only register rt.
extern code_seq code_load_static_link(reg_num_type rb, reg_num_type rt);

// Requires: reg != FP
// Modifies only: register reg
// Return a code sequence that will put the address that corresponds to the
// frame pointer for the given number of scopes outward in register reg
extern code_seq code_compute_fp(reg_num_type reg, unsigned int levelsOut);

// Allocate space on the runtime stack and copy the contents of register reg
// into the newly allocated space on top of the stack.
// Modifies SP and memory.words[SP-BYTES_PER_WORD] when executed
extern code_seq code_push_reg_on_stack(reg_num_type reg);

// Copy the value from the top of the runtime stack into register reg
// and then deallocate the top of the stack.
// Modifies SP and GPR.words[reg] when executed
extern code_seq code_pop_stack_into_reg(reg_num_type reg);

// Requires: bytes > 0 && (bytes % BYTES_PER_WORD) == 0
// Allocate the given number of bytes on the runtime stack
// Modifies when executed: SP register
// At the end of exeuction, SP holds the (byte) address
// of the last allocated word. (So, if the call allocates only one word,
// then SP addresses that word.)
extern code_seq code_allocate_stack_space(immediate_type bytes);

// Requires: bytes > 0 && (bytes % BYTES_PER_WORD) == 0
// Deallocate the given number of bytes on the runtime stack
// Modifies when executed: SP register
extern code_seq code_deallocate_stack_space(immediate_type bytes);

// Set up the runtime stack for a procedure,
// where the static link is found in register $a0.
// Modifies when executed, the SP register, the FP register,
// and memory from SP to SP - MINIMAL_STACK_ALLOC_BYTES
// (inclusive)
extern code_seq code_save_registers_for_AR();

// Finish using the runtime stack, just before exiting a procedure.
// This restores the saved registers, but not $a0.
// Modifies when executed, the SP register, the FP register,
// and registers from $s0 to $s7 (inclusive),
// restoring their saved contents from memory
// (as saved by code_start_AR)
extern code_seq code_restore_registers_from_AR();

// Requires: out is open for writing
// print the instructions in the code_seq to out
// in assembly language format
extern void code_seq_debug_print(FILE *out, code_seq seq);

#endif
