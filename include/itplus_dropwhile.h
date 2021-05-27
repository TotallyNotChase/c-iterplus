/**
 * @file
 * @brief Macros for implementing the `dropWhile` abstraction using the `IterDropWhile` struct.
 *
 * https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:dropWhile
 * An IterDropWhile struct is a struct that stores a source iterable, a predicate function, and a flag to indicate
 * when to stop testing with the predicate function. It keeps dropping from the source iterable until the
 * predicate returns false, and starts returning the elements from the iterators from there on.
 */

#ifndef LIB_ITPLUS_DROPWHILE_H
#define LIB_ITPLUS_DROPWHILE_H

#include "itplus_foreach.h"
#include "itplus_iterator.h"
#include "itplus_macro_utils.h"
#include "itplus_maybe.h"

/**
 * @def IterDropWhile(T)
 * @brief Convenience macro to get the type of the IterDropWhile struct with given element type.
 *
 * # Example
 *
 * @code
 * DefineIterDropWhile(int);
 * IterDropWhile(int) i; // Declares a variable of type IterDropWhile(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterDropWhile` will yield. Must be the same type name
 * passed to #DefineIterDropWhile(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define IterDropWhile(T) ITPL_CONCAT(IterDropWhile_, T)

/**
 * @def DefineIterDropWhile(T)
 * @brief Define an IterDropWhile struct that works on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterDropWhile(int); // Defines an IterDropWhile(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterDropWhile` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterDropWhile(T)                                                                                         \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        bool (*pred)(T x);                                                                                             \
        bool done;                                                                                                     \
        Iterable(T) src;                                                                                               \
    } IterDropWhile(T)

/**
 * @def define_iterdropwhile_func(T, Name)
 * @brief Define a function to turn an #IterDropWhile(T) into an #Iterable(T).
 *
 * Define the `next` function implementation for the #IterDropWhile(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterDropWhile(T)*` and wraps it in an `Iterable(T)`.
 *
 * # Example
 *
 * @code
 * DefineIterDropWhile(int);
 *
 * // Implement `Iterator` for `IterDropWhile(int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrdrpwhl(IterDropWhile(int)* x)`
 * define_iterdropwhile_func(int, wrap_intitrdrpwhl)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * static bool is_even(int x) { return x % 2 == 0; }
 * @endcode
 *
 * @code
 * // Drop (skip) elements from `it` (of type `Iterable(int)`), while they satisfy `is_even`, and create a new iterable
 * Iterable(int) after_evens = wrap_intitrdrpwhl(&(IterDropWhile(int)){ .pred = is_even, .src = it });
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterDropWhile` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterDropWhile(T) for the given `T` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_iterdropwhile_func(T, Name)                                                                             \
    static Maybe(T) ITPL_CONCAT(IterDropWhile(T), _nxt)(IterDropWhile(T) * self)                                       \
    {                                                                                                                  \
        if (self->done) {                                                                                              \
            return Nothing(T);                                                                                         \
        }                                                                                                              \
        Iterable(T) srcit = self->src;                                                                                 \
        foreach (T, x, srcit) {                                                                                        \
            if (!self->pred(x)) {                                                                                      \
                self->done = true;                                                                                     \
                return Just(x, T);                                                                                     \
            }                                                                                                          \
        }                                                                                                              \
        return Nothing(T);                                                                                             \
    }                                                                                                                  \
    impl_iterator(IterDropWhile(T)*, T, Name, ITPL_CONCAT(IterDropWhile(T), _nxt))

#endif /* !LIB_ITPLUS_DROPWHILE_H */
