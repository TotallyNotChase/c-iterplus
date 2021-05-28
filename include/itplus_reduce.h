/**
 * @file
 * @brief Macros for implementing the `reduce`/`foldl1` abstraction.
 *
 * https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:foldl1
 */

#ifndef LIB_ITPLUS_REDUCE_H
#define LIB_ITPLUS_REDUCE_H

#include "itplus_foreach.h"
#include "itplus_iterator.h"
#include "itplus_maybe.h"

/**
 * @def define_iterreduce_func(T, Name)
 * @brief Define the `reduce` function for an iterable.
 *
 * The defined reduce function takes in an iterable of type `T`, and a function of type `T (*const f)(T acc, T x)`, and
 * reduces the iterable to a singular value of type `T`, by repeatedly applying `f` onto it.
 *
 * If the given iterable was empty, `Nothing` is returned. Otherwise, a `Just` value is returned.
 *
 * This defined function will consume the given iterable.
 *
 * # Example
 *
 * @code
 * // The defined function has the signature:-
 * // `Maybe(int) int_reduce(Iterable(int), int (*f)(int acc, int x))`
 * define_iterreduce_func(int, int_reduce)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * // Add 2 ints
 * static int add(int a, int b) { return a + b; }
 * @endcode
 *
 * @code
 * // Reduce `it` (of type `Iterable(int)`) with `add`
 * Maybe(int) maybe_sum = int_reduce(it, add);
 * @endcode
 *
 * @param T The type of value the `Iterable`, for which this is being implemented, yields.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 * @note This should not be delimited with a semicolon.
 */
#define define_iterreduce_func(T, Name)                                                                                \
    Maybe(T) Name(Iterable(T) it, T (*f)(T acc, T x))                                                                  \
    {                                                                                                                  \
        Maybe(T) const res = it.tc->next(it.self);                                                                     \
        if (is_nothing(res)) {                                                                                         \
            return Nothing(T);                                                                                         \
        }                                                                                                              \
        T acc = from_just_(res);                                                                                       \
        foreach (T, x, it) {                                                                                           \
            acc = f(acc, x);                                                                                           \
        }                                                                                                              \
        return Just(acc, T);                                                                                           \
    }

#endif /* !LIB_ITPLUS_REDUCE_H */
