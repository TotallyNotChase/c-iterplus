#include "common.h"
#include "sugar.h"

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define FIBSEQ_MINSZ 10

#define TEST_COUNT 11

#define DECIMAL_BASE 10

/* cheese */
string const cheese[]  = {"Red Leicester",
                         "42",
                         "Tilsit",
                         "EVEN",
                         "EVEN",
                         "Caerphilly",
                         "Bel Paese",
                         "ODD",
                         "94",
                         "41",
                         "3",
                         "Red Windsor",
                         "Stilton",
                         "Gruyere",
                         "Ementhal",
                         "Norweigan Jarlsburg",
                         "ODD",
                         "EVEN",
                         "ODD",
                         "0",
                         "19",
                         "Lipta",
                         "Lancashire",
                         "White Stilton"};
size_t const cheeselen = sizeof(cheese) / sizeof(*cheese);

static bool test_take(void)
{
    /* Build a fibonacci array, for verification later */
    uint32_t fibarr[FIBSEQ_MINSZ * 2] = {0, 1};
    for (size_t i = 2; i < FIBSEQ_MINSZ * 2; i++) {
        fibarr[i] = fibarr[i - 1] + fibarr[i - 2];
    }

    Iterable(uint32_t) it = get_fibitr(); /* Create an infinite fibonacci sequence iterable */
    /* Iterable of the first FIBSEQ_MINSZ number of items in the sequence */
    Iterable(uint32_t) itslice = take(it, FIBSEQ_MINSZ);
    /* Verify the values */
    size_t i = 0;
    foreach (uint32_t, n, itslice) {
        if (n != fibarr[i]) {
            fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 " at index: %zu\n", __func__, fibarr[i], n, i);
            return false;
        }
        i++;
    }
    /* Consumed FIBSEQ_MINSZ number of items from `it` - take the next FIBSEQ_MINSZ number of items */
    Iterable(uint32_t) itslice_ex = take(it, FIBSEQ_MINSZ);
    /* Verify the values */
    foreach (uint32_t, n, itslice_ex) {
        if (n != fibarr[i]) {
            fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 " at index: %zu\n", __func__, fibarr[i], n, i);
            return false;
        }
        i++;
    }
    return true;
}

static bool test_drop(void)
{
    /* Build a fibonacci array, for verification later */
    uint32_t fibarr[FIBSEQ_MINSZ * 2] = {0, 1};
    for (size_t i = 2; i < FIBSEQ_MINSZ * 2; i++) {
        fibarr[i] = fibarr[i - 1] + fibarr[i - 2];
    }

    /* Drop the first FIBSEQ_MINSZ number of items and take the next FIBSEQ_MINSZ number of items */
    Iterable(uint32_t) itslice = take(drop(get_fibitr(), FIBSEQ_MINSZ), FIBSEQ_MINSZ);
    /* Verify the values */
    size_t i = FIBSEQ_MINSZ;
    foreach (uint32_t, n, itslice) {
        if (n != fibarr[i]) {
            fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 " at index: %zu\n", __func__, fibarr[i], n, i);
            return false;
        }
        i++;
    }
    return true;
}

static NumType u32_to_numtype(uint32_t x) { return x % 2 == 0 ? EVEN : ODD; }

static bool test_map(void)
{
    /* Build a NumType array based on the fibonacci sequence, for verification later */
    uint32_t prev                    = 0;
    uint32_t curr                    = 1;
    NumType numtypearr[FIBSEQ_MINSZ] = {0};
    for (size_t i = 0; i < FIBSEQ_MINSZ; i++) {
        numtypearr[i]     = u32_to_numtype(prev);
        uint32_t new_curr = prev + curr;
        prev              = curr;
        curr              = new_curr;
    }
    /* Map u32_to_numtype over it and take the first FIBSEQ_MINSZ number of items */
    Iterable(NumType) itslice = take(map(get_fibitr(), u32_to_numtype), FIBSEQ_MINSZ);

    /* Verify the values */
    size_t i = 0;
    foreach (NumType, n, itslice) {
        if (n != numtypearr[i]) {
            fprintf(stderr, "%s: Expected: %d Actual: %d at index: %zu\n", __func__, numtypearr[i], n, i);
            return false;
        }
        i++;
    }
    return true;
}

