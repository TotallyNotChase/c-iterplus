/**
 * @file
 * @brief Macros for implementing the `filter` abstraction using the `IterFilt` struct.
 *
 * https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:filter
 * An IterFilt struct is a struct that stores a filtering function within it, as well as the source iterable.
 */

#ifndef LIB_ITPLUS_FILT_H
#define LIB_ITPLUS_FILT_H

#include "itplus_iterator.h"
#include "itplus_macro_utils.h"
#include "itplus_maybe.h"

#include <stdbool.h>

/**
 * @def IterFilt(T)
 * @brief Convenience macro to get the type of the IterFilt struct with given element type.
 *
 * # Example
 *
 * @code
 * DefineIterFilt(int);
 * IterFilt(int) i; // Declares a variable of type IterFilt(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterFilt` will yield. Must be the same type name passed to
 * #DefineIterFilt(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define IterFilt(T) ITPL_CONCAT(IterFilt, T)

/**
 * @def DefineIterFilt(T)
 * @brief Define an IterFilt struct that works on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterFilt(int); // Defines an IterFilt(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterFilt` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterFilt(T)                                                                                              \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        bool (*pred)(T x);                                                                                             \
        Iterable(T) src;                                                                                               \
    } IterFilt(T)

/**
 * @def define_iterfilt_func(T, Name)
 * @brief Define a function to turn an #IterFilt(T) into an #Iterable(T).
 *
 * Define the `next` function implementation for the #IterFilt(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterFilt(T)*` and wraps it in an `Iterable(T)`.
 *
 * # Example
 *
 * @code
 * DefineIterFilt(int);
 *
 * // Implement `Iterator` for `IterFilt(int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrfilt(IterFilt(int)* x)`
 * define_iterfilt_func(int, wrap_intitrfilt)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterFilt` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterFilt(T) for the given `T` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_iterfilt_func(T, Name)                                                                                  \
    static Maybe(T) ITPL_CONCAT(IterFilt(T), _nxt)(IterFilt(T) * self)                                                      \
    {                                                                                                                  \
        Iterable(T) const srcit = self->src;                                                                           \
        while (1) {                                                                                                    \
            Maybe(T) res = srcit.tc->next(srcit.self);                                                                 \
            if (is_nothing(res) || self->pred(from_just_(res))) {                                                      \
                return res;                                                                                            \
            }                                                                                                          \
        }                                                                                                              \
    }                                                                                                                  \
    impl_iterator(IterFilt(T)*, T, Name, ITPL_CONCAT(IterFilt(T), _nxt))

#endif /* !LIB_ITPLUS_FILT_H */
