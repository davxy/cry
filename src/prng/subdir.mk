# src/prng/subdir.mk


ifeq ($(CRY_PRNG),posix)
objs-y += prng_posix.o
else ifeq ($(CRY_PRNG),win)
objs-y += prng_win.o
else ifeq ($(CRY_PRNG),aes)
objs-y += prng_aes.o
else
objs-y += prng_weak.o
endif

