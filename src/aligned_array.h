#ifndef ALIGNED_ARRAY_H
#define ALIGNED_ARRAY_H

#include <assert.h>
#include <stdint.h>
#include "aligned.h"

#endif

#ifndef ARRAY_MALLOC
#define ARRAY_MALLOC_DEFINED
#ifndef ARRAY_ALIGNMENT
#define ARRAY_MALLOC(size) default_aligned_malloc(size)
#else
#define ARRAY_MALLOC(size) aligned_malloc(size, ARRAY_ALIGNMENT)
#endif
#endif

#define ARRAY_REALLOC_NEEDS_PREV_SIZE

#ifndef ARRAY_REALLOC
#define ARRAY_REALLOC_DEFINED
#ifndef ARRAY_ALIGNMENT
#define ARRAY_REALLOC(a, prev_size, new_size) default_aligned_resize(a, prev_size, new_size)
#else
#define ARRAY_REALLOC(a, prev_size, new_size) aligned_resize(a, prev_size, new_size, ARRAY_ALIGNMENT)
#endif
#endif

#ifndef ARRAY_FREE
#define ARRAY_FREE_DEFINED
#define ARRAY_FREE(a) default_aligned_free(a)
#endif

#include "array/array.h"

#ifdef ARRAY_MALLOC_DEFINED
#undef ARRAY_MALLOC
#undef ARRAY_MALLOC_DEFINED
#endif

#undef ARRAY_REALLOC_NEEDS_PREV_SIZE

#ifdef ARRAY_REALLOC_DEFINED
#undef ARRAY_REALLOC
#undef ARRAY_REALLOC_DEFINED
#endif

#ifdef ARRAY_FREE_DEFINED
#undef ARRAY_FREE
#undef ARRAY_FREE_DEFINED
#endif
