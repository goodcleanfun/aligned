#include <stdint.h>
#include "greatest/greatest.h"

#include "aligned.h"

TEST test_aligned(void) {
    size_t ALIGNMENT = 16;
    void *ptr = aligned_malloc(100, ALIGNMENT);
    ASSERT(ptr != NULL);
    ASSERT_EQ((uintptr_t)ptr % ALIGNMENT, 0);

    ptr = aligned_resize(ptr, 100, 20000, ALIGNMENT);
    ASSERT(ptr != NULL);
    ASSERT_EQ((uintptr_t)ptr % ALIGNMENT, 0);

    aligned_free(ptr);
    PASS();
}


/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int32_t main(int32_t argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    RUN_TEST(test_aligned);

    GREATEST_MAIN_END();        /* display results */
}
