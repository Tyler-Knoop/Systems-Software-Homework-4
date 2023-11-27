#include <stdbool.h>
#include <stdlib.h>
#include "literal_table.h"
#include "machine_types.h"

#define MAX_SIZE 4096

typedef struct literal_table
{
    word_type literal;
    unsigned int offset;
    struct literal_table *next;
} literal_table;

static unsigned int size;

literal_table *head;     // the head of the literal table linked list, should NOT be iterated over
literal_table *iterator; // iterator for the literal table

// returns the current size of the literal table
unsigned int literal_table_size()
{
    return size;
}

// returns true if the literal table is empty and false if it is full
bool literal_table_empty()
{
    if (size != 0)
    {
        return false;
    }

    return true;
}

// returns true if the literal table is full and false if it is not
bool literal_table_full()
{
    if (size < MAX_SIZE)
    {
        return false;
    }

    return true;
}

// initializes the literal table values
void literal_table_initialize()
{
    size = 0;
    head = NULL;
}

// returns the offset of the value found
int literal_table_find_offset(const char *sought, word_type value)
{
    return iterator->next->offset;
}

// returns true if the value is in that node and false if it is not
bool literal_table_present(const char *sought, word_type value)
{
    if (literal_table_iteration_next() != value)
    {
        return false;
    }

    return true;
}

// returns the offset of the value passed to the function
unsigned int literal_table_lookup(const char *val_string, word_type value)
{
    literal_table_start_iteration();

    while (literal_table_iteration_has_next()) // iterate over the linked list
    {
        if (literal_table_present(val_string, value)) // if the value is at that node
        {
            return literal_table_find_offset(val_string, value); // return the offset
        }

        iterator = iterator->next; // go to the next node if the value is not in that node
    }

    // if it gets here the value is not in the literal table, so create a new node and add it to the literal table
    literal_table *new = malloc(sizeof(literal_table));
    new->literal = value;
    new->next = NULL;

    if (literal_table_empty())
    {
        head = new;
        new->offset = 0;
    }

    else
    {
        new->offset = iterator->offset + 1;
        iterator->next = new;
    }

    ++size;

    literal_table_end_iteration();

    return new->offset; // return the offset of the newly added value in the literal table
}

// starts the iteration by saving the head
void literal_table_start_iteration()
{
    head = iterator; // save the head of the linked list
}

// ends the iteration by resetting the head
void literal_table_end_iteration()
{
    iterator = head; // reset the head of the linked list
}

// returns true if the linked list has a next
bool literal_table_iteration_has_next()
{
    if (iterator->next == NULL)
    {
        return false;
    }

    return true;
}

// returns the next literal in the linked list
word_type literal_table_iteration_next()
{
    iterator = iterator->next;
    return iterator->literal;
}