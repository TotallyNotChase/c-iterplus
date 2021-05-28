/**
 * @file
 * @brief Macros for implementing the `takeWhile` abstraction using the `IterTakeWhile` struct.
 *
 * https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:takeWhile
 * An IterTakeWhile struct is a struct that stores a source iterable, a predicate function, and a flag to indicate
 * when to stop testing with the predicate function. It keeps consuming from the source iterable, and stops once the
 * predicate returns false.
 */

#ifndef LIB_ITPLUS_TAKEWHILE_H
#define LIB_ITPLUS_TAKEWHILE_H

#include "itplus_iterator.h"
#include "itplus_macro_utils.h"
#include "itplus_maybe.h"

/**
 * @def IterTakeWhile(T)
 * @brief Convenience macro to get the type of the IterTakeWhile struct with given element type.
 *
 * # Example
 *
 * @code
 * DefineIterTakeWhile(int);
 * IterTakeWhile(int) i; // Declares a variable of type IterTakeWhile(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterTakeWhile` will yield. Must be the same type name
 * passed to #DefineIterTakeWhile(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define IterTakeWhile(T) ITPL_CONCAT(IterTakeWhile_, T)

/**
 * @def DefineIterTakeWhile(T)
 * @brief Define an IterTakeWhile struct that works on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterTakeWhile(int); // Defines an IterTakeWhile(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterTakeWhile` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterTakeWhile(T)                                                                                         \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        bool (*pred)(T x);                                                                                             \
        bool done;                                                                                                     \
        Iterable(T) src;                                                                                               \
    } IterTakeWhile(T)

/**
 * @def define_itertakewhile_func(T, Name)
 * @brief Define a function to turn an #IterTakeWhile(T) into an #Iterable(T).
 *
 * Define the `next` function implementation for the #IterTakeWhile(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterTakeWhile(T)*` and wraps it in an `Iterable(T)`.
 *
 * # Example
 *
 * @code
 * DefineIterTakeWhile(int);
 *
 * // Implement `Iterator` for `IterTakeWhile(int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrtkwhl(IterTakeWhile(int)* x)`
 * define_itertakewhile_func(int, wrap_intitrtkwhl)
 * @endcode
 *
 * Usage of the defined function-
 *
 * * @code
 * static bool is_even(int x) { return x % 2 == 0; }
 * @endcode
 *
 * @code
 * // Take elements from `it` (of type `Iterable(int)`), while they satisfy `is_even`, and create a new iterable
 * Iterable(int) first_evens = wrap_intitrtkwhl(&(IterTakeWhile(int)){ .pred = is_even, .src = it });
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterTakeWhile` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterTakeWhile(T) for the given `T` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_itertakewhile_func(T, Name)                                                                             \
    static Maybe(T) ITPL_CONCAT(IterTakeWhile(T), _nxt)(IterTakeWhile(T) * self)                                       \
    {                                                                                                                  \
        if (self->done) {                                                                                              \
            return Nothing(T);                                                                                         \
        }                                                                                                              \
        Iterable(T) const srcit = self->src;                                                                           \
        Maybe(T) const res      = srcit.tc->next(srcit.self);                                                          \
        if (is_nothing(res) || !self->pred(from_just_(res))) {                                                         \
            self->done = true;                                                                                         \
            return Nothing(T);                                                                                         \
        }                                                                                                              \
        return res;                                                                                                    \
    }                                                                                                                  \
    impl_iterator(IterTakeWhile(T)*, T, Name, ITPL_CONCAT(IterTakeWhile(T), _nxt))

#endif /* !LIB_ITPLUS_TAKEWHILE_H */
