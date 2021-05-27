/**
 * @file
 * @brief All your iterplus needs in a single header file.
 *
 * Repo: https://github.com/TotallyNotChase/c-iterplus
 *
 * @author TotallyNotChase
 * @copyright MIT License
 *
 * Copyright (c) 2021 TotallyNotChase
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LIB_ITPLUS_H
#define LIB_ITPLUS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define ITPL_CONCAT_(A, B) A##B
#define ITPL_CONCAT(A, B)  ITPL_CONCAT_(A, B)

/**
 * @def typeclass(funcs)
 * @brief Define a typeclass with the given functions.
 *
 * # Example
 *
 * @code
 * typedef typeclass(char* (*show)(void* self)) Show;  // Defines a typeclass and names it `Show`
 * @endcode
 *
 * @param funcs A semicolon separated list of typeclass functions.
 *
 * @note The functions usually take the `self` from the typeclass instance (and possibly more arguments).
 */
#define typeclass(funcs)                                                                                               \
    struct                                                                                                             \
    {                                                                                                                  \
        funcs;                                                                                                         \
    }

/**
 * @def typeclass_instance(Typeclass)
 * @brief Define a typeclass instance for the given typeclass.
 *
 * This just contains a `void* self` member and the typeclass itself.
 * # Example
 *
 * @code
 * typedef typeclass(char* (*show)(void* self)) Show;
 * typedef typeclass_instance(Show) Showable; // Defines the typeclass instance for `Show` typeclass
 * @endcode
 *
 * @param Typeclass The semantic type (C type) of the typeclass defined with #typeclass(funcs).
 */
#define typeclass_instance(Typeclass)                                                                                  \
    struct                                                                                                             \
    {                                                                                                                  \
        void* self;                                                                                                    \
        Typeclass const* tc;                                                                                           \
    }

/**
 * @struct MaybeTag
 * @brief The enum used for tagging the `Maybe` type.
 */
typedef enum
{
    MaybeTag_Nothing = 0, /**< `Nothing` tag - indicates absence of a value. */
    MaybeTag_Just         /**< `Just` tag - indicates presence of a value. */
} MaybeTag;

/**
 * @def Maybe(T)
 * @brief Convenience macro to get the type of the Maybe defined with a certain type.
 *
 * # Example
 *
 * @code
 * DefineMaybe(int)
 * Maybe(int) const x = {0}; // Uses the maybe type defined in the previous line
 * @endcode
 *
 * @param T The type of value the `Maybe` struct will contain. Must be the same type name passed to #DefineMaybe(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define Maybe(T) ITPL_CONCAT(Maybe_, T)

/**
 * @def DefineMaybe(T)
 * @brief Define a Maybe<T> type.
 *
 * # Example
 *
 * @code
 * DefineMaybe(int) // Defines a Maybe(int) type as well as its corresponding functions
 * @endcode
 *
 * @param T The type of value this `Maybe` will hold. Must be alphanumeric.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note This should not be delimited by a semicolon.
 */
#define DefineMaybe(T)                                                                                                 \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        MaybeTag tag;                                                                                                  \
        /* Don't access this member manually */                                                                        \
        T val;                                                                                                         \
    } Maybe(T);                                                                                                        \
    static inline T ITPL_CONCAT(T, _from_just)(Maybe(T) maybex)                                                        \
    {                                                                                                                  \
        if (is_just(maybex)) {                                                                                         \
            return maybex.val;                                                                                         \
        } else {                                                                                                       \
            fputs("Attempted to extract Just value from Nothing", stderr);                                             \
            abort();                                                                                                   \
        }                                                                                                              \
    }

/**
 * @def Just(v, T)
 * @brief Wrap a `Just` value into a #Maybe(T).
 *
 * # Example
 *
 * @code
 * DefineMaybe(int)
 * Maybe(int) const x = Just(42, int); // Initializes a Maybe(int) with the value 42
 * @endcode
 *
 * @param v The concrete value to wrap in `Just` (must be of the correct type).
 * @param T The type of value the `Maybe` will hold. Must be alphanumeric.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note The value is simply assigned to the #Maybe(T) struct. No implicit cloning is done.
 */
#define Just(v, T) ((Maybe(T)){.tag = MaybeTag_Just, .val = (v)})

/**
 * @def Nothing(T)
 * @brief Wrap a `Nothing` value into a #Maybe(T).
 *
 * # Example
 *
 * @code
 * DefineMaybe(int)
 * Maybe(int) const x = Nothing(int); // Initializes a Maybe(int) with no value
 * @endcode
 *
 * @param T The type of value the `Maybe` will hold. Must be alphanumeric.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define Nothing(T) ((Maybe(T)){0})

/**
 * @def is_nothing(x)
 * @brief Check if the given Maybe type is tagged with `Nothing`.
 *
 * @param x The #Maybe(T) struct to check against.
 */
#define is_nothing(x) ((x).tag == MaybeTag_Nothing)
/**
 * @def is_just(x)
 * @brief Check if the given Maybe type is tagged with `Just`.
 *
 * @param x The #Maybe(T) struct to check against.
 */
#define is_just(x) ((x).tag == MaybeTag_Just)

