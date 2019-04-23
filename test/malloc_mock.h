#ifndef MALLOC_MOCK_H_
#define MALLOC_MOCK_H_

#define MALLOC_MOCK_STOPPED 0
#define MALLOC_MOCK_READY   1
#define MALLOC_MOCK_ACTIVE  2
#define MALLOC_MOCK_FAILED  3

extern int g_malloc_mock_count;
extern int g_malloc_mock_state;

#endif /* MALLOC_MOCK_H_ */
