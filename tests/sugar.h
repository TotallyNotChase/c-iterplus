/**
 * @file
 * @brief "Syntactic sugar". Convenience macros for using the iterplus utilities.
 *
 * These examples use C11's `_Generic` to statically dispatch to the correct functions based on the
 * type of the iterplus structs. Syntax sugar, as you may know them. This is the definitions of those sugar
 * macros.
 *
 * Unfortunately (or fortunately), `_Generic` resolves during compile time, not preprocess time. This means the
 * expressions allowed within `_Generic` are much stricter than what you'd be able to get away with in regular macros.
 *
 * For this reason, we need wrapper functions, that take a pre-allocated iterplus struct, the iterable to put into said
 * struct (and other stuff if necessary), and return an Iterable corresponding to that iterplus struct.
 *
 * These wrapper functions are defined in `sugar.c`
 *
 * The structure pre-allocation and passing to this functions is done in the `_Generic` macros by taking the address of
 * a compound literal. The iterable struct, and other such members that have a varying type cannot be filled up inside
 * the `_Generic` assoc list expressions, which is why the functions need to do it for us.
 */

#ifndef LIB_ITPLUS_SUGAR_H
#define LIB_ITPLUS_SUGAR_H

#include "common.h"
#include "impls.h"

#include <stdint.h>

/*
Functions to prepare a pre-allocated itertake, and turn it into its corresponding iterator instance
*/

Iterable(uint32_t) prep_u32tk(IterTake(uint32_t) * tk, Iterable(uint32_t) x);
Iterable(NumType) prep_numtypetk(IterTake(NumType) * tk, Iterable(NumType) x);
Iterable(string) prep_strtk(IterTake(string) * tk, Iterable(string) x);

Iterable(uint32_t) prep_u32drp(IterDrop(uint32_t) * tk, Iterable(uint32_t) x);

Iterable(NumType) prep_u32numtypemap(IterMap(uint32_t, NumType) * tk, Iterable(uint32_t) x, NumType (*fn)(uint32_t));

Iterable(uint32_t) prep_u32filt(IterFilt(uint32_t) * flt, Iterable(uint32_t) x, bool (*pred)(uint32_t));
Iterable(string) prep_strfilt(IterFilt(string) * flt, Iterable(string) x, bool (*pred)(string));

Iterable(uint32_t)
    prep_stru32fltmap(IterFiltMap(string, uint32_t) * tk, Iterable(string) x, Maybe(uint32_t) (*fn)(string));
Iterable(NumType)
    prep_strnumtypefltmap(IterFiltMap(string, NumType) * tk, Iterable(string) x, Maybe(NumType) (*fn)(string));

Iterable(uint32_t) prep_u32chn(IterChain(uint32_t) * chn, Iterable(uint32_t) x, Iterable(uint32_t) y);

Iterable(uint32_t) prep_u32tkwhl(IterTakeWhile(uint32_t) * tkwhl, Iterable(uint32_t) x, bool (*pred)(uint32_t));

Iterable(uint32_t) prep_u32drpwhl(IterDropWhile(uint32_t) * drpwhl, Iterable(uint32_t) x, bool (*pred)(uint32_t));

#define NOIMPL(feat) No_##feat##_impl

/*
Definition less functions, these are just here to raise a compile time error
when a non supported type is used with the _Generic macros below
*/
void NOIMPL(map)(void);
void NOIMPL(take)(void);
void NOIMPL(drop)(void);
void NOIMPL(filter)(void);
void NOIMPL(filter_map)(void);
void NOIMPL(chain)(void);

/*
Generic selection over iterable type

Add more iterable types here if needed
*/
#define itrble_selection(it, when_u32, when_numtype, when_string)                                                      \
    _Generic((it), Iterable(uint32_t)                                                                                  \
             : (when_u32), Iterable(NumType)                                                                           \
             : (when_numtype), Iterable(string)                                                                        \
             : (when_string))
/*
Generic selection over mapping function type

Add more function types here if needed
*/
#define fn_selection(fn, when_u32_numtype)                                                                             \
    _Generic((fn), NumType(*const)(uint32_t) : (when_u32_numtype), NumType(*)(uint32_t) : (when_u32_numtype))

/* NOTE: The values returned by these convenience macros have local scope and lifetime */

/**
 * @def take(it, n)
 * @brief Build an iterable that consists of at most `n` elements from given `it` iterable.
 *
 * @param it The source iterable.
 * @param n The *maximum* number of elements to take from the iterable.
 *
 * @return Iterable of the same type as the source iterable.
 * @note Iterating over the returned iterable also progresses the given iterable.
 * @note If a negative `n` is passed, usual unsigned wrap around takes place. i.e `-1` wraps around to `SIZE_MAX`
 */
#define take(it, n)                                                                                                    \
    itrble_selection((it), prep_u32tk, prep_numtypetk, prep_strtk)(                                                    \
        itrble_selection((it), &(IterTake(uint32_t)){.limit = (n)}, &(IterTake(NumType)){.limit = (n)},                \
                         &(IterTake(string)){.limit = (n)}),                                                           \
        (it))

/**
 * @def drop(it, n)
 * @brief Build an iterable that drops (skips) the first `n` elements from given `it` iterable.
 *
 * @param it The source iterable.
 * @param n The number of elements to drop from the iterable.
 *
 * @return Iterable of the same type as the source iterable.
 * @note Iterating over the returned iterable also progresses the given iterable.
 * @note If a negative `n` is passed, usual unsigned wrap around takes place. i.e `-1` wraps around to `SIZE_MAX`
 */
