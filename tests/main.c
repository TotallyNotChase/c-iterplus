#include "common.h"
#include "impls.h"
#include "sugar.h"

#include <inttypes.h>
#include <stdint.h>

static void test_take(void)
{
    Iterable(uint32_t) it   = get_fibitr();                /* Create an infinite fibonacci sequence iterable */
    Iterable(uint32_t) it10 = take_from(it, 10, uint32_t); /* Iterable of the first 10 items in the sequence */
    /* Print the first 10 items */
    foreach (uint32_t, n, it10) {
        printf("%" PRIu32 " ", n);
    }
    puts("");
    /* `it10` has consumed 10 items from `it` - take the next 10 items */
    Iterable(uint32_t) it10_10 = take_from(it, 10, uint32_t); /* Iterable of the next 10 items from the sequence */
    /* Print the next 10 items */
    foreach (uint32_t, n, it10_10) {
        printf("%" PRIu32 " ", n);
    }
    puts("");
}

int main(void)
{
    test_take();
    return 0;
}