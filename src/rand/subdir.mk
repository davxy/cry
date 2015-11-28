# src/rand/subdir.mk

ifeq ($(CRY_RAND),unix)
objs-y += rand_unix.o
else ifeq ($(CONFIG_RAND),win)
objs-y += rand_win.o
else ifeq ($(CONFIG_RAND),strong)
objs-y += rand_strong.o
else
objs-y += rand_weak.o
endif

