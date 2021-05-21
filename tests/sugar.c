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

/* Macro to define a function that turns a pre-allocated IterTake struct into iterable */
#define prep_tk(T, Name, implfunc)                                                                                     \
    Iterable(T) Name(IterTake(T) * tk, Iterable(T) x)                                                                  \
    {                                                                                                                  \
        tk->src = x;                                                                                                   \
        return implfunc(tk);                                                                                           \
    }

/* Macro to define a function that turns a pre-allocated IterTake struct into iterable */
#define prep_drp(T, Name, implfunc)                                                                                    \
    Iterable(T) Name(IterDrop(T) * tk, Iterable(T) x)                                                                  \
    {                                                                                                                  \
        tk->src = x;                                                                                                   \
        return implfunc(tk);                                                                                           \
    }

/* Macro to define a function that turns a pre-allocated IterMap struct into iterable */
#define prep_mp(A, B, Name, implfunc)                                                                                  \
    Iterable(B) Name(IterMap(A, B) * mp, Iterable(A) x, B(*fn)(A))                                                     \
    {                                                                                                                  \
        mp->f   = fn;                                                                                                  \
        mp->src = x;                                                                                                   \
        return implfunc(mp);                                                                                           \
    }

/* Macro to define a function that turns a pre-allocated IterFilt struct into iterable */
#define prep_flt(T, Name, implfunc)                                                                                    \
    Iterable(T) Name(IterFilt(T) * flt, Iterable(T) x, bool (*pred)(T))                                                \
    {                                                                                                                  \
        flt->pred = pred;                                                                                              \
        flt->src  = x;                                                                                                 \
        return implfunc(flt);                                                                                          \
    }

/* Macro to define a function that turns a pre-allocated IterFiltMap struct into iterable */
#define prep_fltmap(A, B, Name, implfunc)                                                                              \
    Iterable(B) Name(IterFiltMap(A, B) * fltmp, Iterable(A) x, Maybe(B)(*fn)(A))                                       \
    {                                                                                                                  \
        fltmp->f   = fn;                                                                                               \
        fltmp->src = x;                                                                                                \
        return implfunc(fltmp);                                                                                        \
    }

/* Macro to define a function that turns a pre-allocated IterChain struct into iterable */
#define prep_chn(T, Name, implfunc)                                                                                    \
    Iterable(T) Name(IterChain(T) * chn, Iterable(T) x, Iterable(T) y)                                                 \
    {                                                                                                                  \
        chn->curr = x;                                                                                                 \
        chn->nxt  = y;                                                                                                 \
        return implfunc(chn);                                                                                          \
    }

// clang-format off
prep_tk(uint32_t, prep_u32tk, u32tk_to_itr)
prep_tk(NumType, prep_numtypetk, numtypetk_to_itr)
prep_tk(string, prep_strtk, strtk_to_itr)

prep_drp(uint32_t, prep_u32drp, u32drp_to_itr)

prep_mp(uint32_t, NumType, prep_u32numtypemap, u32numtypemap_to_itr)

prep_flt(uint32_t, prep_u32filt, u32filt_to_itr)
prep_flt(string, prep_strfilt, strfilt_to_itr)

prep_fltmap(string, uint32_t, prep_stru32fltmap, stru32filtmap_to_itr)
prep_fltmap(string, NumType, prep_strnumtypefltmap, strnumtypefiltmap_to_itr)

prep_chn(uint32_t, prep_u32chn, u32chn_to_itr)