/**
 * @def from_just(x, T)
 * @brief Extract the `Just` value from given #Maybe(T).
 *
 * @param x The `Maybe` type to extract the value from.
 * @param T The type of value the `Maybe` will hold. Must be alphanumeric.
 *
 * @return `Just` value of type corresponding to the given #Maybe(T) if it's not `Nothing`.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note Aborts the program if given #Maybe(T) struct was tagged with `Nothing`.
 */
#define from_just(x, T) ITPL_CONCAT(T, _from_just(x))

/**
 * @def from_just_(x)
 * @brief "Unsafe" version of #from_just(x, T).
 *
 * @param x The `Maybe` type to extract the value from.
 *
 * @return `Just` value of type corresponding to the given `Maybe` struct.
 *
 * @note This does not check whether the `Maybe` struct actually has a value and hence
 * should only be used when the caller is sure that the Maybe contains a `Just` value. Otherwise
 * the behavior is undefined.
 */
#define from_just_(x) (x).val

/**
 * @def Iterator(T)
 * @brief Convenience macro to get the type of the Iterator (typeclass) with given element type.
 *
 * # Example
 *
 * @code
 * DefineIteratorOf(int);
 * Iterator(int) i; // Declares a variable of type Iterator(int) (a typeclass)
 * @endcode
 *
 * @param T The type of value the `Iterator` instance will yield. Must be the same type name passed to
 * #DefineIteratorOf(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define Iterator(T) ITPL_CONCAT(Iterator_, T)

/**
 * @def Iterable(T)
 * @brief Convenience macro to get the type of the Iterable (typeclass instance) with given element type.
 *
 * # Example
 *
 * @code
 * DefineIteratorOf(int);
 * Iterable(int) i; // Declares a variable of type Iterable(Int) (the typeclass instance)
 * @endcode
 *
 * @param T The type of value the `Iterable` will yield. Must be the same type name passed to #DefineIteratorOf(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define Iterable(T) ITPL_CONCAT(Iterable_, T)

/**
 * @def DefineIteratorOf(T)
 * @brief Define an Iterator typeclass and its Iterable instance for given element type.
 *
 * # Example
 *
 * @code
 * DefineIteratorOf(int); // Defines an Iterator(int) typeclass as well as its instance
 * @endcode
 *
 * @param T The type of value the `Iterator` instance will yield. Must be alphanumeric.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note A #Maybe(T) for the given `T` **must** also exist.
 */
#define DefineIteratorOf(T)                                                                                            \
    typedef typeclass(Maybe(T) (*const next)(void* self)) Iterator(T);                                                 \
    typedef typeclass_instance(Iterator(T)) Iterable(T)

/**
 * @def impl_iterator(IterType, ElmntType, Name, next_f)
 * @brief Define a function to turn given `IterType` into an #Iterable(ElmntType).
 *
 * Implement the Iterator typeclass for a type. Essentially defining a wrapper function that returns the Iterable.
 *
 * The defined function takes in a value of `IterType` and wraps it in an `Iterable` - which can be passed around to
 * generic functions working on an iterable.
 *
 * The term "generic" is used here in the context of the **input**.
 * As in, the function taking a generic iterable, does not care about what type is backing up the iterable; but,
 * does care about what element type the iterator yields.
 *
 * # Example
 *
 * @code
 * // Example of implementing an infinite iterator representing the fibonacci sequence
 *
 * #include <stdint.h>
 *
 * typedef struct fibonacci
 * {
 *     uint32_t curr;
 *     uint32_t next;
 * } Fibonacci;
 *
 * DefineMaybe(uint32_t)
 * DefineIteratorOf(uint32_t);
 *
 * static Maybe(uint32_t) fibnxt(Fibonacci* self)
 * {
 *     uint32_t new_nxt = self->curr + self->next;
 *     self->curr       = self->next;
 *     self->next       = new_nxt;
 *     return Just(new_nxt, uint32_t);
 * }
 *
 * // Define a function named `prep_fib_itr`, which takes in a `Fibonacci*` and returns an `Iterable(int)`
 * // The returned iterable is an infinite fibonacci sequence
 * impl_iterator(Fibonacci*, uint32_t, prep_fib_itr, fibnxt)
 * @endcode
 *
 * @param IterType The semantic type (C type) this impl is for, must be a pointer type.
 * @param ElmntType The type of value the `Iterator` instance will yield.
 * @param Name Name to define the function as.
 * @param next_f Function pointer that serves as the `next` implementation for `IterType`. This function must have
 * the signature of `Maybe(ElmntType) (*)(IterType self)` - i.e, should take IterType and return a value of the
 * corresponding element type wrapped in a `Maybe` - `Nothing` value indicates end of iteration.
 *
 * @note If `ElmntType` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note A #Maybe(T) for the given `ElmntType` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define impl_iterator(IterType, ElmntType, Name, next_f)                                                               \
    Iterable(ElmntType) Name(IterType x)                                                                               \
    {                                                                                                                  \
        Maybe(ElmntType) (*const next_)(IterType self) = (next_f);                                                     \
        (void)next_;                                                                                                   \
        static Iterator(ElmntType) const tc = {.next = (Maybe(ElmntType)(*const)(void*))(next_f)};                     \
        return (Iterable(ElmntType)){.tc = &tc, .self = x};                                                            \
    }

/**
 * @def Pair(T, U)
 * @brief Convenience macro to get the type of the Pair defined with certain types.
 *
 * # Example
 *
 * @code
 * DefinePair(int, char);
 * Pair(int, char) const x = {0}; // Uses the pair type defined in the previous line
 * @endcode
 *
 * @param T The type the first value in this `Pair` struct. Must be the same type name passed to #DefinePair(T, U)
 * @param T The type the second value in this `Pair` struct. Must be the same type name passed to #DefinePair(T, U).
 *
 * @note If `T`, or `U`, is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 */
