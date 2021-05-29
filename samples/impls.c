#include "impls.h"

/* `next` implementation for the `StrArrIter` struct */
static Maybe(char) chrarrnxt(ChrArrIter* self)
{
    return self->i < self->size ? Just(self->arr[self->i++], char) : Nothing(char);
}

/* `next` implementation for the `StrArrIter` struct */
static Maybe(string) strarrnxt(StrArrIter* self)
{
    return self->i < self->size ? Just(self->arr[self->i++], string) : Nothing(string);
}

/* `next` implementation for the `U64ArrIter` struct */
static Maybe(uint64_t) u64arrnxt(U64ArrIter* self)
{
    return self->i < self->size ? Just(self->arr[self->i++], uint64_t) : Nothing(uint64_t);
}

// clang-format off
/* Implement `Iterator` for `ChrArrIter`, `StrArrIter`, and `U64ArrIter` */
impl_iterator(ChrArrIter*, char, prep_chrarr_itr, chrarrnxt)
impl_iterator(StrArrIter*, string, prep_strarr_itr, strarrnxt)
impl_iterator(U64ArrIter*, uint64_t, prep_u64arr_itr, u64arrnxt)
/* Define the iterplus utilities for the necessary types */
DefnIterplus(char, takechr, dropchr, map_chrchr, filtchr, chr_reduce, chr_fold, filtmap_chrchr, chainchr, takewhlchr,
    dropwhlchr, enmrchr, zip_chrchr, chr_collect)

define_itertakewhile_func(Pair(char, char), takewhl_chrchr)
define_itermap_func(Pair(char, char), char, map_chrchr_chr)
define_iterfold_func(string, char*, str_fold)

define_iterzip_func(uint64_t, uint64_t, zip_u64u64)
define_itermap_func(Pair(uint64_t, uint64_t), uint64_t, map_u64u64_u64)
define_iterreduce_func(uint64_t, u64_reduce)