static bool is_even(uint32_t x) { return x % 2 == 0; }
static bool is_odd(uint32_t x) { return x % 2 != 0; }

static bool test_filter(void)
{
    /* Build an array consisting of even fibonacci numbers, for verification later */
    uint32_t prev                      = 0;
    uint32_t curr                      = 1;
    uint32_t filteredarr[FIBSEQ_MINSZ] = {0};
    for (size_t i = 0; i < FIBSEQ_MINSZ;) {
        if (is_even(prev)) {
            filteredarr[i] = prev;
            i++;
        }
        uint32_t new_curr = prev + curr;
        prev              = curr;
        curr              = new_curr;
    }

    /* Filter out only the even fibonacci numbers, and take the first FIBSEQ_MINSZ number of them */
    Iterable(uint32_t) itslice = take(filter(get_fibitr(), is_even), FIBSEQ_MINSZ);
    /* Verify the values */
    size_t i = 0;
    foreach (uint32_t, n, itslice) {
        if (n != filteredarr[i]) {
            fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 " at index: %zu\n", __func__, filteredarr[i],
                    n, i);
            return false;
        }
        i++;
    }
    return true;
}

/* Returns true if length of string is less than 7 */
#define SMALLSTR_MAXLEN 7
static bool is_smallstr(string str) { return strlen(str) < SMALLSTR_MAXLEN; }

/* Try-parse a string to a number */
static Maybe(uint32_t) parse_posu32(string str)
{
    char* end;
    unsigned long l = strtoul(str, &end, DECIMAL_BASE);
    errno           = 0;
    if (errno == ERANGE || *end != '\0') {
        return Nothing(uint32_t);
    }
    return Just(l, uint32_t);
}

/* Try-parse a string to a NumType */
static Maybe(NumType) parse_numtype(string str)
{
    if (strcmp(str, "EVEN") == 0) {
        return Just(EVEN, NumType);
    }
    if (strcmp(str, "ODD") == 0) {
        return Just(ODD, NumType);
    }
    return Nothing(NumType);
}

static bool test_filtermap(void)
{
    /* Build arrays of expected data, for verification later */
    uint32_t expectednums[FIBSEQ_MINSZ];
    NumType expectednumtypes[FIBSEQ_MINSZ];
    for (size_t i = 0, j = 0, k = 0; i < sizeof(cheese) / sizeof(*cheese); i++) {
        string s = cheese[i];
        if (is_smallstr(s)) {
            Maybe(uint32_t) parsednum = parse_posu32(s);
            if (is_just(parsednum)) {
                expectednums[j++] = from_just_(parsednum);
            }
            Maybe(NumType) parsednumtype = parse_numtype(s);
            if (is_just(parsednumtype)) {
                expectednumtypes[k++] = from_just_(parsednumtype);
            }
        }
    }

    /*
    Obtain only the first 10 "small" strings, map a parsing function over it, and obtain only the
    successfully parsed numbers
    */
    Iterable(uint32_t) parsedit =
        filter_map(take(filter(strarr_to_iter(cheese, cheeselen), is_smallstr), 10), parse_posu32);
    size_t i = 0;
    foreach (uint32_t, n, parsedit) {
        if (n != expectednums[i]) {
            fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 " at index: %zu\n", __func__, expectednums[i],
                    n, i);
            return false;
        }
        i++;
    }

    /*
    Obtain only the first 10 "small" strings, map another parsing function over it, and obtain only the
    successfully parsed numtypes
    */
    Iterable(NumType) numtypeit =
        filter_map(take(filter(strarr_to_iter(cheese, cheeselen), is_smallstr), 10), parse_numtype);
    i = 0;
    foreach (NumType, x, numtypeit) {
        if (x != expectednumtypes[i]) {
            fprintf(stderr, "%s: Expected: %d Actual: %d at index: %zu\n", __func__, expectednumtypes[i], x, i);
            return false;
        }
        i++;
    }
    return true;
}

