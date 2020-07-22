#include <stdio.h>
#include "stdint.h"

#define SOME_VAR 1
#ifdef SOME_VAR

#define MULTILINE_MACRO one \
two \
three

static uint64_t intWithSuffix = 42ull + ~1233L;
static int octNum = 07232;
static int invalidOctNum = 09231;
static uint64_t hexNum = 0xDEADBEEF42;
static uint64_t invalidHexNum = 0xGLDFKG;
static char binNum = 0b0101010;

static double d1 = 42. + 0x1.5p+2;
static double d2 = .42;
static double d3 = 42.3e1;
static double d4 = .2e-12;
static double d5 = 32.e+12;
static float floatQualifier = 23.123f;

static const char c1 = 'c';
static const char c2 = '\n';
static const char c2a = '\120'; // octal
static const char c2o = '\0'; // octal, special case
static const char c2b = '\x1f'; // hex
static const char c2c = '\'';
static const char c2d = '\\';
static const char* c3  = "string";
static const char* c4 = "\"string\n\t\012\x12\"";
static const char* c5 = "multiline \
    string";

//BEGIN region
// TODO comment FIXME comment ### comment
//END region

#wrong

/**
 * Doxygen
 * @param p1 text
 * \brief <b>bold text</b>
 * \dot
 * a -> b
 * \enddot
 *
 * \verbatim
 * <dummy>
 * \endverbatim
 * <html>text</html>
 */

#endif

/*
 * Empty lines after a line continuation character (see bug #405903)
 */
#define one \
two \

int i = 0; // this should not be highlighted as a macro
char* str = "string \


int i = 0; // this should not be highlighted as a string

/*
 * Digraphs (see bug #411508)
 */
%:include <stdio.h>
int a()
<%
    [](){%>();
}
