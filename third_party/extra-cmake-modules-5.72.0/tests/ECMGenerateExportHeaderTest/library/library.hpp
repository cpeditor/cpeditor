#include <library_export.h>

enum Enum {
#if LIBRARY_BUILD_DEPRECATED_SINCE(2, 0)
    Enumerator_deprecatedAt2_0,
#endif
    Enumerator_not_deprecated,
};

#if LIBRARY_ENABLE_DEPRECATED_SINCE(2, 0)
LIBRARY_DEPRECATED_VERSION(2, 0, "Deprecated at 2.0")
LIBRARY_EXPORT void function_deprecatedAt2_0();
#endif

#if LIBRARY_ENABLE_DEPRECATED_SINCE(2, 12)
LIBRARY_DEPRECATED_VERSION(2, 12, "Deprecated at 2.12")
LIBRARY_EXPORT void function_deprecatedAt2_12();
#endif

LIBRARY_EXPORT void function_not_deprecated();

class LIBRARY_EXPORT Class {
public:
#if LIBRARY_BUILD_DEPRECATED_SINCE(2, 0)
    LIBRARY_DEPRECATED_VERSION(2, 0, "Deprecated at 2.0")
    virtual void method_deprecatedAt2_0();
#endif

#if LIBRARY_BUILD_DEPRECATED_SINCE(2, 12)
    LIBRARY_DEPRECATED_VERSION(2, 12, "Deprecated at 2.12")
    virtual void method_deprecatedAt2_12();
#endif

    virtual void method_not_deprecated();
};
