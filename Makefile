TARGET := libcry.a

CC := gcc
AR := ar

CFLAGS := -O0 -g
CPPFLAGS := -Iinclude

OBJS := src/cry_version.o \
        src/cry_memxor.o \
        src/cry_base64.o \
        src/cry_des.o \
        src/cry_aes.o \
        src/cry_cbc.o \
        src/cry_gcm.o \
        src/cry_ctr.o \
        src/cry_crc16.o \
        src/cry_crc16_ccitt.o \
        src/cry_crc16_ibm.o \
        src/cry_md5.o \
        src/cry_sha256.o \
        src/cry_cmac.o \
        src/cry_cs.o \
		src/cry_lrc.o \
        src/cry_mpi.o

.PHONY: all clean

all: $(OBJS)
	$(AR) -rcs $(TARGET) $^
	make -C test

clean:
	$(RM) $(TARGET) $(OBJS)
	make -C test clean

