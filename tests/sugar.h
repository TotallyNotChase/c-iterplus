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
#include "impls.h"

#include <stdint.h>

/*
Functions to prepare a pre-allocated itertake, and turn it into its corresponding iterator instance
*/

Iterable(uint32_t) prep_u32tk(IterTake(uint32_t) * tk, Iterable(uint32_t) x);
Iterable(NumType) prep_numtypetk(IterTake(NumType) * tk, Iterable(NumType) x);
Iterable(NumType)
    prep_u32numtypemap(IterMap(uint32_t, NumType) * tk, Iterable(uint32_t) x, NumType (*const fn)(uint32_t));
Iterable(uint32_t) prep_u32filt(IterFilt(uint32_t) * flt, Iterable(uint32_t) x, bool (*const pred)(uint32_t));
Iterable(NumType) prep_numtypefilt(IterFilt(NumType) * flt, Iterable(NumType) x, bool (*const pred)(NumType));

/*
Generic selection over iterable type

Add more iterable types here if needed
*/
#define itrble_selection(it, when_u32, when_numtype)                                                                   \
    _Generic((it), Iterable(uint32_t) : (when_u32), Iterable(NumType) : (when_numtype))

/*
Generic selection over mapping function type over uint32_t iterable

Add more function types here if needed (currently only has `u32 -> NumType`)
*/
#define u32_fn_selection(fn, when_u32_numtype)                                                                         \
    _Generic((fn), NumType(*const)(uint32_t) : (when_u32_numtype), NumType(*)(uint32_t) : (when_u32_numtype))

/* Build an iterable that consists of at most `n` elements from given `it` iterable */
#define take_from(it, n)                                                                                               \
    itrble_selection((it), prep_u32tk, prep_numtypetk)(                                                                \
        itrble_selection((it), &(IterTake(uint32_t)){.limit = (n)}, &(IterTake(NumType)){.limit = (n)}), (it))

#define map_selection(it, fn, when_u32_numtype)                                                                        \
    itrble_selection((it), u32_fn_selection(&(fn), (when_u32_numtype)), NULL)

/* Map the function `fn` of type `FnRetType (*)(ElmntType)` over `it` to make a new iterable */
#define map_over(it, fn)                                                                                               \
    map_selection((it), (fn), prep_u32numtypemap)(                                                 \
        map_selection((it), (fn), &(IterMap(uint32_t, NumType)){0}), (it), (fn))

/* Filter an iterable by given `pred` of type `bool (*)(ElmntType)` and make a new iterable */
#define filter_out(it, pred)                                                                                           \
    itrble_selection((it), prep_u32filt, prep_numtypefilt)(                                                            \
        itrble_selection((it), &(IterFilt(uint32_t)){0}, &(IterFilt(NumType)){0}), (it), (pred))

#endif /* !LIB_ITPLUS_SUGAR_H */
