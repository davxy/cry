/*
 * Copyright (c) 2013-2014, Davide Galassi. All rights reserved.
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

#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <string.h>

//#define NDEBUG

extern int test_runs;
extern int test_level;
extern const char *test_msg;

#define ASSERT(test) do { \
    if (!(test)) { \
        test_msg = #test; \
        return; \
    } \
    } while (0)

#define RUN(test) do { \
    int i; \
    if (test_level == 0) \
        printf("\n"); \
    for (i = 0; i < test_level + 3; i++) \
        printf("%c", '-'); \
    printf(" %s\n", #test); \
    test_level++; \
    test(); \
    test_level--; \
    test_runs++; \
    if (test_msg) \
        return; \
    } while (0)


#ifdef NDEBUG
# define TRACE(fmt, ...)
#else
# define TRACE(...) fprintf(stdout, __VA_ARGS__)
#endif

void print_hex(unsigned char *p, size_t n);

#endif /* _TEST_H_ */

