#include "malloc_mock.h"
#include <stdlib.h>
#include <stdio.h>

int g_malloc_mock_count = -1;
int g_malloc_mock_state = MALLOC_MOCK_IDLE;

void *__real_malloc(size_t size);
void *__real_realloc(void *ptr, size_t size);
void *__real_calloc(size_t nmemb, size_t size);
void __real_free(void *ptr);

void *__wrap_malloc(size_t size)
{
    if (g_malloc_mock_state == MALLOC_MOCK_ACTIVE) {
        if (g_malloc_mock_count == 0) {
            g_malloc_mock_state = MALLOC_MOCK_FAILED;
            return NULL;
        }
        g_malloc_mock_count--;
    }
    return __real_malloc(size);
}

void *__wrap_realloc(void *ptr, size_t size)
{
    if (g_malloc_mock_state == MALLOC_MOCK_ACTIVE) {
        if (g_malloc_mock_count == 0) {
            g_malloc_mock_state = MALLOC_MOCK_FAILED;
            return NULL;
        }
        g_malloc_mock_count--;
    }
    return __real_realloc(ptr, size);
}

void *__wrap_calloc(size_t nmemb, size_t size)
{
    if (g_malloc_mock_state == MALLOC_MOCK_ACTIVE) {
        if (g_malloc_mock_count == 0) {
            g_malloc_mock_state = MALLOC_MOCK_FAILED;
            return NULL;
        }
        g_malloc_mock_count--;
    }
    return __real_calloc(nmemb, size);
}

void __wrap_free(void *ptr)
{
    __real_free(ptr);
}

