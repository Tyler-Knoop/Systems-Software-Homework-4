#include <stdbool.h>
#include "literal_table.h"
#include "machine_types.h"

unsigned int literal_table_size()
{
}

bool literal_table_empty()
{
}

bool literal_table_full()
{
}

void literal_table_initialize()
{
}

int literal_table_find_offset(const char *sought, word_type value)
{
}

bool literal_table_present(const char *sought, word_type value)
{
}

unsigned int literal_table_lookup(const char *val_string, word_type value)
{
}

void literal_table_start_iteration()
{
}

void literal_table_end_iteration()
{
}

bool literal_table_iteration_has_next()
{
}

word_type literal_table_iteration_next()
{
}