static bool test_chain(void)
{
    /* Build a fibonacci array, for verification later */
    uint32_t fibarr[FIBSEQ_MINSZ * 3] = {0, 1};
    for (size_t i = 2; i < FIBSEQ_MINSZ * 3; i++) {
        fibarr[i] = fibarr[i - 1] + fibarr[i - 2];
    }

    /* Chain 2 sequential parts of the fibonacci sequence- 0-10 -> 10-20 */
    Iterable(uint32_t) chained =
        chain(take(get_fibitr(), FIBSEQ_MINSZ), take(drop(get_fibitr(), FIBSEQ_MINSZ), FIBSEQ_MINSZ));
    size_t i = 0;
    foreach (uint32_t, n, chained) {
        if (n != fibarr[i]) {
            fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 " at index: %zu\n", __func__, fibarr[i], n, i);
            return false;
        }
        i++;
    }

    /* Chain various parts of the fibonacci sequence- 0-10 -> (10-20 -> 20-30) */
    Iterable(uint32_t) dual_chained_fr =
        chain(take(get_fibitr(), FIBSEQ_MINSZ), chain(take(drop(get_fibitr(), FIBSEQ_MINSZ), FIBSEQ_MINSZ),
                                                      take(drop(get_fibitr(), FIBSEQ_MINSZ * 2), FIBSEQ_MINSZ)));
    i = 0;
    foreach (uint32_t, n, dual_chained_fr) {
        if (n != fibarr[i]) {
            fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 " at index: %zu\n", __func__, fibarr[i], n, i);
            return false;
        }
        i++;
    }

    /* Chain various parts of the fibonacci sequence- (0-10 -> 10-20) -> 20-30 */
    Iterable(uint32_t) dual_chained_bk =
        chain(chain(take(get_fibitr(), FIBSEQ_MINSZ), take(drop(get_fibitr(), FIBSEQ_MINSZ), FIBSEQ_MINSZ)),
              take(drop(get_fibitr(), FIBSEQ_MINSZ * 2), FIBSEQ_MINSZ));
    i = 0;
    foreach (uint32_t, n, dual_chained_bk) {
        if (n != fibarr[i]) {
            fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 " at index: %zu\n", __func__, fibarr[i], n, i);
            return false;
        }
        i++;
    }
    return true;
}

static uint32_t add_u32(uint32_t x, uint32_t y) { return x + y; }

static bool test_reduce(void)
{
    /* Build an array consisting of even fibonacci numbers, for verification later */
    uint32_t prev   = 0;
    uint32_t curr   = 1;
    uint32_t result = prev;
    for (size_t i = 1; i < FIBSEQ_MINSZ; i++) {
        uint32_t new_curr = prev + curr;
        prev              = curr;
        curr              = new_curr;
        result            = add_u32(result, prev);
    }

    /*
    Reduce an iterable of the first FIBSEQ_MINSZ number of items of the fibonacci
    sequence with add_u32
    */
    Maybe(uint32_t) reduced = reduce(take(get_fibitr(), FIBSEQ_MINSZ), add_u32);
    if (from_just(reduced, uint32_t) != result) {
        fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 "\n", __func__, result, from_just_(reduced));
        return false;
    }

    /* Make sure reducing an empty iterable yields `Nothing` */
    Maybe(uint32_t) nothing = reduce(take(get_fibitr(), 0), add_u32);
    if (is_just(nothing)) {
        fprintf(stderr, "%s: Expected: Nothing Actual: %" PRIu32 "\n", __func__, from_just_(nothing));
        return false;
    }

    return true;
}

static bool test_take_while(void)
{
    /* Build an array of the first odd fibonacci numbers, for verification later */
    uint32_t prev                       = 0;
    uint32_t curr                       = 1;
    uint32_t first_oddarr[FIBSEQ_MINSZ] = {0};
    for (size_t i = 0; i < FIBSEQ_MINSZ; i++) {
        if (!is_odd(prev)) {
            break;
        }
        first_oddarr[i]   = prev;
        uint32_t new_curr = prev + curr;
        prev              = curr;
        curr              = new_curr;
    }

    /* Take the longest prefix of odd numbers from the infinite fibonacci sequence */
    Iterable(uint32_t) first_odds = take_while(get_fibitr(), is_odd);

    size_t i = 0;
    foreach (uint32_t, n, first_odds) {
        if (n != first_oddarr[i]) {
            fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 " at index: %zu\n", __func__, first_oddarr[i],
                    n, i);
            return false;
        }
        i++;
    }

    return true;
}

