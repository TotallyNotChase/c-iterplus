#include "common.h"
#include "sugar.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

static void test_take(void)
{
    Iterable(uint32_t) it   = get_fibitr();      /* Create an infinite fibonacci sequence iterable */
    Iterable(uint32_t) it10 = take_from(it, 10); /* Iterable of the first 10 items in the sequence */
    /* Print the first 10 items */
    foreach (uint32_t, n, it10) {
        printf("%" PRIu32 " ", n);
    }
    puts("");
    /* `it10` has consumed 10 items from `it` - take the next 10 items */
    Iterable(uint32_t) it10_10 = take_from(it, 10); /* Iterable of the next 10 items from the sequence */
    /* Print the next 10 items */
    foreach (uint32_t, n, it10_10) {
        printf("%" PRIu32 " ", n);
    }
    puts("");
}

static NumType u32_to_numtype(uint32_t x) { return x % 2 == 0 ? EVEN : ODD; }

static void test_map(void)
{
    Iterable(uint32_t) it = get_fibitr(); /* Create an infinite fibonacci sequence iterable */
    // clang-format off
    Iterable(NumType) it10 = take_from(   /* Take the first 10 items of the iterable */
        /* Map the `u32_to_numtype` function over `it` */
        map_over(it, u32_to_numtype), 10
    );
    // clang-format on
    /* Print the first 10 items */
    foreach (NumType, n, it10) {
        if (n == EVEN) {
            printf("EVEN ");
        } else if (n == ODD) {
            printf("ODD ");
        } else {
            printf("WHAT? ");
        }
    }
    puts("");
}

static bool is_even(uint32_t x) { return x % 2 == 0; }

static void test_filter(void)
{
    Iterable(uint32_t) it = get_fibitr(); /* Create an infinite fibonacci sequence iterable */
    // clang-format off
    Iterable(uint32_t) it10 = take_from(  /* Take the first 10 items of the iterable */
        /* Filter out only the even numbers */
        filter_out(it, is_even),
    10);
    // clang-format on
    /* Print the first 10 items */
    foreach (uint32_t, n, it10) {
        printf("%" PRIu32 " ", n);
    }
    puts("");
}

int main(void)
{
    test_take();
    test_map();
    test_filter();
    return 0;
}