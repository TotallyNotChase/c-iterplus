/**
 * @file
 * Utilities to define and use a Maybe type.
 *
 * https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-Maybe.html
 *
 * @note The Maybe struct members **must not** be accessed manually unless you know
 * exactly what you're doing. Use `from_just` or `from_just_` *after* `is_just`/`is_nothing` instead.
 */

#ifndef LIB_ITPLUS_MAYBE_H
#define LIB_ITPLUS_MAYBE_H

#include "itplus_macro_utils.h"

#include <stdio.h>
#include <stdlib.h>

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
 * @def fmap_maybe(x, fn, R)
 * @brief Map the function `fn` over `x` to construct a `Maybe(R)`.
 *
 * @param x The `Maybe` type to map over.
 * @param fn The function to map over the value inside the `Maybe` (if any). Should be of type `R (*)(T x)`. Where `T`
 * is the type of value contained within `x`.
 * @param R The return type of the mapping function.
 *
 * @return If `x` had a value, a `Maybe(B)` with a `Just` value after mapping `fn`. Otherwise, a `Maybe(B)` as
 * `Nothing`.
 *
 * @note This uses `x` twice. Do not use it with an `x` that may have side effects.
 */
#define fmap_maybe(x, fn, R) is_nothing(x) ? Nothing(R) : Just(fn(from_just_(x)), R)

#endif /* !LIB_ITPLUS_MAYBE_H */
