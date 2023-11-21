/* $Id: id_use.h,v 1.6 2023/11/13 14:08:44 leavens Exp $ */
#ifndef _ID_USE_H
#define _ID_USE_H
#include "id_attrs.h"
#include "lexical_address.h"

// An id_use struct gives all the information from
// a lookup in the symbol table for a name:
// the (pointer to the) id_attrs (attributes)
// and the number of lexical levels out
// from the current scope to where the name was declared.
typedef struct id_use_s {
    id_attrs *attrs;
    unsigned int levelsOutward;    
} id_use;

// Requires: attrs != NULL
// Allocate and return an  id_use struct
// containing attrs and levelsOut.
extern id_use *id_use_create(
           id_attrs *attrs,
           unsigned int levelsOut);

// Requires: idu != NULL
// Return a pointer to the attributes of this id_use
// and the result will never be NULL
extern id_attrs *id_use_get_attrs(id_use *idu);

// Requires: idu != NULL
// Return (a pointer to) the lexical address for idu.
extern lexical_address *id_use_2_lexical_address(id_use *idu);
#endif
