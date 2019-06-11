#include "malloc_mock.h"
#include "test.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *__real_malloc(size_t size);
void *__real_realloc(void *ptr, size_t size);
void __real_free(void *ptr);


int g_malloc_mock_count;
int g_malloc_mock_state = MALLOC_MOCK_STOPPED;

#define MAGIC   0xABADCAFE


static int mock_update(void)
{
    if (g_malloc_mock_state == MALLOC_MOCK_ACTIVE) {
        if (g_malloc_mock_count == 0) {
            g_malloc_mock_state = MALLOC_MOCK_FAILED;
        }
        g_malloc_mock_count--;
    }
    return g_malloc_mock_state;
}

struct mem_head {
    unsigned long magic;
    size_t        size;
};

typedef struct mem_head mem_head_t;

#define HEAD_SIZE sizeof(mem_head_t)

void *__wrap_malloc(size_t size)
{
    mem_head_t *head;

    if (mock_update() == MALLOC_MOCK_FAILED)
        return NULL;

    head = __real_malloc(HEAD_SIZE + size);
    if (head == NULL)
        return NULL;
    head->magic = MAGIC;
    head->size = size;
    memset(&head[1], 0xFF, size);
    return &head[1];
}

void *__wrap_realloc(void *ptr, size_t size)
{
    mem_head_t *head;

    if (ptr == NULL)
        return __wrap_malloc(size);

    if (mock_update() == MALLOC_MOCK_FAILED)
        return NULL;

    head = ((mem_head_t *)ptr) - 1;
    if (head->magic != MAGIC) {
        TRACE(">>> Warning: realloc of a not alloced buffer\n");
        return NULL;
    }
    if (size < head->size)
        memset((unsigned char *)&head[1] + size, 0xFF, head->size - size);
    head = __real_realloc(head, (size != 0) ? size + HEAD_SIZE : 0);
    if (head == NULL || size == 0)
        return NULL;
    if (size > head->size)
        memset((unsigned char *)&head[1] + head->size, 0xFF, size - head->size);
    head->size = size;
    return &head[1];
}

void *__wrap_calloc(size_t nmemb, size_t size)
{
    void *ptr;

    size = nmemb * size;
    ptr = __wrap_malloc(size);
    if (ptr == NULL || size == 0)
        return NULL;
    memset(ptr, 0, size);
    return ptr;
}

void __wrap_free(void *ptr)
{
    mem_head_t *head;

    if (ptr == NULL)
        return;
    head = ((mem_head_t *)ptr) - 1;
    if (head->magic != MAGIC) {
        TRACE(">>> Warning: free of a not alloced buffer\n");
        return;
    }
    memset(head, 0xFF, HEAD_SIZE + head->size);
    __real_free(head);
}
