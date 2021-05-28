/**
 * @file
 * @brief Macros for implementing the `enumerate` abstraction using the `IterEnumr` struct.
 *
 * https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.enumerate
 * An IterEnumr struct is a struct that stores an iterable, and its iteration index. Its `next` function implementation
 * that returns a `Pair` of 2 elements, first being the index, and second being an element from the source iterable.
 */

#ifndef LIB_ITPLUS_ENUMR_H
#define LIB_ITPLUS_ENUMR_H

#include "itplus_iterator.h"
#include "itplus_macro_utils.h"
#include "itplus_maybe.h"
#include "itplus_pair.h"

#include <stddef.h>

/**
 * @def IterEnumr(T, U)
 * @brief Convenience macro to get the type of the IterEnumr struct with given element types.
 *
 * # Example
 *
 * @code
 * DefineIterEnumr(int);
 * IterEnumr(int) i; // Declares a variable of type IterEnumr(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterEnumr` will yield. Must be the same type name
 * passed to #DefineIterEnumr(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 */
#define IterEnumr(T) ITPL_CONCAT(IterEnumr_, T)

/**
 * @def DefineIterEnumr(T)
 * @brief Define an IterEnumr struct that works with on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterEnumr(int); // Defines an IterEnumr(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterEnumr` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterEnumr(T)                                                                                             \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        Iterable(T) src;                                                                                               \
    } IterEnumr(T)

/**
 * @def define_iterenumr_func(T, Name)
 * @brief Define a function to turn an #IterEnumr(T) into an #Iterable(T) where `T = Pair(size_t, T)`.
 *
 * Define the `next` function implementation for the #IterEnumr(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterEnumr(T)*` and wraps it in an `Iterable(Pair(size_t, T))`.
 *
 * # Example
 *
 * @code
 * DefineIterEnumr(int);
 *
 * // Implement `Iterator` for `IterEnumr(int)`
 * // The defined function has the signature- `Iterable(Pair(size_t, int)) wrap_intenumr(IterEnumr(int)* x)`
 * define_iterenumr_func(int, wrap_intenumr)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * // Enumerate `it` (of type `Iterable(int)`) to create a new iterable
 * Iterable(Pair(size_t, int)) enumerated = wrap_intenumr(&(IterEnumr(int)){ .src = it });
 * @endcode
 *
 * @param T The type of value the first `Iterable` wrapped in this `IterEnumr` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterEnumr(T) for the given `T` **must** exist.
 * @note An #Iterator(T), with `T = Pair(size_t, T)`, for the given `T`.
 * @note This should not be delimited by a semicolon.
 */
#define define_iterenumr_func(T, Name)                                                                                 \
    static Maybe(Pair(size_t, T)) ITPL_CONCAT(IterEnumr(T), _nxt)(IterEnumr(T) * self)                                 \
    {                                                                                                                  \
        Iterable(T) srcit = self->src;                                                                                 \
        Maybe(T) res      = srcit.tc->next(srcit.self);                                                                \
        return is_just(res) ? Just(PairOf(self->i++, from_just_(res), size_t, T), Pair(size_t, T))                     \
                            : Nothing(Pair(size_t, T));                                                                \
    }                                                                                                                  \
    impl_iterator(IterEnumr(T)*, Pair(size_t, T), Name, ITPL_CONCAT(IterEnumr(T), _nxt))

#endif /* !LIB_ITPLUS_ENUMR_H */
