#ifndef ALIGNED_ARRAY_H
#define ALIGNED_ARRAY_H

#include <assert.h>
#include <stdint.h>
#include "aligned.h"

#endif

#ifndef ALIGNMENT
#if ((UINTPTR_MAX == 0xFFFFFFFFFFFFFFFFu))
#define ALIGNMENT 64
#elif ((UINTPTR_MAX == 0xFFFFFFFF))
#define ALIGNMENT 32
#elif ((UINTPTR_MAX == 0xFFFF))
#define ALIGNMENT 16
#else
#error "Unknown pointer size"
#endif
#define ALIGNMENT_DEFINED
#endif

#if (ALIGNMENT) <= 0
#error "ALIGNMENT must be greater than 0"
#endif
#if ((ALIGNMENT) & ((ALIGNMENT) - 1)) != 0
#error "ALIGNMENT must be a power of 2"
#endif
#if (((UINTPTR_MAX == 0xFFFFFFFFFFFFFFFFu) && ((ALIGNMENT) < 64)) \
     || ((UINTPTR_MAX == 0xFFFFFFFF) && ((ALIGNMENT) < 32)) \
     || ((UINTPTR_MAX == 0xFFFF) && ((ALIGNMENT) < 16)))
#error "ALIGNMENT must be at least the size of a pointer"
#endif
#undef POINTER_SIZE

#define ARRAY_MALLOC(size) aligned_malloc(size, ALIGNMENT)
#define ARRAY_REALLOC_NEEDS_PREV_SIZE
#define ARRAY_REALLOC(a, prev_size, new_size) aligned_resize((a), (prev_size), (new_size), ALIGNMENT)
#define ARRAY_FREE aligned_free

#include "array/array.h"

#undef ARRAY_MALLOC
#undef ARRAY_REALLOC_NEEDS_PREV_SIZE
#undef ARRAY_REALLOC
#undef ARRAY_FREE
#ifdef ALIGNMENT_DEFINED
#undef ALIGNMENT
#undef ALIGNMENT_DEFINED
#endif