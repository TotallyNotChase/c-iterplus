/**
 * @file
 * @brief Macros for implementing the `zip` abstraction using the `IterZip` struct.
 *
 * https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:zip
 * An IterZip struct is a struct that stores 2 iterables, and has a `next` function implementation that returns a `Pair`
 * of 2 elements, each from the 2 iterables.
 */

#ifndef LIB_ITPLUS_ZIP_H
#define LIB_ITPLUS_ZIP_H

#include "itplus_iterator.h"
#include "itplus_macro_utils.h"
#include "itplus_maybe.h"
#include "itplus_pair.h"

/**
 * @def IterZip(T, U)
 * @brief Convenience macro to get the type of the IterZip struct with given element types.
 *
 * # Example
 *
 * @code
 * DefineIterZip(int, char);
 * IterZip(int, char) i; // Declares a variable of type IterZip(int, char)
 * @endcode
 *
 * @param T The type of value the first `Iterable` wrapped in this `IterZip` will yield. Must be the same type name
 * passed to #DefineIterZip(T, U).
 * @param U The type of value the second `Iterable` wrapped in this `IterZip` will yield. Must be the same type name
 * passed to #DefineIterZip(T, U).
 *
 * @note If `T`, or `U`, is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 */
#define IterZip(T, U) ITPL_CONCAT(ITPL_CONCAT(IterZip, T), U)

/**
 * @def DefineIterZip(T, U)
 * @brief Define an IterZip struct that works with an `Iterable(T)` and an `Iterable(U)`.
 *
 * # Example
 *
 * @code
 * DefineIterZip(int, char); // Defines an IterZip(int, char) struct
 * @endcode
 *
 * @param T The type of value the first `Iterable` wrapped in this `IterZip` will yield.
 * @param U The type of value the second `Iterable` wrapped in this `IterZip` will yield.
 *
 * @note If `T`, or `U`, is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #Iterator(T) for the given `T` and `U` **must** also exist.
 */
#define DefineIterZip(T, U)                                                                                            \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        Iterable(T) asrc;                                                                                              \
        Iterable(U) bsrc;                                                                                              \
    } IterZip(T, U)

/**
 * @def define_iterzip_func(T, U, Name)
 * @brief Define a function to turn an #IterZip(T, U) into an #Iterable(T) where `T = Pair(T, U)`.
 *
 * Define the `next` function implementation for the #IterZip(T, U) struct, and use it to implement the Iterator
 * typeclass, for given `T` and `U`.
 *
 * The defined function takes in a value of type `IterZip(T, U)*` and wraps it in an `Iterable(Pair(T, U))`.
 *
 * # Example
 *
 * @code
 * DefineIterZip(int, char);
 *
 * // Implement `Iterator` for `IterZip(int, char)`
 * // The defined function has the signature- `Iterable(Pair(int, char)) wrap_intchrzip(IterZip(int, char)* x)`
 * define_iterzip_func(int, char, wrap_intchrzip)
 * @endcode
 *
 * @param T The type of value the first `Iterable` wrapped in this `IterZip` will yield.
 * @param U The type of value the second `Iterable` wrapped in this `IterZip` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T`, or `U`, is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterZip(T, U) for the given `T` and `U` **must** exist.
 * @note An #Iterator(T), with `T = Pair(T, U)`, for the given `T` and `U` must exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_iterzip_func(T, U, Name)                                                                                \
    static Maybe(Pair(T, U)) ITPL_CONCAT(IterZip(T, U), _nxt)(IterZip(T, U) * self)                                         \
    {                                                                                                                  \
        Iterable(T) asrcit = self->asrc;                                                                               \
        Iterable(U) bsrcit = self->bsrc;                                                                               \
        Maybe(T) ares      = asrcit.tc->next(asrcit.self);                                                             \
        if (is_nothing(ares)) {                                                                                        \
            return Nothing(Pair(T, U));                                                                                \
        }                                                                                                              \
        Maybe(U) bres = bsrcit.tc->next(bsrcit.self);                                                                  \
        if (is_nothing(bres)) {                                                                                        \
            return Nothing(Pair(T, U));                                                                                \
        }                                                                                                              \
        return Just(PairOf(from_just_(ares), from_just_(bres), T, U), Pair(T, U));                                     \
    }                                                                                                                  \
    impl_iterator(IterZip(T, U)*, Pair(T, U), Name, ITPL_CONCAT(IterZip(T, U), _nxt))

#endif /* !LIB_ITPLUS_ZIP_H */
