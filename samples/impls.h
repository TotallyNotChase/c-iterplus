#ifndef LIB_ITPLUS_SAMPL_IMPL_H
#define LIB_ITPLUS_SAMPL_IMPL_H

#define ITPLUS_COLLECT_BUFSZ 16 /* Custom collect buffer size for iterplus collect utility */

#include "itplus.h"

#include <stdint.h>

/* Type for string literals, the only type of strings used in the examples */
typedef char const* string;

/* Implement the necessary struct and functions */

Iterplus(char);
DeclIterplus(char, takechr, dropchr, map_chrchr, filtchr, chr_reduce, chr_fold, filtmap_chrchr, chainchr, takewhlchr,
    dropwhlchr, enmrchr, zip_chrchr, chr_collect);

/* Define some extra iterplus utilities for `Pair(char, char)` */
DefineIterTakeWhile(Pair(char, char));
DefineIterMap(Pair(char, char), char);
Iterable(Pair(char, char)) takewhl_chrchr(IterTakeWhile(Pair(char, char)) * x);
Iterable(char) map_chrchr_chr(IterMap(Pair(char, char), char) * x);

/* Also define `Iterable(string)` with a specific folding utility */
// clang-format off
DefineMaybe(string)
DefineIteratorOf(string);
// clang-format on
char* str_fold(Iterable(string) it, char* init, char* (*acc)(char* acc, string x));

/* Also define `Iterable(uint64_t)` with some utilities */
// clang-format off
DefineMaybe(uint64_t)
DefinePair(uint64_t, uint64_t);
DefineMaybe(Pair(uint64_t, uint64_t))
DefineIteratorOf(uint64_t);
DefineIteratorOf(Pair(uint64_t, uint64_t));
// clang-format on
DefineIterZip(uint64_t, uint64_t);
DefineIterMap(Pair(uint64_t, uint64_t), uint64_t);
Iterable(Pair(uint64_t, uint64_t)) zip_u64u64(IterZip(uint64_t, uint64_t)* x);
Iterable(uint64_t) map_u64u64_u64(IterMap(Pair(uint64_t, uint64_t), uint64_t) * x);
Maybe(uint64_t) u64_reduce(Iterable(uint64_t) it, uint64_t (*acc)(uint64_t acc, uint64_t x));

/* ArrIter for the `char` array type */
typedef struct
{
    size_t i;
    size_t const size;
    /* Array of string literals */
    char const* const arr;
} ChrArrIter;

/* ArrIter for the `string` array type */
typedef struct
{
    size_t i;
    size_t const size;
    /* Array of string literals */
    string const* const arr;
} StrArrIter;

/* ArrIter for the `uint64_t` array type */
typedef struct
{
    size_t i;
    size_t const size;
    /* Array of string literals */
    uint64_t const* const arr;
} U64ArrIter;


/* Turn a pointer to a `ChrArrIter` struct to an iterable */
Iterable(char) prep_chrarr_itr(ChrArrIter* self);
/* Turn a pointer to a `StrArrIter` struct to an iterable */
Iterable(string) prep_strarr_itr(StrArrIter* self);
/* Turn a pointer to a `U64ArrIter` struct to an iterable */
Iterable(uint64_t) prep_u64arr_itr(U64ArrIter* self);

/* Convert an array of chars into an `Iterable` */
#define chrarr_to_iter(srcarr, len) prep_chrarr_itr(&(ChrArrIter){.size = (len), .arr = (srcarr)})
/* Convert an array of string literals into an `Iterable` */
#define strarr_to_iter(srcarr, len) prep_strarr_itr(&(StrArrIter){.size = (len), .arr = (srcarr)})
/* Convert an array of string literals into an `Iterable` */
#define u64arr_to_iter(srcarr, len) prep_u64arr_itr(&(U64ArrIter){.size = (len), .arr = (srcarr)})

#endif /* !LIB_ITPLUS_SAMPL_IMPL_H */
