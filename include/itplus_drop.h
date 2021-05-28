/**
 * @file
 * @brief Macros for implementing the `drop` abstraction using the `IterDrop` struct.
 *
 * https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:drop
 * An IterDrop struct is a struct that keeps track of how many elements have been dropped out of an iterator
 * as well as a limit to start extracting the elements at - the iterator impl for this struct will keep dropping from
 * an iterator until it hits that limit, or until the source iterable gets exhausted, whichever comes first. Once enough
 * elements have been dropped, it'll start extracting and returning the rest of the elements.
 */

#ifndef LIB_ITPLUS_DROP_H
#define LIB_ITPLUS_DROP_H

#include "itplus_foreach.h"
#include "itplus_iterator.h"
#include "itplus_macro_utils.h"
#include "itplus_maybe.h"

#include <stddef.h>

/**
 * @def IterDrop(T)
 * @brief Convenience macro to get the type of the IterDrop struct with given element type.
 *
 * # Example
 *
 * @code
 * DefineIterDrop(int);
 * IterDrop(int) i; // Declares a variable of type IterDrop(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterDrop` will yield. Must be the same type name passed to
 * #DefineIterDrop(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define IterDrop(T) ITPL_CONCAT(IterDrop_, T)

/**
 * @def DefineIterDrop(T)
 * @brief Define an IterDrop struct that works on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterDrop(int); // Defines an IterDrop(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterDrop` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterDrop(T)                                                                                              \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        size_t limit;                                                                                                  \
        Iterable(T) src;                                                                                               \
    } IterDrop(T)

/**
 * @def define_iterdrop_func(T, Name)
 * @brief Define a function to turn an #IterDrop(T) into an #Iterable(T).
 *
 * Define the `next` function implementation for the #IterDrop(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterDrop(T)*` and wraps it in an `Iterable(T)`.
 *
 * # Example
 *
 * @code
 * DefineIterDrop(int);
 *
 * // Implement `Iterator` for `IterDrop(int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrdrp(IterDrop(int)* x)`
 * define_iterdrop_func(int, wrap_intitrdrp)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * // Drop (skip) the first 10 elements from `it` (of type `Iterable(int)`) and create a new iterable
 * Iterable(int) after10 = wrap_intitrdrp(&(IterDrop(int)){ .limit = 10, .src = it });
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterDrop` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterDrop(T) for the given `T` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_iterdrop_func(T, Name)                                                                                  \
    static Maybe(T) ITPL_CONCAT(IterDrop(T), _nxt)(IterDrop(T) * self)                                                 \
    {                                                                                                                  \
        Iterable(T) const srcit = self->src;                                                                           \
        foreach (T, x, srcit) {                                                                                        \
            if (self->i >= self->limit) {                                                                              \
                return Just(x, T);                                                                                     \
            }                                                                                                          \
            ++(self->i);                                                                                               \
        }                                                                                                              \
        return Nothing(T);                                                                                             \
    }                                                                                                                  \
    impl_iterator(IterDrop(T)*, T, Name, ITPL_CONCAT(IterDrop(T), _nxt))

#endif /* !LIB_ITPLUS_DROP_H */
