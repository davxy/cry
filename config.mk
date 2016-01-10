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
# Algorithms
################################################################################

# Options: weak|unix|win|strong
CRY_RAND=weak

# Use Extended Euclidean Algorithm to compute inverse.
# If not defined the faster Binary Extended Algorithm (HAC 14.4.3) is used.
#CRY_MPI_INV_EUCLID=y

