/**
 * @file
 * Declarations of iterator and iterplus structs used in the test executable.
 */

#ifndef LIB_ITPLUS_COMMON_H
#define LIB_ITPLUS_COMMON_H

#include "foreach.h"
#include "iterator.h"
#include "map.h"
#include "maybe.h"
#include "take.h"
#include "typeclass.h"

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

#endif /* !LIB_ITPLUS_COMMON_H */
