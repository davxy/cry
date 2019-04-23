#include "malloc_mock.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int g_malloc_mock_count;
int g_malloc_mock_state = MALLOC_MOCK_STOPPED;

#define MAGIC   0xABADCAFE

void *__real_malloc(size_t size);
void *__real_realloc(void *ptr, size_t size);
void __real_free(void *ptr);

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

#define HEAD_SIZE sizeof(struct mem_head)

void *__wrap_malloc(size_t size)
{
    struct mem_head *head;

    if (mock_update() == MALLOC_MOCK_FAILED)
        return NULL;

    if (g_malloc_mock_state == MALLOC_MOCK_ACTIVE) {
        if (g_malloc_mock_count == 0) {
            g_malloc_mock_state = MALLOC_MOCK_FAILED;
            return NULL;
        }
        g_malloc_mock_count--;
    }
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
    struct mem_head *head;

    if (mock_update() == MALLOC_MOCK_FAILED)
        return NULL;

    if (ptr == NULL)
        return __wrap_malloc(size);
    head = ((struct mem_head *)ptr) - 1;
    if (head->magic != MAGIC) {
        printf(">>> Warning: realloc of a not alloced buffer\n");
        return NULL;
    }
    head = __real_realloc(head, (size != 0) ? size + HEAD_SIZE : 0);
    if (size == 0)
        return NULL;
    if (size > head->size)
        memset((unsigned char *)&head[1] + head->size, 0xFF, size - head->size);
    else
        memset((unsigned char *)&head[1] + size, 0xFF, head->size - size);
    head->size = size;
    return &head[1];
}

void __wrap_free(void *ptr)
{
    struct mem_head *head;

    head = ((struct mem_head *)ptr) - 1;
    if (head->magic != MAGIC) {
        printf(">>> Warning: free of a not alloced buffer\n");
        return;
    }
    memset(head, 0xFF, HEAD_SIZE + head->size);
    __real_free(head);
}

