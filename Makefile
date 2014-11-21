TARGET := libcry.a

CC := gcc
AR := ar

CFLAGS := -O0 -g
CPPFLAGS := -Iinclude

OBJS := src/version.o \
        src/memxor.o \
        src/base64.o \
        src/des.o \
        src/aes.o \
        src/cbc.o \
        src/gcm.o \
        src/ctr.o \
        src/crc/crc16.o \
        src/crc/crc16_ccitt.o \
        src/crc/crc16_ibm.o \
        src/md5.o \
        src/sha256.o \
        src/cmac.o \
        src/sum/cs8.o \
        src/sum/lrc.o \
        src/mpi/mpi_core.o \
        src/mpi/mpi_cmp_abs.o \
        src/mpi/mpi_add_abs.o \
        src/mpi/mpi_sub_abs.o \
        src/mpi/mpi_cmp.o \
        src/mpi/mpi_add.o \
        src/mpi/mpi_sub.o \
        src/mpi/mpi_bin.o \
        src/mpi/mpi_str.o \
        src/mpi/mpi_list.o \
        src/mpi/mpi_mul.o

.PHONY: all clean

all: $(OBJS)
	$(AR) -rcs $(TARGET) $^
	make -C test

clean:
	$(RM) $(TARGET) $(OBJS)
	make -C test clean

