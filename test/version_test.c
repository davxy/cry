#include "test.h"
#include <cry/version.h>

void version_test(void)
{
    printf("* Version: %d.%d.%d\n", CRY_MAJOR, CRY_MINOR, CRY_PATCH);
    ASSERT(cry_version() == CRY_VERSION);
    printf("\n");
}
