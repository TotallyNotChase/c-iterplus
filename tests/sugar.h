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
#include "preproc_map.h"

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

Iterable(size_t) prep_u32elmindcs(IterElemIndices(uint32_t) * elmindcs, Iterable(uint32_t) x);

Iterable(Pair(size_t, uint32_t)) prep_u32enumr(IterEnumr(uint32_t) * enumr, Iterable(uint32_t) x);

/*
Macro to generate a generic selection association list element.

Each element of the var args will be of form `(Type, expr)` - each element, a "tuple".

Mapping `IT_GEN_ASSOC` over such an element results in `IT_GEN_ASSOC((Type, expr))`, which is then fed into
`IT_GEN_ASSOC_` without the extra parens- `IT_GEN_ASSOC_ (Type, expr)`.

`IT_GEN_ASSOC_`, then creates the assoc list element - `Iterable(Type): (expr)`. A valid element to be used in the
`_Generic` selection assoc list.
*/
#define IT_GEN_ASSOC_(T, expr) Iterable(T) : (expr)
#define IT_GEN_ASSOC(Texpr)    IT_GEN_ASSOC_ Texpr

/*
Similar to the `IT_GEN_ASSOC` macro in semantics and usage. This one is for functions, `A -> B`
*/
#define FN_GEN_ASSOC_(A, B, expr) B (*)(A) : (expr)
#define FN_GEN_ASSOC(ABexpr)      FN_GEN_ASSOC_ ABexpr

#define itrble_selection(it, ...) _Generic((it), MAP(IT_GEN_ASSOC, __VA_ARGS__))

#define fn_selection(fn, ...) _Generic((fn), MAP(FN_GEN_ASSOC, __VA_ARGS__))

/* NOTE: The values returned by the following convenience macros have local scope and lifetime */

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
    itrble_selection((it), (uint32_t, prep_u32tk), (NumType, prep_numtypetk), (string, prep_strtk))(                   \
        itrble_selection((it), (uint32_t, &(IterTake(uint32_t)){.limit = (n)}),                                        \
            (NumType, &(IterTake(NumType)){.limit = (n)}), (string, &(IterTake(string)){.limit = (n)})),               \
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
    itrble_selection((it), (uint32_t, prep_u32drp))(                                                                   \
        itrble_selection((it), (uint32_t, &(IterDrop(uint32_t)){.limit = (n)})), (it))

#define map_selection(it, fn, when_u32_numtype)                                                                        \
    itrble_selection((it), (uint32_t, fn_selection(&(fn), (uint32_t, NumType, when_u32_numtype))))

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
    map_selection((it), (fn), prep_u32numtypemap)(                                                                     \
        map_selection((it), (fn), &(IterMap(uint32_t, NumType)){0}), (it), (fn))

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
    itrble_selection((it), (uint32_t, prep_u32filt), (string, prep_strfilt))(                                          \
        itrble_selection((it), (uint32_t, &(IterFilt(uint32_t)){0}), (string, &(IterFilt(string)){0})), (it), (pred))

#define filtmap_selection(it, fn, when_str_u32, when_str_numtype)                                                      \
    itrble_selection((it), (string, fn_selection(&(fn), (string, Maybe(uint32_t), (when_str_u32)),                     \
                                        (string, Maybe(NumType), (when_str_numtype)))))

/**
 * @def filter_map(it, fn)
 * Map and filter an iterable at the same time.
 *
 * @param it The source iterable.
 * @param pred The function to filter the map and then filter the iterable with. Must be a function returning a `Maybe`,
 * and taking a singular argument, the type of which should be the same as the type the iterable yields. `Nothing`
 * values are filtered out, `Just` values are kept in (only the raw value).
 *
 * @return Iterable of the same type as the source iterable.
 * @note Iterating over the returned iterable also progresses the given iterable.
 */
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
    itrble_selection((itx), (uint32_t, prep_u32chn))(                                                                  \
        itrble_selection((itx), (uint32_t, &(IterChain(uint32_t)){0})), (itx), (ity))

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
#define reduce(it, fn) itrble_selection((it), (uint32_t, reduce_u32))(it, fn)

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
    itrble_selection((it), (uint32_t, prep_u32tkwhl))(                                                                 \
        itrble_selection((it), (uint32_t, &(IterTakeWhile(uint32_t)){0})), (it), (pred))

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
    itrble_selection((it), (uint32_t, prep_u32drpwhl))(                                                                \
        itrble_selection((it), (uint32_t, &(IterDropWhile(uint32_t)){0})), (it), (pred))

/**
 * @def collect(it, len)
 * @brief Collect the given iterable, `it`, into an array, and store its length in len.
 *
 * @param it The iterable to reduce.
 * @param lenstore Pointer to a `size_t` variable, to store the length in.
 *
 * @return Array of collected values. The elements are of the same type as the element type of the iterable.
 * @note Returned array must be freed.
 * @note This consumes the given iterable.
 */
#define collect(it, lenstore) itrble_selection((it), (uint32_t, collect_u32))(it, lenstore)

#define fold_selection(it, fn, when_str_str, when_str_u32)                                                             \
    itrble_selection((it), (string, _Generic(&(fn), uint32_t(*const)(uint32_t, string)                                 \
                                             : (when_str_u32), uint32_t(*)(uint32_t, string)                           \
                                             : (when_str_u32), string(*const)(string, string)                          \
                                             : (when_str_str), string(*)(string, string)                               \
                                             : (when_str_str))))

/**
 * @def fold(it, init, fn)
 * Left fold an iterable, `it`, with the accumulating function `fn` and starting value `init`.
 *
 * @param it The source iterable to fold.
 * @param init The starting accumulator value.
 * @param fn The accumulating function. Must be a function returning the same type as init, and taking 2 arguments-
 * first one being the accumulator, and second one being the iterable element.
 *
 * @return Accumulated value after folding the iterable.
 * @note This consumes the given iterable.
 */
#define fold(it, init, fn) fold_selection((it), (fn), fold_str_str, fold_str_u32)((it), (init), (fn))

/**
 * @def elem_indices(it)
 * @brief Build an iterable that consists of the indices of the elements in given iterable `it`.
 *
 * @param it The source iterable.
 *
 * @return Iterable yielding `size_t` elements.
 * @note Iterating over the returned iterable also progresses the given iterable.
 */
#define elem_indices(it)                                                                                               \
    itrble_selection((it), (uint32_t, prep_u32elmindcs))(                                                              \
        itrble_selection((it), (uint32_t, &(IterElemIndices(uint32_t)){0})), (it))

/**
 * @def enumerate(it)
 * @brief Build an iterable that consists of a Pair of the indices and the elements in given iterable `it`.
 *
 * @param it The source iterable.
 *
 * @return Iterable yielding a Pair of size_t and and the same type as the given iterable element.
 * @note Iterating over the returned iterable also progresses the given iterable.
 */
#define enumerate(it)                                                                                                  \
    itrble_selection((it), (uint32_t, prep_u32enumr))(                                                                 \
        itrble_selection((it), (uint32_t, &(IterEnumr(uint32_t)){0})), (it))

#endif /* !LIB_ITPLUS_SUGAR_H */
