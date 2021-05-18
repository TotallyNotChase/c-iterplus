/**
 * @file
 * @brief Macros for implementing the `take` abstraction using the `IterTake` struct.
 *
 * https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:take
 * An IterTake struct is a struct that keeps track of how many elements have been extracted out of an iterator
 * as well as a limit to stop at - the iterator impl for this struct will keep consuming from an iterator until
 * it hits that limit, or until the source iterable gets exhausted, whichever comes first.
 */

#ifndef LIB_ITPLUS_TAKE_H
#define LIB_ITPLUS_TAKE_H

#include "itplus_iterator.h"
#include "itplus_macro_utils.h"
#include "itplus_maybe.h"

/**
 * @def IterTake(T)
 * @brief Convenience macro to get the type of the IterTake struct with given element type.
 *
 * # Example
 *
 * @code
 * DefineIterTake(int);
 * IterTake(int) i; // Declares a variable of type IterTake(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterTake` will yield. Must be the same type name passed to
 * #DefineIterTake(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define IterTake(T) IterTake##T

/**
 * @def DefineIterTake(T)
 * @brief Define an IterTake struct that works on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterTake(int); // Defines an IterTake(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterTake` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterTake(T)                                                                                              \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        size_t const limit;                                                                                            \
        Iterable(T) const src;                                                                                         \
    } IterTake(T)

/**
 * @def define_itertake_func(T, Name)
 * @brief Define a function to turn an #IterTake(T) into an #Iterable(T).
 *
 * Define the `next` function implementation for the #IterTake(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterTake(T)*` and wraps it in an `Iterable(T)`.
 *
 * # Example
 *
 * @code
 * DefineIterTake(int);
 *
 * // Implement `Iterator` for `IterTake(int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrtk(IterTake(int)* x)`
 * define_itertake_func(int, wrap_intitrtk)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterTake` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterTake(T) for the given `T` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_itertake_func(T, Name)                                                                                  \
    static Maybe(T) CONCAT(IterTake(T), _nxt)(IterTake(T) * self)                                                      \
    {                                                                                                                  \
        if (self->i < self->limit) {                                                                                   \
            ++(self->i);                                                                                               \
            Iterable(T) srcit = self->src;                                                                             \
            return srcit.tc->next(srcit.self);                                                                         \
        }                                                                                                              \
        return Nothing(T);                                                                                             \
    }                                                                                                                  \
    impl_iterator(IterTake(T)*, T, Name, CONCAT(IterTake(T), _nxt))

#endif /* !LIB_ITPLUS_TAKE_H */
