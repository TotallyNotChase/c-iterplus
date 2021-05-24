/**
 * @file
 * @brief Macros for implementing the `chain` abstraction using the `IterChain` struct.
 *
 * https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.chain
 * An IterChain struct is a struct that stores 2 iterables, and starts consuming from the other one
 * once one of them has been fully consumed. Essentially chaining together the 2 iterables.
 */

#ifndef LIB_ITPLUS_CHAIN_H
#define LIB_ITPLUS_CHAIN_H

#include "itplus_iterator.h"
#include "itplus_macro_utils.h"
#include "itplus_maybe.h"

/**
 * @def IterChain(T)
 * @brief Convenience macro to get the type of the IterChain struct with given element type.
 *
 * # Example
 *
 * @code
 * DefineIterChain(int);
 * IterChain(int) i; // Declares a variable of type IterChain(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterChain` will yield. Must be the same type name passed
 * to #DefineIterChain(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define IterChain(T) ITPL_CONCAT(IterChain_, T)

/**
 * @def DefineIterChain(T)
 * @brief Define an IterChain struct that works on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterChain(int); // Defines an IterChain(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterChain` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterChain(T)                                                                                             \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        Iterable(T) curr;                                                                                              \
        Iterable(T) nxt;                                                                                               \
    } IterChain(T)

/**
 * @def define_iterchain_func(T, Name)
 * @brief Define a function to turn an #IterChain(T) into an #Iterable(T).
 *
 * Define the `next` function implementation for the #IterChain(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterChain(T)*` and wraps it in an `Iterable(T)`.
 *
 * # Example
 *
 * @code
 * DefineIterChain(int);
 *
 * // Implement `Iterator` for `IterChain(int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrchn(IterChain(int)* x)`
 * define_iterchain_func(int, wrap_intitrchn)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterChain` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterChain(T) for the given `T` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_iterchain_func(T, Name)                                                                                 \
    static Maybe(T) ITPL_CONCAT(IterChain(T), _nxt)(IterChain(T) * self)                                                    \
    {                                                                                                                  \
        Iterable(T) srcit = self->curr;                                                                                \
        Maybe(T) res      = srcit.tc->next(srcit.self);                                                                \
        if (is_just(res)) {                                                                                            \
            return res;                                                                                                \
        }                                                                                                              \
        self->curr           = self->nxt;                                                                              \
        Iterable(T) re_srcit = self->curr;                                                                             \
        return re_srcit.tc->next(re_srcit.self);                                                                       \
    }                                                                                                                  \
    impl_iterator(IterChain(T)*, T, Name, ITPL_CONCAT(IterChain(T), _nxt))

#endif /* !LIB_ITPLUS_CHAIN_H */
