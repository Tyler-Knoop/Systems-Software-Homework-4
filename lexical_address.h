/* $Id: lexical_address.h,v 1.3 2023/11/13 05:13:56 leavens Exp $ */
#ifndef _LEXICAL_ADDRESS_H
#define _LEXICAL_ADDRESS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned int levelsOutward;
    unsigned int offsetInAR; // in bytes
} lexical_address;

// Allocate and return a (fresh) lexical address
// where the levelsOutwards field is levelsOut
// and the offsetInAR field is the byte offset in the AR
extern lexical_address *lexical_address_create(unsigned int levelsOut,
					       unsigned int offset);

// Requires: out is not NULL and is open for writing
// Requires: la is not NULL
extern void lexical_address_print(FILE *out, lexical_address *la);

#endif
