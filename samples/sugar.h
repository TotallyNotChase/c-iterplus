#ifndef LIB_ITPLUS_SAMPL_SUGAR_H
#define LIB_ITPLUS_SAMPL_SUGAR_H

#include "impls.h"

#define MAP_1(f, x)      f(x)
#define MAP_2(f, x, ...) f(x), MAP_1(f, __VA_ARGS__)
#define MAP_3(f, x, ...) f(x), MAP_2(f, __VA_ARGS__)
#define MAP_4(f, x, ...) f(x), MAP_3(f, __VA_ARGS__)
#define MAP_5(f, x, ...) f(x), MAP_4(f, __VA_ARGS__)
#define MAP_6(f, x, ...) f(x), MAP_5(f, __VA_ARGS__)
#define MAP_7(f, x, ...) f(x), MAP_6(f, __VA_ARGS__)
#define MAP_8(f, x, ...) f(x), MAP_7(f, __VA_ARGS__)

#define MAP_RSEQ_N()                                      8, 7, 6, 5, 4, 3, 2, 1, 0
#define MAP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define MAP_NARG_(...)                                    MAP_ARG_N(__VA_ARGS__)
#define MAP_NARG(...)                                     MAP_NARG_(__VA_ARGS__, MAP_RSEQ_N())

#define MAP_(N, f, ...) ITPL_CONCAT(MAP_, N)(f, __VA_ARGS__)
#define MAP(f, ...)     MAP_(MAP_NARG(__VA_ARGS__), f, __VA_ARGS__)

/*
Functions to prepare a pre-allocated itertake, and turn it into its corresponding iterator instance
*/

Iterable(Pair(char, char)) prep_chrchr_tkwhl(
    IterTakeWhile(Pair(char, char)) * tkwhl, Iterable(Pair(char, char)) x, bool (*pred)(Pair(char, char)));

Iterable(Pair(char, char)) prep_chrchr_zip(IterZip(char, char) * zipstr, Iterable(char) x, Iterable(char) y);
Iterable(Pair(uint64_t, uint64_t))
    prep_u64u64_zip(IterZip(uint64_t, uint64_t) * zipstr, Iterable(uint64_t) x, Iterable(uint64_t) y);

Iterable(char) prep_chrchr_chr_map(
    IterMap(Pair(char, char), char) * mp, Iterable(Pair(char, char)) x, char (*fn)(Pair(char, char)));
Iterable(uint64_t) prep_u64u64_u64_map(IterMap(Pair(uint64_t, uint64_t), uint64_t) * mp,
    Iterable(Pair(uint64_t, uint64_t)) x, uint64_t (*fn)(Pair(uint64_t, uint64_t)));

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
Similar to the `IT_GEN_ASSOC` macro in semantics and usage. This one is for functions.

The first argument is the return type of the function, second is the expression to select if type matches, the rest of
the arguments are the function's argument types.
*/
#define FN_GEN_ASSOC_(Ret, expr, ...) Ret (*)(__VA_ARGS__) : (expr), Ret(*const)(__VA_ARGS__) : (expr)
#define FN_GEN_ASSOC(RetExprArgs)     FN_GEN_ASSOC_ RetExprArgs

#define itrble_selection(it, ...) _Generic((it), MAP(IT_GEN_ASSOC, __VA_ARGS__))

#define fn_selection(fn, ...) _Generic((fn), MAP(FN_GEN_ASSOC, __VA_ARGS__))

/* NOTE: The values returned by the following convenience macros have local scope and lifetime */

#define map_selection(it, fn, when_chrchr_chr, when_u64u64_u64)                                                        \
    itrble_selection((it), (Pair(char, char), (when_chrchr_chr)), (Pair(uint64_t, uint64_t), (when_u64u64_u64)))

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
    map_selection((it), (fn), prep_chrchr_chr_map, prep_u64u64_u64_map)(                                               \
        map_selection(                                                                                                 \
            (it), (fn), &(IterMap(Pair(char, char), char)){0}, &(IterMap(Pair(uint64_t, uint64_t), uint64_t)){0}),     \
        (it), (fn))

/**
 * @def takewhile(it, pred)
 * @brief Build an iterable that continuously consumes elements from given `it` while `pred` is satisfied.
 *
 * @param it The source iterable.
 * @param pred The The function to test each element with. Must be a function returning `bool`, and taking a singular
 * argument, the type of which should be the same as the type the iterable yields.
 *
 * @return Iterable of the same type as the source iterables.
 * @note Iterating over the returned iterable also progresses the given iterable.
 */
#define takewhile(it, pred)                                                                                            \
    itrble_selection((it), (Pair(char, char), prep_chrchr_tkwhl))(                                                     \
        itrble_selection((it), (Pair(char, char), &(IterTakeWhile(Pair(char, char))){0})), (it), (pred))

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
#define collect(it, lenstore) itrble_selection((it), (char, chr_collect))(it, lenstore)

#define fold_selection(it, fn, when_str_charp)                                                                         \
    itrble_selection((it), (string, fn_selection((fn), (char*, (when_str_charp), char*, string))))

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
#define fold(it, init, fn) fold_selection((it), (fn), str_fold)((it), (init), (fn))

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
#define reduce(it, fn) itrble_selection((it), (uint64_t, u64_reduce))(it, fn)

/**
 * @def zip(itx, ity)
 * @brief Build an iterable that zips together the given iterables `itx`, and `ity` to yield Pair structs.
 *
 * @param itx The first iterable.
 * @param ity The second iterable.
 *
 * @return Iterable yielding Pair structs. First element being from `itx`, second element being from `ity`.
 * @note Iterating over the returned iterable also progresses the given iterables.
 */
#define zip(itx, ity)                                                                                                  \
    itrble_selection((itx), (char, prep_chrchr_zip), (uint64_t, prep_u64u64_zip))(                                     \
        itrble_selection((itx), (char, &(IterZip(char, char)){0}), (uint64_t, &(IterZip(uint64_t, uint64_t)){0})),     \
        (itx), (ity))

#endif /* !LIB_ITPLUS_SUGAR_H */
