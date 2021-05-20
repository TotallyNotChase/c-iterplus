#include "common.h"
#include "sugar.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#define FIBSEQ_MINSZ 10

#define TEST_COUNT 3

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
    // clang-format off
    Iterable(NumType) itslice = take_from(   /* Take the first FIBSEQ_MINSZ number of items of the iterable */
        /* Map the `u32_to_numtype` function over `it` */
        map_over(it, u32_to_numtype), FIBSEQ_MINSZ
    );
    // clang-format on
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
    uint32_t prev = 0, curr = 1;
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
    // clang-format off
    Iterable(uint32_t) itslice = take_from(  /* Take the first 10 items of the iterable */
        /* Filter out only the even numbers */
        filter_out(it, is_even),
    FIBSEQ_MINSZ);
    // clang-format on
    /* Verify the values */
    size_t i = 0;
    foreach (uint32_t, n, itslice) {
        if (n != filteredarr[i]) {
            fprintf(stderr, "%s: Expected: %d Actual: %d at index: %zu\n", __func__, filteredarr[i], n, i);
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
    };
    if (test_map()) {
        passed++;
    };
    if (test_filter()) {
        passed++;
    };
    if (passed == TEST_COUNT) {
        puts("All tests passing....");
    } else {
        puts("Some tests failed....");
    }
    return 0;
}
