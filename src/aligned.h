#ifndef ALIGNED_H
#define ALIGNED_H

#include <stdint.h>
#include <stddef.h>

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
#include <malloc.h>
static inline void *aligned_malloc(size_t size, size_t alignment) {
    return _aligned_malloc(size, alignment);
}
static inline void *aligned_resize(void *p, size_t old_size, size_t new_size, size_t alignment) {
    return _aligned_realloc(p, new_size, alignment);
}
static inline void aligned_free(void *p) {
    _aligned_free(p);
}
#else
#include <stdlib.h>
#if defined(_ISOC11_SOURCE)
static inline void *aligned_malloc(size_t size, size_t alignment) {
    return aligned_alloc(alignment, size);
}
#else
#if !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif
static inline void *aligned_malloc(size_t size, size_t alignment)
{
    void *p;
    int ret = posix_memalign(&p, alignment, size);
    return (ret == 0) ? p : NULL;
}
#endif
static inline void *aligned_resize(void *p, size_t old_size, size_t new_size, size_t alignment)
{
    if ((alignment == 0) || ((alignment & (alignment - 1)) != 0) || (alignment < sizeof(void *))) {
        return NULL;
    }

    if (p == NULL) {
        return NULL;
    }

    void *p1 = aligned_malloc(new_size, alignment);
    if (p1 == NULL) {
        free(p);
        return NULL;
    }

    memcpy(p1, p, old_size);
    free(p);
    return p1;
}
static inline void aligned_free(void *p)
{
    free(p);
}
#endif

#ifdef _MSC_VER
#define MIE_ALIGN(x) __declspec(align(x))
#else
#define MIE_ALIGN(x) __attribute__((aligned(x)))
#endif



#if !defined(POINTER_SIZE)
#if ((UINTPTR_MAX == 0xFFFFFFFFFFFFFFFFu))
#define POINTER_SIZE 8
#elif ((UINTPTR_MAX == 0xFFFFFFFF))
#define POINTER_SIZE 4
#elif ((UINTPTR_MAX == 0xFFFF))
#define POINTER_SIZE 2
#else
#error "Unknown pointer size"
#endif
#endif

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (POINTER_SIZE)
#endif

#if (DEFAULT_ALIGNMENT) <= 0
#error "DEFAULT_ALIGNMENT must be greater than 0"
#endif
#if ((DEFAULT_ALIGNMENT) & ((DEFAULT_ALIGNMENT) - 1)) != 0
#error "DEFAULT_ALIGNMENT must be a power of 2"
#endif
#if (((UINTPTR_MAX == 0xFFFFFFFFFFFFFFFFu) && ((DEFAULT_ALIGNMENT) < 8)) \
     || ((UINTPTR_MAX == 0xFFFFFFFF) && ((DEFAULT_ALIGNMENT) < 4)) \
     || ((UINTPTR_MAX == 0xFFFF) && ((DEFAULT_ALIGNMENT) < 2)))
#error "DEFAULT_ALIGNMENT must be at least the size of a pointer"
#endif

static void *default_aligned_malloc(size_t size) {
    return aligned_malloc(size, DEFAULT_ALIGNMENT);
}

static void *default_aligned_resize(void *p, size_t old_size, size_t new_size) {
    return aligned_resize(p, old_size, new_size, DEFAULT_ALIGNMENT);
}

static void default_aligned_free(void *p) {
    aligned_free(p);
}

#endif // ALIGNED_H
