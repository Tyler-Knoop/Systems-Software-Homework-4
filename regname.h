// $Id: regname.h,v 1.1 2023/11/13 05:14:01 leavens Exp $
#ifndef _REGNAME_H
#define _REGNAME_H
#define NUM_REGISTERS 32

// some important register numbers in the ISA
// no name for register 0
#define AT 1
#define V0 2
#define V1 3
#define A0 4
#define A1 5
#define A2 6
#define A3 7
#define T0 8
#define T1 9
#define T2 10
#define T3 11
#define T4 12
#define T5 13
#define T6 14
#define T7 15
#define S0 16
#define S1 17
#define S2 18
#define S3 19
#define S4 20
#define S5 21
#define S6 22
#define S7 23
#define T8 24
#define T9 25
// registers 26 and 27 are reserved for the OS
#define GP 28
#define SP 29
#define FP 30
#define RA 31


// requires 0 <= n && n < NUM_REGISTERS
// Return the standard symbolic name for n
extern const char *regname_get(int n);
#endif

