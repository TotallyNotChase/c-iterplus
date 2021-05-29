#include "impls.h"
#include "sugar.h"

#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

static inline char* strdup_(char const* x)
{
    char* s = malloc((strlen(x) + 1) * sizeof(*s));
    strcpy(s, x);
    return s;
}

static bool pair_is_equal(Pair(char, char) x) { return fst(x) == snd(x); }
static char fst_chr(Pair(char, char) chrpair) { return fst(chrpair); }

/* Using iterators to find common prefix - without C11 sugar */
static char* cmn_prefx(string s1, string s2)
{
    /* Turn the 2 strings into iterables */
    Iterable(char) s1it = chrarr_to_iter(s1, strlen(s1));
    Iterable(char) s2it = chrarr_to_iter(s2, strlen(s2));
    /* Zip together the 2 iterables */
    Iterable(Pair(char, char)) zipped = zip_chrchr(&(IterZip(char, char)){.asrc = s1it, .bsrc = s2it});
    /* Keep taking pairs while they match up - essentially extracting the common prefix */
    Iterable(Pair(char, char)) common =
        takewhl_chrchr(&(IterTakeWhile(Pair(char, char))){.pred = pair_is_equal, .src = zipped});

    /* Only need either the first or second element from the equal pairs */
    Iterable(char) fstchars = map_chrchr_chr(&(IterMap(Pair(char, char), char)){.f = fst_chr, .src = common});

    /* Collect the iterable into a string */
    size_t len        = 0;
    char* const prefx = chr_collect(fstchars, &len);
    prefx[len]        = '\0';
    return prefx;
}

/* Using iterators to find common prefix - with C11 sugar */
static char* cmn_prefx_sugar(string s1, string s2)
{
    /* Common prefix but in an expression oriented way with C11 `_Generic` */
    size_t len = 0;

    // clang-format off
    /* Collect the prefix into a string */
    char* const prefx = collect(
        /* Extract out only one of the chars */
        map(
            /* Take the common prefix */
            takewhile(
                /* Zip together the strings as iterables */
                zip(chrarr_to_iter(s1, strlen(s1)), chrarr_to_iter(s2, strlen(s2))),
            pair_is_equal),
        fst_chr),
    &len);
    // clang-format on
    prefx[len] = '\0';
    return prefx;
}

/*
Accumulator function to find longest common prefix by folding over a string array
NOTE: This takes ownership of `acc`
*/
static inline char* acc_cmn_prefx(char* acc, string s)
{
    char* const res = cmn_prefx(acc, s);
    free(acc);
    return res;
}

/*
Accumulator function to find longest common prefix by folding over a string array - with C11 sugar
NOTE: This takes ownership of `acc`
*/
static inline char* acc_cmn_prefx_sugar(char* acc, string s)
{
    char* const res = cmn_prefx_sugar(acc, s);
    free(acc);
    return res;
}

static inline uint64_t sum_u64(uint64_t x, uint64_t y) { return x + y; }
static inline uint64_t mult_u64u64(Pair(uint64_t, uint64_t) x) { return x.a * x.b; }

#define ARRSZ 100000

int main(void)
{
    string arr[]  = {"flower", "flow", "flight"};
    size_t arrlen = sizeof(arr) / sizeof(*arr);

    /* Longest prefix fold without C11 sugar */
    char* lngest_prefx = str_fold(strarr_to_iter(arr + 1, arrlen - 1), strdup_(arr[0]), acc_cmn_prefx);
    puts(lngest_prefx);
    free(lngest_prefx);

    /* Longest prefix fold with C11 sugar */
    lngest_prefx = fold(strarr_to_iter(arr + 1, arrlen - 1), strdup_(arr[0]), acc_cmn_prefx_sugar);
    puts(lngest_prefx);
    free(lngest_prefx);

    srand(time(NULL));
    uint64_t arr1[ARRSZ];
    for (size_t i = 0; i < ARRSZ; i++) {
        arr1[i] = rand() % 100;
    }
    uint64_t arr2[ARRSZ];
    for (size_t i = 0; i < ARRSZ; i++) {
        arr2[i] = rand() % 100;
    }

    /* Dot product sum without C11 sugar */
    Iterable(Pair(uint64_t, uint64_t)) dot = zip_u64u64(
        &(IterZip(uint64_t, uint64_t)){.asrc = u64arr_to_iter(arr1, ARRSZ), .bsrc = u64arr_to_iter(arr2, ARRSZ)});
    Iterable(uint64_t) dot_product =
        map_u64u64_u64(&(IterMap(Pair(uint64_t, uint64_t), uint64_t)){.f = mult_u64u64, .src = dot});
    uint64_t dot_product_sum = from_just(u64_reduce(dot_product, sum_u64), uint64_t);
    printf("Sum: %" PRIi64 "\n", dot_product_sum);

    /* Dot product sum with C11 sugar */
    dot_product_sum = from_just(
        reduce(map(zip(u64arr_to_iter(arr1, ARRSZ), u64arr_to_iter(arr2, ARRSZ)), mult_u64u64), sum_u64), uint64_t);
    printf("Sum: %" PRIi64 "\n", dot_product_sum);

    return 0;
}
