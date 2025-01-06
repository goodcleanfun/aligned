#include <stdint.h>
#include "greatest/greatest.h"

#define ARRAY_NAME test_array
#define ARRAY_TYPE int32_t
#define DEFAULT_ARRAY_SIZE 8
#define ARRAY_ALIGNMENT 16
#include "aligned_array.h"
#undef ARRAY_NAME
#undef ARRAY_TYPE
#undef ARRAY_ALIGNMENT

#define ARRAY_NAME page_array
#define ARRAY_TYPE int32_t
#define DEFAULT_ARRAY_SIZE 8
#define ARRAY_MALLOC page_aligned_malloc
#define ARRAY_REALLOC page_aligned_resize
#define ARRAY_FREE page_aligned_free
#include "aligned_array.h"
#undef ARRAY_NAME
#undef ARRAY_TYPE
#undef ARRAY_MALLOC
#undef ARRAY_REALLOC
#undef ARRAY_FREE


TEST test_array_resizing(void) {
    test_array *v = test_array_new();
    ASSERT_EQ(v->m, DEFAULT_ARRAY_SIZE);
    ASSERT(test_array_empty(v));
    ASSERT_EQ(v->n, 0);

    page_array *p = page_array_new();
    ASSERT_EQ(p->m, DEFAULT_ARRAY_SIZE);
    ASSERT(page_array_empty(p));
    ASSERT_EQ(p->n, 0);

    for (int32_t i = 0; i < 10; i++) {
        test_array_push(v, i);
    }
    size_t expected_size = DEFAULT_ARRAY_SIZE * 3 / 2;
    ASSERT_EQ(v->m, expected_size);
    ASSERT(!test_array_empty(v));
    ASSERT_EQ(v->n, 10);

    for (int32_t i = 0; i < 10; i++) {
        ASSERT_EQ(v->a[i], i);
    }

    test_array *w = test_array_new_size(16);
    ASSERT_EQ(w->m, 16);
    ASSERT_EQ(w->n, 0);

    for (int32_t i = 0; i < 17; i++) {
        test_array_push(w, i);
    }
    ASSERT_EQ(w->m, 16 * 3 / 2);
    ASSERT_EQ(w->n, 17);

    test_array_concat(v, w);
    expected_size = expected_size * 3 / 2 * 3 / 2;
    ASSERT_EQ(v->m, expected_size);
    ASSERT_EQ(v->n, 27);
    size_t current_cap = v->m;
    ASSERT(test_array_resize_to_fit(v, current_cap));
    ASSERT_EQ(v->m, current_cap);

    test_array_extend(v, (int32_t[]){1, 2, 3}, 3);
    expected_size = expected_size * 3 / 2;
    ASSERT_EQ(v->m, expected_size);
    ASSERT_EQ(v->n, 30);
    current_cap = v->m;
    ASSERT(test_array_resize_to_fit(v, (current_cap * 3 / 2) - 1));
    expected_size = expected_size * 3 / 2;
    ASSERT_EQ(v->m, expected_size);

    test_array_destroy(v);
    PASS();
}

/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int32_t main(int32_t argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    RUN_TEST(test_array_resizing);

    GREATEST_MAIN_END();        /* display results */
}
