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

/* Function to turn an `IterDrop(uint32_t)*` into its `Iterable` impl, defined using `define_iterdrop_of` */
Iterable(uint32_t) u32drp_to_itr(IterDrop(uint32_t) * x);

/* Function to turn an `IterMap(uint32_t, NumType)*` into its `Iterable` impl, defined using `define_itermap_of` */
Iterable(NumType) u32numtypemap_to_itr(IterMap(uint32_t, NumType) * x);

/* Function to turn an `IterFilt(uint32_t)*` into its `Iterable` impl, defined using `define_iterfilt_of` */
Iterable(uint32_t) u32filt_to_itr(IterFilt(uint32_t) * x);

/* Function to turn an `IterFilt(string)*` into its `Iterable` impl, defined using `define_iterfilt_of` */
Iterable(string) strfilt_to_itr(IterFilt(string) * x);

/* Function to turn an `IterFiltMap(string, uint32_t)*` into its `Iterable` impl, defined using `define_iterfiltmap_of`
 */
Iterable(uint32_t) stru32filtmap_to_itr(IterFiltMap(string, uint32_t) * x);

/* Function to turn an `IterFiltMap(string, NumType)*` into its `Iterable` impl, defined using `define_iterfiltmap_of`
 */
Iterable(NumType) strnumtypefiltmap_to_itr(IterFiltMap(string, NumType) * x);

/* Function to turn an `IterChain(uint32_t)*` into its `Iterable` impl, defined using `define_iterchain_of`
 */
Iterable(uint32_t) u32chn_to_itr(IterChain(uint32_t) * x);

/* Function to reduce an `Iterable(uint32_t)` with an accumulating function */
Maybe(uint32_t) reduce_u32(Iterable(uint32_t) it, uint32_t (*f)(uint32_t acc, uint32_t x));

/* Function to turn an `IterTakeWhile(uint32_t)*` into its `Iterable` impl, defined using `define_itertakewhile_func`
 */
Iterable(uint32_t) u32tkwhl_to_itr(IterTakeWhile(uint32_t) * x);

/* Function to turn an `IterDropWhile(uint32_t)*` into its `Iterable` impl, defined using `define_iterdropwhile_func`
 */
Iterable(uint32_t) u32drpwhl_to_itr(IterDropWhile(uint32_t) * x);

/* Function to turn an `Iterable(uint32_t)` into an array of uint32_t elements */
uint32_t* collect_u32(Iterable(uint32_t) it, size_t* len);

/* Function to fold an `Iterable(string)` with a folding function `f`, and accumulator type `string` */
string fold_str_str(Iterable(string) it, string init, string (*f)(string acc, string x));

/* Function to fold an `Iterable(string)` with a folding function `f`, and accumulator type `uint32_t` */
uint32_t fold_str_u32(Iterable(string) it, uint32_t init, uint32_t (*f)(uint32_t acc, string x));

#endif /* !IT_FIB_H */
