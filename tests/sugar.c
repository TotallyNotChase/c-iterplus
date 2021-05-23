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

/* Macro to define a function that turns a pre-allocated IterTakeWhile struct into iterable */
#define prep_tkwhl(T, Name, implfunc)                                                                                  \
    Iterable(T) Name(IterTakeWhile(T) * tkwhl, Iterable(T) x, bool (*pred)(T))                                         \
    {                                                                                                                  \
        tkwhl->pred = pred;                                                                                            \
        tkwhl->src  = x;                                                                                               \
        return implfunc(tkwhl);                                                                                        \
    }

/* Macro to define a function that turns a pre-allocated IterTakeWhile struct into iterable */
#define prep_drpwhl(T, Name, implfunc)                                                                                 \
    Iterable(T) Name(IterDropWhile(T) * drpwhl, Iterable(T) x, bool (*pred)(T))                                        \
    {                                                                                                                  \
        drpwhl->pred = pred;                                                                                           \
        drpwhl->src  = x;                                                                                              \
        return implfunc(drpwhl);                                                                                       \
    }

/* Macro to define a function that turns a pre-allocated IterElemIndices struct into iterable */
#define prep_elmindcs(T, Name, implfunc)                                                                               \
    Iterable(size_t) Name(IterElemIndices(T) * elmindcs, Iterable(T) x)                                                \
    {                                                                                                                  \
        elmindcs->src = x;                                                                                             \
        return implfunc(elmindcs);                                                                                     \
    }

/* Macro to define a function that turns a pre-allocated IterEnumr struct into iterable */
#define prep_enumr(T, Name, implfunc)                                                                                  \
    Iterable(Pair(size_t, T)) Name(IterEnumr(T) * enumr, Iterable(T) x)                                         \
    {                                                                                                                  \
        enumr->src = x;                                                                                                \
        return implfunc(enumr);                                                                                        \
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

prep_tkwhl(uint32_t, prep_u32tkwhl, u32tkwhl_to_itr)

prep_drpwhl(uint32_t, prep_u32drpwhl, u32drpwhl_to_itr)

prep_elmindcs(uint32_t, prep_u32elmindcs, u32elmindcs_to_itr)

prep_enumr(uint32_t, prep_u32enumr, u32enumr_to_itr)
