/*
 * Copyright (c) 2013-2018, Davide Galassi. All rights reserved.
 *
 * This file is part of CRY software.
 *
 * CRY is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with CRY; if not, see <http://www.gnu/licenses/>.
 */

#ifndef CRY_TEST_H_
#define CRY_TEST_H_

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

extern int test_runs;
extern int test_fails;
extern int test_level;
extern int test_cont;
extern const char *test_msg;
extern unsigned char buf[BUFSIZ];

#define ARLEN(ar) (sizeof(ar)/sizeof(ar[0]))

#define TS2US(ts) \
        ((unsigned long)(ts)->tv_sec * 1000000 + (ts)->tv_nsec / 1000)

void run(const char *name, void (* test)(void),
         void (* setup)(void), void (* teardown)(void));

#define RUNX(test, setup, teardown) do { \
    run(#test, test, setup, teardown); \
    if (test_msg != NULL) return; \
    } while(0)

#define RUN(test) RUNX(test, NULL, NULL)


#define CONTINUE(exp) do { \
    int __tmp = test_cont; \
    test_cont = 1; \
    exp; \
    test_cont = __tmp; \
    } while (0)

#define ASSERT(test) do { \
    if ((test) == 0) { \
        TRACE(">>> ASSERTION FAIL: %s\n", #test); \
        if (test_cont == 0) { test_msg = #test; return; } \
        else { test_fails++; } \
    } \
    } while (0)

#define ASSERT_EQ(i1, i2) \
    ASSERT((i1) == (i2))

#define ASSERT_NE(i1, i2) \
    ASSERT((i1) != (i2))

#define ASSERT_EQ_BUF(b1, b2, len) \
    ASSERT(memcmp(b1, b2, len) == 0)

#define ASSERT_EQ_MPI(mpi, rad, str) do { \
    cry_mpi_store_str(mpi, rad, buf, sizeof(buf)); \
    ASSERT(strcmp(buf, str) == 0); \
    } while (0)

#define ASSERT_OK(e) \
        ASSERT_EQ((e), 0)


#ifdef NDEBUG

#define TRACE(fmt, ...)
#define PRINT_HEX(msg, buf, siz)
#define PRINT_ASC(msg, buf, siz)
#define PRINT_MPI(msg, mpi, rad)

#else /* !NDEBUG */

# define TRACE(...) do { \
    fprintf(stdout, __VA_ARGS__); \
    fflush(stdout); \
    } while(0)

# define PRINT_HEX(msg, buf, siz) do { \
    size_t __siz = siz; \
    unsigned char *__p = buf; \
    if (msg) TRACE("%s: ", msg); \
    while (__siz-- > 0) TRACE("%02x", *__p++); \
    TRACE("\n"); \
    } while(0)

# define PRINT_ASC(msg, buf, siz) \
    TRACE("%s: %.*s\n", msg, (int)(siz), buf)

# define PRINT_MPI(msg, mpi, rad) do { \
    TRACE("%s:\t", msg); \
    cry_mpi_print(mpi, rad); \
    } while(0)

#endif /* NDEBUG */


#endif /* _TEST_H_ */

