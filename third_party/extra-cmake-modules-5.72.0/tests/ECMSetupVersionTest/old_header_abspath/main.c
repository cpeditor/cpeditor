#include <ecm_old_header_abspath_version.h>
#include <string.h>
#include <stdio.h>

#define intcheck(macro,val) \
    if (macro != val) { \
        printf(#macro " was %d instead of %d", macro, val); \
        return 1; \
    }
#define strcheck(macro,val) \
    if (strcmp(macro,val) != 0) { \
        printf(#macro " was %s instead of %s", macro, val); \
        return 1; \
    }

int main()
{
    intcheck(Foo_VERSION_MAJOR,2)
    intcheck(Foo_VERSION_MINOR,3)
    intcheck(Foo_VERSION_PATCH,4)
    intcheck(Foo_VERSION,((2 << 16) + (3 << 8) + 4))
    strcheck(Foo_VERSION_STRING,"2.3.4")
    return 0;
}
