/**
 * @file
 * @brief Macros for implementing the `foldl` abstraction.
 *
 * https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:foldl
 */

#ifndef LIB_ITPLUS_FOLD_H
#define LIB_ITPLUS_FOLD_H

#include "itplus_foreach.h"
#include "itplus_iterator.h"

/**
 * @def define_iterfold_func(T, Acc, Name)
 * @brief Define the `fold` function for an iterable and an accumulator type.
 *
 * The defined fold function takes in an iterable of type `T`, a function of type `Acc (*const f)(Acc acc, T x)`, and
 * a starting value of type `Acc`, and folds the iterable to a singular value of type `Acc`, by repeatedly applying `f`
 * onto it.
 *
 * This defined function will consume the given iterable.
 *
 * @param T The type of value the `Iterable`, for which this is being implemented, yields.
 * @param Acc The accumulator type the fold function being defined should work on.
 * @param Name Name to define the function as.
 *
 * @note If `T` (or `Acc`) is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 * @note This should not be delimited with a semicolon.
 */
#define define_iterfold_func(T, Acc, Name)                                                                             \
    Acc Name(Iterable(T) it, Acc init, Acc (*f)(Acc acc, T x))                                                         \
    {                                                                                                                  \
        Acc acc = init;                                                                                                \
        foreach (T, x, it) {                                                                                           \
            acc = f(acc, x);                                                                                           \
        }                                                                                                              \
        return acc;                                                                                                    \
    }

#endif /* !LIB_ITPLUS_FOLD_H */
