/**
 * @file
 * @brief Macros for implementing the `collect` abstraction.
 *
 * https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.collect
 * Note: Rust's `collect` has a polymorphic return type. The `collect` here just turns an iterable into an array.
 */

#ifndef LIB_ITPLUS_COLLECT_H
#define LIB_ITPLUS_COLLECT_H

#include "itplus_foreach.h"
#include "itplus_iterator.h"
#include "itplus_maybe.h"

#include <stdlib.h>

#define ITPLUS_COLLECT_BUFSZ 64

/**
 * @def define_itercollect_func(T, Name)
 * @brief Define the `collect` function for an iterable.
 *
 * The defined function takes in an iterable of type `T`, and turns it into an array. Each element of said array
 * is of type `T`. Nothing is implicitly cloned. The same values from the iterable are assigned to the array.
 *
 * This defined function will consume the given iterable.
 *
 * # Example
 *
 * @code
 * // Defines a function with the signature- `int* collect_int(Iterable(int) x, size_t* len)`
 * define_itercollect_func(int, collect_int)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * size_t arrlen = 0;
 * // Collect `it` (of type `Iterable(int)`) into an array
 * int* intarr = collect_int(it, &arrlen);
 * @endcode
 *
 * @param T The type of value the `Iterable`, for which this is being implemented, yields.
 * @param Name Name to define the function as.
 *
 * @note The returned array must be freed.
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define define_itercollect_func(T, Name)                                                                               \
    T* Name(Iterable(T) it, size_t* len)                                                                               \
    {                                                                                                                  \
        size_t size = ITPLUS_COLLECT_BUFSZ;                                                                            \
        *len        = 0;                                                                                               \
        T* arr      = malloc(size * sizeof(*arr));                                                                     \
        if (arr == NULL) {                                                                                             \
            return NULL;                                                                                               \
        }                                                                                                              \
        foreach (T, x, it) {                                                                                           \
            if (*len == size) {                                                                                        \
                T* temp = realloc(arr, (size *= 2) * sizeof(*arr));                                                    \
                if (temp == NULL) {                                                                                    \
                    free(arr);                                                                                         \
                    return NULL;                                                                                       \
                }                                                                                                      \
                arr = temp;                                                                                            \
            }                                                                                                          \
            arr[(*len)++] = x;                                                                                         \
        }                                                                                                              \
        return arr;                                                                                                    \
    }

#endif /* !LIB_ITPLUS_COLLECT_H */
