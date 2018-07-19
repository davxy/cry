#include "test.h"
#include <cry/version.h>

void version_test(void)
{
    TRACE("CRY version: %d.%d.%d (%d)\n",
            CRY_MAJOR, CRY_MINOR, CRY_PATCH, CRY_VERSION);
    TRACE("CRY version (build-time): %ld\n", cry_version());
    ASSERT(cry_version() == CRY_VERSION);
}