static bool test_drop_while(void)
{
    /* Build an array of the first odd fibonacci numbers, for verification later */
    uint32_t prev                              = 0;
    uint32_t curr                              = 1;
    uint32_t odds_after_evensarr[FIBSEQ_MINSZ] = {0};
    for (; is_even(prev);) {
        uint32_t new_curr = prev + curr;
        prev              = curr;
        curr              = new_curr;
    }
    for (size_t i = 0; i < FIBSEQ_MINSZ; i++) {
        if (!is_odd(prev)) {
            break;
        }
        odds_after_evensarr[i] = prev;
        uint32_t new_curr      = prev + curr;
        prev                   = curr;
        curr                   = new_curr;
    }

    /* Take the longest prefix of odd numbers from the infinite fibonacci sequence */
    Iterable(uint32_t) odds_after_evens = take_while(drop_while(get_fibitr(), is_even), is_odd);

    size_t i = 0;
    foreach (uint32_t, n, odds_after_evens) {
        if (n != odds_after_evensarr[i]) {
            fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 " at index: %zu\n", __func__,
                    odds_after_evensarr[i], n, i);
            return false;
        }
        i++;
    }

    return true;
}

static bool test_collect(void)
{
    /* Build a fibonacci array, for verification later */
    uint32_t fibarr[FIBSEQ_MINSZ] = {0, 1};
    for (size_t i = 2; i < FIBSEQ_MINSZ; i++) {
        fibarr[i] = fibarr[i - 1] + fibarr[i - 2];
    }

    /* Collect the first `FIBSEQ_MINSZ` number of items from the fibonacci sequence into an array */
    size_t fibarrsz             = 0;
    uint32_t* const clct_fibarr = collect(take(get_fibitr(), FIBSEQ_MINSZ), &fibarrsz);
    if (clct_fibarr == NULL) {
        fprintf(stderr, "Collect returned NULL\n");
        return false;
    }
    for (size_t i = 0; i < fibarrsz; i++) {
        if (clct_fibarr[i] != fibarr[i]) {
            fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 " at index: %zu\n", __func__, fibarr[i],
                    clct_fibarr[i], i);
            return false;
        }
    }
    free(clct_fibarr);
    return true;
}

/* Accumulating function to parse strings to numbers and add them */
static uint32_t addparse_u32(uint32_t acc, string s)
{
    Maybe(uint32_t) parsed = parse_posu32(s);
    return is_just(parsed) ? acc + from_just_(parsed) : acc;
}

/* Function that takes 2 strings and returns the second one */
static string unconst_str(string a, string b)
{
    (void)a;
    return b;
}

static bool test_fold(void)
{
    uint32_t expectedsum = 0;
    for (size_t i = 0; i < sizeof(cheese) / sizeof(*cheese); i++) {
        string s                  = cheese[i];
        Maybe(uint32_t) parsednum = parse_posu32(s);
        if (is_just(parsednum)) {
            expectedsum += from_just_(parsednum);
        }
    }

    uint32_t sum = fold(strarr_to_iter(cheese, cheeselen), 0, addparse_u32);
    if (sum != expectedsum) {
        fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 "\n", __func__, expectedsum, sum);
        return false;
    }

    string lastcheese = fold(strarr_to_iter(cheese, cheeselen), "", unconst_str);
    if (lastcheese != cheese[cheeselen - 1]) {
        fprintf(stderr, "%s: Expected: %s Actual: %s\n", __func__, cheese[cheeselen - 1], lastcheese);
        return false;
    }
    return true;
}

int main(void)
{
    size_t passed = 0;
    if (test_take()) {
        passed++;
    }
    if (test_map()) {
        passed++;
    }
    if (test_filter()) {
        passed++;
    }
    if (test_drop()) {
        passed++;
    }
    if (test_filtermap()) {
        passed++;
    }
    if (test_chain()) {
        passed++;
    }
    if (test_reduce()) {
        passed++;
    }
    if (test_take_while()) {
        passed++;
    }
    if (test_drop_while()) {
        passed++;
    }
    if (test_collect()) {
        passed++;
    }
    if (test_fold()) {
        passed++;
    }
    if (passed == TEST_COUNT) {
        puts("All tests passing....");
    } else {
        puts("Some tests failed....");
    }
    return 0;
}
