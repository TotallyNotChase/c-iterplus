/**
 * @file
 * @brief Helpers for implementing all iterplus utilities at once, for given type.
 */

#ifndef LIB_ITPLUS_DEFN_H
#define LIB_ITPLUS_DEFN_H

#include "itplus_chain.h"
#include "itplus_collect.h"
#include "itplus_drop.h"
#include "itplus_dropwhile.h"
#include "itplus_elemindices.h"
#include "itplus_enumerate.h"
#include "itplus_filter.h"
#include "itplus_filtermap.h"
#include "itplus_fold.h"
#include "itplus_foreach.h"
#include "itplus_iterator.h"
#include "itplus_map.h"
#include "itplus_maybe.h"
#include "itplus_pair.h"
#include "itplus_reduce.h"
#include "itplus_take.h"
#include "itplus_takewhile.h"
#include "itplus_typeclass.h"
#include "itplus_zip.h"

#include <stddef.h>

/**
 * @def Iterplus(T)
 * @brief Define all structs needed for implementing `Iterator`, as well as iterplus utilities, for given `T`.
 *
 * # Example
 *
 * @code
 * Iterplus(int); // Defines the following:-
 * // Maybe(T)
 * // Pair(size_t, T)
 * // Pair(T, T)
 * // Maybe(Pair(T, T))
 * // Iterator(T)
 * // Iterator(Pair(size_t, T))
 * // Iterator(Pair(T, T))
 * // IterTake(T)
 * // IterDrop(T)
 * // IterMap(T, T)
 * // IterFilt(T)
 * // IterFiltMap(T, T)
 * // IterChain(T)
 * // IterTakeWhile(T)
 * // IterDropWhile(T)
 * // IterEnumr(T)
 * // IterZip(T, T)
 *
 * @endcode
 *
 * @param T The element type of the `Iterator` and iterplus utilities being defined.
 */
#define Iterplus(T)                                                                                                    \
    DefineMaybe(T) DefinePair(size_t, T);                                                                              \
    DefinePair(T, T);                                                                                                  \
    DefineMaybe(Pair(size_t, uint32_t)) DefineIteratorOf(T);                                                           \
    DefineIteratorOf(Pair(size_t, T));                                                                                 \
    DefineIteratorOf(Pair(T, T));                                                                                      \
    DefineIterTake(T);                                                                                                 \
    DefineIterDrop(T);                                                                                                 \
    DefineIterMap(T, T);                                                                                               \
    DefineIterFilt(T);                                                                                                 \
    DefineIterFiltMap(T, T);                                                                                           \
    DefineIterChain(T);                                                                                                \
    DefineIterTakeWhile(T);                                                                                            \
    DefineIterDropWhile(T);                                                                                            \
    DefineIterEnumr(T);                                                                                                \
    DefineIterZip(T, T)
/**
 * @def DeclIterplus(T, takefn, dropfn, mapfn, filterfn, reducefn, foldfn, filter_mapfn, chainfn, takewhilefn,
 * dropwhilefn, enumeratefn, zipfn, collectfn)
 * @brief Declare iterplus utility functions defined using `DefnIterplus`.
 *
 * # Example
 *
 * @code
 * DeclIterplus(int, inttake_itr, intdrop_itr, intintmap_itr, intfilt_itr, intreduce, intfold, intfiltmap_itr,
 * intchain_itr, inttkwhl_itr, intdrpwhl_itr, intenumr_itr, intzip_itr, intclct);
 * // Expands to the following:-
 * // Iterable(int) inttake_itr(IterTake(int) * x);
 * // Iterable(int) intdrop_itr(IterDrop(int) * x);
 * // Iterable(int) intintmap_itr(IterMap(int, int) * x);
 * // Iterable(int) intfilt_itr(IterFilt(int) * x);
 * // Iterable(int) intfiltmap_itr(IterFiltMap(int, int) * x);
 * // Iterable(int) intchain_itr(IterChain(int) * x);
 * // Maybe(int) intreduce(Iterable(int) it, int (*f)(int acc, int x));
 * // Iterable(int) inttkwhl_itr(IterTakeWhile(int) * x);
 * // Iterable(int) intdrpwhl_itr(IterDropWhile(int) * x);
 * // int* intclct(Iterable(int) it, size_t* len);
 * // int intfold(Iterable(int) it, int init, int (*f)(int acc, int x));
 * // Iterable(Pair(size_t, int)) intenumr_itr(IterEnumr(int) * x);
 * // Iterable(Pair(int, int)) intzip_itr(IterZip(int, int) * x);
 * @endcode
 *
 * The parameters to this should be the same as the ones passed to #DefnIterplus(T, takefn, dropfn, mapfn, filterfn,
 * reducefn, foldfn, filter_mapfn, chainfn, takewhilefn, dropwhilefn, enumeratefn, zipfn, collectfn)
 */
