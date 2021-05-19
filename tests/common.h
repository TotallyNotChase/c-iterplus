/**
 * @file
 * Declarations of iterator and iterplus structs used in the test executable.
 */

#ifndef LIB_ITPLUS_COMMON_H
#define LIB_ITPLUS_COMMON_H

#include "itplus_filter.h"
#include "itplus_foreach.h"
#include "itplus_iterator.h"
#include "itplus_map.h"
#include "itplus_maybe.h"
#include "itplus_take.h"
#include "itplus_typeclass.h"

#include <stdint.h>

typedef enum
{
    EVEN,
    ODD
} NumType;

// clang-format off
/* Define `Iterator` for uint32_t, NumType elements */
DefineMaybe(uint32_t)
DefineMaybe(NumType)
DefineIteratorOf(uint32_t);
DefineIteratorOf(NumType);
// clang-format on
/* Implement `IterTake` struct for uint32_t, NumType iterables */
DefineIterTake(uint32_t);
DefineIterTake(NumType);
/* Implement `IterMap` struct for uint32_t -> NumType */
DefineIterMap(uint32_t, NumType);
/* Implement `IterFilt` struct for uint32_t, NumType iterables */
DefineIterFilt(uint32_t);
DefineIterFilt(NumType);

#endif /* !LIB_ITPLUS_COMMON_H */
