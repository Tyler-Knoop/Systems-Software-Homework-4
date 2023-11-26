#include <stdbool.h>
#include "literal_table.h"
#include "machine_types.h"

typedef struct literal_table{
    word_type literal;
    int offset;
    static int size;
    literal_table* next;    //for linked list
} literal_table;

literal_table* lit_tab; //the full literal table, can be iterated over
literal_table* head;    //the head of the literal table linked list, should NOT be iterated over

//returns the current size of the literal table
unsigned int literal_table_size()
{
    return lit_tab.size;
}

//returns true if the literal table is empty and false if it is full
bool literal_table_empty()
{
    if (lit_tab.size == 0) return true;
    else return false;
}

//returns true if the literal table is full and false if it is not
bool literal_table_full()
{
    if (lit_tab.size == 4096) return true;
    else return false;
}

//initializes the literal table values
void literal_table_initialize()
{
    lit_tab.offset = 0;
    lit_tab.literal = 0;
    lit_tab.next = NULL;
    lit_tab.size = 0;
}

//returns the offset of the value found
int literal_table_find_offset(const char *sought, word_type value)
{
    return lit_tab.next.offset;
}

//returns true if the value is in that node and false if it is not
bool literal_table_present(const char *sought, word_type value)
{
    if (literal_table_iteration_next() == value) return true;
    else return false;
}

//returns the offset of the value passed to the function
unsigned int literal_table_lookup(const char *val_string, word_type value)
{
    literal_table_start_iteration();

    while (literal_table_iteration_has_next())  //iterate over the linked list
    {
        if (literal_table_present(val_string, value))   //if the value is at that node
        {
            return literal_table_find_offset(vale_string, value);   //return the offset
        }
        lit_tab = lit_tab.next; //go to the next node if the value is not in that node
    }

    //if it gets here the value is not in the literal table, so create a new node and add it to the literal table
    literal_table* new;
    new.literal = value;
    new.next = NULL;
    new.offset = lit_tab.offset + 1;
    lit_tab.next = new;
    lit_tab.size++;

    literal_table_end_iteration();

    return new.offset;  //return the offset of the newly added value in the literal table
}

//starts the iteration by saving the head
void literal_table_start_iteration()
{
    head = lit_tab; //save the head of the linked list
}

//ends the iteration by resetting the head
void literal_table_end_iteration()
{
    lit_tab = head; //reset the head of the linked list
}

//returns true if the linked list has a next
bool literal_table_iteration_has_next()
{
    if (lit_tab.next != NULL) return true;
    else return false;
}

//returns the next literal in the linked list
word_type literal_table_iteration_next()
{
    return lit_tab.next.literal;
}