#define Pair(T, U) ITPL_CONCAT(ITPL_CONCAT(Pair_, T), ITPL_CONCAT(_, U))

/**
 * @def DefinePair(T, U)
 * @brief Define a Pair<T, U> type.
 *
 * # Example
 *
 * @code
 * DefinePair(int, char); // Defines a Pair(int, char) type
 * @endcode
 *
 * @param T The type the first value in this `Pair` struct.
 * @param T The type the second value in this `Pair` struct.
 *
 * @note If `T`, or `U`, is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 */
#define DefinePair(T, U)                                                                                               \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        T a;                                                                                                           \
        U b;                                                                                                           \
    } Pair(T, U)

/**
 * @def PairOf(x, y, T, U)
 * @brief Wrap 2 values into a #Pair(T, U).
 *
 * # Example
 *
 * @code
 * DefinePair(int, char);
 * Pair(int, char) const x = PairOf(42, 'c', int, char); // Initializes a Pair(int, char) with the value 42 and 'c'
 * @endcode
 *
 * @param x The value to put as the first member of the pair struct.
 * @param y The value to put as the second member of the pair struct.
 * @param T The type of the first value.
 * @param U The type of the second value.
 *
 * @note If `T`, or `U`, is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note A #Pair(T, U) for given `T` and `U` must exist.
 * @note The values are simply assigned to the #Pair(T, U) struct. No implicit cloning is done.
 */
#define PairOf(x, y, T, U) ((Pair(T, U)){.a = (x), .b = (y)})

/**
 * @def fst(pair)
 * @brief Get the first value from given pair.
 */
#define fst(pair) (pair).a

/**
 * @def snd(pair)
 * @brief Get the second value from given pair.
 */
#define snd(pair) (pair).b

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

/**
 * @def IterChain(T)
 * @brief Convenience macro to get the type of the IterChain struct with given element type.
 *
 * # Example
 *
 * @code
 * DefineIterChain(int);
 * IterChain(int) i; // Declares a variable of type IterChain(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterChain` will yield. Must be the same type name passed
 * to #DefineIterChain(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define IterChain(T) ITPL_CONCAT(IterChain_, T)

/**
 * @def DefineIterChain(T)
 * @brief Define an IterChain struct that works on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterChain(int); // Defines an IterChain(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterChain` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterChain(T)                                                                                             \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        Iterable(T) curr;                                                                                              \
        Iterable(T) nxt;                                                                                               \
    } IterChain(T)

/**
 * @def define_iterchain_func(T, Name)
 * @brief Define a function to turn an #IterChain(T) into an #Iterable(T).
 *
 * Define the `next` function implementation for the #IterChain(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterChain(T)*` and wraps it in an `Iterable(T)`.
 *
 * # Example
 *
 * @code
 * DefineIterChain(int);
 *
 * // Implement `Iterator` for `IterChain(int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrchn(IterChain(int)* x)`
 * define_iterchain_func(int, wrap_intitrchn)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * // Chain together fst_it and snd_it (both of type `Iterable(int)`)
 * Iterable(int) it = wrap_intitrchn(&(IterChain(int)){ .curr = fst_it, .nxt = snd_it });
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterChain` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterChain(T) for the given `T` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_iterchain_func(T, Name)                                                                                 \
    static Maybe(T) ITPL_CONCAT(IterChain(T), _nxt)(IterChain(T) * self)                                               \
    {                                                                                                                  \
        Iterable(T) srcit = self->curr;                                                                                \
        Maybe(T) res      = srcit.tc->next(srcit.self);                                                                \
        if (is_just(res)) {                                                                                            \
            return res;                                                                                                \
        }                                                                                                              \
        self->curr           = self->nxt;                                                                              \
        Iterable(T) re_srcit = self->curr;                                                                             \
        return re_srcit.tc->next(re_srcit.self);                                                                       \
    }                                                                                                                  \
    impl_iterator(IterChain(T)*, T, Name, ITPL_CONCAT(IterChain(T), _nxt))

#ifndef ITPLUS_COLLECT_BUFSZ
#define ITPLUS_COLLECT_BUFSZ 64
#endif /* !ITPLUS_COLLECT_BUFSZ */

