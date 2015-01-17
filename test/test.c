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

#include "test.h"
#include <stdlib.h>
#include <string.h>


#define TEST_WRAP(name) \
    void name ## _test(void); \
    void name ## _wrap(void) { RUN( name ## _test ); }

TEST_WRAP(version)
TEST_WRAP(base64)
TEST_WRAP(des)
TEST_WRAP(aes)
TEST_WRAP(cbc)
TEST_WRAP(ctr)
TEST_WRAP(gcm)
TEST_WRAP(md5)
TEST_WRAP(sha256)
TEST_WRAP(cmac)
TEST_WRAP(crc)
TEST_WRAP(sum)
TEST_WRAP(mpi)
TEST_WRAP(rsa)


struct test_def {
    const char *name;
    void (*func)(void);
};

#define TEST_ELEM(name) { #name , name ## _wrap }

static struct test_def tests[] = {
    TEST_ELEM(version),
    TEST_ELEM(base64),
    TEST_ELEM(des),
    TEST_ELEM(aes),
    TEST_ELEM(cbc),
    TEST_ELEM(ctr),
    TEST_ELEM(gcm),
    TEST_ELEM(md5),
    TEST_ELEM(sha256),
    TEST_ELEM(cmac),
    TEST_ELEM(crc),
    TEST_ELEM(sum),
    TEST_ELEM(mpi),
    TEST_ELEM(rsa),
};

#define TESTS_NUM   (sizeof(tests)/sizeof(*tests))

void print_hex(unsigned char *p, size_t size)
{
    while (size-- > 0)
        TRACE("%02x", *p++);
    TRACE("\n");
}


int test_runs;
int test_level;
const char *test_msg;
unsigned char buf[BUFSIZ];

int main(int argc, char *argv[])
{
    int i, j;

    /* Reset test variables */
    test_runs = 0;
    test_level = 0;
    test_msg = NULL;

    printf("\nCRY(T_T)EST\n");

    if (argc == 1) {
        for (j = 0; j < TESTS_NUM; j++) {
            tests[j].func();
            if (test_msg != NULL)
                break;
        }
    } else {
        for (i = 1; i < argc; i++) {
            for (j = 0; j < TESTS_NUM; j++) {
                if (strcmp(argv[i], tests[j].name) == 0) {
                    tests[j].func();
                    if (test_msg != NULL) {
                        i = argc;
                        break;
                    }
                }
            }
        }
    }

    if (test_msg)
        printf("Assertion failure: \"%s\"\n\n", test_msg);
    else
        printf("\nALL TESTS PASSED!!!\n");
    printf("Tests run: %d\n", test_runs);
    printf("\n-------------------------------------\n\n");

    return test_msg != NULL;
}

