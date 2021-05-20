#include "common.h"
#include "sugar.h"

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define FIBSEQ_MINSZ 10

#define TEST_COUNT 5

static bool test_take(void)
{
    /* Build a fibonacci array */
    uint32_t fibarr[FIBSEQ_MINSZ * 2] = {0, 1};
    for (size_t i = 2; i < FIBSEQ_MINSZ * 2; i++) {
        fibarr[i] = fibarr[i - 1] + fibarr[i - 2];
    }

    Iterable(uint32_t) it = get_fibitr(); /* Create an infinite fibonacci sequence iterable */
    Iterable(uint32_t) itslice =
        take_from(it, FIBSEQ_MINSZ); /* Iterable of the first FIBSEQ_MINSZ number of items in the sequence */
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
    Iterable(uint32_t) itslice_ex =
        take_from(it, FIBSEQ_MINSZ); /* Iterable of the next FIBSEQ_MINSZ number of items from the sequence */
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
    /* Build a fibonacci array */
    uint32_t fibarr[FIBSEQ_MINSZ * 2] = {0, 1};
    for (size_t i = 2; i < FIBSEQ_MINSZ * 2; i++) {
        fibarr[i] = fibarr[i - 1] + fibarr[i - 2];
    }

    Iterable(uint32_t) it = get_fibitr(); /* Create an infinite fibonacci sequence iterable */
    /* Drop the first FIBSEQ_MINSZ number of items and take the next FIBSEQ_MINSZ number of items */
    Iterable(uint32_t) itslice = take_from(drop_from(it, FIBSEQ_MINSZ), FIBSEQ_MINSZ);
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
    /* Build a NumType array based on the fibonacci sequence */
    uint32_t prev = 0, curr = 1;
    NumType numtypearr[FIBSEQ_MINSZ] = {0};
    for (size_t i = 0; i < FIBSEQ_MINSZ; i++) {
        numtypearr[i]     = u32_to_numtype(prev);
        uint32_t new_curr = prev + curr;
        prev              = curr;
        curr              = new_curr;
    }

    Iterable(uint32_t) it = get_fibitr(); /* Create an infinite fibonacci sequence iterable */
    /* Map u32_to_numtype over it and take the first FIBSEQ_MINSZ number of items */
    Iterable(NumType) itslice = take_from(map_over(it, u32_to_numtype), FIBSEQ_MINSZ);

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

static bool test_filter(void)
{
    /* Build an array consisting of even fibonacci numbers */
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

    Iterable(uint32_t) it = get_fibitr(); /* Create an infinite fibonacci sequence iterable */

    /* Filter out only the even fibonacci numbers, and take the first FIBSEQ_MINSZ number of them */
    Iterable(uint32_t) itslice = take_from(filter_from(it, is_even), FIBSEQ_MINSZ);
    /* Verify the values */
    size_t i = 0;
    foreach (uint32_t, n, itslice) {
        if (n != filteredarr[i]) {
            fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 " at index: %zu\n", __func__, filteredarr[i], n, i);
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
    unsigned long l = strtoul(str, &end, 10);
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
    } else if (strcmp(str, "ODD") == 0) {
        return Just(ODD, NumType);
    } else {
        return Nothing(NumType);
    }
}

static bool test_filtermap(void)
{
    string cheese[] = {"Red Leicester",
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
    uint32_t expectednums[10];
    NumType expectednumtypes[10];
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

    Iterable(string) strit =
        strarr_to_iter(cheese, sizeof(cheese) / sizeof(*cheese)); /* Build an iterable from the array */
    /*
    Obtain only the first 10 "small" strings, map a parsing function over it, and obtain only the
    successfully parsed numbers
    */
    Iterable(uint32_t) parsedit = filter_map(take_from(filter_from(strit, is_smallstr), 10), parse_posu32);
    size_t i                    = 0;
    foreach (uint32_t, n, parsedit) {
        if (n != expectednums[i]) {
            fprintf(stderr, "%s: Expected: %" PRIu32 " Actual: %" PRIu32 " at index: %zu\n", __func__, expectednums[i], n, i);
            return false;
        }
        i++;
    }

    Iterable(string) re_strit =
        strarr_to_iter(cheese, sizeof(cheese) / sizeof(*cheese)); /* Build another iterable from the array */
    /*
    Obtain only the first 10 "small" strings, map another parsing function over it, and obtain only the
    successfully parsed numtypes
    */
    Iterable(NumType) numtypeit = filter_map(take_from(filter_from(re_strit, is_smallstr), 10), parse_numtype);
    i                    = 0;
    foreach (NumType, x, numtypeit) {
        if (x != expectednumtypes[i]) {
            fprintf(stderr, "%s: Expected: %d Actual: %d at index: %zu\n", __func__, expectednumtypes[i], x, i);
            return false;
        }
        i++;
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
    if (passed == TEST_COUNT) {
        puts("All tests passing....");
    } else {
        puts("Some tests failed....");
    }
    return 0;
}
