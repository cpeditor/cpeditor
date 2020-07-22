#include "library.hpp"

#if LIBRARY_BUILD_DEPRECATED_SINCE(2, 0)
void function_deprecatedAt2_0()
{
}
#endif

#if LIBRARY_BUILD_DEPRECATED_SINCE(2, 12)
void function_deprecatedAt2_12()
{
}
#endif

void function_not_deprecated()
{
}

#if LIBRARY_BUILD_DEPRECATED_SINCE(2, 0)
void Class::method_deprecatedAt2_0()
{
}
#endif

#if LIBRARY_BUILD_DEPRECATED_SINCE(2, 12)
void Class::method_deprecatedAt2_12()
{
}
#endif

void Class::method_not_deprecated()
{
}
