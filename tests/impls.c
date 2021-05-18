/**
 * @file
 * Implementations of iterables and iterplus utilities.
 */

#include "impls.h"

#include "common.h"
#include "stdnames.h"

#include <stdint.h>

/* `next` implementation for the `Fibonacci` struct */
static Maybe(uint32_t) fibnxt(Fibonacci* self)
{
    uint32_t new_nxt = self->curr + self->next;
    self->curr       = self->next;
    self->next       = new_nxt;
    return Just(new_nxt, uint32_t);
}

// clang-format off
/* Implement `Iterator` for `Fibonacci*` */
impl_iterator(Fibonacci*, uint32_t, prep_fib_itr, fibnxt)
/* Implement `take` functionality for uint32_t iterables */
define_itertake_func(uint32_t, prep_itertake_of(uint32_t))
/* Implement `take` functionality for NumType iterables */
define_itertake_func(NumType, prep_itertake_of(NumType))
/* Implement `map` functionality for uint32_t -> NumType */
define_itermap_func(uint32_t, NumType, prep_itermap_of(uint32_t, NumType))
