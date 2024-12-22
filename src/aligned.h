#ifndef ALIGNED_H
#define ALIGNED_H

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
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
static inline void *aligned_malloc(size_t size, size_t alignment)
{
    void *p;
    int ret = posix_memalign(&p, alignment, size);
    return (ret == 0) ? p : NULL;
}
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

#endif // ALIGNED_H
