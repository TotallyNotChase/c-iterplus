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
    uint32_t prev_curr = self->curr;
    self->curr         = self->next;
    self->next += prev_curr;
    return Just(prev_curr, uint32_t);
}

/* `next` implementation for the `StrArrIter` struct */
static Maybe(string) strarrnxt(StrArrIter* self)
{
    return self->i < self->size ? Just(self->arr[self->i++], string) : Nothing(string);
}

// clang-format off
/* Implement `Iterator` for `Fibonacci*` */
impl_iterator(Fibonacci*, uint32_t, prep_fib_itr, fibnxt)
/* Implement `Iterator` for `StrArrIter` */
impl_iterator(StrArrIter*, string, prep_strarr_itr, strarrnxt)
/* Implement `take` functionality for uint32_t iterables */
define_itertake_func(uint32_t, u32tk_to_itr)
/* Implement `take` functionality for NumType iterables */
define_itertake_func(NumType, numtypetk_to_itr)
/* Implement `take` functionality for string iterables */
define_itertake_func(string, strtk_to_itr)
/* Implement `drop` functionality for uint32_t iterables */
define_iterdrop_func(uint32_t, u32drp_to_itr)
/* Implement `map` functionality for uint32_t -> NumType */
define_itermap_func(uint32_t, NumType, u32numtypemap_to_itr)
/* Implement `filter` functionality for uint32_t iterables */
define_iterfilt_func(uint32_t, u32filt_to_itr)
/* Implement `filter` functionality for string iterables */
define_iterfilt_func(string, strfilt_to_itr)
/* Implement `filter_map` functionality for string -> uint32_t, and string -> NumType */
define_iterfiltmap_func(string, uint32_t, stru32filtmap_to_itr)
define_iterfiltmap_func(string, NumType, strnumtypefiltmap_to_itr)
/* Imeplement `chain` functionality for uint32_t iterables */
define_iterchain_func(uint32_t, u32chn_to_itr)
