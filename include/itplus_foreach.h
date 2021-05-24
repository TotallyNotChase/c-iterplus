/**
 * @file
 * Definition of the `foreach` macro.
 */

#ifndef LIB_ITPLUS_FOREACH_UTILS_H
#define LIB_ITPLUS_FOREACH_UTILS_H

#include "itplus_macro_utils.h"

#define UNIQVAR(x) ITPL_CONCAT(ITPL_CONCAT(x, _4x2_), __LINE__) /* "Unique" variable name */

/**
 * @def foreach(T, x, it)
 * @brief Iterate through given iterable and store each element in `x`
 *
 * @param T Type of the elements the iterable yields.
 * @param x The variable name to store each element in. Available only inside the loop.
 * @param it The iterable to iterate over. This will be consumed.
 *
 * @note `it` must not be an unevaluated expression. Otherwise, it will be evaluated multiple times in this macro.
 * @note This macro cannot be used multiple times *in the same line*. Due to naming conflicts from an implicitly defined
 * variable.
 */
#define foreach(T, x, it)                                                                                              \
    Maybe(T) UNIQVAR(res) = (it).tc->next((it).self);                                                                  \
    for (T x          = from_just_(UNIQVAR(res)); is_just(UNIQVAR(res));                                               \
         UNIQVAR(res) = (it).tc->next((it).self), x = from_just_(UNIQVAR(res)))

#endif /* !LIB_ITPLUS_FOREACH_UTILS_H */
