/**
 * @file
 * Declarations of iterator and iterplus structs used in the test executable.
 */

#ifndef LIB_ITPLUS_COMMON_H
#define LIB_ITPLUS_COMMON_H

#include "itplus_chain.h"
#include "itplus_collect.h"
#include "itplus_defn.h"
#include "itplus_drop.h"
#include "itplus_dropwhile.h"
#include "itplus_enumerate.h"
#include "itplus_filter.h"
#include "itplus_filtermap.h"
#include "itplus_fold.h"
#include "itplus_foreach.h"
#include "itplus_iterator.h"
#include "itplus_macro_utils.h"
#include "itplus_map.h"
#include "itplus_maybe.h"
#include "itplus_pair.h"
#include "itplus_reduce.h"
#include "itplus_take.h"
#include "itplus_takewhile.h"
#include "itplus_typeclass.h"
#include "itplus_zip.h"

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
