/**
 * @file
 * @brief Macros for implementing the `map` abstraction.
 *
 * https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:map
 * An IterMap struct is a struct that stores a mapping function within it, as well as the source iterable.
 */

#ifndef LIB_ITPLUS_MAP_H
#define LIB_ITPLUS_MAP_H

#include "itplus_iterator.h"
#include "itplus_macro_utils.h"
#include "itplus_maybe.h"

/**
 * @def IterMap(ElmntType, FnRetType)
 * @brief Convenience macro to get the type of the IterMap struct with given element type and function return type.
 *
 * # Example
 *
 * @code
 * DefineIterMap(int, int);
 * IterMap(int, int) i; // Declares a variable of type IterMap(int, int)
 * @endcode
 *
 * @param ElmntType The type of value the `Iterable` wrapped in this `IterTake` will yield. Must be the same type name
 * passed to #DefineIterMap(ElmntType, FnRetType).
 * @param FnRetType The type of value the function contained within an `IterMap` struct will return. Must be the same
 * type name passed to #DefineIterMap(ElmntType, FnRetType).
 *
 * @note If `ElmntType` (or `FnRetType`) is a pointer, it needs to be typedef-ed into a type that does not contain the
 * `*`. Only alphanumerics.
 */
#define IterMap(ElmntType, FnRetType) ITPL_CONCAT(ITPL_CONCAT(IterMap_, ElmntType), ITPL_CONCAT(_, FnRetType))

/**
 * @def DefineIterMap(ElmntType, FnRetType)
 * @brief Define an IterMap struct that maps a function of type `FnRetType (*)(ElmntType)` over an
 * `Iterable(ElmntType)`.
 *
 * # Example
 *
 * @code
 * DefineIterMap(int, int); // Defines an IterMap(int, int) struct
 * @endcode
 *
 * @param ElmntType The type of value the `Iterable` wrapped in this `IterMap` will yield.
 * @param FnRetType The type of value the function contained within an `IterMap` struct will return.
 *
 * @note If `ElmntType` (or `FnRetType`) is a pointer, it needs to be typedef-ed into a type that does not contain the
 * `*`. Only alphanumerics.
 * @note An #Iterator(T) for given `ElmntType`, and `FnRetType` **must** also exist.
 */
#define DefineIterMap(ElmntType, FnRetType)                                                                            \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        FnRetType (*f)(ElmntType x);                                                                                   \
        Iterable(ElmntType) src;                                                                                       \
    } IterMap(ElmntType, FnRetType)

/**
 * @def define_itermap_func(ElmntType, FnRetType, Name)
 * @brief Define a function to turn an #IterMap(ElmntType, FnRetType) into an #Iterable(FnRetType).
 *
 * Define the `next` function implementation for the #IterMap(ElmntType, FnRetType) struct, and use it to implement the
 * Iterator typeclass, for given `ElmntType` and `FnRetType`.
 *
 * The defined function takes in a value of type `IterMap(ElmntType, FnRetType)*` and wraps it in an
 * `Iterable(FnRetType)`.
 *
 * # Example
 *
 * @code
 * DefineIterMap(int);
 *
 * // Implement `Iterator` for `IterMap(int, int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrmp(IterMap(int, int)* x)`
 * define_itermap_func(int, int, wrap_intitrmp)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * // Decrement an int
 * static int decr(int a) { return a - 1; }
 * @endcode
 *
 * @code
 * // Map `decr` over `it` (of type `Iterable(int)`)
 * Iterable(int) decr_it = wrap_intitrmp(&(IterMap(int, int)){ .f = decr, .src = it });
 * @endcode
 *
 * @param ElmntType The type of value the `Iterable` wrapped in this `IterMap` will yield.
 * @param FnRetType The type of value the function contained within an `IterMap` struct will return.
 * @param Name Name to define the function as.
 *
 * @note If `ElmntType` (or `FnRetType`) is a pointer, it needs to be typedef-ed into a type that does not contain the
 * `*`. Only alphanumerics..
 * @note An #IterMap(ElmntType, FnRetType) for the given `ElmntType` and `FnRetType` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_itermap_func(ElmntType, FnRetType, Name)                                                                \
    static Maybe(FnRetType) ITPL_CONCAT(IterMap(ElmntType, FnRetType), _nxt)(IterMap(ElmntType, FnRetType) * self)     \
    {                                                                                                                  \
        Iterable(ElmntType) const srcit = self->src;                                                                   \
        Maybe(ElmntType) res            = srcit.tc->next(srcit.self);                                                  \
        return fmap_maybe(res, self->f, FnRetType);                                                                    \
    }                                                                                                                  \
    impl_iterator(IterMap(ElmntType, FnRetType)*, FnRetType, Name, ITPL_CONCAT(IterMap(ElmntType, FnRetType), _nxt))

#endif /* !LIB_ITPLUS_MAP_H */
