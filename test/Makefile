libcry := ../libcry.a

CC := gcc

CFLAGS := -O0 -g3 -Wall
CPPFLAGS = -I. -I../include -I../src
LDFLAGS := --coverage


objs := \
	test.o \
	version_test.o \
	memxor_test.o \
	base64_test.o \
	mpi_test.o \
	des_test.o \
	aes_test.o \
	trivium_test.o \
	hill_test.o \
	affine_test.o \
	crc_test.o \
	hmac_test.o \
	cmac_test.o \
#	md5_test.o \
	sha256_test.o \
	sum_test.o \
	rsa_test.o \
	rand_test.o \
	dh_test.o \
	dsa_test.o \
	ecp_test.o \
	ecdsa_test.o \
	ecdh_test.o \

.PHONY: all clean 

all: $(objs) $(libcry)
	$(CC) $(LDFLAGS) -o test $^

clean:
	$(RM) $(objs) test
	$(RM) `find . -type f \( -name \*.gcda -o -name \*.gcno -o -name \*.gcov \)`

coverage: test
	@mkdir -p out/coverage
	@lcov -q -z -d ../build
	@lcov -q -c -i -d ../build -o base.info
	@./test $(TESTS)
	@lcov -q -c -d ../build -o cry.info
	@lcov -q -a base.info -a cry.info -o cry.info
	@genhtml -q -o out/coverage cry.info 
	@rm base.info cry.info

valgrind: all
	@mkdir -p out/valgrind
	valgrind --log-file=val.log --leak-check=full --show-leak-kinds=all --track-origins=yes ./test $(TESTS)
	@echo "<html><pre>" > valgrind.html
	@cat val.log >> valgrind.html
	@echo "</pre></html>" >> valgrind.html
	@mv valgrind.html out/valgrind/index.html
	@rm val.log

