#ifndef LIB_ITPLUS_FOREACH_UTILS_H
#define LIB_ITPLUS_FOREACH_UTILS_H

#include "macro_utils.h"

#define UNIQVAR(x) CONCAT(CONCAT(x, _4x2_), __LINE__) /* "Unique" variable name */

/* Iterate through given `it` iterable that contains elements of type `T` - store each element in `x` */
#define foreach(T, x, it)                                                                                              \
    Maybe(T) UNIQVAR(res) = (it).tc->next((it).self);                                                                  \
    for (T x          = from_just_(UNIQVAR(res)); is_just(UNIQVAR(res));                                               \
         UNIQVAR(res) = (it).tc->next((it).self), x = from_just_(UNIQVAR(res)))

#endif /* !LIB_ITPLUS_FOREACH_UTILS_H */
