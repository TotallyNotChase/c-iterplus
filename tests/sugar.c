#include "sugar.h"

#include "common.h"
#include "impls.h"

#include <stdint.h>

/* Utility functions to fill pre-allocated iterplus utility structs and turn them into iterables */

Iterable(uint32_t) prep_u32tk(IterTake(uint32_t) * tk, Iterable(uint32_t) x)
{
    tk->src = x;
    return u32tk_to_itr(tk);
}

Iterable(NumType) prep_numtypetk(IterTake(NumType) * tk, Iterable(NumType) x)
{
    tk->src = x;
    return numtypetk_to_itr(tk);
}

Iterable(NumType)
    prep_u32numtypemap(IterMap(uint32_t, NumType) * mp, Iterable(uint32_t) x, NumType (*const fn)(uint32_t))
{
    mp->f   = fn;
    mp->src = x;
    return u32numtypemap_to_itr(mp);
}

Iterable(uint32_t)
    prep_numtypeu32map(IterMap(NumType, uint32_t) * mp, Iterable(NumType) x, uint32_t (*const fn)(NumType))
{
    mp->f   = fn;
    mp->src = x;
    return numtypeu32map_to_itr(mp);
}

Iterable(uint32_t) prep_u32filt(IterFilt(uint32_t) * flt, Iterable(uint32_t) x, bool (*const pred)(uint32_t))
{
    flt->pred = pred;
    flt->src  = x;
    return u32filt_to_itr(flt);
}

Iterable(NumType) prep_numtypefilt(IterFilt(NumType) * flt, Iterable(NumType) x, bool (*const pred)(NumType))
{
    flt->pred = pred;
    flt->src  = x;
    return numtypefilt_to_itr(flt);
}
