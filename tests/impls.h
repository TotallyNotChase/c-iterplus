/**
 * @file
 * Declarations of utilities, structs, and functions for which iterator and
 * iterplus utilities have been implemented.
 */

#ifndef IT_FIB_H
#define IT_FIB_H

#include "common.h"

#include <stdint.h>

typedef struct fibonacci
{
    uint32_t curr;
    uint32_t next;
} Fibonacci;

typedef struct
{
    size_t i;
    size_t const size;
    /* Array of string literals */
    string const* const arr;
} StrArrIter;

/* Turn a pointer to a `Fibonacci` struct to an iterable */
Iterable(uint32_t) prep_fib_itr(Fibonacci* self);

/* Turn a pointer to a `StrArrIter` struct to an iterable */
Iterable(string) prep_strarr_itr(StrArrIter* self);

/* Create an infinite `Iterable` representing the fibonacci sequence */
#define get_fibitr() prep_fib_itr(&(Fibonacci){.curr = 0, .next = 1})

/* Convert an array of string literals into an `Iterable` */
#define strarr_to_iter(srcarr, len) prep_strarr_itr(&(StrArrIter){.i = 0, .size = (len), .arr = (srcarr)})

/* Function to turn an `IterTake(uint32_t)*` into its `Iterable` impl, defined using `define_itertake_of` */
Iterable(uint32_t) u32tk_to_itr(IterTake(uint32_t) * x);

/* Function to turn an `IterTake(uint32_t)*` into its `Iterable` impl, defined using `define_itertake_of` */
Iterable(NumType) numtypetk_to_itr(IterTake(NumType) * x);

/* Function to turn an `IterTake(string)*` into its `Iterable` impl, defined using `define_itertake_of` */
Iterable(string) strtk_to_itr(IterTake(string) * x);

/* Function to turn an `IterMap(uint32_t, NumType)*` into its `Iterable` impl, defined using `define_itermap_of` */
Iterable(NumType) u32numtypemap_to_itr(IterMap(uint32_t, NumType) * x);

/* Function to turn an `IterFilt(uint32_t)*` into its `Iterable` impl, defined using `define_iterfilt_of` */
Iterable(uint32_t) u32filt_to_itr(IterFilt(uint32_t) * x);

/* Function to turn an `IterFilt(string)*` into its `Iterable` impl, defined using `define_iterfilt_of` */
Iterable(string) strfilt_to_itr(IterFilt(string) * x);

#endif /* !IT_FIB_H */
