/**
 * @file
 * Implementations of iterables and iterplus utilities.
 */

#include "impls.h"

#include "common.h"

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
define_itertake_func(uint32_t, u32tk_to_itr)
/* Implement `take` functionality for NumType iterables */
define_itertake_func(NumType, numtypetk_to_itr)
/* Implement `map` functionality for uint32_t -> NumType */
define_itermap_func(uint32_t, NumType, u32numtypemap_to_itr)
/* Implement `filter` functionality for uint32_t iterables */
define_iterfilt_func(uint32_t, u32filt_to_itr)
/* Implement `filter` functionality for NumType iterables */
define_iterfilt_func(NumType, numtypefilt_to_itr)
