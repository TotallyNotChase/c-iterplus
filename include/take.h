#ifndef IT_TAKE_H
#define IT_TAKE_H

#include "iterator.h"
#include "macro_utils.h"
#include "maybe.h"

/*
Utilities to define an IterTake type for a specific element type and its corresponding iterator impl.

An IterTake struct is a struct that keeps track of how many elements have been extracted out of an iterator
as well as a limit to stop at - the iterator impl for this struct will keep consuming from an iterator until
it hits that limit. (or if the source iterable has been exhausted)

This allows to implement the `take_from` macro - which takes in the source iterable, and the number of elements
to extract from it, and returns an iterable with an iterable of those elements.

This is identical to the `take` function iterator typeclasses usually have.
*/

#define IterTake(ElmntType) IterTake##ElmntType

#define DefineIterTake(ElmntType)                                                                                      \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        size_t const limit;                                                                                            \
        Iterable(ElmntType) const src;                                                                                 \
    } IterTake(ElmntType)

/*
Define the iterator implementation function for an IterTake struct

The function is named `prep_itertake_of(ElmntType)`
*/
#define define_itertake_func(ElmntType, Name)                                                                          \
    static Maybe(ElmntType) CONCAT(IterTake(ElmntType), _nxt)(IterTake(ElmntType) * self)                              \
    {                                                                                                                  \
        if (self->i < self->limit) {                                                                                   \
            ++(self->i);                                                                                               \
            Iterable(ElmntType) srcit = self->src;                                                                     \
            return srcit.tc->next(srcit.self);                                                                         \
        }                                                                                                              \
        return Nothing(ElmntType);                                                                                     \
    }                                                                                                                  \
    impl_iterator(IterTake(ElmntType)*, ElmntType, Name, CONCAT(IterTake(ElmntType), _nxt))

#endif /* !IT_TAKE_H */
