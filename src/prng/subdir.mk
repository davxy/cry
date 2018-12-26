# src/prng/subdir.mk

ifeq ($(CRY_PRNG),posix)
objects-y += prng_posix.o
else ifeq ($(CRY_PRNG),win)
objects-y += prng_win.o
else ifeq ($(CRY_PRNG),aes)
objects-y += prng_aes.o
else
objects-y += prng_weak.o
endif

