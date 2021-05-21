/**
 * @file
 * @brief Macros for implementing the `filter_map` abstraction.
 *
 * https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.filter_map
 * An IterFiltMap struct is a struct that stores a filter-mapping function within it, as well as the source iterable.
 * The filter-mapping function is just a function that returns a Maybe(T). A `Just` value indicates that the raw
 * value should be kept, a `Nothing` value indicates it should be filtered out.
 */

#ifndef LIB_ITPLUS_FILTMAP_H
#define LIB_ITPLUS_FILTMAP_H

#include "itplus_iterator.h"
#include "itplus_macro_utils.h"
#include "itplus_maybe.h"

/**
 * @def IterFiltMap(ElmntType, FnRetType)
 * @brief Convenience macro to get the type of the IterFiltMap struct with given element type and function raw return
 * type.
 *
 * # Example
 *
 * @code
 * DefineIterFiltMap(int, int);
 * IterFiltMap(int, int) i; // Declares a variable of type IterFiltMap(int, int)
 * @endcode
 *
 * @param ElmntType The type of value the `Iterable` wrapped in this `IterTake` will yield. Must be the same type name
 * passed to #DefineIterFiltMap(ElmntType, FnRetType).
 * @param FnRetType The type of value the function contained within an `IterFiltMap` struct will return. Must be the
 * same type name passed to #DefineIterFiltMap(ElmntType, FnRetType).
 *
 * @note If `ElmntType` (or `FnRetType`) is a pointer, it needs to be typedef-ed into a type that does not contain the
 * `*`. Only alphanumerics.
 */
#define IterFiltMap(ElmntType, FnRetType) CONCAT(CONCAT(IterFiltMap, ElmntType), FnRetType)

/**
 * @def DefineIterFiltMap(ElmntType, FnRetType)
 * @brief Define an IterFiltMap struct that maps a function of type `FnRetType (*)(ElmntType)` over an
 * `Iterable(ElmntType)`.
 *
 * # Example
 *
 * @code
 * DefineIterFiltMap(int, int); // Defines an IterFiltMap(int, int) struct
 * @endcode
 *
 * @param ElmntType The type of value the `Iterable` wrapped in this `IterFiltMap` will yield.
 * @param FnRetType The **raw** type of value the function contained within an `IterFiltMap` struct will return. A
 * #Maybe(T) for given `FnRetType` must exist.
 *
 * @note If `ElmntType` (or `FnRetType`) is a pointer, it needs to be typedef-ed into a type that does not contain the
 * `*`. Only alphanumerics.
 * @note An #Iterator(T) for given `ElmntType`, and `FnRetType` **must** also exist.
 * @note A filter-map function returns a #Maybe(T), where `T` is `FnRetType`, also referred to as the "raw" return type.
 */
#define DefineIterFiltMap(ElmntType, FnRetType)                                                                        \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        Maybe(FnRetType) (*f)(ElmntType x);                                                                            \
        Iterable(ElmntType) src;                                                                                       \
    } IterFiltMap(ElmntType, FnRetType)

/**
 * @def define_iterfiltmap_func(ElmntType, FnRetType, Name)
 * @brief Define a function to turn an #IterFiltMap(ElmntType, FnRetType) into an #Iterable(FnRetType).
 *
 * Define the `next` function implementation for the #IterFiltMap(ElmntType, FnRetType) struct, and use it to implement
 * the Iterator typeclass, for given `ElmntType` and `FnRetType`.
 *
 * The defined function takes in a value of type `IterFiltMap(ElmntType, FnRetType)*` and wraps it in an
 * `Iterable(FnRetType)`.
 *
 * # Example
 *
 * @code
 * DefineIterFiltMap(int);
 *
 * // Implement `Iterator` for `IterFiltMap(int, int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrfiltmp(IterFiltMap(int, int)* x)`
 * define_itertake_func(int, wrap_intitrfiltmp)
 * @endcode
 *
 * @param ElmntType The type of value the `Iterable` wrapped in this `IterFiltMap` will yield.
 * @param FnRetType The **raw** type of value the function contained within an `IterFiltMap` struct will return.
 * @param Name Name to define the function as.
 *
 * @note If `ElmntType` (or `FnRetType`) is a pointer, it needs to be typedef-ed into a type that does not contain the
 * `*`. Only alphanumerics..
 * @note An #IterFiltMap(ElmntType, FnRetType) for the given `ElmntType` and `FnRetType` **must** exist.
 * @note A filter-map function returns a #Maybe(T), where `T` is `FnRetType`, also referred to as the "raw" return type.
 * @note This should not be delimited by a semicolon.
 */
#define define_iterfiltmap_func(ElmntType, FnRetType, Name)                                                            \
    static Maybe(FnRetType) CONCAT(IterFiltMap(ElmntType, FnRetType), _nxt)(IterFiltMap(ElmntType, FnRetType) * self)  \
    {                                                                                                                  \
        Iterable(ElmntType) const srcit = self->src;                                                                   \
        while (1) {                                                                                                    \
            Maybe(ElmntType) res = srcit.tc->next(srcit.self);                                                         \
            if (is_nothing(res)) {                                                                                     \
                return Nothing(FnRetType);                                                                             \
            }                                                                                                          \
            Maybe(FnRetType) mapped = self->f(from_just_(res));                                                        \
            if (is_just(mapped)) {                                                                                     \
                return mapped;                                                                                         \
            }                                                                                                          \
        }                                                                                                              \
    }                                                                                                                  \
    impl_iterator(IterFiltMap(ElmntType, FnRetType)*, FnRetType, Name, CONCAT(IterFiltMap(ElmntType, FnRetType), _nxt))

#endif /* !LIB_ITPLUS_FILTMAP_H */
