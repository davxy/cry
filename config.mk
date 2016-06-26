################################################################################
# CRY build configuration.
#
# This file is directly feed to Makefile.
################################################################################

################################################################################
# Compiler
################################################################################

CRY_DEBUG=y
#CRY_OMIT_FRAME_POINTER=y
#CRY_SMALL_SIZE=y
#CRY_STACK_PROTECTOR=y

################################################################################
# Platform
################################################################################

#CRY_ARCH_X86_64=y
#CRY_ARCH_x86=y

################################################################################
# Algorithms
################################################################################

# Options: aes|weak|posix|win
CRY_PRNG=aes

# Use Extended Euclidean Algorithm to compute inverse.
# If not defined the faster Binary Extended Algorithm (HAC 14.4.3) is used.
#CRY_MPI_INV_EUCLID=y

# Small footprint division. Slower.
#CRY_MPI_DIV_SMALL=y

# Comba as baseline multiplier
CRY_MPI_MUL_COMBA=y

# Karatsuba multiplier enabled, see the mpi_mul.c to set the CUTOFF
CRY_MPI_MUL_KARATSUBA=y

