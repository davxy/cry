CC := gcc

CFLAGS := -O0 -g
CPPFLAGS := -I../include
LDLIBS := ../libcry.a

TESTS := version_test \
         base64_test

.PHONY: all clean

all: $(TESTS)

clean:
	$(RM) $(TESTS)

