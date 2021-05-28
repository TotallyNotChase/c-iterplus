/**
 * @file
 * Declarations of utilities, structs, and functions for which iterator and
 * iterplus utilities have been implemented.
 */

#ifndef LIB_ITPLUS_IMPL_H
#define LIB_ITPLUS_IMPL_H

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

/* Declaration of the iterplus utilities implemented for uint32_t iterables */
DeclIterplus(uint32_t, u32tk_to_itr, u32drp_to_itr, u32u32map_to_itr, u32filt_to_itr, reduce_u32, fold_u32_u32,
    u32u32filtmap_to_itr, u32chn_to_itr, u32tkwhl_to_itr, u32drpwhl_to_itr, u32enumr_to_itr, u32u32zip_to_itr,
    collect_u32);

/* Declarations of some specific iterplus utilities defined for string and NumType iterables */
Iterable(NumType) numtypetk_to_itr(IterTake(NumType) * x);
Iterable(string) strtk_to_itr(IterTake(string) * x);
Iterable(string) strfilt_to_itr(IterFilt(string) * x);
Iterable(uint32_t) stru32filtmap_to_itr(IterFiltMap(string, uint32_t) * x);
Iterable(NumType) strnumtypefiltmap_to_itr(IterFiltMap(string, NumType) * x);
string fold_str_str(Iterable(string) it, string init, string (*f)(string acc, string x));
uint32_t fold_str_u32(Iterable(string) it, uint32_t init, uint32_t (*f)(uint32_t acc, string x));

/* Declaration for `Iterplus(uint32_t)` map support to uint32_t -> NumType */
Iterable(NumType) u32numtypemap_to_itr(IterMap(uint32_t, NumType) * x);

#endif /* !LIB_ITPLUS_IMPL_H */
