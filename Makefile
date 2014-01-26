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
        src/cry_gcm.o

.PHONY: all clean

all: $(OBJS)
	$(AR) -rcs $(TARGET) $^

clean:
	$(RM) $(TARGET) $(OBJS)

