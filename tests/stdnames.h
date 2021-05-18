/**
 * @file
 * @brief Macros to standardize implementation function names based on type.
 *
 * Naming Implementation functions (functions converting a type to an Iterable),
 * defined using `impl_iterator`, in a predictable manner based on just the type, allows you to define
 * handly little macros that can call these functions for you, with the necessary arguments, directly.
 *
 * Having handly macros isn't a requirement of this pattern of course, but the tests and examples here
 * *do* use the handy macros such as `take_from`, `map_over` and so on. We'll need to standardize the function
 * names for these macros to work.
 *
 * This file defines macros to create consistent function names based on the type an iterable yields.
 */

#ifndef LIB_ITPLUS_STDNAMES_H
#define LIB_ITPLUS_STDNAMES_H

#include "common.h"

/* Name of the function defined using `define_itertake_of` */
#define prep_itertake_of(ElmntType) CONCAT(CONCAT(prep_, IterTake(ElmntType)), _itr)

/* Name of the function defined using `define_itermap_of`*/
#define prep_itermap_of(ElmntType, FnRetType) CONCAT(CONCAT(prep_, IterMap(ElmntType, FnRetType)), _itr)

#endif /* !LIB_ITPLUS_STDNAMES_H */
