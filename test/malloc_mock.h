#ifndef MALLOC_MOCK_H_
#define MALLOC_MOCK_H_

#define MALLOC_MOCK_IDLE    0
#define MALLOC_MOCK_ACTIVE  1
#define MALLOC_MOCK_FAILED  2

extern int g_malloc_mock_count;
extern int g_malloc_mock_state;

#endif /* MALLOC_MOCK_H_ */
