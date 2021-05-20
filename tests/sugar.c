/**
 * @file
 * @brief Wrapper functions to fill pre-allocated iterplus structs and turn them into Iterables.
 *
 * See `sugar.h`.
 */

#include "sugar.h"

#include "common.h"
#include "impls.h"

#include <stdint.h>

/* Utility functions to fill pre-allocated iterplus structs and turn them into iterables */

/* Define the utility function to turn a pre-allocated IterTake struct into iterable */
#define prep_tk(T, Name, implfunc)                                                                                     \
    Iterable(T) Name(IterTake(T) * tk, Iterable(T) x)                                                                  \
    {                                                                                                                  \
        tk->src = x;                                                                                                   \
        return implfunc(tk);                                                                                           \
    }

/* Define the utility function to turn a pre-allocated IterMap struct into iterable */
#define prep_mp(A, B, Name, implfunc)                                                                                  \
    Iterable(B) Name(IterMap(A, B) * mp, Iterable(A) x, B(*const fn)(A))                                               \
    {                                                                                                                  \
        mp->f   = fn;                                                                                                  \
        mp->src = x;                                                                                                   \
        return implfunc(mp);                                                                                           \
    }

#define prep_flt(T, Name, implfunc)                                                                                    \
    Iterable(T) Name(IterFilt(T) * flt, Iterable(T) x, bool (*const pred)(T))                                          \
    {                                                                                                                  \
        flt->pred = pred;                                                                                              \
        flt->src  = x;                                                                                                 \
        return implfunc(flt);                                                                                          \
    }

// clang-format off
prep_tk(uint32_t, prep_u32tk, u32tk_to_itr)
prep_tk(NumType, prep_numtypetk, numtypetk_to_itr)
prep_tk(string, prep_strtk, strtk_to_itr)

prep_mp(uint32_t, NumType, prep_u32numtypemap, u32numtypemap_to_itr)

prep_flt(uint32_t, prep_u32filt, u32filt_to_itr)
prep_flt(string, prep_strfilt, strfilt_to_itr)
