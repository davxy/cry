LIBCRY := ../libcry.a

CC := gcc

CFLAGS := -O0 -g
CPPFLAGS := -I../include
LDLIBS := $(LIBCRY)

TESTS := version_test \
         base64_test \
         des_test \
         aes_test \
         cbc_test \
         gcm_test \
         ctr_test \
         crc16_test \
         md5_test \
         sha256_test \
         cmac_test \
         sum_test \
         mpi_test

.PHONY: all clean

all: $(TESTS) $(LIBCRY)

clean:
	$(RM) $(TESTS)

