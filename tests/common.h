/**
 * @file
 * Declarations of iterator and iterplus structs used in the test executable.
 */

#ifndef LIB_ITPLUS_COMMON_H
#define LIB_ITPLUS_COMMON_H

#include "itplus.h"

#include <stdint.h>

typedef enum
{
    EVEN,
    ODD
} NumType;

/* Type for string literals, the only type of strings used in the examples */
typedef char const* string;

/* Define iterplus for uint32_t iterables */
Iterplus(uint32_t);

// clang-format off
/* Also manually define `Iterator` and some iterplus utilities for NumType and string elements */
DefineMaybe(NumType)
DefineMaybe(string)
DefineIteratorOf(NumType);
DefineIteratorOf(string);
// clang-format on
DefineIterTake(NumType);
DefineIterTake(string);
DefineIterFilt(string);
/* Define `IterFiltMap` struct for string -> uint32_t, and string -> NumType */
DefineIterFiltMap(string, uint32_t);
DefineIterFiltMap(string, NumType);

/* Extend `Iterplus(uint32_t)` map support to uint32_t -> NumType */
DefineIterMap(uint32_t, NumType);

#endif /* !LIB_ITPLUS_COMMON_H */