#define DeclIterplus(T, takefn, dropfn, mapfn, filterfn, reducefn, foldfn, filter_mapfn, chainfn, takewhilefn,         \
    dropwhilefn, enumeratefn, zipfn, collectfn)                                                                        \
    Iterable(T) takefn(IterTake(T) * x);                                                                               \
    Iterable(T) dropfn(IterDrop(T) * x);                                                                               \
    Iterable(T) mapfn(IterMap(T, T) * x);                                                                              \
    Iterable(T) filterfn(IterFilt(T) * x);                                                                             \
    Iterable(T) filter_mapfn(IterFiltMap(T, T) * x);                                                                   \
    Iterable(T) chainfn(IterChain(T) * x);                                                                             \
    Maybe(T) reducefn(Iterable(T) it, T (*f)(T acc, T x));                                                             \
    Iterable(T) takewhilefn(IterTakeWhile(T) * x);                                                                     \
    Iterable(T) dropwhilefn(IterDropWhile(T) * x);                                                                     \
    T* collectfn(Iterable(T) it, size_t* len);                                                                         \
    T foldfn(Iterable(T) it, T init, T (*f)(T acc, T x));                                                              \
    Iterable(Pair(size_t, T)) enumeratefn(IterEnumr(T) * x);                                                           \
    Iterable(Pair(T, T)) zipfn(IterZip(T, T) * x)

