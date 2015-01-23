LIBCRY := ../libcry.a

CC := gcc

CFLAGS := -O0 -g
CPPFLAGS := -I../include
LDLIBS := $(LIBCRY)

OBJS := test.o \
		version_test.o \
		base64_test.o \
		des_test.o \
		aes_test.o \
	    cbc_test.o \
	    ctr_test.o \
	    gcm_test.o \
	    crc_test.o \
	    md5_test.o \
	    sha256_test.o \
	    cmac_test.o \
	    sum_test.o \
	    mpi_test.o \
	    rsa_test.o

.PHONY: all clean

all: $(OBJS) $(LIBCRY)
	$(CC) -o test $^

clean:
	$(RM) $(OBJS) test
