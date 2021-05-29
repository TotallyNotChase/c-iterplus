#include "sugar.h"

#include "impls.h"

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

/* Macro to define a function that turns a pre-allocated IterEnumr struct into iterable */
#define prep_enumr(T, Name, implfunc)                                                                                  \
    Iterable(Pair(size_t, T)) Name(IterEnumr(T) * enumr, Iterable(T) x)                                                \
    {                                                                                                                  \
        enumr->src = x;                                                                                                \
        return implfunc(enumr);                                                                                        \
    }

#define prep_zip(T, U, Name, implfunc)                                                                                 \
    Iterable(Pair(T, U)) Name(IterZip(T, U) * zipstrct, Iterable(T) x, Iterable(U) y)                                  \
    {                                                                                                                  \
        zipstrct->asrc = x;                                                                                            \
        zipstrct->bsrc = y;                                                                                            \
        return implfunc(zipstrct);                                                                                     \
    }

// clang-format off
prep_tkwhl(Pair(char, char), prep_chrchr_tkwhl, takewhl_chrchr)

prep_zip(char, char, prep_chrchr_zip, zip_chrchr)
prep_zip(uint64_t, uint64_t, prep_u64u64_zip, zip_u64u64)

prep_mp(Pair(char, char), char, prep_chrchr_chr_map, map_chrchr_chr)
prep_mp(Pair(uint64_t, uint64_t), uint64_t, prep_u64u64_u64_map, map_u64u64_u64)
