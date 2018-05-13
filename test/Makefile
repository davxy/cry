libcry := ../libcry.a

CC := gcc

CFLAGS := -O0 -g3 -Wall --coverage
CPPFLAGS = -I. -I../include -I../src
LDFLAGS := --coverage


objs := \
	test.o \
	version_test.o \
	memxor_test.o \
	base64_test.o \
	mpi/mpi_test.o \
	mpi/mpi_core_test.o \
	mpi/mpi_cmp_test.o \
	mpi/mpi_add_test.o \
	mpi/mpi_mul_test.o \
#	des_test.o \
	aes_test.o \
	cbc_test.o \
	ctr_test.o \
	gcm_test.o \
	crc_test.o \
	md5_test.o \
	sha256_test.o \
	cmac_test.o \
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
	$(RM) `find . -type f \( -name \*.gcda -o -name \*.gcno \)`