/**
 * @def define_itercollect_func(T, Name)
 * @brief Define the `collect` function for an iterable.
 *
 * The defined function takes in an iterable of type `T`, and turns it into an array. Each element of said array
 * is of type `T`.
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

/**
 * @def IterDrop(T)
 * @brief Convenience macro to get the type of the IterDrop struct with given element type.
 *
 * # Example
 *
 * @code
 * DefineIterDrop(int);
 * IterDrop(int) i; // Declares a variable of type IterDrop(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterDrop` will yield. Must be the same type name passed to
 * #DefineIterDrop(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define IterDrop(T) ITPL_CONCAT(IterDrop_, T)

/**
 * @def DefineIterDrop(T)
 * @brief Define an IterDrop struct that works on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterDrop(int); // Defines an IterDrop(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterDrop` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterDrop(T)                                                                                              \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        size_t limit;                                                                                                  \
        Iterable(T) src;                                                                                               \
    } IterDrop(T)

/**
 * @def define_iterdrop_func(T, Name)
 * @brief Define a function to turn an #IterDrop(T) into an #Iterable(T).
 *
 * Define the `next` function implementation for the #IterDrop(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterDrop(T)*` and wraps it in an `Iterable(T)`.
 *
 * # Example
 *
 * @code
 * DefineIterDrop(int);
 *
 * // Implement `Iterator` for `IterDrop(int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrdrp(IterDrop(int)* x)`
 * define_iterdrop_func(int, wrap_intitrdrp)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * // Drop (skip) the first 10 elements from `it` (of type `Iterable(int)`) and create a new iterable
 * Iterable(int) after10 = wrap_intitrdrp(&(IterDrop(int)){ .limit = 10, .src = it });
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterDrop` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterDrop(T) for the given `T` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_iterdrop_func(T, Name)                                                                                  \
    static Maybe(T) ITPL_CONCAT(IterDrop(T), _nxt)(IterDrop(T) * self)                                                 \
    {                                                                                                                  \
        Iterable(T) srcit = self->src;                                                                                 \
        foreach (T, x, srcit) {                                                                                        \
            if (self->i >= self->limit) {                                                                              \
                return Just(x, T);                                                                                     \
            }                                                                                                          \
            ++(self->i);                                                                                               \
        }                                                                                                              \
        return Nothing(T);                                                                                             \
    }                                                                                                                  \
    impl_iterator(IterDrop(T)*, T, Name, ITPL_CONCAT(IterDrop(T), _nxt))

/**
 * @def IterDropWhile(T)
 * @brief Convenience macro to get the type of the IterDropWhile struct with given element type.
 *
 * # Example
 *
 * @code
 * DefineIterDropWhile(int);
 * IterDropWhile(int) i; // Declares a variable of type IterDropWhile(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterDropWhile` will yield. Must be the same type name
 * passed to #DefineIterDropWhile(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define IterDropWhile(T) ITPL_CONCAT(IterDropWhile_, T)

/**
 * @def DefineIterDropWhile(T)
 * @brief Define an IterDropWhile struct that works on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterDropWhile(int); // Defines an IterDropWhile(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterDropWhile` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterDropWhile(T)                                                                                         \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        bool (*pred)(T x);                                                                                             \
        bool done;                                                                                                     \
        Iterable(T) src;                                                                                               \
    } IterDropWhile(T)

/**
 * @def define_iterdropwhile_func(T, Name)
 * @brief Define a function to turn an #IterDropWhile(T) into an #Iterable(T).
 *
 * Define the `next` function implementation for the #IterDropWhile(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterDropWhile(T)*` and wraps it in an `Iterable(T)`.
 *
 * # Example
 *
 * @code
 * DefineIterDropWhile(int);
 *
 * // Implement `Iterator` for `IterDropWhile(int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrdrpwhl(IterDropWhile(int)* x)`
 * define_iterdropwhile_func(int, wrap_intitrdrpwhl)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * static bool is_even(int x) { return x % 2 == 0; }
 * @endcode
 *
 * @code
 * // Drop (skip) elements from `it` (of type `Iterable(int)`), while they satisfy `is_even`, and create a new iterable
 * Iterable(int) after_evens = wrap_intitrdrpwhl(&(IterDropWhile(int)){ .pred = is_even, .src = it });
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterDropWhile` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterDropWhile(T) for the given `T` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_iterdropwhile_func(T, Name)                                                                             \
    static Maybe(T) ITPL_CONCAT(IterDropWhile(T), _nxt)(IterDropWhile(T) * self)                                       \
    {                                                                                                                  \
        if (self->done) {                                                                                              \
            return Nothing(T);                                                                                         \
        }                                                                                                              \
        Iterable(T) srcit = self->src;                                                                                 \
        foreach (T, x, srcit) {                                                                                        \
            if (!self->pred(x)) {                                                                                      \
                self->done = true;                                                                                     \
                return Just(x, T);                                                                                     \
            }                                                                                                          \
        }                                                                                                              \
        return Nothing(T);                                                                                             \
    }                                                                                                                  \
    impl_iterator(IterDropWhile(T)*, T, Name, ITPL_CONCAT(IterDropWhile(T), _nxt))

/**
 * @def IterEnumr(T, U)
 * @brief Convenience macro to get the type of the IterEnumr struct with given element types.
 *
 * # Example
 *
 * @code
 * DefineIterEnumr(int);
 * IterEnumr(int) i; // Declares a variable of type IterEnumr(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterEnumr` will yield. Must be the same type name
 * passed to #DefineIterEnumr(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 */
#define IterEnumr(T) ITPL_CONCAT(IterEnumr_, T)

