/**
 * @file
 * Utilities to define and use a Pair type. A Tuple of 2 elements.
 *
 * https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-Tuple.html
 */

#ifndef LIB_ITPLUS_PAIR_H
#define LIB_ITPLUS_PAIR_H

#include "itplus_macro_utils.h"

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
#define Pair(T, U) ITPL_CONCAT(ITPL_CONCAT(Pair, T), U)

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
 * @note If `T`, or `U`, is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note A #Pair(T, U) for given `T` and `U` must exist.
 * @note The values are simply assigned to the #Pair(T, U) struct. No implicit copying is done.
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

#endif /* !LIB_ITPLUS_PAIR_H */
