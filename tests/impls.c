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

/* Implement the iterplus utilities for `uint32_t` iterables */
DefnIterplus(
    uint32_t, u32tk_to_itr, u32drp_to_itr, u32u32map_to_itr, u32filt_to_itr, reduce_u32, fold_u32_u32,
    u32u32filtmap_to_itr, u32chn_to_itr, u32tkwhl_to_itr, u32drpwhl_to_itr, u32enumr_to_itr, u32u32zip_to_itr,
    collect_u32
)

/* Implement some specific iterplus utilities for NumType and string iterables */
/* Implement `take` functionality for NumType iterables */
define_itertake_func(NumType, numtypetk_to_itr)
/* Implement `take` functionality for string iterables */
define_itertake_func(string, strtk_to_itr)
/* Implement `filter` functionality for string iterables */
define_iterfilt_func(string, strfilt_to_itr)
/* Implement `filter_map` functionality for string -> uint32_t, and string -> NumType */
define_iterfiltmap_func(string, uint32_t, stru32filtmap_to_itr)
define_iterfiltmap_func(string, NumType, strnumtypefiltmap_to_itr)
/* Implement `fold` functionality for string -> string, and string -> uint32_t */
define_iterfold_func(string, string, fold_str_str)
define_iterfold_func(string, uint32_t, fold_str_u32)

/* Extend `Iterplus(uint32_t)` map support to uint32_t -> NumType */
define_itermap_func(uint32_t, NumType, u32numtypemap_to_itr)