/**
 * @def DefineIterEnumr(T)
 * @brief Define an IterEnumr struct that works with on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterEnumr(int); // Defines an IterEnumr(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterEnumr` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterEnumr(T)                                                                                             \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        Iterable(T) src;                                                                                               \
    } IterEnumr(T)

/**
 * @def define_iterenumr_func(T, Name)
 * @brief Define a function to turn an #IterEnumr(T) into an #Iterable(T) where `T = Pair(size_t, T)`.
 *
 * Define the `next` function implementation for the #IterEnumr(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterEnumr(T)*` and wraps it in an `Iterable(Pair(size_t, T))`.
 *
 * # Example
 *
 * @code
 * DefineIterEnumr(int);
 *
 * // Implement `Iterator` for `IterEnumr(int)`
 * // The defined function has the signature- `Iterable(Pair(size_t, int)) wrap_intenumr(IterEnumr(int)* x)`
 * define_iterenumr_func(int, wrap_intenumr)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * // Enumerate `it` (of type `Iterable(int)`) to create a new iterable
 * Iterable(Pair(size_t, int)) enumerated = wrap_intenumr(&(IterEnumr(int)){ .src = it });
 * @endcode
 *
 * @param T The type of value the first `Iterable` wrapped in this `IterEnumr` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterEnumr(T) for the given `T` **must** exist.
 * @note An #Iterator(T), with `T = Pair(size_t, T)`, for the given `T`.
 * @note This should not be delimited by a semicolon.
 */
#define define_iterenumr_func(T, Name)                                                                                 \
    static Maybe(Pair(size_t, T)) ITPL_CONCAT(IterEnumr(T), _nxt)(IterEnumr(T) * self)                                 \
    {                                                                                                                  \
        Iterable(T) srcit = self->src;                                                                                 \
        Maybe(T) res      = srcit.tc->next(srcit.self);                                                                \
        return is_just(res) ? Just(PairOf(self->i++, from_just_(res), size_t, T), Pair(size_t, T))                     \
                            : Nothing(Pair(size_t, T));                                                                \
    }                                                                                                                  \
    impl_iterator(IterEnumr(T)*, Pair(size_t, T), Name, ITPL_CONCAT(IterEnumr(T), _nxt))

/**
 * @def IterFilt(T)
 * @brief Convenience macro to get the type of the IterFilt struct with given element type.
 *
 * # Example
 *
 * @code
 * DefineIterFilt(int);
 * IterFilt(int) i; // Declares a variable of type IterFilt(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterFilt` will yield. Must be the same type name passed to
 * #DefineIterFilt(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define IterFilt(T) ITPL_CONCAT(IterFilt_, T)

/**
 * @def DefineIterFilt(T)
 * @brief Define an IterFilt struct that works on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterFilt(int); // Defines an IterFilt(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterFilt` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterFilt(T)                                                                                              \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        bool (*pred)(T x);                                                                                             \
        Iterable(T) src;                                                                                               \
    } IterFilt(T)

/**
 * @def define_iterfilt_func(T, Name)
 * @brief Define a function to turn an #IterFilt(T) into an #Iterable(T).
 *
 * Define the `next` function implementation for the #IterFilt(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterFilt(T)*` and wraps it in an `Iterable(T)`.
 *
 * # Example
 *
 * @code
 * DefineIterFilt(int);
 *
 * // Implement `Iterator` for `IterFilt(int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrfilt(IterFilt(int)* x)`
 * define_iterfilt_func(int, wrap_intitrfilt)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * static bool is_even(int x) { return x % 2 == 0; }
 * @endcode
 *
 * @code
 * // Filter `it` (of type `Iterable(int)`) by `is_even`
 * Iterable(int) evens = wrap_intitrfilt(&(IterFilt(int)){ .pred = is_even, .src = it });
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterFilt` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterFilt(T) for the given `T` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_iterfilt_func(T, Name)                                                                                  \
    static Maybe(T) ITPL_CONCAT(IterFilt(T), _nxt)(IterFilt(T) * self)                                                 \
    {                                                                                                                  \
        Iterable(T) const srcit = self->src;                                                                           \
        while (1) {                                                                                                    \
            Maybe(T) res = srcit.tc->next(srcit.self);                                                                 \
            if (is_nothing(res) || self->pred(from_just_(res))) {                                                      \
                return res;                                                                                            \
            }                                                                                                          \
        }                                                                                                              \
    }                                                                                                                  \
    impl_iterator(IterFilt(T)*, T, Name, ITPL_CONCAT(IterFilt(T), _nxt))

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
#define IterFiltMap(ElmntType, FnRetType) ITPL_CONCAT(ITPL_CONCAT(IterFiltMap_, ElmntType), ITPL_CONCAT(_, FnRetType))

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
 * define_iterfiltmap_func(int, int, wrap_intitrfiltmp)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * // Filter evens and then increment the even numbers
 * static Maybe(int) is_even_incr(int x) { return x % 2 == 0 ? Just(x + 1, int) : Nothing(int); }
 * @endcode
 *
 * @code
 * // Filter Map `it` (of type `Iterable(int)`) by `is_even_incr`
 * Iterable(int) incr_evens = wrap_intitrfiltmp(&(IterFiltMap(int, int)){ .f = is_even_incr, .src = it });
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
    static Maybe(FnRetType)                                                                                            \
        ITPL_CONCAT(IterFiltMap(ElmntType, FnRetType), _nxt)(IterFiltMap(ElmntType, FnRetType) * self)                 \
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
    impl_iterator(                                                                                                     \
        IterFiltMap(ElmntType, FnRetType)*, FnRetType, Name, ITPL_CONCAT(IterFiltMap(ElmntType, FnRetType), _nxt))

/**
 * @def define_iterfold_func(T, Acc, Name)
 * @brief Define the `fold` function for an iterable and an accumulator type.
 *
 * The defined fold function takes in an iterable of type `T`, a function of type `Acc (*const f)(Acc acc, T x)`, and
 * a starting value of type `Acc`, and folds the iterable to a singular value of type `Acc`, by repeatedly applying `f`
 * onto it.
 *
 * This defined function will consume the given iterable.
 *
 * # Example
 *
 * @code
 * typedef struct boxint { int x; } BoxInt;
 *
 * // The defined function has the signature:-
 * // `BoxInt int_boxint_fold(Iterable(int), BoxInt init, BoxInt (*f)(BoxInt acc, int x))`
 * define_iterfold_func(int, boxint, int_boxint_fold)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * // Add 2 ints within 2 BoxInts
 * static BoxInt boxed_add(BoxInt a, BoxInt b) { return (BoxInt){ .x = a.x + b.x }; }
 * @endcode
 *
 * @code
 * // Fold `it` with `boxed_add`
 * BoxInt boxed_sum = int_boxint_fold(it, (BoxInt){0}, boxed_add);
 * @endcode
 *
 * @param T The type of value the `Iterable`, for which this is being implemented, yields.
 * @param Acc The accumulator type the fold function being defined should work on.
 * @param Name Name to define the function as.
 *
 * @note If `T` (or `Acc`) is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 * @note This should not be delimited with a semicolon.
 */
