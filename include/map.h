/**
 * @file
 * @brief Macros for implementing the `map` abstraction.
 *
 * https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:map
 * An IterMap struct is a struct that stores a mapping function within it, as well as the source iterable.
 */

#ifndef IT_MAP_H
#define IT_MAP_H

#include "iterator.h"
#include "macro_utils.h"
#include "maybe.h"

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
#define IterMap(ElmntType, FnRetType) IterMap##ElmntType##FnRetType

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
        FnRetType (*const mapfn)(ElmntType x);                                                                         \
        Iterable(ElmntType) const src;                                                                                 \
    } IterMap(ElmntType, FnRetType)


/**
 * @def define_itermap_func(ElmntType, FnRetType, Name)
 * @brief Define a function to turn given `IterMap` into an #Iterable(FnRetType).
 *
 * Define the `next` function implementation for the `IterMap` struct, and use it to implement the Iterator typeclass
 * for a #IterTake(ElmntType, FnRetType), for given `ElmntType` and `FnRetType`.
 *
 * The defined function takes in a value of `IterMap(ElmntType, FnRetType)*` and wraps it in an `Iterable(FnRetType)`.
 *
 * # Example
 *
 * @code
 * DefineIterMap(int);
 *
 * // Implement `Iterator` for `IterMap(int, int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrmp(IterMap(int, int)* x)`
 * define_itertake_func(int, wrap_intitrmp)
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
    static Maybe(FnRetType) CONCAT(IterMap(ElmntType, FnRetType), _nxt)(IterMap(ElmntType, FnRetType) * self)          \
    {                                                                                                                  \
        Iterable(ElmntType) const srcit = self->src;                                                                   \
        Maybe(ElmntType) res            = srcit.tc->next(srcit.self);                                                  \
        if (is_nothing(res)) {                                                                                         \
            return Nothing(FnRetType);                                                                                 \
        }                                                                                                              \
        return Just(self->mapfn(from_just_(res)), FnRetType);                                                          \
    }                                                                                                                  \
    impl_iterator(IterMap(ElmntType, FnRetType)*, FnRetType, Name, CONCAT(IterMap(ElmntType, FnRetType), _nxt))

#endif /* !IT_MAP_H */
