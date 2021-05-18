/**
 * @file
 * "Syntactic sugar". Convenience macros for using the iterplus utilities.
 *
 * These examples use macros to consistently name the implementation functions of the utilities. This allows for macros
 * that can take care of all the struct building and converting to Iterable directly. That's what these are.
 *
 * These abstractions do come at a cost though. Anything returned from these macros have lifetime contained within the
 * immediately local scope, since they just create and take address of a compound literal. **Do not** use their return
 * values outside of this scope.
 */

#ifndef LIB_ITPLUS_SUGAR_H
#define LIB_ITPLUS_SUGAR_H

#include "common.h"
#include "stdnames.h"

/* Build an iterable that consists of at most `n` elements from given `it` iterable */
#define take_from(it, n, T) prep_itertake_of(T)(&(IterTake(T)){.i = 0, .limit = n, .src = it})

/* Map the function `fn` of type `FnRetType (*)(ElmntType)` over `it` to make a new iterable */
#define map_over(it, fn, ElmntType, FnRetType)                                                                         \
    prep_itermap_of(ElmntType, FnRetType)(&(IterMap(ElmntType, FnRetType)){.mapfn = fn, .src = it})

#endif /* !LIB_ITPLUS_SUGAR_H */