#define define_iterfold_func(T, Acc, Name)                                                                             \
    Acc Name(Iterable(T) it, Acc init, Acc (*f)(Acc acc, T x))                                                         \
    {                                                                                                                  \
        Acc acc = init;                                                                                                \
        foreach (T, x, it) {                                                                                           \
            acc = f(acc, x);                                                                                           \
        }                                                                                                              \
        return acc;                                                                                                    \
    }

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
#define IterMap(ElmntType, FnRetType) ITPL_CONCAT(ITPL_CONCAT(IterMap_, ElmntType), ITPL_CONCAT(_, FnRetType))

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
        FnRetType (*f)(ElmntType x);                                                                                   \
        Iterable(ElmntType) src;                                                                                       \
    } IterMap(ElmntType, FnRetType)

/**
 * @def define_itermap_func(ElmntType, FnRetType, Name)
 * @brief Define a function to turn an #IterMap(ElmntType, FnRetType) into an #Iterable(FnRetType).
 *
 * Define the `next` function implementation for the #IterMap(ElmntType, FnRetType) struct, and use it to implement the
 * Iterator typeclass, for given `ElmntType` and `FnRetType`.
 *
 * The defined function takes in a value of type `IterMap(ElmntType, FnRetType)*` and wraps it in an
 * `Iterable(FnRetType)`.
 *
 * # Example
 *
 * @code
 * DefineIterMap(int);
 *
 * // Implement `Iterator` for `IterMap(int, int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrmp(IterMap(int, int)* x)`
 * define_itermap_func(int, int, wrap_intitrmp)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * // Decrement an int
 * static int decr(int a) { return a - 1; }
 * @endcode
 *
 * @code
 * // Map `decr` over `it` (of type `Iterable(int)`)
 * Iterable(int) decr_it = wrap_intitrmp(&(IterMap(int, int)){ .f = decr, .src = it });
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
    static Maybe(FnRetType) ITPL_CONCAT(IterMap(ElmntType, FnRetType), _nxt)(IterMap(ElmntType, FnRetType) * self)     \
    {                                                                                                                  \
        Iterable(ElmntType) const srcit = self->src;                                                                   \
        Maybe(ElmntType) res            = srcit.tc->next(srcit.self);                                                  \
        if (is_nothing(res)) {                                                                                         \
            return Nothing(FnRetType);                                                                                 \
        }                                                                                                              \
        return Just(self->f(from_just_(res)), FnRetType);                                                              \
    }                                                                                                                  \
    impl_iterator(IterMap(ElmntType, FnRetType)*, FnRetType, Name, ITPL_CONCAT(IterMap(ElmntType, FnRetType), _nxt))

/**
 * @def define_iterreduce_func(T, Name)
 * @brief Define the `reduce` function for an iterable.
 *
 * The defined reduce function takes in an iterable of type `T`, and a function of type `T (*const f)(T acc, T x)`, and
 * reduces the iterable to a singular value of type `T`, by repeatedly applying `f` onto it.
 *
 * If the given iterable was empty, `Nothing` is returned. Otherwise, a `Just` value is returned.
 *
 * This defined function will consume the given iterable.
 *
 * # Example
 *
 * @code
 * // The defined function has the signature:-
 * // `Maybe(int) int_reduce(Iterable(int), int (*f)(int acc, int x))`
 * define_iterreduce_func(int, int_reduce)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * // Add 2 ints
 * static int add(int a, int b) { return a + b; }
 * @endcode
 *
 * @code
 * // Reduce `it` (of type `Iterable(int)`) with `add`
 * Maybe(int) maybe_sum = int_reduce(it, add);
 * @endcode
 *
 * @param T The type of value the `Iterable`, for which this is being implemented, yields.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 * @note This should not be delimited with a semicolon.
 */