/**
 * @def DefnIterplus(T, takefn, dropfn, mapfn, filterfn, reducefn, foldfn, filter_mapfn, chainfn, takewhilefn,
 * dropwhilefn, enumeratefn, zipfn, collectfn)
 * @brief Define all iterplus utilities for #Iterable(T) for given `T`, with given names.
 *
 * The utilities defined are-
 * `take` for #Iterable(T). #define_itertake_func(T, Name)
 *
 * `drop` for #Iterable(T). #define_iterdrop_func(T, Name)
 *
 * `map` for #Iterable(T) -> #Iterable(T) with function type - `T (*)(T)` (`T -> T`). #define_itermap_func(ElmntType,
 * FnRetType, Name)
 *
 * `filter` for #Iterable(T). #define_iterfilt_func(T, Name)
 *
 * `reduce` for #Iterable(T). #define_iterreduce_func(T, Name)
 *
 * `fold` for #Iterable(T) -> #Iterable(T) with function type - `T (*)(T, T)` (`T -> T -> T`). define_iterfold_func(T,
 * Acc, Name)
 *
 * `filter_map` for #Iterable(T) -> #Iterable(T) with function type -
 * `Maybe(T) (*)(T)` (`T -> Maybe(T)`). #define_iterfiltmap_func(ElmntType, FnRetType, Name)
 *
 * `chain` for #Iterable(T)s. #define_iterchain_func(T, Name)
 *
 * `takewhile` for #Iterable(T). #define_itertakewhile_func(T, Name)
 *
 * `dropwhile` for #Iterable(T). #define_iterdropwhile_func(T, Name)
 *
 * `enumerate` for #Iterable(T). #define_iterenumr_func(T, Name)
 *
 * `zip` for #Iterable(T)s. #define_iterzip_func(T, U, Name)
 *
 * `collect` for #Iterable(T). #define_itercollect_func(T, Name)
 *
 * Refer to the `define_` macros for each of these utilities for usage.
 *
 * You can use #DeclIterplus(T, takefn, dropfn, mapfn, filterfn, reducefn, foldfn, filter_mapfn, chainfn, takewhilefn,
 * dropwhilefn, enumeratefn, zipfn, collectfn) with the same arguments to declare all the function signatures.
 *
 * # Example
 *
 * @code
 * DefnIterplus(int, inttake_itr, intdrop_itr, intintmap_itr, intfilt_itr, intreduce, intfold, intfiltmap_itr,
 * intchain_itr, inttkwhl_itr, intdrpwhl_itr, intenumr_itr, intzip_itr, intclct);
 * // Defines functions with the following signatures:-
 * // Iterable(int) inttake_itr(IterTake(int) * x);
 * // Iterable(int) intdrop_itr(IterDrop(int) * x);
 * // Iterable(int) intintmap_itr(IterMap(int, int) * x);
 * // Iterable(int) intfilt_itr(IterFilt(int) * x);
 * // Iterable(int) intfiltmap_itr(IterFiltMap(int, int) * x);
 * // Iterable(int) intchain_itr(IterChain(int) * x);
 * // Maybe(int) intreduce(Iterable(int) it, int (*f)(int acc, int x));
 * // Iterable(int) inttkwhl_itr(IterTakeWhile(int) * x);
 * // Iterable(int) intdrpwhl_itr(IterDropWhile(int) * x);
 * // int* intclct(Iterable(int) it, size_t* len);
 * // int intfold(Iterable(int) it, int init, int (*f)(int acc, int x));
 * // Iterable(Pair(size_t, int)) intenumr_itr(IterEnumr(int) * x);
 * // Iterable(Pair(int, int)) intzip_itr(IterZip(int, int) * x);
 * @endcode
 *
 * @param T The element type of the `Iterator` and iterplus utilities being defined.
 * @param takefn Name to define the `take` impl as.
 * @param dropfn Name to define the `drop` impl as.
 * @param mapfn Name to define the `map` (Iterable(T) -> Iterable(T)) impl as.
 * @param filterfn Name to define the `filter` impl as.
 * @param reducefn Name to define the `reduce` impl as.
 * @param foldfn Name to define the `fold` (Accumulator type `T`) impl as.
 * @param filter_mapfn Name to define the `filter_map` impl as.
 * @param chainfn Name to define the `chain` impl as.
 * @param takewhilefn Name to define the `takewhile` impl as.
 * @param dropwhilefn Name to define the `dropwhile` impl as.
 * @param enumeratefn Name to define the `enumerate` impl as.
 * @param zipfn Name to define the `zip` (To zip 2 `Iterable(T)`s) impl as.
 * @param collectfn Name to define the `collect` impl as.
 *
 * @note This should not be delimited by a semicolon.
 */
#define DefnIterplus(T, takefn, dropfn, mapfn, filterfn, reducefn, foldfn, filter_mapfn, chainfn, takewhilefn,         \
    dropwhilefn, enumeratefn, zipfn, collectfn)                                                                        \
    define_itertake_func(T, takefn) define_iterdrop_func(T, dropfn) define_itermap_func(T, T, mapfn)                   \
        define_iterfilt_func(T, filterfn) define_iterfiltmap_func(T, T, filter_mapfn)                                  \
            define_iterchain_func(T, chainfn) define_iterreduce_func(T, reducefn)                                      \
                define_itertakewhile_func(T, takewhilefn) define_iterdropwhile_func(T, dropwhilefn)                    \
                    define_itercollect_func(T, collectfn) define_iterfold_func(T, T, foldfn)                           \
                        define_iterenumr_func(T, enumeratefn) define_iterzip_func(T, T, zipfn)

#endif /* !LIB_ITPLUS_DEFN_H */
