/**
 * @file
 * Macro to map a given function/macro over __VA_ARGS__ - supports upto 8 arguments.
 * 
 * https://stackoverflow.com/questions/1872220/is-it-possible-to-iterate-over-arguments-in-variadic-macros
 */

#ifndef LIB_ITPLUS_PREPROC_MAP_H
#define LIB_ITPLUS_PREPROC_MAP_H

#include "itplus_macro_utils.h"

#define MAP_1(f, x)      f(x)
#define MAP_2(f, x, ...) f(x), MAP_1(f, __VA_ARGS__)
#define MAP_3(f, x, ...) f(x), MAP_2(f, __VA_ARGS__)
#define MAP_4(f, x, ...) f(x), MAP_3(f, __VA_ARGS__)
#define MAP_5(f, x, ...) f(x), MAP_4(f, __VA_ARGS__)
#define MAP_6(f, x, ...) f(x), MAP_5(f, __VA_ARGS__)
#define MAP_7(f, x, ...) f(x), MAP_6(f, __VA_ARGS__)
#define MAP_8(f, x, ...) f(x), MAP_7(f, __VA_ARGS__)

#define MAP_RSEQ_N()                                      8, 7, 6, 5, 4, 3, 2, 1, 0
#define MAP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define MAP_NARG_(...)                                    MAP_ARG_N(__VA_ARGS__)
#define MAP_NARG(...)                                     MAP_NARG_(__VA_ARGS__, MAP_RSEQ_N())

#define MAP_(N, f, ...) CONCAT(MAP_, N)(f, __VA_ARGS__)
#define MAP(f, ...)     MAP_(MAP_NARG(__VA_ARGS__), f, __VA_ARGS__)

#endif /* !LIB_ITPLUS_PREPROC_MAP_H */
