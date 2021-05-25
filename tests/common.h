/**
 * @file
 * Declarations of iterator and iterplus structs used in the test executable.
 */

#ifndef LIB_ITPLUS_COMMON_H
#define LIB_ITPLUS_COMMON_H

#include "itplus_chain.h"
#include "itplus_collect.h"
#include "itplus_drop.h"
#include "itplus_dropwhile.h"
#include "itplus_enumerate.h"
#include "itplus_filter.h"
#include "itplus_filtermap.h"
#include "itplus_fold.h"
#include "itplus_foreach.h"
#include "itplus_iterator.h"
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

// clang-format off
/* Define `Iterator` for uint32_t, NumType, string, and size_t elements */
DefineMaybe(uint32_t)
DefineMaybe(NumType)
DefineMaybe(string)
DefineMaybe(size_t)
DefineIteratorOf(uint32_t);
DefineIteratorOf(NumType);
DefineIteratorOf(string);
DefineIteratorOf(size_t);
/* Also define `Iterator` for `Pair(size_t, uint32_t)` and `Pair(uint32_t, uint32_t)` */
DefinePair(size_t, uint32_t);
DefinePair(uint32_t, uint32_t);
DefineMaybe(Pair(size_t, uint32_t))
DefineMaybe(Pair(uint32_t, uint32_t))
DefineIteratorOf(Pair(size_t, uint32_t));
DefineIteratorOf(Pair(uint32_t, uint32_t));
// clang-format on
/* Define `IterTake` struct for uint32_t, NumType, and string iterables */
DefineIterTake(uint32_t);
DefineIterTake(NumType);
DefineIterTake(string);
/* Define `IterDrop` struct for uint32_t iterables */
DefineIterDrop(uint32_t);
/* Define `IterMap` struct for uint32_t -> NumType */
DefineIterMap(uint32_t, NumType);
/* Define `IterFilt` struct for uint32_t, and string iterables */
DefineIterFilt(uint32_t);
DefineIterFilt(string);
/* Define `IterFiltMap` struct for string -> uint32_t, and string -> NumType */
DefineIterFiltMap(string, uint32_t);
DefineIterFiltMap(string, NumType);
/* Define `IterChain` struct for uint32_t iterables */
DefineIterChain(uint32_t);
/* Define `IterTakeWhile` struct for uint32_t iterables */
DefineIterTakeWhile(uint32_t);
/* Define `IterDropWhile` struct for uint32_t iterables */
DefineIterDropWhile(uint32_t);
/* Define `IterEnumr` struct for `uint32_t` iterables */
DefineIterEnumr(uint32_t);
/* Define `IterZip` struct for (`uint32_t`, `uint32_t`) iterables */
DefineIterZip(uint32_t, uint32_t);

#endif /* !LIB_ITPLUS_COMMON_H */