#define define_iterreduce_func(T, Name)                                                                                \
    Maybe(T) Name(Iterable(T) it, T (*f)(T acc, T x))                                                                  \
    {                                                                                                                  \
        Maybe(T) res = it.tc->next(it.self);                                                                           \
        if (is_nothing(res)) {                                                                                         \
            return Nothing(T);                                                                                         \
        }                                                                                                              \
        T acc = from_just_(res);                                                                                       \
        foreach (T, x, it) {                                                                                           \
            acc = f(acc, x);                                                                                           \
        }                                                                                                              \
        return Just(acc, T);                                                                                           \
    }

/**
 * @def IterTake(T)
 * @brief Convenience macro to get the type of the IterTake struct with given element type.
 *
 * # Example
 *
 * @code
 * DefineIterTake(int);
 * IterTake(int) i; // Declares a variable of type IterTake(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterTake` will yield. Must be the same type name passed to
 * #DefineIterTake(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define IterTake(T) ITPL_CONCAT(IterTake, T)

/**
 * @def DefineIterTake(T)
 * @brief Define an IterTake struct that works on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterTake(int); // Defines an IterTake(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterTake` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterTake(T)                                                                                              \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        size_t limit;                                                                                                  \
        Iterable(T) src;                                                                                               \
    } IterTake(T)

/**
 * @def define_itertake_func(T, Name)
 * @brief Define a function to turn an #IterTake(T) into an #Iterable(T).
 *
 * Define the `next` function implementation for the #IterTake(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterTake(T)*` and wraps it in an `Iterable(T)`.
 *
 * # Example
 *
 * @code
 * DefineIterTake(int);
 *
 * // Implement `Iterator` for `IterTake(int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrtk(IterTake(int)* x)`
 * define_itertake_func(int, wrap_intitrtk)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * // Take the first 10 elements from `it` (of type `Iterable(int)`)
 * Iterable(int) it10 = wrap_intitrtk(&(IterTake(int)){ .limit = 10, .src = it });
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterTake` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterTake(T) for the given `T` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_itertake_func(T, Name)                                                                                  \
    static Maybe(T) ITPL_CONCAT(IterTake(T), _nxt)(IterTake(T) * self)                                                 \
    {                                                                                                                  \
        if (self->i < self->limit) {                                                                                   \
            ++(self->i);                                                                                               \
            Iterable(T) srcit = self->src;                                                                             \
            return srcit.tc->next(srcit.self);                                                                         \
        }                                                                                                              \
        return Nothing(T);                                                                                             \
    }                                                                                                                  \
    impl_iterator(IterTake(T)*, T, Name, ITPL_CONCAT(IterTake(T), _nxt))

/**
 * @def IterTakeWhile(T)
 * @brief Convenience macro to get the type of the IterTakeWhile struct with given element type.
 *
 * # Example
 *
 * @code
 * DefineIterTakeWhile(int);
 * IterTakeWhile(int) i; // Declares a variable of type IterTakeWhile(int)
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterTakeWhile` will yield. Must be the same type name
 * passed to #DefineIterTakeWhile(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define IterTakeWhile(T) ITPL_CONCAT(IterTakeWhile_, T)

/**
 * @def DefineIterTakeWhile(T)
 * @brief Define an IterTakeWhile struct that works on `Iterable(T)`s.
 *
 * # Example
 *
 * @code
 * DefineIterTakeWhile(int); // Defines an IterTakeWhile(int) struct
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterTakeWhile` will yield.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note An #Iterator(T) for the given `T` **must** also exist.
 */
#define DefineIterTakeWhile(T)                                                                                         \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        bool (*pred)(T x);                                                                                             \
        bool done;                                                                                                     \
        Iterable(T) src;                                                                                               \
    } IterTakeWhile(T)

/**
 * @def define_itertakewhile_func(T, Name)
 * @brief Define a function to turn an #IterTakeWhile(T) into an #Iterable(T).
 *
 * Define the `next` function implementation for the #IterTakeWhile(T) struct, and use it to implement the Iterator
 * typeclass, for given `T`.
 *
 * The defined function takes in a value of type `IterTakeWhile(T)*` and wraps it in an `Iterable(T)`.
 *
 * # Example
 *
 * @code
 * DefineIterTakeWhile(int);
 *
 * // Implement `Iterator` for `IterTakeWhile(int)`
 * // The defined function has the signature- `Iterable(int) wrap_intitrtkwhl(IterTakeWhile(int)* x)`
 * define_itertakewhile_func(int, wrap_intitrtkwhl)
 * @endcode
 *
 * Usage of the defined function-
 *
 * * @code
 * static bool is_even(int x) { return x % 2 == 0; }
 * @endcode
 *
 * @code
 * // Take elements from `it` (of type `Iterable(int)`), while they satisfy `is_even`, and create a new iterable
 * Iterable(int) first_evens = wrap_intitrtkwhl(&(IterTakeWhile(int)){ .pred = is_even, .src = it });
 * @endcode
 *
 * @param T The type of value the `Iterable` wrapped in this `IterTakeWhile` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterTakeWhile(T) for the given `T` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_itertakewhile_func(T, Name)                                                                             \
    static Maybe(T) ITPL_CONCAT(IterTakeWhile(T), _nxt)(IterTakeWhile(T) * self)                                       \
    {                                                                                                                  \
        if (self->done) {                                                                                              \
            return Nothing(T);                                                                                         \
        }                                                                                                              \
        Iterable(T) srcit = self->src;                                                                                 \
        Maybe(T) res      = srcit.tc->next(srcit.self);                                                                \
        if (is_nothing(res) || !self->pred(from_just_(res))) {                                                         \
            self->done = true;                                                                                         \
            return Nothing(T);                                                                                         \
        }                                                                                                              \
        return res;                                                                                                    \
    }                                                                                                                  \
    impl_iterator(IterTakeWhile(T)*, T, Name, ITPL_CONCAT(IterTakeWhile(T), _nxt))

/**
 * @def IterZip(T, U)
 * @brief Convenience macro to get the type of the IterZip struct with given element types.
 *
 * # Example
 *
 * @code
 * DefineIterZip(int, char);
 * IterZip(int, char) i; // Declares a variable of type IterZip(int, char)
 * @endcode
 *
 * @param T The type of value the first `Iterable` wrapped in this `IterZip` will yield. Must be the same type name
 * passed to #DefineIterZip(T, U).
 * @param U The type of value the second `Iterable` wrapped in this `IterZip` will yield. Must be the same type name
 * passed to #DefineIterZip(T, U).
 *
 * @note If `T`, or `U`, is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 */