#define drop(it, n)                                                                                                    \
    itrble_selection((it), prep_u32drp, NOIMPL(drop), NOIMPL(drop))(                                                   \
        itrble_selection((it), &(IterDrop(uint32_t)){.limit = (n)}, NOIMPL(drop), NOIMPL(drop)), (it))

#define map_selection(it, fn, when_u32_numtype)                                                                        \
    itrble_selection((it), fn_selection(&(fn), (when_u32_numtype)), NOIMPL(map), NOIMPL(map))

/**
 * @def map(it, fn)
 * @brief Map the function `fn` over `it` to make a new iterable.
 *
 * @param it The source iterable.
 * @param fn The function to map over the iterable. Must be a function taking a singular argument, the type of which
 * should be the same as the type the iterable yields.
 *
 * @return Iterable yielding elements of the given function's return type.
 * @note Iterating over the returned iterable also progresses the given iterable.
 */
#define map(it, fn)                                                                                                    \
    map_selection((it), (fn), prep_u32numtypemap)(map_selection((it), (fn), &(IterMap(uint32_t, NumType)){0}), (it),   \
                                                  (fn))

/**
 * @def filter(it, pred)
 * Filter an iterable by given `pred` and make a new iterable.
 *
 * @param it The source iterable.
 * @param pred The function to filter the iterable with. Must be a function returning `bool`, and taking a singular
 * argument, the type of which should be the same as the type the iterable yields.
 *
 * @return Iterable of the same type as the source iterable.
 * @note Iterating over the returned iterable also progresses the given iterable.
 */
#define filter(it, pred)                                                                                               \
    itrble_selection((it), prep_u32filt, NOIMPL(filter), prep_strfilt)(                                                \
        itrble_selection((it), &(IterFilt(uint32_t)){0}, NOIMPL(filter), &(IterFilt(string)){0}), (it), (pred))

#define filtmap_selection(it, fn, when_str_u32, when_str_numtype)                                                      \
    itrble_selection((it), NOIMPL(map), NOIMPL(map),                                                                   \
                     _Generic(&(fn), Maybe(uint32_t)(*const)(string)                                                   \
                              : (when_str_u32), Maybe(uint32_t)(*)(string)                                             \
                              : (when_str_u32), Maybe(NumType)(*const)(string)                                         \
                              : (when_str_numtype), Maybe(NumType)(*)(string)                                          \
                              : (when_str_numtype)))

#define filter_map(it, fn)                                                                                             \
    filtmap_selection((it), (fn), prep_stru32fltmap, prep_strnumtypefltmap)(                                           \
        filtmap_selection((it), (fn), &(IterFiltMap(string, uint32_t)){0}, &(IterFiltMap(string, NumType)){0}), (it),  \
        (fn))

/**
 * @def chain(itx, ity)
 * @brief Chain together 2 iterables of the same element type.
 *
 * @param it The first iterable.
 * @param n The second iterable.
 *
 * @return Iterable of the same type as the source iterables.
 * @note Iterating over the returned iterable also progresses the given iterables.
 */
#define chain(itx, ity)                                                                                                \
    itrble_selection((itx), prep_u32chn, NOIMPL(drop), NOIMPL(drop))(                                                  \
        itrble_selection((itx), &(IterChain(uint32_t)){0}, NOIMPL(drop), NOIMPL(drop)), (itx), (ity))

/**
 * @def reduce(it, fn)
 * @brief Reduce the given iterable, `it`, by the accumulating function `fn`.
 *
 * @param it The iterable to reduce.
 * @param fn The accumulating function.
 *
 * @return Reduced value, same type as the element type of the iterable.
 * @note This consumes the given iterable.
 */
#define reduce(it, fn) itrble_selection((it), reduce_u32, NOIMPL(drop), NOIMPL(drop))(it, fn)

/**
 * @def take_while(it, pred)
 * @brief Build an iterable that continuously consumes elements from given `it` while `pred` is satisfied.
 *
 * @param it The source iterable.
 * @param pred The The function to test each element with. Must be a function returning `bool`, and taking a singular
 * argument, the type of which should be the same as the type the iterable yields.
 *
 * @return Iterable of the same type as the source iterables.
 * @note Iterating over the returned iterable also progresses the given iterable.
 */
#define take_while(it, pred)                                                                                           \
    itrble_selection((it), prep_u32tkwhl, NOIMPL(drop), NOIMPL(drop))(                                                 \
        itrble_selection((it), &(IterTakeWhile(uint32_t)){0}, NOIMPL(drop), NOIMPL(drop)), (it), (pred))

/**
 * @def drop_while(it, pred)
 * @brief Build an iterable that continuously drops elements from given `it` until `pred` is no longer satisfied.
 *
 * @param it The source iterable.
 * @param pred The The function to test each element with. Must be a function returning `bool`, and taking a singular
 * argument, the type of which should be the same as the type the iterable yields.
 *
 * @return Iterable of the same type as the source iterables.
 * @note Iterating over the returned iterable also progresses the given iterable.
 */
#define drop_while(it, pred)                                                                                           \
    itrble_selection((it), prep_u32drpwhl, NOIMPL(drop), NOIMPL(drop))(                                                \
        itrble_selection((it), &(IterDropWhile(uint32_t)){0}, NOIMPL(drop), NOIMPL(drop)), (it), (pred))

#endif /* !LIB_ITPLUS_SUGAR_H */
