/**
 * @file
 * Declarations of utilities, structs, and functions for which iterator and
 * iterplus utilities have been implemented.
 */

#ifndef IT_FIB_H
#define IT_FIB_H

#include "common.h"
#include "stdnames.h"

#include <stdint.h>

typedef struct fibonacci
{
    uint32_t curr;
    uint32_t next;
} Fibonacci;

/* Create an infinite `Iterable` representing the fibonacci sequence */
#define get_fibitr() prep_fib_itr(&(Fibonacci){.curr = 0, .next = 1})

/* Turn a pointer to a `Fibonacci` struct to an iterable */
Iterable(uint32_t) prep_fib_itr(Fibonacci* self);

/* Function to turn an `IterTake(uint32_t)` into its `Iterable` impl, defined using `define_itertake_of` */
Iterable(uint32_t) prep_itertake_of(uint32_t)(IterTake(uint32_t) * x);

/* Function to turn an `IterTake(uint32_t)` into its `Iterable` impl, defined using `define_itertake_of` */
Iterable(NumType) prep_itertake_of(NumType)(IterTake(NumType) * x);

/* Function to turn an `IterMap(uint32_t, NumType)` into its `Iterable` impl, defined using `define_itermap_of` */
Iterable(NumType) prep_itermap_of(uint32_t, NumType)(IterMap(uint32_t, NumType) * x);

#endif /* !IT_FIB_H */
