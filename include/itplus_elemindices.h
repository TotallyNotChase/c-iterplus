/**
 * @file
 * @brief Macros for implementing the `elem_indices` abstraction using the `IterElemIndices` struct.
 *
 * https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:elemIndices
 * An IterElemIndices struct is a struct that keeps track of the index the iteration is at. In each `next` call,
 * it consumes from the source iterable, and returns the iteration index.
 */

#ifndef LIB_ITPLUS_ELEMINDC_H
#define LIB_ITPLUS_ELEMINDC_H

#include "itplus_iterator.h"
#include "itplus_macro_utils.h"
#include "itplus_maybe.h"

#include <stddef.h>

/**
 * @def IterElemIndices(T)
 * @brief Convenience macro to get the type of the IterElemIndices struct with given element type.
 *
 * # Example
 *
 * @code
 * DefineIterElemIndices(int);
 * IterElemIndices(int) i; // Declares a variable of type IterElemIndices(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterElemIndices` will yield. Must be the same type name
 * passed to #DefineIterElemIndices(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define IterElemIndices(T) CONCAT(IterElemIndices, T)

/**
 * @def DefineIterElemIndices(T)
 * @brief Define an IterElemIndices struct that works on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterElemIndices(int); // Defines an IterElemIndices(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterElemIndices` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterElemIndices(T)                                                                                       \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        Iterable(T) src;                                                                                               \
    } IterElemIndices(T)

/**
 * @def define_iterelemindc_func(T, Name)
 * @brief Define a function to turn an #IterElemIndices(T) into an #Iterable(T).
 *
 * Define the `next` function implementation for the #IterElemIndices(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterElemIndices(T)*` and wraps it in an `Iterable(size_t)`.
 *
 * # Example
 *
 * @code
 * DefineIterElemIndices(int);
 *
 * // Implement `Iterator` for `IterElemIndices(int)`
 * // The defined function has the signature- `Iterable(size_t) wrap_intitrelmindc(IterElemIndices(int)* x)`
 * define_iterelemindc_func(int, wrap_intitrelmindc)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterElemIndices` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterElemIndices(T) for the given `T` **must** exist.
 * @note An #Iterator(T) for T = `size_t` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_iterelemindc_func(T, Name)                                                                              \
    static Maybe(size_t) CONCAT(IterElemIndices(T), _nxt)(IterElemIndices(T) * self)                                   \
    {                                                                                                                  \
        Iterable(T) srcit = self->src;                                                                                 \
        return is_just(srcit.tc->next(srcit.self)) ? Just(self->i++, size_t) : Nothing(size_t);                        \
    }                                                                                                                  \
    impl_iterator(IterElemIndices(T)*, size_t, Name, CONCAT(IterElemIndices(T), _nxt))

#endif /* !LIB_ITPLUS_ELEMINDC_H */
