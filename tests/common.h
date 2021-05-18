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

// clang-format off
/* Define `Iterator` for uint32_t elements */
DefineMaybe(uint32_t)
DefineIteratorOf(uint32_t);
// clang-format on
/* Implement `IterTake` struct for uint32_t iterables */
DefineIterTake(uint32_t);

#endif /* !LIB_ITPLUS_COMMON_H */