#define IterZip(T, U) ITPL_CONCAT(ITPL_CONCAT(IterZip_, T), ITPL_CONCAT(_, U))

/**
 * @def DefineIterZip(T, U)
 * @brief Define an IterZip struct that works with an `Iterable(T)` and an `Iterable(U)`.
 *
 * # Example
 *
 * @code
 * DefineIterZip(int, char); // Defines an IterZip(int, char) struct
 * @endcode
 *
 * @param T The type of value the first `Iterable` wrapped in this `IterZip` will yield.
 * @param U The type of value the second `Iterable` wrapped in this `IterZip` will yield.
 *
 * @note If `T`, or `U`, is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #Iterator(T) for the given `T` and `U` **must** also exist.
 */
#define DefineIterZip(T, U)                                                                                            \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        Iterable(T) asrc;                                                                                              \
        Iterable(U) bsrc;                                                                                              \
    } IterZip(T, U)

/**
 * @def define_iterzip_func(T, U, Name)
 * @brief Define a function to turn an #IterZip(T, U) into an #Iterable(T) where `T = Pair(T, U)`.
 *
 * Define the `next` function implementation for the #IterZip(T, U) struct, and use it to implement the Iterator
 * typeclass, for given `T` and `U`.
 *
 * The defined function takes in a value of type `IterZip(T, U)*` and wraps it in an `Iterable(Pair(T, U))`.
 *
 * # Example
 *
 * @code
 * DefineIterZip(int, char);
 *
 * // Implement `Iterator` for `IterZip(int, char)`
 * // The defined function has the signature- `Iterable(Pair(int, char)) wrap_intchrzip(IterZip(int, char)* x)`
 * define_iterzip_func(int, char, wrap_intchrzip)
 * @endcode
 *
 * Usage of the defined function-
 *
 * @code
 * // Zip together `intit` (of type `Iterable(int)`) and `chrit` (of type `Iterable(char)`)
 * Iterable(Pair(int, char)) int_chr_it = wrap_intchrzip(&(IterZip(int, char)){ .asrc = intit, .bsrc = chrit });
 * @endcode
 *
 * @param T The type of value the first `Iterable` wrapped in this `IterZip` will yield.
 * @param U The type of value the second `Iterable` wrapped in this `IterZip` will yield.
 * @param Name Name to define the function as.
 *
 * @note If `T`, or `U`, is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note An #IterZip(T, U) for the given `T` and `U` **must** exist.
 * @note An #Iterator(T), with `T = Pair(T, U)`, for the given `T` and `U` must exist.
 * @note This should not be delimited by a semicolon.
 */
#define define_iterzip_func(T, U, Name)                                                                                \
    static Maybe(Pair(T, U)) ITPL_CONCAT(IterZip(T, U), _nxt)(IterZip(T, U) * self)                                    \
    {                                                                                                                  \
        Iterable(T) asrcit = self->asrc;                                                                               \
        Iterable(U) bsrcit = self->bsrc;                                                                               \
        Maybe(T) ares      = asrcit.tc->next(asrcit.self);                                                             \
        if (is_nothing(ares)) {                                                                                        \
            return Nothing(Pair(T, U));                                                                                \
        }                                                                                                              \
        Maybe(U) bres = bsrcit.tc->next(bsrcit.self);                                                                  \
        if (is_nothing(bres)) {                                                                                        \
            return Nothing(Pair(T, U));                                                                                \
        }                                                                                                              \
        return Just(PairOf(from_just_(ares), from_just_(bres), T, U), Pair(T, U));                                     \
    }                                                                                                                  \
    impl_iterator(IterZip(T, U)*, Pair(T, U), Name, ITPL_CONCAT(IterZip(T, U), _nxt))

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
    DefineMaybe(Pair(size_t, T)) DefineMaybe(Pair(T, T)) DefineIteratorOf(T);                                          \
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

#endif /* !LIB_ITPLUS_H */
