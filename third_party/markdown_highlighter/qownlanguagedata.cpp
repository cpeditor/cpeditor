/*
 * Copyright (c) 2019-2020 Waqar Ahmed -- <waqar.17a@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 */
#include "qownlanguagedata.h"

#include <QLatin1String>
#include <QMultiHash>
/* ------------------------
 * TEMPLATE FOR LANG DATA
 * -------------------------
 *
 * loadXXXData, where XXX is the language
 * keywords are the language keywords e.g, const
 * types are built-in types i.e, int, char, var
 * literals are words like, true false
 * builtin are the library functions
 * other can contain any other thing, for e.g, in cpp it contains the
 preprocessor

    xxx_keywords = {
    };

    xxx_types = {
    };

    xxx_literals = {
    };

    xxx_builtin = {
    };

    xxx_other = {
    };

*/

/**********************************************************/
/* C/C++ Data *********************************************/
/**********************************************************/

static bool cppDataInitialized = false;
static QMultiHash<char, QLatin1String> cpp_keywords;
static QMultiHash<char, QLatin1String> cpp_types;
static QMultiHash<char, QLatin1String> cpp_builtin;
static QMultiHash<char, QLatin1String> cpp_literals;
static QMultiHash<char, QLatin1String> cpp_other;
void initCppData()
{
    cpp_keywords = QMultiHash<char, QLatin1String>{
        {('a'), QLatin1String("asm")},         {('a'), QLatin1String("auto")},
        {('b'), QLatin1String("break")},       {('c'), QLatin1String("case")},
        {('c'), QLatin1String("catch")},       {('c'), QLatin1String("const")},
        {('c'), QLatin1String("const_cast")},  {('c'), QLatin1String("continue")},
        {('d'), QLatin1String("default")},     {('d'), QLatin1String("delete")},
        {('d'), QLatin1String("do")},          {('d'), QLatin1String("dynamic_cast")},
        {('e'), QLatin1String("else")},        {('e'), QLatin1String("explicit")},
        {('f'), QLatin1String("for")},         {('g'), QLatin1String("goto")},
        {('i'), QLatin1String("if")},          {('m'), QLatin1String("mutable")},
        {('n'), QLatin1String("namespace")},   {('n'), QLatin1String("new")},
        {('o'), QLatin1String("operator")},    {('p'), QLatin1String("private")},
        {('p'), QLatin1String("protected")},   {('p'), QLatin1String("public")},
        {('r'), QLatin1String("register")},    {('r'), QLatin1String("reinterpret_cast")},
        {('r'), QLatin1String("return")},      {('s'), QLatin1String("signal")},
        {('s'), QLatin1String("signed")},      {('s'), QLatin1String("sizeof")},
        {('s'), QLatin1String("slot")},        {('s'), QLatin1String("static")},
        {('s'), QLatin1String("static_cast")}, {('s'), QLatin1String("switch")},
        {('t'), QLatin1String("template")},    {('t'), QLatin1String("this")},
        {('t'), QLatin1String("throw")},       {('t'), QLatin1String("try")},
        {('t'), QLatin1String("typedef")},     {('u'), QLatin1String("unsigned")},
        {('u'), QLatin1String("using")},       {('v'), QLatin1String("volatile")},
        {('w'), QLatin1String("while")}};

    cpp_types = {
        {('b'), QLatin1String("bool")},     {('c'), QLatin1String("char")},     {('c'), QLatin1String("class")},
        {('d'), QLatin1String("double")},   {('e'), QLatin1String("enum")},     {('f'), QLatin1String("float")},
        {('i'), QLatin1String("int")},      {('l'), QLatin1String("long")},     {('Q'), QLatin1String("QHash")},
        {('Q'), QLatin1String("QList")},    {('Q'), QLatin1String("QMap")},     {('Q'), QLatin1String("QString")},
        {('Q'), QLatin1String("QVector")},  {('s'), QLatin1String("short")},    {('s'), QLatin1String("size_t")},
        {('s'), QLatin1String("ssize_t")},  {('s'), QLatin1String("struct")},   {('u'), QLatin1String("union")},
        {('u'), QLatin1String("uint8_t")},  {('u'), QLatin1String("uint16_t")}, {('u'), QLatin1String("uint32_t")},
        {('u'), QLatin1String("uint64_t")}, {('v'), QLatin1String("void")},     {('w'), QLatin1String("wchar_t")}};

    cpp_literals = {{('f'), QLatin1String("false")},
                    {('n'), QLatin1String("nullptr")},
                    {('N'), QLatin1String("NULL")},
                    {('t'), QLatin1String("true")}};

    cpp_builtin = {{('s'), QLatin1String("std")},
                   {('s'), QLatin1String("string")},
                   {('w'), QLatin1String("wstring")},
                   {('c'), QLatin1String("cin")},
                   {('c'), QLatin1String("cout")},
                   {('c'), QLatin1String("cerr")},
                   {('c'), QLatin1String("clog")},
                   {('s'), QLatin1String("stdin")},
                   {('s'), QLatin1String("stdout")},
                   {('s'), QLatin1String("stderr")},
                   {('s'), QLatin1String("stringstream")},
                   {('i'), QLatin1String("istringstream")},
                   {('o'), QLatin1String("ostringstream")},
                   {('a'), QLatin1String("auto_ptr")},
                   {('d'), QLatin1String("deque")},
                   {('l'), QLatin1String("list")},
                   {('q'), QLatin1String("queue")},
                   {('s'), QLatin1String("stack")},
                   {('v'), QLatin1String("vector")},
                   {('m'), QLatin1String("map")},
                   {('s'), QLatin1String("set")},
                   {('b'), QLatin1String("bitset")},
                   {('m'), QLatin1String("multiset")},
                   {('m'), QLatin1String("multimap")},
                   {('u'), QLatin1String("unordered_set")},
                   {('u'), QLatin1String("unordered_map")},
                   {('u'), QLatin1String("unordered_multiset")},
                   {('u'), QLatin1String("unordered_multimap")},
                   {('a'), QLatin1String("array")},
                   {('s'), QLatin1String("shared_ptr")},
                   {('a'), QLatin1String("abort")},
                   {('t'), QLatin1String("terminate")},
                   {('a'), QLatin1String("abs")},
                   {('a'), QLatin1String("acos")},
                   {('a'), QLatin1String("asin")},
                   {('a'), QLatin1String("atan2")},
                   {('a'), QLatin1String("atan")},
                   {('c'), QLatin1String("calloc")},
                   {('c'), QLatin1String("ceil")},
                   {('c'), QLatin1String("cosh")},
                   {('c'), QLatin1String("cos")},
                   {('e'), QLatin1String("exit")},
                   {('e'), QLatin1String("exp")},
                   {('f'), QLatin1String("fabs")},
                   {('f'), QLatin1String("floor")},
                   {('f'), QLatin1String("fmod")},
                   {('f'), QLatin1String("fprintf")},
                   {('f'), QLatin1String("fputs")},
                   {('f'), QLatin1String("free")},
                   {('f'), QLatin1String("frexp")},
                   {('f'), QLatin1String("fscanf")},
                   {('f'), QLatin1String("future")},
                   {('i'), QLatin1String("isalnum")},
                   {('i'), QLatin1String("isalpha")},
                   {('i'), QLatin1String("iscntrl")},
                   {('i'), QLatin1String("isdigit")},
                   {('i'), QLatin1String("isgraph")},
                   {('i'), QLatin1String("islower")},
                   {('i'), QLatin1String("isprint")},
                   {('i'), QLatin1String("ispunct")},
                   {('i'), QLatin1String("isspace")},
                   {('i'), QLatin1String("isupper")},
                   {('i'), QLatin1String("isxdigit")},
                   {('t'), QLatin1String("tolower")},
                   {('t'), QLatin1String("toupper")},
                   {('l'), QLatin1String("labs")},
                   {('l'), QLatin1String("ldexp")},
                   {('l'), QLatin1String("log10")},
                   {('l'), QLatin1String("log")},
                   {('m'), QLatin1String("malloc")},
                   {('r'), QLatin1String("realloc")},
                   {('m'), QLatin1String("main")},
                   {('m'), QLatin1String("memchr")},
                   {('m'), QLatin1String("memcmp")},
                   {('m'), QLatin1String("memcpy")},
                   {('m'), QLatin1String("memset")},
                   {('m'), QLatin1String("modf")},
                   {('p'), QLatin1String("pow")},
                   {('p'), QLatin1String("printf")},
                   {('p'), QLatin1String("putchar")},
                   {('p'), QLatin1String("puts")},
                   {('s'), QLatin1String("scanf")},
                   {('s'), QLatin1String("sinh")},
                   {('s'), QLatin1String("sin")},
                   {('s'), QLatin1String("snprintf")},
                   {('s'), QLatin1String("sprintf")},
                   {('s'), QLatin1String("sqrt")},
                   {('s'), QLatin1String("sscanf")},
                   {('s'), QLatin1String("strcat")},
                   {('s'), QLatin1String("strchr")},
                   {('s'), QLatin1String("strcmp")},
                   {('s'), QLatin1String("strcpy")},
                   {('s'), QLatin1String("strcspn")},
                   {('s'), QLatin1String("strlen")},
                   {('s'), QLatin1String("strncat")},
                   {('s'), QLatin1String("strncmp")},
                   {('s'), QLatin1String("strncpy")},
                   {('s'), QLatin1String("strpbrk")},
                   {('s'), QLatin1String("strrchr")},
                   {('s'), QLatin1String("strspn")},
                   {('s'), QLatin1String("strstr")},
                   {('t'), QLatin1String("tanh")},
                   {('t'), QLatin1String("tan")},
                   {('v'), QLatin1String("vfprintf")},
                   {('v'), QLatin1String("vprintf")},
                   {('v'), QLatin1String("vsprintf")},
                   {('e'), QLatin1String("endl")},
                   {('i'), QLatin1String("initializer_list")},
                   {('u'), QLatin1String("unique_ptr")},
                   {('c'), QLatin1String("complex")},
                   {('i'), QLatin1String("imaginary")}};

    cpp_other = {{('d'), QLatin1String("define")}, {('e'), QLatin1String("else")},   {('e'), QLatin1String("elif")},
                 {('e'), QLatin1String("endif")},  {('e'), QLatin1String("error")},  {('i'), QLatin1String("if")},
                 {('i'), QLatin1String("ifdef")},  {('i'), QLatin1String("ifndef")}, {('i'), QLatin1String("include")},
                 {('l'), QLatin1String("line")},   {('p'), QLatin1String("pragma")}, {('P'), QLatin1String("_Pragma")},
                 {('u'), QLatin1String("undef")},  {('w'), QLatin1String("warning")}};
}
void loadCppData(QMultiHash<char, QLatin1String> &typess, QMultiHash<char, QLatin1String> &keywordss,
                 QMultiHash<char, QLatin1String> &builtins, QMultiHash<char, QLatin1String> &literalss,
                 QMultiHash<char, QLatin1String> &others)
{
    if (!cppDataInitialized)
    {
        initCppData();
        cppDataInitialized = true;
    }

    typess = cpp_types;
    keywordss = cpp_keywords;
    builtins = cpp_builtin;
    literalss = cpp_literals;
    others = cpp_other;
}

/**********************************************************/
/* Shell Data *********************************************/
/**********************************************************/

static bool shellDataInitialized = false;
static QMultiHash<char, QLatin1String> shell_keywords;
static QMultiHash<char, QLatin1String> shell_types;
static QMultiHash<char, QLatin1String> shell_literals;
static QMultiHash<char, QLatin1String> shell_builtin;
static QMultiHash<char, QLatin1String> shell_other;
void initShellData()
{
    shell_keywords = {{('i'), QLatin1String("if")},      {('t'), QLatin1String("then")}, {('e'), QLatin1String("else")},
                      {('e'), QLatin1String("elif")},    {('f'), QLatin1String("fi")},   {('f'), QLatin1String("for")},
                      {('w'), QLatin1String("while")},   {('i'), QLatin1String("in")},   {('d'), QLatin1String("do")},
                      {('d'), QLatin1String("done")},    {('c'), QLatin1String("case")}, {('e'), QLatin1String("esac")},
                      {('f'), QLatin1String("function")}};

    shell_types = {};

    shell_literals = {{('f'), QLatin1String("false")}, {('t'), QLatin1String("true")}};

    shell_builtin = {{('b'), QLatin1String("break")},
                     {('c'), QLatin1String("cd")},
                     {('c'), QLatin1String("continue")},
                     {('e'), QLatin1String("eval")},
                     {('e'), QLatin1String("exec")},
                     {('e'), QLatin1String("exit")},
                     {('e'), QLatin1String("export")},
                     {('g'), QLatin1String("getopts")},
                     {('h'), QLatin1String("hash")},
                     {('p'), QLatin1String("pwd")},
                     {('r'), QLatin1String("readonly")},
                     {('r'), QLatin1String("return")},
                     {('s'), QLatin1String("shift")},
                     {('t'), QLatin1String("test")},
                     {('t'), QLatin1String("timestrap")},
                     {('u'), QLatin1String("umask")},
                     {('u'), QLatin1String("unset")},
                     {('B'), QLatin1String("Bash")},
                     {('a'), QLatin1String("alias")},
                     {('b'), QLatin1String("bind")},
                     {('b'), QLatin1String("builtin")},
                     {('c'), QLatin1String("caller")},
                     {('c'), QLatin1String("command")},
                     {('d'), QLatin1String("declare")},
                     {('e'), QLatin1String("echo")},
                     {('e'), QLatin1String("enable")},
                     {('h'), QLatin1String("help")},
                     {('l'), QLatin1String("let")},
                     {('l'), QLatin1String("local")},
                     {('l'), QLatin1String("logout")},
                     {('m'), QLatin1String("mapfile")},
                     {('p'), QLatin1String("printfread")},
                     {('r'), QLatin1String("readarray")},
                     {('s'), QLatin1String("source")},
                     {('t'), QLatin1String("type")},
                     {('t'), QLatin1String("typeset")},
                     {('u'), QLatin1String("ulimit")},
                     {('u'), QLatin1String("unalias")},
                     {('m'), QLatin1String("modifiers")},
                     {('s'), QLatin1String("set")},
                     {('s'), QLatin1String("shopt")},
                     {('a'), QLatin1String("autoload")},
                     {('b'), QLatin1String("bg")},
                     {('b'), QLatin1String("bindkey")},
                     {('b'), QLatin1String("bye")},
                     {('c'), QLatin1String("cap")},
                     {('c'), QLatin1String("chdir")},
                     {('c'), QLatin1String("clone")},
                     {('c'), QLatin1String("comparguments")},
                     {('c'), QLatin1String("compcall")},
                     {('c'), QLatin1String("compctl")},
                     {('c'), QLatin1String("compdescribe")},
                     {('c'), QLatin1String("compfilescompgroups")},
                     {('c'), QLatin1String("compquote")},
                     {('c'), QLatin1String("comptags")},
                     {('c'), QLatin1String("comptry")},
                     {('c'), QLatin1String("compvalues")},
                     {('d'), QLatin1String("dirs")},
                     {('d'), QLatin1String("disable")},
                     {('d'), QLatin1String("disown")},
                     {('e'), QLatin1String("echotc")},
                     {('e'), QLatin1String("echoti")},
                     {('e'), QLatin1String("emulatefc")},
                     {('f'), QLatin1String("fg")},
                     {('f'), QLatin1String("float")},
                     {('f'), QLatin1String("functions")},
                     {('g'), QLatin1String("getcap")},
                     {('g'), QLatin1String("getln")},
                     {('h'), QLatin1String("history")},
                     {('i'), QLatin1String("integer")},
                     {('j'), QLatin1String("jobs")},
                     {('k'), QLatin1String("kill")},
                     {('l'), QLatin1String("limit")},
                     {('l'), QLatin1String("log")},
                     {('n'), QLatin1String("noglob")},
                     {('p'), QLatin1String("popd")},
                     {('p'), QLatin1String("printpushd")},
                     {('p'), QLatin1String("pushln")},
                     {('r'), QLatin1String("rehash")},
                     {('s'), QLatin1String("sched")},
                     {('s'), QLatin1String("setcap")},
                     {('s'), QLatin1String("setopt")},
                     {('s'), QLatin1String("stat")},
                     {('s'), QLatin1String("suspend")},
                     {('t'), QLatin1String("ttyctl")},
                     {('u'), QLatin1String("unfunction")},
                     {('u'), QLatin1String("unhash")},
                     {('u'), QLatin1String("unlimitunsetopt")},
                     {('v'), QLatin1String("vared")},
                     {('w'), QLatin1String("wait")},
                     {('w'), QLatin1String("whence")},
                     {('w'), QLatin1String("where")},
                     {('w'), QLatin1String("which")},
                     {('z'), QLatin1String("zcompile")},
                     {('z'), QLatin1String("zformat")},
                     {('z'), QLatin1String("zftp")},
                     {('z'), QLatin1String("zle")},
                     {('z'), QLatin1String("zmodload")},
                     {('z'), QLatin1String("zparseopts")},
                     {('z'), QLatin1String("zprof")},
                     {('z'), QLatin1String("zpty")},
                     {('z'), QLatin1String("zregexparse")},
                     {('z'), QLatin1String("zsocket")},
                     {('z'), QLatin1String("zstyle")},
                     {('z'), QLatin1String("ztcp")},
                     {('g'), QLatin1String("git")},
                     {('r'), QLatin1String("rm")},
                     {('s'), QLatin1String("sudo")},
                     {('f'), QLatin1String("fdisk")},
                     {('a'), QLatin1String("apt")},
                     {('s'), QLatin1String("snap")},
                     {('f'), QLatin1String("flatpak")},
                     {('s'), QLatin1String("snapcraft")},
                     {('y'), QLatin1String("yaourt")},
                     {('n'), QLatin1String("nmcli")},
                     {('p'), QLatin1String("pacman")},
                     {('p'), QLatin1String("pamac")},
                     {('f'), QLatin1String("fsck")},
                     {('m'), QLatin1String("mount")},
                     {('m'), QLatin1String("mkdir")},
                     {('m'), QLatin1String("mkswap")},
                     {('s'), QLatin1String("sleep")},
                     {('l'), QLatin1String("ls")},
                     {('w'), QLatin1String("wget")},
                     {('k'), QLatin1String("kill")},
                     {('k'), QLatin1String("killall")},
                     {('g'), QLatin1String("gdb")},
                     {('Q'), QLatin1String("QOwnNotes")},
                     {('q'), QLatin1String("qownnotes")},
                     {('d'), QLatin1String("docker")},
                     {('o'), QLatin1String("openssl")},
                     {('p'), QLatin1String("php")},
                     {('p'), QLatin1String("python")},
                     {('p'), QLatin1String("perl")},
                     {('g'), QLatin1String("go")},
                     {('c'), QLatin1String("curl")}};

    shell_other = {};
}

void loadShellData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                   QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                   QMultiHash<char, QLatin1String> &other)
{
    if (!shellDataInitialized)
    {
        initShellData();
        shellDataInitialized = true;
    }
    types = shell_types;
    keywords = shell_keywords;
    builtin = shell_builtin;
    literals = shell_literals;
    other = shell_other;
}

/**********************************************************/
/* JS Data *********************************************/
/**********************************************************/
static bool JSDataInitialized = false;
static QMultiHash<char, QLatin1String> js_keywords;
static QMultiHash<char, QLatin1String> js_types;
static QMultiHash<char, QLatin1String> js_literals;
static QMultiHash<char, QLatin1String> js_builtin;
static QMultiHash<char, QLatin1String> js_other;
void initJSData()
{
    js_keywords = {
        {('i'), QLatin1String("in")},       {('o'), QLatin1String("of")},       {('i'), QLatin1String("if")},
        {('f'), QLatin1String("for")},      {('w'), QLatin1String("while")},    {('f'), QLatin1String("finally")},
        {('n'), QLatin1String("new")},      {('f'), QLatin1String("function")}, {('d'), QLatin1String("do")},
        {('r'), QLatin1String("return")},   {('v'), QLatin1String("void")},     {('e'), QLatin1String("else")},
        {('b'), QLatin1String("break")},    {('c'), QLatin1String("catch")},    {('i'), QLatin1String("instanceof")},
        {('w'), QLatin1String("with")},     {('t'), QLatin1String("throw")},    {('c'), QLatin1String("case")},
        {('d'), QLatin1String("default")},  {('t'), QLatin1String("try")},      {('t'), QLatin1String("this")},
        {('s'), QLatin1String("switch")},   {('c'), QLatin1String("continue")}, {('t'), QLatin1String("typeof")},
        {('d'), QLatin1String("delete")},   {('l'), QLatin1String("let")},      {('y'), QLatin1String("yield")},
        {('c'), QLatin1String("const")},    {('e'), QLatin1String("export")},   {('s'), QLatin1String("super")},
        {('d'), QLatin1String("debugger")}, {('a'), QLatin1String("as")},       {('a'), QLatin1String("async")},
        {('a'), QLatin1String("await")},    {('s'), QLatin1String("static")},   {('i'), QLatin1String("import")},
        {('f'), QLatin1String("from")},     {('a'), QLatin1String("as")}};

    js_types = {{('v'), QLatin1String("var")},     {('c'), QLatin1String("class")}, {('b'), QLatin1String("byte")},
                {('e'), QLatin1String("enum")},    {('f'), QLatin1String("float")}, {('s'), QLatin1String("short")},
                {('l'), QLatin1String("long")},    {('i'), QLatin1String("int")},   {('v'), QLatin1String("void")},
                {('b'), QLatin1String("boolean")}, {('d'), QLatin1String("double")}};

    js_literals = {{('f'), QLatin1String("false")}, {('n'), QLatin1String("null")},
                   {('t'), QLatin1String("true")},  {('u'), QLatin1String("undefined")},
                   {('N'), QLatin1String("NaN")},   {('I'), QLatin1String("Infinity")}};

    js_builtin = {{('e'), QLatin1String("eval")},
                  {('i'), QLatin1String("isFinite")},
                  {('i'), QLatin1String("isNaN")},
                  {('p'), QLatin1String("parseFloat")},
                  {('p'), QLatin1String("parseInt")},
                  {('d'), QLatin1String("decodeURI")},
                  {('d'), QLatin1String("decodeURIComponent")},
                  {('e'), QLatin1String("encodeURI")},
                  {('e'), QLatin1String("encodeURIComponent")},
                  {('e'), QLatin1String("escape")},
                  {('u'), QLatin1String("unescape")},
                  {('O'), QLatin1String("Object")},
                  {('F'), QLatin1String("Function")},
                  {('B'), QLatin1String("Boolean")},
                  {('E'), QLatin1String("Error")},
                  {('E'), QLatin1String("EvalError")},
                  {('I'), QLatin1String("InternalError")},
                  {('R'), QLatin1String("RangeError")},
                  {('R'), QLatin1String("ReferenceError")},
                  {('S'), QLatin1String("StopIteration")},
                  {('S'), QLatin1String("SyntaxError")},
                  {('T'), QLatin1String("TypeError")},
                  {('U'), QLatin1String("URIError")},
                  {('N'), QLatin1String("Number")},
                  {('M'), QLatin1String("Math")},
                  {('D'), QLatin1String("Date")},
                  {('S'), QLatin1String("String")},
                  {('R'), QLatin1String("RegExp")},
                  {('A'), QLatin1String("Array")},
                  {('F'), QLatin1String("Float32Array")},
                  {('F'), QLatin1String("Float64Array")},
                  {('I'), QLatin1String("Int16Array")},
                  {('I'), QLatin1String("Int32Array")},
                  {('I'), QLatin1String("Int8Array")},
                  {('U'), QLatin1String("Uint16Array")},
                  {('U'), QLatin1String("Uint32Array")},
                  {('U'), QLatin1String("Uint8Array")},
                  {('U'), QLatin1String("Uint8ClampedArray")},
                  {('A'), QLatin1String("ArrayBuffer")},
                  {('D'), QLatin1String("DataView")},
                  {('J'), QLatin1String("JSON")},
                  {('I'), QLatin1String("Intl")},
                  {('a'), QLatin1String("arguments")},
                  {('r'), QLatin1String("require")},
                  {('m'), QLatin1String("module")},
                  {('c'), QLatin1String("console")},
                  {('w'), QLatin1String("window")},
                  {('d'), QLatin1String("document")},
                  {('S'), QLatin1String("Symbol")},
                  {('S'), QLatin1String("Set")},
                  {('M'), QLatin1String("Map")},
                  {('W'), QLatin1String("WeakSet")},
                  {('W'), QLatin1String("WeakMap")},
                  {('P'), QLatin1String("Proxy")},
                  {('R'), QLatin1String("Reflect")},
                  {('P'), QLatin1String("Promise")}};

    js_other = {};
}

void loadJSData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                QMultiHash<char, QLatin1String> &other)
{
    if (!JSDataInitialized)
    {
        initJSData();
        JSDataInitialized = true;
    }
    types = js_types;
    keywords = js_keywords;
    builtin = js_builtin;
    literals = js_literals;
    other = js_other;
}

/**********************************************************/
/* PHP Data *********************************************/
/**********************************************************/
static bool PHPDataInitialized = false;
static QMultiHash<char, QLatin1String> php_keywords;
static QMultiHash<char, QLatin1String> php_types;
static QMultiHash<char, QLatin1String> php_literals;
static QMultiHash<char, QLatin1String> php_builtin;
static QMultiHash<char, QLatin1String> php_other;
void initPHPData()
{
    php_keywords = {
        {('a'), QLatin1String("and")},        {('l'), QLatin1String("list")},     {('a'), QLatin1String("abstract")},
        {('g'), QLatin1String("global")},     {('p'), QLatin1String("private")},  {('e'), QLatin1String("echo")},
        {('i'), QLatin1String("interface")},  {('a'), QLatin1String("as")},       {('s'), QLatin1String("static")},
        {('e'), QLatin1String("endswitch")},  {('i'), QLatin1String("if")},       {('e'), QLatin1String("endwhile")},
        {('o'), QLatin1String("or")},         {('c'), QLatin1String("const")},    {('f'), QLatin1String("for")},
        {('e'), QLatin1String("endforeach")}, {('s'), QLatin1String("self")},     {('w'), QLatin1String("while")},
        {('i'), QLatin1String("isset")},      {('p'), QLatin1String("public")},   {('p'), QLatin1String("protected")},
        {('e'), QLatin1String("exit")},       {('f'), QLatin1String("foreach")},  {('t'), QLatin1String("throw")},
        {('e'), QLatin1String("elseif")},     {('e'), QLatin1String("empty")},    {('d'), QLatin1String("do")},
        {('x'), QLatin1String("xor")},        {('r'), QLatin1String("return")},   {('p'), QLatin1String("parent")},
        {('c'), QLatin1String("clone")},      {('u'), QLatin1String("use")},      {('e'), QLatin1String("else")},
        {('b'), QLatin1String("break")},      {('p'), QLatin1String("print")},    {('e'), QLatin1String("eval")},
        {('n'), QLatin1String("new")},        {('c'), QLatin1String("catch")},    {('c'), QLatin1String("case")},
        {('e'), QLatin1String("exception")},  {('d'), QLatin1String("default")},  {('d'), QLatin1String("die")},
        {('e'), QLatin1String("enddeclare")}, {('f'), QLatin1String("final")},    {('t'), QLatin1String("try")},
        {('s'), QLatin1String("switch")},     {('c'), QLatin1String("continue")}, {('e'), QLatin1String("endfor")},
        {('e'), QLatin1String("endif")},      {('d'), QLatin1String("declare")},  {('u'), QLatin1String("unset")},
        {('t'), QLatin1String("trait")},      {('g'), QLatin1String("goto")},     {('i'), QLatin1String("instanceof")},
        {('i'), QLatin1String("insteadof")},  {('y'), QLatin1String("yield")},    {('f'), QLatin1String("finally")}};

    php_types = {{('v'), QLatin1String("var")},
                 {('c'), QLatin1String("class")},
                 {('e'), QLatin1String("enum")},
                 {('a'), QLatin1String("array")}};

    php_literals = {{('f'), QLatin1String("false")}, {('t'), QLatin1String("true")}, {('n'), QLatin1String("null")}};

    php_builtin = {

    };

    php_other = {{('i'), QLatin1String("include_once")},  {('i'), QLatin1String("include")},
                 {('_'), QLatin1String("__FILE__")},      {('r'), QLatin1String("require")},
                 {('r'), QLatin1String("require_once")},  {('_'), QLatin1String("__CLASS__")},
                 {('_'), QLatin1String("__LINE__")},      {('_'), QLatin1String("__METHOD__")},
                 {('_'), QLatin1String("__FUNCTION__")},  {('_'), QLatin1String("__DIR__")},
                 {('_'), QLatin1String("__NAMESPACE__")},

                 {('S'), QLatin1String("SERVER")},        {('G'), QLatin1String("GET")},
                 {('P'), QLatin1String("POST")},          {('F'), QLatin1String("FILES")},
                 {('R'), QLatin1String("REQUEST")},       {('S'), QLatin1String("SESSION")},
                 {('E'), QLatin1String("ENV")},           {('C'), QLatin1String("COOKIE")},
                 {('G'), QLatin1String("GLOBALS")},       {('H'), QLatin1String("HTTP_RAW_POST_DATA")},
                 {('a'), QLatin1String("argc")},          {('a'), QLatin1String("argv")},
                 {('p'), QLatin1String("php_errormsg")},  {('h'), QLatin1String("http_response_header")}};
}
void loadPHPData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                 QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                 QMultiHash<char, QLatin1String> &other)
{
    if (!PHPDataInitialized)
    {
        initPHPData();
        PHPDataInitialized = true;
    }
    types = php_types;
    keywords = php_keywords;
    builtin = php_builtin;
    literals = php_literals;
    other = php_other;
}

/**********************************************************/
/* QML Data *********************************************/
/**********************************************************/
static bool QMLDataInitialized = false;
static QMultiHash<char, QLatin1String> qml_keywords;
static QMultiHash<char, QLatin1String> qml_types;
static QMultiHash<char, QLatin1String> qml_literals;
static QMultiHash<char, QLatin1String> qml_builtin;
static QMultiHash<char, QLatin1String> qml_other;

void initQMLData()
{
    qml_keywords = {{('d'), QLatin1String("default")},
                    {('p'), QLatin1String("property")},
                    {('i'), QLatin1String("int")},
                    {('v'), QLatin1String("var")},
                    {('s'), QLatin1String("string")},
                    {('f'), QLatin1String("function")},
                    {('r'), QLatin1String("readonly")},
                    {('M'), QLatin1String("MouseArea")},
                    {('d'), QLatin1String("delegate")},
                    {('i'), QLatin1String("if")},
                    {('e'), QLatin1String("else")},

                    {('e'), QLatin1String("eval")},
                    {('i'), QLatin1String("isFinite")},
                    {('i'), QLatin1String("isNaN")},
                    {('p'), QLatin1String("parseFloat")},
                    {('p'), QLatin1String("parseInt")},
                    {('d'), QLatin1String("decodeURI")},
                    {('d'), QLatin1String("decodeURIComponent")},
                    {('e'), QLatin1String("encodeURI")},
                    {('e'), QLatin1String("encodeURIComponent")},
                    {('e'), QLatin1String("escape")},
                    {('u'), QLatin1String("unescape")},
                    {('O'), QLatin1String("Object")},
                    {('E'), QLatin1String("Error")},
                    {('E'), QLatin1String("EvalError")},
                    {('I'), QLatin1String("InternalError")},
                    {('R'), QLatin1String("RangeError")},
                    {('R'), QLatin1String("ReferenceError")},
                    {('S'), QLatin1String("StopIteration")},
                    {('S'), QLatin1String("SyntaxError")},
                    {('T'), QLatin1String("TypeError")},
                    {('U'), QLatin1String("URIError")},
                    {('N'), QLatin1String("Number")},
                    {('M'), QLatin1String("Math")},
                    {('D'), QLatin1String("Date")},
                    {('S'), QLatin1String("String")},
                    {('R'), QLatin1String("RegExp")},
                    {('A'), QLatin1String("Array")},
                    {('F'), QLatin1String("Float32Array")},
                    {('F'), QLatin1String("Float64Array")},
                    {('I'), QLatin1String("Int16Array")},
                    {('I'), QLatin1String("Int32Array")},
                    {('I'), QLatin1String("Int8Array")},
                    {('U'), QLatin1String("Uint16Array")},
                    {('U'), QLatin1String("Uint32Array")},
                    {('U'), QLatin1String("Uint8Array")},
                    {('U'), QLatin1String("Uint8ClampedArray")},
                    {('A'), QLatin1String("ArrayBuffer")},
                    {('D'), QLatin1String("DataView")},
                    {('J'), QLatin1String("JSON")},
                    {('I'), QLatin1String("Intl")},
                    {('a'), QLatin1String("arguments")},
                    {('m'), QLatin1String("module")},
                    {('c'), QLatin1String("console")},
                    {('w'), QLatin1String("window")},
                    {('d'), QLatin1String("document")},
                    {('S'), QLatin1String("Symbol")},
                    {('S'), QLatin1String("Set")},
                    {('M'), QLatin1String("Map")},
                    {('W'), QLatin1String("WeakSet")},
                    {('W'), QLatin1String("WeakMap")},
                    {('P'), QLatin1String("Proxy")},
                    {('R'), QLatin1String("Reflect")},
                    {('B'), QLatin1String("Behavior")},
                    {('c'), QLatin1String("color")},
                    {('c'), QLatin1String("coordinate")},
                    {('d'), QLatin1String("date")},
                    {('e'), QLatin1String("enumeration")},
                    {('f'), QLatin1String("font")},
                    {('g'), QLatin1String("geocircle")},
                    {('g'), QLatin1String("georectangle")},
                    {('g'), QLatin1String("geoshape")},
                    {('l'), QLatin1String("list")},
                    {('m'), QLatin1String("matrix4x4")},
                    {('p'), QLatin1String("parent")},
                    {('p'), QLatin1String("point")},
                    {('q'), QLatin1String("quaternion")},
                    {('r'), QLatin1String("real")},
                    {('s'), QLatin1String("size")},
                    {('s'), QLatin1String("string")},
                    {('v'), QLatin1String("variant")},
                    {('v'), QLatin1String("vector2d")},
                    {('v'), QLatin1String("vector3d")},
                    {('v'), QLatin1String("vector4d")},
                    {('P'), QLatin1String("Promise")}};

    qml_types = {
        {('R'), QLatin1String("Rectangle")}, {('T'), QLatin1String("Text")},      {('c'), QLatin1String("color")},
        {('I'), QLatin1String("Item")},      {('u'), QLatin1String("url")},       {('C'), QLatin1String("Component")},
        {('B'), QLatin1String("Button")},    {('T'), QLatin1String("TextInput")}, {('L'), QLatin1String("ListView")},

    };

    qml_literals = {{('f'), QLatin1String("false")}, {('t'), QLatin1String("true")}};

    qml_builtin = {

    };

    qml_other = {{('i'), QLatin1String("import")}};
}
void loadQMLData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                 QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                 QMultiHash<char, QLatin1String> &other)
{
    if (!QMLDataInitialized)
    {
        initQMLData();
        QMLDataInitialized = true;
    }
    types = qml_types;
    keywords = qml_keywords;
    builtin = qml_builtin;
    literals = qml_literals;
    other = qml_other;
}

/**********************************************************/
/* Python Data *********************************************/
/**********************************************************/
static bool PyDataInitialized = false;
static QMultiHash<char, QLatin1String> py_keywords;
static QMultiHash<char, QLatin1String> py_types;
static QMultiHash<char, QLatin1String> py_literals;
static QMultiHash<char, QLatin1String> py_builtin;
static QMultiHash<char, QLatin1String> py_other;

void initPyData()
{
    py_keywords = {
        {('a'), QLatin1String("and")},    {('e'), QLatin1String("elif")},     {('i'), QLatin1String("is")},
        {('g'), QLatin1String("global")}, {('a'), QLatin1String("as")},       {('i'), QLatin1String("in")},
        {('i'), QLatin1String("if")},     {('f'), QLatin1String("from")},     {('r'), QLatin1String("raise")},
        {('f'), QLatin1String("for")},    {('e'), QLatin1String("except")},   {('f'), QLatin1String("finally")},
        {('p'), QLatin1String("print")},  {('p'), QLatin1String("pass")},     {('r'), QLatin1String("return")},
        {('e'), QLatin1String("exec")},   {('e'), QLatin1String("else")},     {('b'), QLatin1String("break")},
        {('n'), QLatin1String("not")},    {('w'), QLatin1String("with")},     {('c'), QLatin1String("class")},
        {('a'), QLatin1String("assert")}, {('y'), QLatin1String("yield")},    {('t'), QLatin1String("try")},
        {('w'), QLatin1String("while")},  {('c'), QLatin1String("continue")}, {('d'), QLatin1String("del")},
        {('o'), QLatin1String("or")},     {('d'), QLatin1String("def")},      {('l'), QLatin1String("lambda")},
        {('a'), QLatin1String("async")},  {('a'), QLatin1String("await")},    {('n'), QLatin1String("nonlocal")},
    };

    py_types = {

    };

    py_literals = {{('F'), QLatin1String("False")}, {('T'), QLatin1String("True")}, {('N'), QLatin1String("None")}};

    py_builtin = {{('_'), QLatin1String("__import__")},   {('a'), QLatin1String("abs")},
                  {('a'), QLatin1String("all")},          {('a'), QLatin1String("any")},
                  {('a'), QLatin1String("apply")},        {('a'), QLatin1String("ascii")},
                  {('b'), QLatin1String("basestring")},   {('b'), QLatin1String("bin")},
                  {('b'), QLatin1String("bool")},         {('b'), QLatin1String("buffer")},
                  {('b'), QLatin1String("bytearray")},    {('b'), QLatin1String("bytes")},
                  {('c'), QLatin1String("callable")},     {('c'), QLatin1String("chr")},
                  {('c'), QLatin1String("classmethod")},  {('c'), QLatin1String("cmp")},
                  {('c'), QLatin1String("coerce")},       {('c'), QLatin1String("compile")},
                  {('c'), QLatin1String("complex")},      {('d'), QLatin1String("delattr")},
                  {('d'), QLatin1String("dict")},         {('d'), QLatin1String("dir")},
                  {('d'), QLatin1String("divmod")},       {('e'), QLatin1String("enumerate")},
                  {('e'), QLatin1String("eval")},         {('e'), QLatin1String("execfile")},
                  {('f'), QLatin1String("file")},         {('f'), QLatin1String("filter")},
                  {('f'), QLatin1String("float")},        {('f'), QLatin1String("format")},
                  {('f'), QLatin1String("frozenset")},    {('g'), QLatin1String("getattr")},
                  {('g'), QLatin1String("globals")},      {('h'), QLatin1String("hasattr")},
                  {('h'), QLatin1String("hash")},         {('h'), QLatin1String("help")},
                  {('h'), QLatin1String("hex")},          {('i'), QLatin1String("id")},
                  {('i'), QLatin1String("input")},        {('i'), QLatin1String("int")},
                  {('i'), QLatin1String("intern")},       {('i'), QLatin1String("isinstance")},
                  {('i'), QLatin1String("issubclass")},   {('i'), QLatin1String("iter")},
                  {('l'), QLatin1String("len")},          {('l'), QLatin1String("list")},
                  {('l'), QLatin1String("locals")},       {('l'), QLatin1String("long")},
                  {('m'), QLatin1String("map")},          {('m'), QLatin1String("max")},
                  {('m'), QLatin1String("memoryview")},   {('m'), QLatin1String("min")},
                  {('n'), QLatin1String("next")},         {('o'), QLatin1String("object")},
                  {('o'), QLatin1String("oct")},          {('o'), QLatin1String("open")},
                  {('o'), QLatin1String("ord")},          {('p'), QLatin1String("pow")},
                  {('p'), QLatin1String("property")},     {('r'), QLatin1String("range")},
                  {('r'), QLatin1String("raw_input")},    {('r'), QLatin1String("reduce")},
                  {('r'), QLatin1String("reload")},       {('r'), QLatin1String("repr")},
                  {('r'), QLatin1String("reversed")},     {('r'), QLatin1String("round")},
                  {('s'), QLatin1String("set")},          {('s'), QLatin1String("setattr")},
                  {('s'), QLatin1String("slice")},        {('s'), QLatin1String("sorted")},
                  {('s'), QLatin1String("staticmethod")}, {('s'), QLatin1String("str")},
                  {('s'), QLatin1String("sum")},          {('s'), QLatin1String("super")},
                  {('t'), QLatin1String("tuple")},        {('t'), QLatin1String("type")},
                  {('u'), QLatin1String("unichr")},       {('u'), QLatin1String("unicode")},
                  {('v'), QLatin1String("vars")},         {('x'), QLatin1String("xrange")},
                  {('z'), QLatin1String("zip")}};

    py_other = {{('i'), QLatin1String("import")}};
}
void loadPythonData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                    QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                    QMultiHash<char, QLatin1String> &other)
{
    if (!PyDataInitialized)
    {
        initPyData();
        PyDataInitialized = true;
    }
    types = py_types;
    keywords = py_keywords;
    builtin = py_builtin;
    literals = py_literals;
    other = py_other;
}

/********************************************************/
/***   Rust DATA      ***********************************/
/********************************************************/
static bool rustDataInitialized = false;
static QMultiHash<char, QLatin1String> rust_keywords;
static QMultiHash<char, QLatin1String> rust_types;
static QMultiHash<char, QLatin1String> rust_literals;
static QMultiHash<char, QLatin1String> rust_builtin;
static QMultiHash<char, QLatin1String> rust_other;
void initRustData()
{
    rust_keywords = {
        {('a'), QLatin1String("abstract")}, {('a'), QLatin1String("alignof")}, {('a'), QLatin1String("as")},
        {('a'), QLatin1String("async")},    {('a'), QLatin1String("await")},   {('b'), QLatin1String("be")},
        {('b'), QLatin1String("box")},      {('b'), QLatin1String("break")},   {('c'), QLatin1String("const")},
        {('c'), QLatin1String("continue")}, {('c'), QLatin1String("crate")},   {('d'), QLatin1String("do")},
        {('d'), QLatin1String("dyn")},      {('e'), QLatin1String("else")},    {('e'), QLatin1String("extern")},
        {('f'), QLatin1String("final")},    {('f'), QLatin1String("fn")},      {('f'), QLatin1String("for")},
        {('i'), QLatin1String("if")},       {('i'), QLatin1String("impl")},    {('i'), QLatin1String("in")},
        {('l'), QLatin1String("let")},      {('l'), QLatin1String("loop")},    {('m'), QLatin1String("match")},
        {('m'), QLatin1String("mod")},      {('m'), QLatin1String("move")},    {('m'), QLatin1String("mut")},
        {('o'), QLatin1String("offsetof")}, {('o'), QLatin1String("once")},    {('o'), QLatin1String("override")},
        {('p'), QLatin1String("priv")},     {('p'), QLatin1String("pub")},     {('p'), QLatin1String("pure")},
        {('r'), QLatin1String("ref")},      {('r'), QLatin1String("return")},  {('s'), QLatin1String("sizeof")},
        {('s'), QLatin1String("static")},   {('s'), QLatin1String("self")},    {('S'), QLatin1String("Self")},
        {('s'), QLatin1String("super")},    {('t'), QLatin1String("trait")},   {('t'), QLatin1String("type")},
        {('t'), QLatin1String("typeof")},   {('u'), QLatin1String("unsafe")},  {('u'), QLatin1String("unsized")},
        {('u'), QLatin1String("use")},      {('v'), QLatin1String("virtual")}, {('w'), QLatin1String("where")},
        {('w'), QLatin1String("while")},    {('y'), QLatin1String("yield")},
    };

    rust_types = {{('u'), QLatin1String("union")},  {('e'), QLatin1String("enum")},   {('s'), QLatin1String("struct")},

                  {('i'), QLatin1String("i8")},     {('i'), QLatin1String("i16")},    {('i'), QLatin1String("i32")},
                  {('i'), QLatin1String("i64")},    {('i'), QLatin1String("i128")},   {('i'), QLatin1String("isize")},
                  {('u'), QLatin1String("u8")},     {('u'), QLatin1String("u16")},    {('u'), QLatin1String("u32")},
                  {('u'), QLatin1String("u64")},    {('u'), QLatin1String("u128")},   {('u'), QLatin1String("usize")},
                  {('f'), QLatin1String("f32")},    {('f'), QLatin1String("f64")},    {('s'), QLatin1String("str")},
                  {('c'), QLatin1String("char")},   {('b'), QLatin1String("bool")},   {('B'), QLatin1String("Box")},
                  {('O'), QLatin1String("Option")}, {('R'), QLatin1String("Result")}, {('S'), QLatin1String("String")},
                  {('V'), QLatin1String("Vec")}};

    rust_literals = {{('f'), QLatin1String("false")}, {('t'), QLatin1String("true")}};

    rust_builtin = {

    };

    rust_other = {{('a'), QLatin1String("assert!")},
                  {('a'), QLatin1String("assert_eq!")},
                  {('b'), QLatin1String("bitflags!")},
                  {('b'), QLatin1String("bytes!")},
                  {('c'), QLatin1String("cfg!")},
                  {('c'), QLatin1String("col!")},
                  {('c'), QLatin1String("concat!")},
                  {('c'), QLatin1String("concat_idents!")},
                  {('d'), QLatin1String("debug_assert!")},
                  {('d'), QLatin1String("debug_assert_eq!")},
                  {('e'), QLatin1String("env!")},
                  {('p'), QLatin1String("panic!")},
                  {('f'), QLatin1String("file!")},
                  {('f'), QLatin1String("format!")},
                  {('f'), QLatin1String("format_args!")},
                  {('i'), QLatin1String("include_bin!")},
                  {('i'), QLatin1String("include_str!")},
                  {('l'), QLatin1String("line!")},
                  {('l'), QLatin1String("local_data_key!")},
                  {('m'), QLatin1String("module_path!")},
                  {('o'), QLatin1String("option_env!")},
                  {('p'), QLatin1String("print!")},
                  {('p'), QLatin1String("println!")},
                  {('s'), QLatin1String("select!")},
                  {('s'), QLatin1String("stringify!")},
                  {('t'), QLatin1String("try!")},
                  {('u'), QLatin1String("unimplemented!")},
                  {('u'), QLatin1String("unreachable!")},
                  {('v'), QLatin1String("vec!")},
                  {('w'), QLatin1String("write!")},
                  {('w'), QLatin1String("writeln!")},
                  {('m'), QLatin1String("macro_rules!")},
                  {('a'), QLatin1String("assert_ne!")},
                  {('d'), QLatin1String("debug_assert_ne!")}};
}
void loadRustData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                  QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                  QMultiHash<char, QLatin1String> &other)
{
    if (!rustDataInitialized)
    {
        initRustData();
        rustDataInitialized = true;
    }
    types = rust_types;
    keywords = rust_keywords;
    builtin = rust_builtin;
    literals = rust_literals;
    other = rust_other;
}

/********************************************************/
/***   Java DATA      ***********************************/
/********************************************************/
static bool javaDataInitialized = false;
static QMultiHash<char, QLatin1String> java_keywords;
static QMultiHash<char, QLatin1String> java_types;
static QMultiHash<char, QLatin1String> java_literals;
static QMultiHash<char, QLatin1String> java_builtin;
static QMultiHash<char, QLatin1String> java_other;
void initJavaData()
{
    java_keywords = {{('a'), QLatin1String("abstract")},  {('a'), QLatin1String("assert")},
                     {('b'), QLatin1String("break")},     {('c'), QLatin1String("case")},
                     {('c'), QLatin1String("catch")},     {('c'), QLatin1String("const")},
                     {('c'), QLatin1String("continue")},  {('d'), QLatin1String("default")},
                     {('d'), QLatin1String("do")},        {('e'), QLatin1String("else")},
                     {('e'), QLatin1String("exports")},   {('e'), QLatin1String("extends")},
                     {('f'), QLatin1String("final")},     {('f'), QLatin1String("finally")},
                     {('f'), QLatin1String("for")},       {('g'), QLatin1String("goto")},
                     {('i'), QLatin1String("if")},        {('i'), QLatin1String("implements")},
                     {('i'), QLatin1String("import")},    {('i'), QLatin1String("instanceof")},
                     {('i'), QLatin1String("interface")}, {('l'), QLatin1String("long")},
                     {('m'), QLatin1String("module")},    {('n'), QLatin1String("native")},
                     {('n'), QLatin1String("new")},       {('n'), QLatin1String("null")},
                     {('o'), QLatin1String("open")},      {('o'), QLatin1String("opens")},
                     {('p'), QLatin1String("package")},   {('p'), QLatin1String("private")},
                     {('p'), QLatin1String("protected")}, {('p'), QLatin1String("provides")},
                     {('p'), QLatin1String("public")},    {('r'), QLatin1String("requires")},
                     {('r'), QLatin1String("return")},    {('s'), QLatin1String("static")},
                     {('s'), QLatin1String("strictfp")},  {('s'), QLatin1String("super")},
                     {('s'), QLatin1String("switch")},    {('s'), QLatin1String("synchronized")},
                     {('t'), QLatin1String("this")},      {('t'), QLatin1String("throw")},
                     {('t'), QLatin1String("throws")},    {('t'), QLatin1String("to")},
                     {('t'), QLatin1String("transient")}, {('t'), QLatin1String("transitive")},
                     {('t'), QLatin1String("try")},       {('u'), QLatin1String("uses")},
                     {('v'), QLatin1String("var")},       {('v'), QLatin1String("volatile")},
                     {('w'), QLatin1String("while")},     {('w'), QLatin1String("with")},
                     {('y'), QLatin1String("yield")}};

    java_types = {
        {('v'), QLatin1String("void")},  {('f'), QLatin1String("float")},  {('b'), QLatin1String("boolean")},
        {('b'), QLatin1String("byte")},  {('i'), QLatin1String("int")},    {('c'), QLatin1String("char")},
        {('c'), QLatin1String("class")}, {('d'), QLatin1String("double")}, {('e'), QLatin1String("enum")},
        {('s'), QLatin1String("short")},

    };

    java_literals = {{('f'), QLatin1String("false")}, {('t'), QLatin1String("true")}};

    java_builtin = {

    };

    java_other = {

    };
}
void loadJavaData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                  QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                  QMultiHash<char, QLatin1String> &other)
{
    if (!javaDataInitialized)
    {
        initJavaData();
        javaDataInitialized = true;
    }
    types = java_types;
    keywords = java_keywords;
    builtin = java_builtin;
    literals = java_literals;
    other = java_other;
}

/********************************************************/
/***   C# DATA      *************************************/
/********************************************************/
static bool csharpDataInitialized = false;
static QMultiHash<char, QLatin1String> csharp_keywords;
static QMultiHash<char, QLatin1String> csharp_types;
static QMultiHash<char, QLatin1String> csharp_literals;
static QMultiHash<char, QLatin1String> csharp_builtin;
static QMultiHash<char, QLatin1String> csharp_other;
void initCSharpData()
{
    csharp_keywords = {
        {('a'), QLatin1String("abstract")},  {('a'), QLatin1String("add")},        {('a'), QLatin1String("alias")},
        {('a'), QLatin1String("as")},        {('a'), QLatin1String("ascending")},  {('a'), QLatin1String("async")},
        {('a'), QLatin1String("await")},     {('b'), QLatin1String("base")},       {('b'), QLatin1String("break")},
        {('c'), QLatin1String("case")},      {('c'), QLatin1String("catch")},      {('c'), QLatin1String("checked")},
        {('c'), QLatin1String("const")},     {('c'), QLatin1String("continue")},   {('d'), QLatin1String("decimal")},
        {('d'), QLatin1String("default")},   {('d'), QLatin1String("delegate")},   {('d'), QLatin1String("descending")},
        {('d'), QLatin1String("do")},        {('d'), QLatin1String("dynamic")},    {('e'), QLatin1String("else")},
        {('e'), QLatin1String("event")},     {('e'), QLatin1String("explicit")},   {('e'), QLatin1String("extern")},
        {('f'), QLatin1String("finally")},   {('f'), QLatin1String("fixed")},      {('f'), QLatin1String("for")},
        {('f'), QLatin1String("foreach")},   {('f'), QLatin1String("from")},       {('g'), QLatin1String("get")},
        {('g'), QLatin1String("global")},    {('g'), QLatin1String("goto")},       {('g'), QLatin1String("group")},
        {('i'), QLatin1String("if")},        {('i'), QLatin1String("implicit")},   {('i'), QLatin1String("in")},
        {('i'), QLatin1String("interface")}, {('i'), QLatin1String("internal")},   {('i'), QLatin1String("into")},
        {('i'), QLatin1String("is")},        {('j'), QLatin1String("join")},       {('l'), QLatin1String("let")},
        {('l'), QLatin1String("lock")},      {('l'), QLatin1String("long")},       {('n'), QLatin1String("namespace")},
        {('n'), QLatin1String("new")},       {('o'), QLatin1String("object")},     {('o'), QLatin1String("operator")},
        {('o'), QLatin1String("orderby")},   {('o'), QLatin1String("out")},        {('o'), QLatin1String("override")},
        {('p'), QLatin1String("params")},    {('p'), QLatin1String("partial")},    {('p'), QLatin1String("private")},
        {('p'), QLatin1String("protected")}, {('p'), QLatin1String("public")},     {('r'), QLatin1String("readonly")},
        {('r'), QLatin1String("ref")},       {('r'), QLatin1String("remove")},     {('r'), QLatin1String("return")},
        {('s'), QLatin1String("sealed")},    {('s'), QLatin1String("select")},     {('s'), QLatin1String("set")},
        {('s'), QLatin1String("sizeof")},    {('s'), QLatin1String("stackalloc")}, {('s'), QLatin1String("static")},
        {('s'), QLatin1String("switch")},    {('t'), QLatin1String("this")},       {('t'), QLatin1String("throw")},
        {('t'), QLatin1String("try")},       {('t'), QLatin1String("typeof")},     {('u'), QLatin1String("unchecked")},
        {('u'), QLatin1String("unsafe")},    {('u'), QLatin1String("using")},      {('v'), QLatin1String("value")},
        {('v'), QLatin1String("virtual")},   {('v'), QLatin1String("volatile")},   {('w'), QLatin1String("where")},
        {('w'), QLatin1String("while")},     {('y'), QLatin1String("yield")}};

    csharp_types = {
        {('b'), QLatin1String("bool")},  {('b'), QLatin1String("byte")},   {('c'), QLatin1String("char")},
        {('c'), QLatin1String("class")}, {('d'), QLatin1String("double")}, {('e'), QLatin1String("enum")},
        {('f'), QLatin1String("float")}, {('i'), QLatin1String("int")},    {('s'), QLatin1String("sbyte")},
        {('s'), QLatin1String("short")}, {('s'), QLatin1String("string")}, {('s'), QLatin1String("struct")},
        {('u'), QLatin1String("uint")},  {('u'), QLatin1String("ulong")},  {('u'), QLatin1String("ushort")},
        {('v'), QLatin1String("var")},   {('v'), QLatin1String("void")},
    };

    csharp_literals = {{('f'), QLatin1String("false")}, {('t'), QLatin1String("true")}, {('n'), QLatin1String("null")}};

    csharp_builtin = {

    };

    csharp_other = {
        {('d'), QLatin1String("define")}, {('e'), QLatin1String("elif")},      {('e'), QLatin1String("else")},
        {('e'), QLatin1String("endif")},  {('e'), QLatin1String("endregion")}, {('e'), QLatin1String("error")},
        {('i'), QLatin1String("if")},     {('l'), QLatin1String("line")},      {('p'), QLatin1String("pragma")},
        {('r'), QLatin1String("region")}, {('u'), QLatin1String("undef")},     {('w'), QLatin1String("warning")}};
}
void loadCSharpData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                    QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                    QMultiHash<char, QLatin1String> &other)
{
    if (!csharpDataInitialized)
    {
        initCSharpData();
        csharpDataInitialized = true;
    }
    types = csharp_types;
    keywords = csharp_keywords;
    builtin = csharp_builtin;
    literals = csharp_literals;
    other = csharp_other;
}

/********************************************************/
/***   Go DATA      *************************************/
/********************************************************/
static bool goDataInitialized = false;
static QMultiHash<char, QLatin1String> go_keywords;
static QMultiHash<char, QLatin1String> go_types;
static QMultiHash<char, QLatin1String> go_literals;
static QMultiHash<char, QLatin1String> go_builtin;
static QMultiHash<char, QLatin1String> go_other;
void initGoData()
{
    go_keywords = {
        {('b'), QLatin1String("break")},     {('c'), QLatin1String("case")},     {('c'), QLatin1String("chan")},
        {('c'), QLatin1String("const")},     {('c'), QLatin1String("continue")}, {('d'), QLatin1String("default")},
        {('d'), QLatin1String("defer")},     {('e'), QLatin1String("else")},     {('f'), QLatin1String("fallthrough")},
        {('f'), QLatin1String("for")},       {('f'), QLatin1String("func")},     {('g'), QLatin1String("go")},
        {('t'), QLatin1String("to")},        {('i'), QLatin1String("if")},       {('i'), QLatin1String("import")},
        {('i'), QLatin1String("interface")}, {('p'), QLatin1String("package")},  {('r'), QLatin1String("range")},
        {('r'), QLatin1String("return")},    {('s'), QLatin1String("select")},   {('s'), QLatin1String("struct")},
        {('s'), QLatin1String("switch")},    {('t'), QLatin1String("type")},
    };

    go_types = {
        {('m'), QLatin1String("map")},        {('s'), QLatin1String("struct")},  {('v'), QLatin1String("var")},
        {('b'), QLatin1String("bool")},       {('b'), QLatin1String("byte")},    {('c'), QLatin1String("complex64")},
        {('c'), QLatin1String("complex128")}, {('f'), QLatin1String("float32")}, {('f'), QLatin1String("float64")},
        {('i'), QLatin1String("int8")},       {('i'), QLatin1String("int16")},   {('i'), QLatin1String("int32")},
        {('i'), QLatin1String("int64")},      {('s'), QLatin1String("string")},  {('u'), QLatin1String("uint8")},
        {('u'), QLatin1String("uint16")},     {('u'), QLatin1String("uint32")},  {('u'), QLatin1String("uint64")},
        {('i'), QLatin1String("int")},        {('u'), QLatin1String("uint")},    {('u'), QLatin1String("uintptr")},
        {('r'), QLatin1String("rune")}};

    go_literals = {{('f'), QLatin1String("false")},
                   {('t'), QLatin1String("true")},
                   {('n'), QLatin1String("nil")},
                   {('i'), QLatin1String("iota")}};

    go_builtin = {
        {('a'), QLatin1String("append")},  {('c'), QLatin1String("cap")},     {('c'), QLatin1String("close")},
        {('c'), QLatin1String("complex")}, {('c'), QLatin1String("copy")},    {('i'), QLatin1String("imag")},
        {('l'), QLatin1String("len")},     {('m'), QLatin1String("make")},    {('n'), QLatin1String("new")},
        {('p'), QLatin1String("panic")},   {('p'), QLatin1String("print")},   {('p'), QLatin1String("println")},
        {('r'), QLatin1String("real")},    {('r'), QLatin1String("recover")}, {('d'), QLatin1String("delete")}};

    go_other = {

    };
}
void loadGoData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                QMultiHash<char, QLatin1String> &other)
{
    if (!goDataInitialized)
    {
        initGoData();
        goDataInitialized = true;
    }
    types = go_types;
    keywords = go_keywords;
    builtin = go_builtin;
    literals = go_literals;
    other = go_other;
}

/********************************************************/
/***   V DATA      **************************************/
/********************************************************/
static bool vDataInitialized = false;
static QMultiHash<char, QLatin1String> v_keywords;
static QMultiHash<char, QLatin1String> v_types;
static QMultiHash<char, QLatin1String> v_literals;
static QMultiHash<char, QLatin1String> v_builtin;
static QMultiHash<char, QLatin1String> v_other;
void initVData()
{
    v_keywords = {
        {('b'), QLatin1String("break")},  {('c'), QLatin1String("const")},  {('c'), QLatin1String("continue")},
        {('d'), QLatin1String("defer")},  {('e'), QLatin1String("else")},   {('f'), QLatin1String("for")},
        {('f'), QLatin1String("fn")},     {('g'), QLatin1String("go")},     {('g'), QLatin1String("goto")},
        {('i'), QLatin1String("if")},     {('i'), QLatin1String("import")}, {('i'), QLatin1String("interface")},
        {('r'), QLatin1String("return")}, {('s'), QLatin1String("struct")}, {('s'), QLatin1String("switch")},
        {('t'), QLatin1String("type")},   {('p'), QLatin1String("pub")},    {('o'), QLatin1String("or")},
        {('n'), QLatin1String("none")}};

    v_types = {{('m'), QLatin1String("map")},  {('s'), QLatin1String("struct")},  {('b'), QLatin1String("bool")},
               {('b'), QLatin1String("byte")}, {('f'), QLatin1String("f32")},     {('f'), QLatin1String("f64")},
               {('i'), QLatin1String("i8")},   {('i'), QLatin1String("i16")},     {('i'), QLatin1String("int")},
               {('i'), QLatin1String("i64")},  {('i'), QLatin1String("i128")},    {('s'), QLatin1String("string")},
               {('u'), QLatin1String("u16")},  {('u'), QLatin1String("u32")},     {('u'), QLatin1String("u64")},
               {('u'), QLatin1String("u128")}, {('u'), QLatin1String("byteptr")}, {('u'), QLatin1String("voidptr")},
               {('r'), QLatin1String("rune")}};

    v_literals = {
        {('f'), QLatin1String("false")},
        {('t'), QLatin1String("true")},
    };

    v_builtin = {};

    v_other = {

    };
}
void loadVData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
               QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
               QMultiHash<char, QLatin1String> &other)
{
    if (!vDataInitialized)
    {
        initVData();
        vDataInitialized = true;
    }
    types = v_types;
    keywords = v_keywords;
    builtin = v_builtin;
    literals = v_literals;
    other = v_other;
}

/********************************************************/
/***   SQL DATA      ************************************/
/********************************************************/
static bool sqlDataInitialized = false;
static QMultiHash<char, QLatin1String> sql_keywords;
static QMultiHash<char, QLatin1String> sql_types;
static QMultiHash<char, QLatin1String> sql_literals;
static QMultiHash<char, QLatin1String> sql_builtin;
static QMultiHash<char, QLatin1String> sql_other;
void initSQLData()
{
    sql_keywords = {{('A'), QLatin1String("ACTION")},
                    {('A'), QLatin1String("ADD")},
                    {('A'), QLatin1String("AFTER")},
                    {('A'), QLatin1String("ALGORITHM")},
                    {('A'), QLatin1String("ALL")},
                    {('A'), QLatin1String("ALTER")},
                    {('A'), QLatin1String("ANALYZE")},
                    {('A'), QLatin1String("ANY")},
                    {('A'), QLatin1String("APPLY")},
                    {('A'), QLatin1String("AS")},
                    {('A'), QLatin1String("ASC")},
                    {('A'), QLatin1String("AUTHORIZATION")},
                    {('A'), QLatin1String("AUTO_INCREMENT")},
                    {('B'), QLatin1String("BACKUP")},
                    {('B'), QLatin1String("BDB")},
                    {('B'), QLatin1String("BEGIN")},
                    {('B'), QLatin1String("BERKELEYDB")},
                    {('B'), QLatin1String("BIGINT")},
                    {('B'), QLatin1String("BINARY")},
                    {('B'), QLatin1String("BIT")},
                    {('B'), QLatin1String("BLOB")},
                    {('B'), QLatin1String("BOOL")},
                    {('B'), QLatin1String("BOOLEAN")},
                    {('B'), QLatin1String("BREAK")},
                    {('B'), QLatin1String("BROWSE")},
                    {('B'), QLatin1String("BTREE")},
                    {('B'), QLatin1String("BULK")},
                    {('B'), QLatin1String("BY")},
                    {('C'), QLatin1String("CALL")},
                    {('C'), QLatin1String("CASCADED")},
                    {('C'), QLatin1String("CASE")},
                    {('C'), QLatin1String("CHAIN")},
                    {('C'), QLatin1String("CHARACTER")},
                    {('S'), QLatin1String("SET")},
                    {('C'), QLatin1String("CHECKPOINT")},
                    {('C'), QLatin1String("CLOSE")},
                    {('C'), QLatin1String("CLUSTERED")},
                    {('C'), QLatin1String("COALESCE")},
                    {('C'), QLatin1String("COLLATE")},
                    {('C'), QLatin1String("COLUMNS")},
                    {('C'), QLatin1String("COMMENT")},
                    {('C'), QLatin1String("COMMITTED")},
                    {('C'), QLatin1String("COMPUTE")},
                    {('C'), QLatin1String("CONNECT")},
                    {('C'), QLatin1String("CONSISTENT")},
                    {('C'), QLatin1String("CONSTRAINT")},
                    {('C'), QLatin1String("CONTAINSTABLE")},
                    {('C'), QLatin1String("CONTINUE")},
                    {('C'), QLatin1String("CONVERT")},
                    {('C'), QLatin1String("CREATE")},
                    {('C'), QLatin1String("CROSS")},
                    {('C'), QLatin1String("CURRENT_DATE")},
                    {('_'), QLatin1String("_TIME")},
                    {('_'), QLatin1String("_TIMESTAMP")},
                    {('_'), QLatin1String("_USER")},
                    {('C'), QLatin1String("CURSOR")},
                    {('C'), QLatin1String("CYCLE")},
                    {('D'), QLatin1String("DATABASES")},
                    {('D'), QLatin1String("DATETIME")},
                    {('D'), QLatin1String("DAY")},
                    {('D'), QLatin1String("DBCC")},
                    {('D'), QLatin1String("DEALLOCATE")},
                    {('D'), QLatin1String("DEC")},
                    {('D'), QLatin1String("DECIMAL")},
                    {('D'), QLatin1String("DECLARE")},
                    {('D'), QLatin1String("DEFAULT")},
                    {('D'), QLatin1String("DEFINER")},
                    {('D'), QLatin1String("DELAYED")},
                    {('D'), QLatin1String("DELETE")},
                    {('D'), QLatin1String("DELIMITERS")},
                    {('D'), QLatin1String("DENY")},
                    {('D'), QLatin1String("DESC")},
                    {('D'), QLatin1String("DESCRIBE")},
                    {('D'), QLatin1String("DETERMINISTIC")},
                    {('D'), QLatin1String("DISABLE")},
                    {('D'), QLatin1String("DISCARD")},
                    {('D'), QLatin1String("DISK")},
                    {('D'), QLatin1String("DISTINCT")},
                    {('D'), QLatin1String("DISTINCTROW")},
                    {('D'), QLatin1String("DISTRIBUTED")},
                    {('D'), QLatin1String("DO")},
                    {('D'), QLatin1String("DOUBLE")},
                    {('D'), QLatin1String("DROP")},
                    {('D'), QLatin1String("DUMMY")},
                    {('D'), QLatin1String("DUMPFILE")},
                    {('D'), QLatin1String("DUPLICATE")},
                    {('E'), QLatin1String("ELSEIF")},
                    {('E'), QLatin1String("ENABLE")},
                    {('E'), QLatin1String("ENCLOSED")},
                    {('E'), QLatin1String("END")},
                    {('E'), QLatin1String("ENGINE")},
                    {('E'), QLatin1String("ENUM")},
                    {('E'), QLatin1String("ERRLVL")},
                    {('E'), QLatin1String("ERRORS")},
                    {('E'), QLatin1String("ESCAPED")},
                    {('E'), QLatin1String("EXCEPT")},
                    {('E'), QLatin1String("EXECUTE")},
                    {('E'), QLatin1String("EXISTS")},
                    {('E'), QLatin1String("EXIT")},
                    {('E'), QLatin1String("EXPLAIN")},
                    {('E'), QLatin1String("EXTENDED")},
                    {('F'), QLatin1String("FETCH")},
                    {('F'), QLatin1String("FIELDS")},
                    {('F'), QLatin1String("FILE")},
                    {('F'), QLatin1String("FILLFACTOR")},
                    {('F'), QLatin1String("FIRST")},
                    {('F'), QLatin1String("FIXED")},
                    {('F'), QLatin1String("FLOAT")},
                    {('F'), QLatin1String("FOLLOWING")},
                    {('F'), QLatin1String("FOR")},
                    {('E'), QLatin1String("EACH")},
                    {('R'), QLatin1String("ROW")},
                    {('F'), QLatin1String("FORCE")},
                    {('F'), QLatin1String("FOREIGN")},
                    {('F'), QLatin1String("FREETEXTTABLE")},
                    {('F'), QLatin1String("FROM")},
                    {('F'), QLatin1String("FULL")},
                    {('F'), QLatin1String("FUNCTION")},
                    {('G'), QLatin1String("GEOMETRYCOLLECTION")},
                    {('G'), QLatin1String("GLOBAL")},
                    {('G'), QLatin1String("GOTO")},
                    {('G'), QLatin1String("GRANT")},
                    {('G'), QLatin1String("GROUP")},
                    {('H'), QLatin1String("HANDLER")},
                    {('H'), QLatin1String("HASH")},
                    {('H'), QLatin1String("HAVING")},
                    {('H'), QLatin1String("HOLDLOCK")},
                    {('H'), QLatin1String("HOUR")},
                    {('I'), QLatin1String("IDENTITY_INSERT")},
                    {('C'), QLatin1String("COL")},
                    {('I'), QLatin1String("IF")},
                    {('I'), QLatin1String("IGNORE")},
                    {('I'), QLatin1String("IMPORT")},
                    {('I'), QLatin1String("INDEX")},
                    {('I'), QLatin1String("INFILE")},
                    {('I'), QLatin1String("INNER")},
                    {('I'), QLatin1String("INNODB")},
                    {('I'), QLatin1String("INOUT")},
                    {('I'), QLatin1String("INSERT")},
                    {('I'), QLatin1String("INT")},
                    {('I'), QLatin1String("INTEGER")},
                    {('I'), QLatin1String("INTERSECT")},
                    {('I'), QLatin1String("INTERVAL")},
                    {('I'), QLatin1String("INTO")},
                    {('I'), QLatin1String("INVOKER")},
                    {('I'), QLatin1String("ISOLATION")},
                    {('I'), QLatin1String("ITERATE")},
                    {('J'), QLatin1String("JOIN")},
                    {('K'), QLatin1String("KEYS")},
                    {('K'), QLatin1String("KILL")},
                    {('L'), QLatin1String("LANGUAGE")},
                    {('L'), QLatin1String("LAST")},
                    {('L'), QLatin1String("LEAVE")},
                    {('L'), QLatin1String("LEFT")},
                    {('L'), QLatin1String("LEVEL")},
                    {('L'), QLatin1String("LIMIT")},
                    {('L'), QLatin1String("LINENO")},
                    {('L'), QLatin1String("LINES")},
                    {('L'), QLatin1String("LINESTRING")},
                    {('L'), QLatin1String("LOAD")},
                    {('L'), QLatin1String("LOCAL")},
                    {('L'), QLatin1String("LOCK")},
                    {('L'), QLatin1String("LONGBLOB")},
                    {('T'), QLatin1String("TEXT")},
                    {('L'), QLatin1String("LOOP")},
                    {('M'), QLatin1String("MATCHED")},
                    {('M'), QLatin1String("MEDIUMBLOB")},
                    {('I'), QLatin1String("INT")},
                    {('T'), QLatin1String("TEXT")},
                    {('M'), QLatin1String("MERGE")},
                    {('M'), QLatin1String("MIDDLEINT")},
                    {('M'), QLatin1String("MINUTE")},
                    {('M'), QLatin1String("MODE")},
                    {('M'), QLatin1String("MODIFIES")},
                    {('M'), QLatin1String("MODIFY")},
                    {('M'), QLatin1String("MONTH")},
                    {('M'), QLatin1String("MULTILINESTRING")},
                    {('P'), QLatin1String("POINT")},
                    {('P'), QLatin1String("POLYGON")},
                    {('N'), QLatin1String("NATIONAL")},
                    {('N'), QLatin1String("NATURAL")},
                    {('N'), QLatin1String("NCHAR")},
                    {('N'), QLatin1String("NEXT")},
                    {('N'), QLatin1String("NO")},
                    {('N'), QLatin1String("NONCLUSTERED")},
                    {('N'), QLatin1String("NULLIF")},
                    {('N'), QLatin1String("NUMERIC")},
                    {('O'), QLatin1String("OFF")},
                    {('O'), QLatin1String("OFFSETS")},
                    {('O'), QLatin1String("ON")},
                    {('O'), QLatin1String("OPENDATASOURCE")},
                    {('Q'), QLatin1String("QUERY")},
                    {('R'), QLatin1String("ROWSET")},
                    {('O'), QLatin1String("OPTIMIZE")},
                    {('O'), QLatin1String("OPTIONALLY")},
                    {('O'), QLatin1String("ORDER")},
                    {('O'), QLatin1String("OUTER")},
                    {('F'), QLatin1String("FILE")},
                    {('O'), QLatin1String("OVER")},
                    {('P'), QLatin1String("PARTIAL")},
                    {('P'), QLatin1String("PARTITION")},
                    {('P'), QLatin1String("PERCENT")},
                    {('P'), QLatin1String("PIVOT")},
                    {('P'), QLatin1String("PLAN")},
                    {('P'), QLatin1String("POINT")},
                    {('P'), QLatin1String("POLYGON")},
                    {('P'), QLatin1String("PRECEDING")},
                    {('P'), QLatin1String("PRECISION")},
                    {('P'), QLatin1String("PREPARE")},
                    {('P'), QLatin1String("PREV")},
                    {('P'), QLatin1String("PRIMARY")},
                    {('P'), QLatin1String("PRINT")},
                    {('P'), QLatin1String("PRIVILEGES")},
                    {('P'), QLatin1String("PROCEDURE")},
                    {('P'), QLatin1String("PUBLIC")},
                    {('P'), QLatin1String("PURGE")},
                    {('Q'), QLatin1String("QUICK")},
                    {('R'), QLatin1String("RAISERROR")},
                    {('R'), QLatin1String("READS")},
                    {('R'), QLatin1String("REAL")},
                    {('R'), QLatin1String("RECONFIGURE")},
                    {('R'), QLatin1String("REFERENCES")},
                    {('R'), QLatin1String("RELEASE")},
                    {('R'), QLatin1String("RENAME")},
                    {('R'), QLatin1String("REPEATABLE")},
                    {('R'), QLatin1String("REPLACE")},
                    {('R'), QLatin1String("REPLICATION")},
                    {('R'), QLatin1String("REQUIRE")},
                    {('R'), QLatin1String("RESIGNAL")},
                    {('R'), QLatin1String("RESTORE")},
                    {('R'), QLatin1String("RESTRICT")},
                    {('R'), QLatin1String("RETURNS")},
                    {('R'), QLatin1String("REVOKE")},
                    {('R'), QLatin1String("RIGHT")},
                    {('R'), QLatin1String("ROLLBACK")},
                    {('R'), QLatin1String("ROUTINE")},
                    {('R'), QLatin1String("ROWCOUNT")},
                    {('G'), QLatin1String("GUIDCOL")},
                    {('R'), QLatin1String("RTREE")},
                    {('R'), QLatin1String("RULE")},
                    {('S'), QLatin1String("SAVEPOINT")},
                    {('S'), QLatin1String("SCHEMA")},
                    {('S'), QLatin1String("SECOND")},
                    {('S'), QLatin1String("SELECT")},
                    {('S'), QLatin1String("SERIALIZABLE")},
                    {('S'), QLatin1String("SESSION_USER")},
                    {('S'), QLatin1String("SETUSER")},
                    {('S'), QLatin1String("SHARE")},
                    {('S'), QLatin1String("SHOW")},
                    {('S'), QLatin1String("SHUTDOWN")},
                    {('S'), QLatin1String("SIMPLE")},
                    {('S'), QLatin1String("SMALLINT")},
                    {('S'), QLatin1String("SNAPSHOT")},
                    {('S'), QLatin1String("SOME")},
                    {('S'), QLatin1String("SONAME")},
                    {('S'), QLatin1String("SQL")},
                    {('S'), QLatin1String("STARTING")},
                    {('S'), QLatin1String("STATISTICS")},
                    {('S'), QLatin1String("STATUS")},
                    {('S'), QLatin1String("STRIPED")},
                    {('S'), QLatin1String("SYSTEM_USER")},
                    {('T'), QLatin1String("TABLES")},
                    {('T'), QLatin1String("TABLESPACE")},
                    {('T'), QLatin1String("TEMPORARY")},
                    {('T'), QLatin1String("TABLE")},
                    {('T'), QLatin1String("TERMINATED")},
                    {('T'), QLatin1String("TEXTSIZE")},
                    {('T'), QLatin1String("THEN")},
                    {('T'), QLatin1String("TIMESTAMP")},
                    {('T'), QLatin1String("TINYBLOB")},
                    {('I'), QLatin1String("INT")},
                    {('T'), QLatin1String("TEXT")},
                    {('T'), QLatin1String("TOP")},
                    {('T'), QLatin1String("TRANSACTIONS")},
                    {('T'), QLatin1String("TRIGGER")},
                    {('T'), QLatin1String("TRUNCATE")},
                    {('T'), QLatin1String("TSEQUAL")},
                    {('T'), QLatin1String("TYPES")},
                    {('U'), QLatin1String("UNBOUNDED")},
                    {('U'), QLatin1String("UNCOMMITTED")},
                    {('U'), QLatin1String("UNDEFINED")},
                    {('U'), QLatin1String("UNION")},
                    {('U'), QLatin1String("UNIQUE")},
                    {('U'), QLatin1String("UNLOCK")},
                    {('U'), QLatin1String("UNPIVOT")},
                    {('U'), QLatin1String("UNSIGNED")},
                    {('U'), QLatin1String("UPDATETEXT")},
                    {('U'), QLatin1String("USAGE")},
                    {('U'), QLatin1String("USE")},
                    {('U'), QLatin1String("USER")},
                    {('U'), QLatin1String("USING")},
                    {('V'), QLatin1String("VALUES")},
                    {('V'), QLatin1String("VARBINARY")},
                    {('C'), QLatin1String("CHAR")},
                    {('C'), QLatin1String("CHARACTER")},
                    {('Y'), QLatin1String("YING")},
                    {('V'), QLatin1String("VIEW")},
                    {('W'), QLatin1String("WAITFOR")},
                    {('W'), QLatin1String("WARNINGS")},
                    {('W'), QLatin1String("WHEN")},
                    {('W'), QLatin1String("WHERE")},
                    {('W'), QLatin1String("WHILE")},
                    {('W'), QLatin1String("WITH")},
                    {('R'), QLatin1String("ROLLUP")},
                    {('I'), QLatin1String("IN")},
                    {('W'), QLatin1String("WORK")},
                    {('W'), QLatin1String("WRITETEXT")},
                    {('Y'), QLatin1String("YEAR")}};

    sql_types = {

    };

    sql_literals = {
        {('A'), QLatin1String("TRUE")},
        {('F'), QLatin1String("FALSE")},
        {('N'), QLatin1String("NULL")},
    };

    sql_builtin = {{('A'), QLatin1String("AVG")},    {('C'), QLatin1String("COUNT")}, {('F'), QLatin1String("FIRST")},
                   {('F'), QLatin1String("FORMAT")}, {('L'), QLatin1String("LAST")},  {('L'), QLatin1String("LCASE")},
                   {('L'), QLatin1String("LEN")},    {('M'), QLatin1String("MAX")},   {('M'), QLatin1String("MID")},
                   {('M'), QLatin1String("MIN")},    {('M'), QLatin1String("MOD")},   {('N'), QLatin1String("NOW")},
                   {('R'), QLatin1String("ROUND")},  {('S'), QLatin1String("SUM")},   {('U'), QLatin1String("UCASE")}};

    sql_other = {

    };
}
void loadSQLData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                 QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                 QMultiHash<char, QLatin1String> &other)
{
    if (!sqlDataInitialized)
    {
        initSQLData();
        sqlDataInitialized = true;
    }
    types = sql_types;
    keywords = sql_keywords;
    builtin = sql_builtin;
    literals = sql_literals;
    other = sql_other;
}

/********************************************************/
/***   JSON DATA      ***********************************/
/********************************************************/
static bool jsonDataInitialized = false;
static QMultiHash<char, QLatin1String> json_keywords;
static QMultiHash<char, QLatin1String> json_types;
static QMultiHash<char, QLatin1String> json_literals;
static QMultiHash<char, QLatin1String> json_builtin;
static QMultiHash<char, QLatin1String> json_other;
void initJSONData()
{
    json_keywords = {};

    json_types = {};

    json_literals = {{('f'), QLatin1String("false")}, {('t'), QLatin1String("true")}, {('n'), QLatin1String("null")}};

    json_builtin = {};

    json_other = {};
}
void loadJSONData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                  QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                  QMultiHash<char, QLatin1String> &other)
{
    if (!jsonDataInitialized)
    {
        initJSONData();
        jsonDataInitialized = true;
    }
    types = json_types;
    keywords = json_keywords;
    builtin = json_builtin;
    literals = json_literals;
    other = json_other;
}

/********************************************************/
/***   CSS DATA      ***********************************/
/********************************************************/
static bool cssDataInitialized = false;
static QMultiHash<char, QLatin1String> css_keywords;
static QMultiHash<char, QLatin1String> css_types;
static QMultiHash<char, QLatin1String> css_literals;
static QMultiHash<char, QLatin1String> css_builtin;
static QMultiHash<char, QLatin1String> css_other;
void initCSSData()
{
    css_keywords = {{'i', QLatin1String("important")}, {'p', QLatin1String("px")}, {'e', QLatin1String("em")}};

    css_types = {
        {'a', QLatin1String("align")},       {'c', QLatin1String("content")},    {'i', QLatin1String("items")},
        {'s', QLatin1String("self")},        {'a', QLatin1String("all")},        {'a', QLatin1String("animation")},
        {'d', QLatin1String("delay")},       {'d', QLatin1String("direction")},  {'d', QLatin1String("duration")},
        {'f', QLatin1String("fill")},        {'m', QLatin1String("mode")},       {'i', QLatin1String("iteration")},
        {'c', QLatin1String("count")},       {'n', QLatin1String("name")},       {'p', QLatin1String("play")},
        {'s', QLatin1String("state")},       {'t', QLatin1String("timing")},     {'f', QLatin1String("function")},
        {'a', QLatin1String("azimuth")},     {'b', QLatin1String("backface")},   {'v', QLatin1String("visibility")},
        {'a', QLatin1String("attachment")},  {'b', QLatin1String("blend")},      {'m', QLatin1String("mode")},
        {'c', QLatin1String("clip")},        {'c', QLatin1String("color")},      {'i', QLatin1String("image")},
        {'o', QLatin1String("origin")},      {'p', QLatin1String("position")},   {'r', QLatin1String("repeat")},
        {'s', QLatin1String("size")},        {'b', QLatin1String("background")}, {'b', QLatin1String("bleed")},
        {'c', QLatin1String("color")},       {'r', QLatin1String("radius")},     {'r', QLatin1String("radius")},
        {'s', QLatin1String("style")},       {'w', QLatin1String("width")},      {'b', QLatin1String("bottom")},
        {'c', QLatin1String("collapse")},    {'c', QLatin1String("color")},      {'i', QLatin1String("image")},
        {'o', QLatin1String("outset")},      {'r', QLatin1String("repeat")},     {'s', QLatin1String("source")},
        {'s', QLatin1String("slice")},       {'w', QLatin1String("width")},      {'c', QLatin1String("color")},
        {'s', QLatin1String("style")},       {'w', QLatin1String("width")},      {'l', QLatin1String("left")},
        {'r', QLatin1String("radius")},      {'c', QLatin1String("color")},      {'s', QLatin1String("style")},
        {'w', QLatin1String("width")},       {'r', QLatin1String("right")},      {'s', QLatin1String("spacing")},
        {'s', QLatin1String("style")},       {'c', QLatin1String("color")},      {'l', QLatin1String("left")},
        {'r', QLatin1String("radius")},      {'r', QLatin1String("radius")},     {'s', QLatin1String("style")},
        {'w', QLatin1String("width")},       {'t', QLatin1String("top")},        {'w', QLatin1String("width")},
        {'b', QLatin1String("border")},      {'b', QLatin1String("bottom")},     {'b', QLatin1String("break")},
        {'b', QLatin1String("box")},         {'s', QLatin1String("shadow")},     {'b', QLatin1String("box")},
        {'s', QLatin1String("sizing")},      {'a', QLatin1String("after")},      {'b', QLatin1String("before")},
        {'b', QLatin1String("break")},       {'i', QLatin1String("inside")},     {'c', QLatin1String("caption")},
        {'s', QLatin1String("side")},        {'c', QLatin1String("caret")},      {'c', QLatin1String("color")},
        {'c', QLatin1String("clear")},       {'c', QLatin1String("clip")},       {'c', QLatin1String("color")},
        {'c', QLatin1String("columns")},     {'c', QLatin1String("column")},     {'c', QLatin1String("count")},
        {'f', QLatin1String("fill")},        {'g', QLatin1String("gap")},        {'r', QLatin1String("rule")},
        {'c', QLatin1String("color")},       {'s', QLatin1String("style")},      {'w', QLatin1String("width")},
        {'s', QLatin1String("span")},        {'w', QLatin1String("width")},      {'c', QLatin1String("content")},
        {'i', QLatin1String("increment")},   {'c', QLatin1String("counter")},    {'r', QLatin1String("reset")},
        {'a', QLatin1String("after")},       {'b', QLatin1String("before")},     {'c', QLatin1String("cue")},
        {'c', QLatin1String("cursor")},      {'d', QLatin1String("direction")},  {'d', QLatin1String("display")},
        {'e', QLatin1String("elevation")},   {'e', QLatin1String("empty")},      {'c', QLatin1String("cells")},
        {'f', QLatin1String("filter")},      {'f', QLatin1String("flex")},       {'b', QLatin1String("basis")},
        {'d', QLatin1String("direction")},   {'f', QLatin1String("feature")},    {'s', QLatin1String("settings")},
        {'f', QLatin1String("flex")},        {'f', QLatin1String("flow")},       {'g', QLatin1String("grow")},
        {'s', QLatin1String("shrink")},      {'w', QLatin1String("wrap")},       {'f', QLatin1String("float")},
        {'f', QLatin1String("family")},      {'k', QLatin1String("kerning")},    {'l', QLatin1String("language")},
        {'o', QLatin1String("override")},    {'a', QLatin1String("adjust")},     {'s', QLatin1String("size")},
        {'s', QLatin1String("stretch")},     {'s', QLatin1String("style")},      {'s', QLatin1String("synthesis")},
        {'v', QLatin1String("variant")},     {'a', QLatin1String("alternates")}, {'c', QLatin1String("caps")},
        {'e', QLatin1String("east")},        {'a', QLatin1String("asian")},      {'l', QLatin1String("ligatures")},
        {'n', QLatin1String("numeric")},     {'p', QLatin1String("position")},   {'w', QLatin1String("weight")},
        {'f', QLatin1String("font")},        {'a', QLatin1String("area")},       {'a', QLatin1String("auto")},
        {'c', QLatin1String("columns")},     {'f', QLatin1String("flow")},       {'r', QLatin1String("rows")},
        {'e', QLatin1String("end")},         {'g', QLatin1String("gap")},        {'s', QLatin1String("start")},
        {'c', QLatin1String("column")},      {'g', QLatin1String("gap")},        {'e', QLatin1String("end")},
        {'g', QLatin1String("gap")},         {'s', QLatin1String("start")},      {'r', QLatin1String("row")},
        {'a', QLatin1String("areas")},       {'c', QLatin1String("columns")},    {'r', QLatin1String("rows")},
        {'t', QLatin1String("template")},    {'g', QLatin1String("grid")},       {'h', QLatin1String("hanging")},
        {'p', QLatin1String("punctuation")}, {'h', QLatin1String("height")},     {'h', QLatin1String("hyphens")},
        {'i', QLatin1String("isolation")},   {'j', QLatin1String("justify")},    {'c', QLatin1String("content")},
        {'i', QLatin1String("items")},       {'s', QLatin1String("self")},       {'l', QLatin1String("leftimage")},
        {'l', QLatin1String("letter")},      {'s', QLatin1String("spacing")},    {'b', QLatin1String("break")},
        {'l', QLatin1String("line")},        {'s', QLatin1String("style")},      {'i', QLatin1String("image")},
        {'s', QLatin1String("style")},       {'p', QLatin1String("position")},   {'t', QLatin1String("type")},
        {'l', QLatin1String("list")},        {'s', QLatin1String("style")},      {'b', QLatin1String("bottom")},
        {'l', QLatin1String("left")},        {'r', QLatin1String("right")},      {'t', QLatin1String("top")},
        {'m', QLatin1String("margin")},      {'m', QLatin1String("marker")},     {'o', QLatin1String("offset")},
        {'m', QLatin1String("marks")},       {'m', QLatin1String("max")},        {'h', QLatin1String("height")},
        {'w', QLatin1String("width")},       {'m', QLatin1String("min")},        {'m', QLatin1String("mix")},
        {'b', QLatin1String("blend")},       {'m', QLatin1String("mode")},       {'n', QLatin1String("nav")},
        {'u', QLatin1String("up")},          {'d', QLatin1String("down")},       {'l', QLatin1String("left")},
        {'r', QLatin1String("right")},       {'o', QLatin1String("opacity")},    {'o', QLatin1String("order")},
        {'o', QLatin1String("orphans")},     {'c', QLatin1String("color")},      {'o', QLatin1String("offset")},
        {'s', QLatin1String("style")},       {'w', QLatin1String("width")},      {'o', QLatin1String("outline")},
        {'w', QLatin1String("wrap")},        {'o', QLatin1String("overflow")},   {'b', QLatin1String("bottom")},
        {'l', QLatin1String("left")},        {'r', QLatin1String("right")},      {'t', QLatin1String("top")},
        {'p', QLatin1String("padding")},     {'b', QLatin1String("break")},      {'a', QLatin1String("after")},
        {'b', QLatin1String("before")},      {'i', QLatin1String("inside")},     {'p', QLatin1String("page")},
        {'a', QLatin1String("after")},       {'b', QLatin1String("before")},     {'p', QLatin1String("pause")},
        {'p', QLatin1String("perspective")}, {'o', QLatin1String("origin")},     {'r', QLatin1String("range")},
        {'p', QLatin1String("pitch")},       {'c', QLatin1String("content")},    {'i', QLatin1String("items")},
        {'p', QLatin1String("place")},       {'s', QLatin1String("self")},       {'p', QLatin1String("play")},
        {'d', QLatin1String("during")},      {'p', QLatin1String("position")},   {'q', QLatin1String("quotes")},
        {'r', QLatin1String("resize")},      {'r', QLatin1String("rest")},       {'a', QLatin1String("after")},
        {'b', QLatin1String("before")},      {'r', QLatin1String("rest")},       {'r', QLatin1String("richness")},
        {'r', QLatin1String("right")},       {'s', QLatin1String("size")},       {'h', QLatin1String("header")},
        {'n', QLatin1String("numeral")},     {'s', QLatin1String("speak")},      {'p', QLatin1String("punctuation")},
        {'s', QLatin1String("speak")},       {'s', QLatin1String("speech")},     {'r', QLatin1String("rate")},
        {'s', QLatin1String("stress")},      {'t', QLatin1String("tab")},        {'s', QLatin1String("size")},
        {'t', QLatin1String("table")},       {'l', QLatin1String("layout")},     {'t', QLatin1String("text")},
        {'a', QLatin1String("align")},       {'l', QLatin1String("last")},       {'d', QLatin1String("decoration")},
        {'c', QLatin1String("color")},       {'l', QLatin1String("line")},       {'s', QLatin1String("skip")},
        {'s', QLatin1String("style")},       {'i', QLatin1String("indent")},     {'o', QLatin1String("overflow")},
        {'s', QLatin1String("shadow")},      {'t', QLatin1String("transform")},  {'u', QLatin1String("underline")},
        {'p', QLatin1String("position")},    {'t', QLatin1String("top")},        {'t', QLatin1String("transform")},
        {'o', QLatin1String("origin")},      {'s', QLatin1String("style")},      {'t', QLatin1String("transition")},
        {'d', QLatin1String("delay")},       {'d', QLatin1String("duration")},   {'p', QLatin1String("property")},
        {'t', QLatin1String("timing")},      {'f', QLatin1String("function")},   {'u', QLatin1String("unicode")},
        {'b', QLatin1String("bidi")},        {'v', QLatin1String("vertical")},   {'a', QLatin1String("align")},
        {'v', QLatin1String("visibility")},  {'b', QLatin1String("balance")},    {'d', QLatin1String("duration")},
        {'f', QLatin1String("family")},      {'p', QLatin1String("pitch")},      {'r', QLatin1String("range")},
        {'r', QLatin1String("rate")},        {'s', QLatin1String("stress")},     {'v', QLatin1String("volume")},
        {'v', QLatin1String("voice")},       {'v', QLatin1String("volume")},     {'w', QLatin1String("white")},
        {'s', QLatin1String("space")},       {'w', QLatin1String("widows")},     {'w', QLatin1String("width")},
        {'w', QLatin1String("will")},        {'c', QLatin1String("change")},     {'w', QLatin1String("word")},
        {'b', QLatin1String("break")},       {'s', QLatin1String("spacing")},    {'w', QLatin1String("wrap")},
        {'x', QLatin1String("x")},           {'y', QLatin1String("y")},          {'z', QLatin1String("z")},
        {'i', QLatin1String("index")},       {'r', QLatin1String("rgb")},        {'s', QLatin1String("sans")},
        {'s', QLatin1String("serif")},       {'n', QLatin1String("normal")}};

    css_literals = {};

    css_builtin = {};

    css_other = {};
}
void loadCSSData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                 QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                 QMultiHash<char, QLatin1String> &other)
{
    if (!cssDataInitialized)
    {
        initCSSData();
        cssDataInitialized = true;
    }
    types = css_types;
    keywords = css_keywords;
    builtin = css_builtin;
    literals = css_literals;
    other = css_other;
}

/********************************************************/
/***   Typescript DATA  *********************************/
/********************************************************/
static bool typescriptDataInitialized = false;
static QMultiHash<char, QLatin1String> typescript_keywords;
static QMultiHash<char, QLatin1String> typescript_types;
static QMultiHash<char, QLatin1String> typescript_literals;
static QMultiHash<char, QLatin1String> typescript_builtin;
static QMultiHash<char, QLatin1String> typescript_other;
void initTypescriptData()
{
    typescript_keywords = {
        {'i', QLatin1String("in")},        {'i', QLatin1String("if")},         {'f', QLatin1String("for")},
        {'w', QLatin1String("while")},     {'f', QLatin1String("finally")},    {'n', QLatin1String("new")},
        {'f', QLatin1String("function")},  {'d', QLatin1String("do")},         {'r', QLatin1String("return")},
        {'v', QLatin1String("void")},      {'e', QLatin1String("else")},       {'b', QLatin1String("break")},
        {'c', QLatin1String("catch")},     {'i', QLatin1String("instanceof")}, {'w', QLatin1String("with")},
        {'t', QLatin1String("throw")},     {'c', QLatin1String("case")},       {'d', QLatin1String("default")},
        {'t', QLatin1String("try")},       {'t', QLatin1String("this")},       {'s', QLatin1String("switch")},
        {'c', QLatin1String("continue")},  {'t', QLatin1String("typeof")},     {'d', QLatin1String("delete")},
        {'l', QLatin1String("let")},       {'y', QLatin1String("yield")},      {'c', QLatin1String("const")},
        {'p', QLatin1String("public")},    {'p', QLatin1String("private")},    {'p', QLatin1String("protected")},
        {'g', QLatin1String("get")},       {'s', QLatin1String("set")},        {'s', QLatin1String("super")},
        {'s', QLatin1String("static")},    {'i', QLatin1String("implements")}, {'e', QLatin1String("export")},
        {'i', QLatin1String("import")},    {'d', QLatin1String("declare")},    {'t', QLatin1String("type")},
        {'n', QLatin1String("namespace")}, {'a', QLatin1String("abstract")},   {'a', QLatin1String("as")},
        {'f', QLatin1String("from")},      {'e', QLatin1String("extends")},    {'a', QLatin1String("async")},
        {'a', QLatin1String("await")}};

    typescript_types = {{'v', QLatin1String("var")}, {'c', QLatin1String("class")}, {'e', QLatin1String("enum")}};

    typescript_literals = {{('f'), QLatin1String("false")}, {('n'), QLatin1String("null")},
                           {('t'), QLatin1String("true")},  {('u'), QLatin1String("undefined")},
                           {('N'), QLatin1String("NaN")},   {('I'), QLatin1String("Infinity")}};

    typescript_builtin = {{'e', QLatin1String("eval")},
                          {'i', QLatin1String("isFinite")},
                          {'i', QLatin1String("isNaN")},
                          {'p', QLatin1String("parseFloat")},
                          {'p', QLatin1String("parseInt")},
                          {'d', QLatin1String("decodeURI")},
                          {'d', QLatin1String("decodeURIComponent")},
                          {'e', QLatin1String("encodeURI")},
                          {'e', QLatin1String("encodeURIComponent")},
                          {'e', QLatin1String("escape")},
                          {'u', QLatin1String("unescape")},
                          {'O', QLatin1String("Object")},
                          {'F', QLatin1String("Function")},
                          {'B', QLatin1String("Boolean")},
                          {'E', QLatin1String("Error")},
                          {'E', QLatin1String("EvalError")},
                          {'I', QLatin1String("InternalError")},
                          {'R', QLatin1String("RangeError")},
                          {'R', QLatin1String("ReferenceError")},
                          {'S', QLatin1String("StopIteration")},
                          {'S', QLatin1String("SyntaxError")},
                          {'T', QLatin1String("TypeError")},
                          {'U', QLatin1String("URIError")},
                          {'N', QLatin1String("Number")},
                          {'M', QLatin1String("Math")},
                          {'D', QLatin1String("Date")},
                          {'S', QLatin1String("String")},
                          {'R', QLatin1String("RegExp")},
                          {'A', QLatin1String("Array")},
                          {'F', QLatin1String("Float32Array")},
                          {'F', QLatin1String("Float64Array")},
                          {'I', QLatin1String("Int16Array")},
                          {'I', QLatin1String("Int32Array")},
                          {'I', QLatin1String("Int8Array")},
                          {'U', QLatin1String("Uint16Array")},
                          {'U', QLatin1String("Uint32Array")},
                          {'U', QLatin1String("Uint8Array")},
                          {'U', QLatin1String("Uint8ClampedArray")},
                          {'A', QLatin1String("ArrayBuffer")},
                          {'D', QLatin1String("DataView")},
                          {'J', QLatin1String("JSON")},
                          {'I', QLatin1String("Intl")},
                          {'a', QLatin1String("arguments")},
                          {'r', QLatin1String("require")},
                          {'m', QLatin1String("module")},
                          {'c', QLatin1String("console")},
                          {'w', QLatin1String("window")},
                          {'d', QLatin1String("document")},
                          {'a', QLatin1String("any")},
                          {'n', QLatin1String("number")},
                          {'b', QLatin1String("boolean")},
                          {'s', QLatin1String("string")},
                          {'v', QLatin1String("void")},
                          {'P', QLatin1String("Promise")}};

    typescript_other = {};
}
void loadTypescriptData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                        QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                        QMultiHash<char, QLatin1String> &other)
{
    if (!typescriptDataInitialized)
    {
        initTypescriptData();
        typescriptDataInitialized = true;
    }
    types = typescript_types;
    keywords = typescript_keywords;
    builtin = typescript_builtin;
    literals = typescript_literals;
    other = typescript_other;
}

/********************************************************/
/***   YAML DATA  ***************************************/
/********************************************************/
static bool YAMLDataInitialized = false;
static QMultiHash<char, QLatin1String> YAML_keywords;
static QMultiHash<char, QLatin1String> YAML_types;
static QMultiHash<char, QLatin1String> YAML_literals;
static QMultiHash<char, QLatin1String> YAML_builtin;
static QMultiHash<char, QLatin1String> YAML_other;
void initYAMLData()
{
    YAML_keywords = {};
    YAML_types = {};
    YAML_literals = {
        {('f'), QLatin1String("false")},
        {('t'), QLatin1String("true")},
        {('n'), QLatin1String("null")},
    };

    YAML_builtin = {};
    YAML_other = {};
}
void loadYAMLData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                  QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                  QMultiHash<char, QLatin1String> &other)
{
    if (!YAMLDataInitialized)
    {
        initYAMLData();
        YAMLDataInitialized = true;
    }
    types = YAML_types;
    keywords = YAML_keywords;
    builtin = YAML_builtin;
    literals = YAML_literals;
    other = YAML_other;
}

/********************************************************/
/***   VEX DATA   ***************************************/
/********************************************************/
static bool vexDataInitialized = false;
static QMultiHash<char, QLatin1String> vex_keywords;
static QMultiHash<char, QLatin1String> vex_types;
static QMultiHash<char, QLatin1String> vex_literals;
static QMultiHash<char, QLatin1String> vex_builtin;
static QMultiHash<char, QLatin1String> vex_other;
void initVEXData()
{
    vex_keywords = {
        {'b', QLatin1String("break")},     {'c', QLatin1String("continue")},    {'d', QLatin1String("do")},
        {'e', QLatin1String("else")},      {'f', QLatin1String("for")},         {'f', QLatin1String("foreach")},
        {'f', QLatin1String("forpoints")}, {'f', QLatin1String("function")},    {'g', QLatin1String("gather")},
        {'i', QLatin1String("if")},        {'i', QLatin1String("illuminance")}, {'r', QLatin1String("return")},
        {'w', QLatin1String("while")}};
    vex_types = {
        {'b', QLatin1String("bsdf")},    {'c', QLatin1String("char")},    {'c', QLatin1String("color")},
        {'f', QLatin1String("float")},   {'i', QLatin1String("int")},     {'i', QLatin1String("integer")},
        {'m', QLatin1String("matrix")},  {'m', QLatin1String("matrix2")}, {'m', QLatin1String("matrix3")},
        {'m', QLatin1String("matrix4")}, {'n', QLatin1String("normal")},  {'p', QLatin1String("point")},
        {'s', QLatin1String("string")},  {'s', QLatin1String("struct")},  {'t', QLatin1String("typedef")},
        {'u', QLatin1String("union")},   {'v', QLatin1String("vector")},  {'v', QLatin1String("vector2")},
        {'v', QLatin1String("vector4")}, {'v', QLatin1String("void")},
    };
    vex_literals = {
        {('f'), QLatin1String("false")},
        {('t'), QLatin1String("true")},
        {('n'), QLatin1String("null")},
    };

    vex_builtin = {{'D', QLatin1String("Du")},
                   {'D', QLatin1String("Dv")},
                   {'D', QLatin1String("Dw")},
                   {'a', QLatin1String("abs")},
                   {'a', QLatin1String("accessframe")},
                   {'a', QLatin1String("acos")},
                   {'a', QLatin1String("addattrib")},
                   {'a', QLatin1String("addattribute")},
                   {'a', QLatin1String("adddetailattrib")},
                   {'a', QLatin1String("addgroup")},
                   {'a', QLatin1String("addpoint")},
                   {'a', QLatin1String("addpointattrib")},
                   {'a', QLatin1String("addprim")},
                   {'a', QLatin1String("addprimattrib")},
                   {'a', QLatin1String("addvariablename")},
                   {'a', QLatin1String("addvertex")},
                   {'a', QLatin1String("addvertexattrib")},
                   {'a', QLatin1String("addvisualizer")},
                   {'a', QLatin1String("agentaddclip")},
                   {'a', QLatin1String("agentclipcatalog")},
                   {'a', QLatin1String("agentclipchannel")},
                   {'a', QLatin1String("agentcliplength")},
                   {'a', QLatin1String("agentclipnames")},
                   {'a', QLatin1String("agentclipsample")},
                   {'a', QLatin1String("agentclipsamplelocal")},
                   {'a', QLatin1String("agentclipsamplerate")},
                   {'a', QLatin1String("agentclipsampleworld")},
                   {'a', QLatin1String("agentcliptimes")},
                   {'a', QLatin1String("agentclipweights")},
                   {'a', QLatin1String("agentcollisionlayer")},
                   {'a', QLatin1String("agentcurrentlayer")},
                   {'a', QLatin1String("agentlayerbindings")},
                   {'a', QLatin1String("agentlayers")},
                   {'a', QLatin1String("agentlayershapes")},
                   {'a', QLatin1String("agentlocaltransform")},
                   {'a', QLatin1String("agentlocaltransforms")},
                   {'a', QLatin1String("agentrigchildren")},
                   {'a', QLatin1String("agentrigfind")},
                   {'a', QLatin1String("agentrigparent")},
                   {'a', QLatin1String("agenttransformcount")},
                   {'a', QLatin1String("agenttransformnames")},
                   {'a', QLatin1String("agenttransformtolocal")},
                   {'a', QLatin1String("agenttransformtoworld")},
                   {'a', QLatin1String("agentworldtransform")},
                   {'a', QLatin1String("agentworldtransforms")},
                   {'a', QLatin1String("albedo")},
                   {'a', QLatin1String("alphaname")},
                   {'a', QLatin1String("ambient")},
                   {'a', QLatin1String("anoise")},
                   {'a', QLatin1String("append")},
                   {'a', QLatin1String("area")},
                   {'a', QLatin1String("argsort")},
                   {'a', QLatin1String("array")},
                   {'a', QLatin1String("ashikhmin")},
                   {'a', QLatin1String("asin")},
                   {'a', QLatin1String("assert_enabled")},
                   {'a', QLatin1String("assign")},
                   {'a', QLatin1String("atan")},
                   {'a', QLatin1String("atan2")},
                   {'a', QLatin1String("atof")},
                   {'a', QLatin1String("atoi")},
                   {'a', QLatin1String("atten")},
                   {'a', QLatin1String("attrib")},
                   {'a', QLatin1String("attribclass")},
                   {'a', QLatin1String("attribsize")},
                   {'a', QLatin1String("attribtype")},
                   {'a', QLatin1String("attribtypeinfo")},
                   {'a', QLatin1String("avg")},
                   {'b', QLatin1String("binput")},
                   {'b', QLatin1String("blackbody")},
                   {'b', QLatin1String("blinn")},
                   {'b', QLatin1String("blinnBRDF")},
                   {'b', QLatin1String("bouncelabel")},
                   {'b', QLatin1String("bouncemask")},
                   {'b', QLatin1String("bumpmap")},
                   {'b', QLatin1String("bumpmapA")},
                   {'b', QLatin1String("bumpmapB")},
                   {'b', QLatin1String("bumpmapG")},
                   {'b', QLatin1String("bumpmapL")},
                   {'b', QLatin1String("bumpmapR")},
                   {'b', QLatin1String("bumpname")},
                   {'c', QLatin1String("cbrt")},
                   {'c', QLatin1String("ceil")},
                   {'c', QLatin1String("ch")},
                   {'c', QLatin1String("ch3")},
                   {'c', QLatin1String("ch4")},
                   {'c', QLatin1String("chend")},
                   {'c', QLatin1String("chendf")},
                   {'c', QLatin1String("chendt")},
                   {'c', QLatin1String("chf")},
                   {'c', QLatin1String("chi")},
                   {'c', QLatin1String("chinput")},
                   {'c', QLatin1String("chname")},
                   {'c', QLatin1String("chnumchan")},
                   {'c', QLatin1String("chp")},
                   {'c', QLatin1String("chr")},
                   {'c', QLatin1String("chramp")},
                   {'c', QLatin1String("chrate")},
                   {'c', QLatin1String("chs")},
                   {'c', QLatin1String("chsraw")},
                   {'c', QLatin1String("chstart")},
                   {'c', QLatin1String("chstartf")},
                   {'c', QLatin1String("chstartt")},
                   {'c', QLatin1String("chv")},
                   {'c', QLatin1String("cinput")},
                   {'c', QLatin1String("ckspline")},
                   {'c', QLatin1String("clamp")},
                   {'c', QLatin1String("clip")},
                   {'c', QLatin1String("colormap")},
                   {'c', QLatin1String("colorname")},
                   {'c', QLatin1String("computenormal")},
                   {'c', QLatin1String("concat")},
                   {'c', QLatin1String("cone")},
                   {'c', QLatin1String("cos")},
                   {'c', QLatin1String("cosh")},
                   {'c', QLatin1String("cracktransform")},
                   {'c', QLatin1String("cross")},
                   {'c', QLatin1String("cspline")},
                   {'c', QLatin1String("ctransform")},
                   {'c', QLatin1String("curlnoise")},
                   {'c', QLatin1String("curlnoise2d")},
                   {'c', QLatin1String("curlxnoise")},
                   {'c', QLatin1String("curlxnoise2d")},
                   {'c', QLatin1String("cvex_bsdf")},
                   {'d', QLatin1String("degrees")},
                   {'d', QLatin1String("depthmap")},
                   {'d', QLatin1String("depthname")},
                   {'d', QLatin1String("detail")},
                   {'d', QLatin1String("detailattrib")},
                   {'d', QLatin1String("detailattribsize")},
                   {'d', QLatin1String("detailattribtype")},
                   {'d', QLatin1String("detailattribtypeinfo")},
                   {'d', QLatin1String("detailintrinsic")},
                   {'d', QLatin1String("determinant")},
                   {'d', QLatin1String("diffuse")},
                   {'d', QLatin1String("diffuseBRDF")},
                   {'d', QLatin1String("dihedral")},
                   {'d', QLatin1String("dimport")},
                   {'d', QLatin1String("distance")},
                   {'d', QLatin1String("distance2")},
                   {'d', QLatin1String("dot")},
                   {'d', QLatin1String("dsmpixel")},
                   {'e', QLatin1String("eigenvalues")},
                   {'e', QLatin1String("endswith")},
                   {'e', QLatin1String("environment")},
                   {'e', QLatin1String("erf")},
                   {'e', QLatin1String("erf_inv")},
                   {'e', QLatin1String("erfc")},
                   {'e', QLatin1String("error")},
                   {'e', QLatin1String("eulertoquaternion")},
                   {'e', QLatin1String("eval_bsdf")},
                   {'e', QLatin1String("exp")},
                   {'e', QLatin1String("expand_udim")},
                   {'e', QLatin1String("expandpointgroup")},
                   {'e', QLatin1String("expandprimgroup")},
                   {'f', QLatin1String("fastshadow")},
                   {'f', QLatin1String("filamentsample")},
                   {'f', QLatin1String("file_stat")},
                   {'f', QLatin1String("filtershadow")},
                   {'f', QLatin1String("filterstep")},
                   {'f', QLatin1String("find")},
                   {'f', QLatin1String("findattribval")},
                   {'f', QLatin1String("findattribvalcount")},
                   {'f', QLatin1String("finput")},
                   {'f', QLatin1String("fit")},
                   {'f', QLatin1String("fit01")},
                   {'f', QLatin1String("fit10")},
                   {'f', QLatin1String("fit11")},
                   {'f', QLatin1String("floor")},
                   {'f', QLatin1String("flownoise")},
                   {'f', QLatin1String("flowpnoise")},
                   {'f', QLatin1String("frac")},
                   {'f', QLatin1String("fresnel")},
                   {'f', QLatin1String("fromNDC")},
                   {'f', QLatin1String("frontface")},
                   {'f', QLatin1String("fuzzify")},
                   {'f', QLatin1String("fuzzy_and")},
                   {'f', QLatin1String("fuzzy_defuzz_centroid")},
                   {'f', QLatin1String("fuzzy_nand")},
                   {'f', QLatin1String("fuzzy_nor")},
                   {'f', QLatin1String("fuzzy_not")},
                   {'f', QLatin1String("fuzzy_nxor")},
                   {'f', QLatin1String("fuzzy_or")},
                   {'f', QLatin1String("fuzzy_xor")},
                   {'g', QLatin1String("geoself")},
                   {'g', QLatin1String("getattrib")},
                   {'g', QLatin1String("getattribute")},
                   {'g', QLatin1String("getbbox")},
                   {'g', QLatin1String("getblurP")},
                   {'g', QLatin1String("getbounces")},
                   {'g', QLatin1String("getbounds")},
                   {'g', QLatin1String("getcomp")},
                   {'g', QLatin1String("getcomponents")},
                   {'g', QLatin1String("getderiv")},
                   {'g', QLatin1String("getfogname")},
                   {'g', QLatin1String("getglobalraylevel")},
                   {'g', QLatin1String("getlight")},
                   {'g', QLatin1String("getlightid")},
                   {'g', QLatin1String("getlightname")},
                   {'g', QLatin1String("getlights")},
                   {'g', QLatin1String("getlightscope")},
                   {'g', QLatin1String("getmaterial")},
                   {'g', QLatin1String("getobjectname")},
                   {'g', QLatin1String("getphotonlight")},
                   {'g', QLatin1String("getpointbbox")},
                   {'g', QLatin1String("getprimid")},
                   {'g', QLatin1String("getptextureid")},
                   {'g', QLatin1String("getraylevel")},
                   {'g', QLatin1String("getrayweight")},
                   {'g', QLatin1String("getsamplestore")},
                   {'g', QLatin1String("getscope")},
                   {'g', QLatin1String("getsmoothP")},
                   {'g', QLatin1String("getspace")},
                   {'g', QLatin1String("getuvobjects")},
                   {'g', QLatin1String("getuvtangents")},
                   {'g', QLatin1String("gradient")},
                   {'h', QLatin1String("hair")},
                   {'h', QLatin1String("hasattrib")},
                   {'h', QLatin1String("hasdetailattrib")},
                   {'h', QLatin1String("haslight")},
                   {'h', QLatin1String("hasplane")},
                   {'h', QLatin1String("haspointattrib")},
                   {'h', QLatin1String("hasprimattrib")},
                   {'h', QLatin1String("hasvertexattrib")},
                   {'h', QLatin1String("hedge_dstpoint")},
                   {'h', QLatin1String("hedge_dstvertex")},
                   {'h', QLatin1String("hedge_equivcount")},
                   {'h', QLatin1String("hedge_isequiv")},
                   {'h', QLatin1String("hedge_isprimary")},
                   {'h', QLatin1String("hedge_isvalid")},
                   {'h', QLatin1String("hedge_next")},
                   {'h', QLatin1String("hedge_nextequiv")},
                   {'h', QLatin1String("hedge_postdstpoint")},
                   {'h', QLatin1String("hedge_postdstvertex")},
                   {'h', QLatin1String("hedge_presrcpoint")},
                   {'h', QLatin1String("hedge_presrcvertex")},
                   {'h', QLatin1String("hedge_prev")},
                   {'h', QLatin1String("hedge_prim")},
                   {'h', QLatin1String("hedge_primary")},
                   {'h', QLatin1String("hedge_srcpoint")},
                   {'h', QLatin1String("hedge_srcvertex")},
                   {'h', QLatin1String("henyeygreenstein")},
                   {'h', QLatin1String("hscript_noise")},
                   {'h', QLatin1String("hscript_rand")},
                   {'h', QLatin1String("hscript_snoise")},
                   {'h', QLatin1String("hscript_sturb")},
                   {'h', QLatin1String("hscript_turb")},
                   {'h', QLatin1String("hsvtorgb")},
                   {'i', QLatin1String("iaspect")},
                   {'i', QLatin1String("ichname")},
                   {'i', QLatin1String("ident")},
                   {'i', QLatin1String("idtopoint")},
                   {'i', QLatin1String("idtoprim")},
                   {'i', QLatin1String("iend")},
                   {'i', QLatin1String("iendtime")},
                   {'i', QLatin1String("ihasplane")},
                   {'i', QLatin1String("import")},
                   {'i', QLatin1String("ingroup")},
                   {'i', QLatin1String("inpointgroup")},
                   {'i', QLatin1String("inprimgroup")},
                   {'i', QLatin1String("insert")},
                   {'i', QLatin1String("instance")},
                   {'i', QLatin1String("interpolate")},
                   {'i', QLatin1String("intersect")},
                   {'i', QLatin1String("intersect_all")},
                   {'i', QLatin1String("intersect_lights")},
                   {'i', QLatin1String("inumplanes")},
                   {'i', QLatin1String("invert")},
                   {'i', QLatin1String("invertexgroup")},
                   {'i', QLatin1String("iplaneindex")},
                   {'i', QLatin1String("iplanename")},
                   {'i', QLatin1String("iplanesize")},
                   {'i', QLatin1String("irate")},
                   {'i', QLatin1String("irradiance")},
                   {'i', QLatin1String("isalpha")},
                   {'i', QLatin1String("isbound")},
                   {'i', QLatin1String("isconnected")},
                   {'i', QLatin1String("isdigit")},
                   {'i', QLatin1String("isfinite")},
                   {'i', QLatin1String("isfogray")},
                   {'i', QLatin1String("isframes")},
                   {'i', QLatin1String("isnan")},
                   {'i', QLatin1String("isotropic")},
                   {'i', QLatin1String("israytracing")},
                   {'i', QLatin1String("issamples")},
                   {'i', QLatin1String("isseconds")},
                   {'i', QLatin1String("isshadowray")},
                   {'i', QLatin1String("istart")},
                   {'i', QLatin1String("istarttime")},
                   {'i', QLatin1String("isuvrendering")},
                   {'i', QLatin1String("isvalidindex")},
                   {'i', QLatin1String("isvarying")},
                   {'i', QLatin1String("itoa")},
                   {'i', QLatin1String("ixres")},
                   {'i', QLatin1String("iyres")},
                   {'j', QLatin1String("join")},
                   {'k', QLatin1String("kspline")},
                   {'l', QLatin1String("len")},
                   {'l', QLatin1String("length")},
                   {'l', QLatin1String("length2")},
                   {'l', QLatin1String("lerp")},
                   {'l', QLatin1String("lightid")},
                   {'l', QLatin1String("limit_sample_space")},
                   {'l', QLatin1String("limport")},
                   {'l', QLatin1String("lkspline")},
                   {'l', QLatin1String("log")},
                   {'l', QLatin1String("log10")},
                   {'l', QLatin1String("lookat")},
                   {'l', QLatin1String("lspline")},
                   {'l', QLatin1String("lstrip")},
                   {'l', QLatin1String("luminance")},
                   {'l', QLatin1String("lumname")},
                   {'m', QLatin1String("makebasis")},
                   {'m', QLatin1String("maketransform")},
                   {'m', QLatin1String("maskname")},
                   {'m', QLatin1String("match")},
                   {'m', QLatin1String("matchvex_blinn")},
                   {'m', QLatin1String("matchvex_specular")},
                   {'m', QLatin1String("mattrib")},
                   {'m', QLatin1String("max")},
                   {'m', QLatin1String("mdensity")},
                   {'m', QLatin1String("metaimport")},
                   {'m', QLatin1String("metamarch")},
                   {'m', QLatin1String("metanext")},
                   {'m', QLatin1String("metastart")},
                   {'m', QLatin1String("metaweight")},
                   {'m', QLatin1String("min")},
                   {'m', QLatin1String("minpos")},
                   {'m', QLatin1String("mspace")},
                   {'n', QLatin1String("nametopoint")},
                   {'n', QLatin1String("nametoprim")},
                   {'n', QLatin1String("nbouncetypes")},
                   {'n', QLatin1String("nearpoint")},
                   {'n', QLatin1String("nearpoints")},
                   {'n', QLatin1String("neighbour")},
                   {'n', QLatin1String("neighbourcount")},
                   {'n', QLatin1String("neighbours")},
                   {'n', QLatin1String("newgroup")},
                   {'n', QLatin1String("newsampler")},
                   {'n', QLatin1String("nextsample")},
                   {'n', QLatin1String("ninput")},
                   {'n', QLatin1String("noise")},
                   {'n', QLatin1String("noised")},
                   {'n', QLatin1String("normal_bsdf")},
                   {'n', QLatin1String("normalize")},
                   {'n', QLatin1String("normalname")},
                   {'n', QLatin1String("npoints")},
                   {'n', QLatin1String("npointsgroup")},
                   {'n', QLatin1String("nprimitives")},
                   {'n', QLatin1String("nprimitivesgroup")},
                   {'n', QLatin1String("nrandom")},
                   {'n', QLatin1String("ntransform")},
                   {'n', QLatin1String("nuniqueval")},
                   {'n', QLatin1String("nvertices")},
                   {'n', QLatin1String("nverticesgroup")},
                   {'o', QLatin1String("occlusion")},
                   {'o', QLatin1String("onoise")},
                   {'o', QLatin1String("opdigits")},
                   {'o', QLatin1String("opend")},
                   {'o', QLatin1String("opfullpath")},
                   {'o', QLatin1String("opstart")},
                   {'o', QLatin1String("optransform")},
                   {'o', QLatin1String("ord")},
                   {'o', QLatin1String("osd_facecount")},
                   {'o', QLatin1String("osd_firstpatch")},
                   {'o', QLatin1String("osd_limitsurface")},
                   {'o', QLatin1String("osd_limitsurfacevertex")},
                   {'o', QLatin1String("osd_patchcount")},
                   {'o', QLatin1String("osd_patches")},
                   {'o', QLatin1String("outerproduct")},
                   {'o', QLatin1String("ow_nspace")},
                   {'o', QLatin1String("ow_space")},
                   {'o', QLatin1String("ow_vspace")},
                   {'p', QLatin1String("pack_inttosafefloat")},
                   {'p', QLatin1String("pathtrace")},
                   {'p', QLatin1String("pcclose")},
                   {'p', QLatin1String("pcconvex")},
                   {'p', QLatin1String("pcexport")},
                   {'p', QLatin1String("pcfarthest")},
                   {'p', QLatin1String("pcfilter")},
                   {'p', QLatin1String("pcfind")},
                   {'p', QLatin1String("pcfind_radius")},
                   {'p', QLatin1String("pcgenerate")},
                   {'p', QLatin1String("pcimport")},
                   {'p', QLatin1String("pcimportbyidx3")},
                   {'p', QLatin1String("pcimportbyidx4")},
                   {'p', QLatin1String("pcimportbyidxf")},
                   {'p', QLatin1String("pcimportbyidxi")},
                   {'p', QLatin1String("pcimportbyidxp")},
                   {'p', QLatin1String("pcimportbyidxs")},
                   {'p', QLatin1String("pcimportbyidxv")},
                   {'p', QLatin1String("pciterate")},
                   {'p', QLatin1String("pcnumfound")},
                   {'p', QLatin1String("pcopen")},
                   {'p', QLatin1String("pcopenlod")},
                   {'p', QLatin1String("pcsampleleaf")},
                   {'p', QLatin1String("pcsize")},
                   {'p', QLatin1String("pcunshaded")},
                   {'p', QLatin1String("pcwrite")},
                   {'p', QLatin1String("pgfind")},
                   {'p', QLatin1String("phong")},
                   {'p', QLatin1String("phongBRDF")},
                   {'p', QLatin1String("phonglobe")},
                   {'p', QLatin1String("photonmap")},
                   {'p', QLatin1String("planeindex")},
                   {'p', QLatin1String("planename")},
                   {'p', QLatin1String("planesize")},
                   {'p', QLatin1String("pluralize")},
                   {'p', QLatin1String("pnoise")},
                   {'p', QLatin1String("point")},
                   {'p', QLatin1String("pointattrib")},
                   {'p', QLatin1String("pointattribsize")},
                   {'p', QLatin1String("pointattribtype")},
                   {'p', QLatin1String("pointattribtypeinfo")},
                   {'p', QLatin1String("pointedge")},
                   {'p', QLatin1String("pointhedge")},
                   {'p', QLatin1String("pointhedgenext")},
                   {'p', QLatin1String("pointname")},
                   {'p', QLatin1String("pointprims")},
                   {'p', QLatin1String("pointvertex")},
                   {'p', QLatin1String("pointvertices")},
                   {'p', QLatin1String("polardecomp")},
                   {'p', QLatin1String("pop")},
                   {'p', QLatin1String("pow")},
                   {'p', QLatin1String("prim")},
                   {'p', QLatin1String("prim_attribute")},
                   {'p', QLatin1String("prim_normal")},
                   {'p', QLatin1String("primattrib")},
                   {'p', QLatin1String("primattribsize")},
                   {'p', QLatin1String("primattribtype")},
                   {'p', QLatin1String("primattribtypeinfo")},
                   {'p', QLatin1String("primhedge")},
                   {'p', QLatin1String("primintrinsic")},
                   {'p', QLatin1String("primpoint")},
                   {'p', QLatin1String("primpoints")},
                   {'p', QLatin1String("primuv")},
                   {'p', QLatin1String("primvertex")},
                   {'p', QLatin1String("primvertexcount")},
                   {'p', QLatin1String("primvertices")},
                   {'p', QLatin1String("print_once")},
                   {'p', QLatin1String("printf")},
                   {'p', QLatin1String("product")},
                   {'p', QLatin1String("ptexture")},
                   {'p', QLatin1String("ptlined")},
                   {'p', QLatin1String("ptransform")},
                   {'p', QLatin1String("push")},
                   {'q', QLatin1String("qconvert")},
                   {'q', QLatin1String("qdistance")},
                   {'q', QLatin1String("qinvert")},
                   {'q', QLatin1String("qmultiply")},
                   {'q', QLatin1String("qrotate")},
                   {'q', QLatin1String("quaternion")},
                   {'r', QLatin1String("radians")},
                   {'r', QLatin1String("rand")},
                   {'r', QLatin1String("random")},
                   {'r', QLatin1String("random_fhash")},
                   {'r', QLatin1String("random_ihash")},
                   {'r', QLatin1String("random_shash")},
                   {'r', QLatin1String("random_sobol")},
                   {'r', QLatin1String("rawbumpmap")},
                   {'r', QLatin1String("rawbumpmapA")},
                   {'r', QLatin1String("rawbumpmapB")},
                   {'r', QLatin1String("rawbumpmapG")},
                   {'r', QLatin1String("rawbumpmapL")},
                   {'r', QLatin1String("rawbumpmapR")},
                   {'r', QLatin1String("rawcolormap")},
                   {'r', QLatin1String("rayhittest")},
                   {'r', QLatin1String("rayimport")},
                   {'r', QLatin1String("re_find")},
                   {'r', QLatin1String("re_findall")},
                   {'r', QLatin1String("re_match")},
                   {'r', QLatin1String("re_replace")},
                   {'r', QLatin1String("re_split")},
                   {'r', QLatin1String("reflect")},
                   {'r', QLatin1String("reflectlight")},
                   {'r', QLatin1String("refract")},
                   {'r', QLatin1String("refractlight")},
                   {'r', QLatin1String("relativepath")},
                   {'r', QLatin1String("relbbox")},
                   {'r', QLatin1String("relpointbbox")},
                   {'r', QLatin1String("removegroup")},
                   {'r', QLatin1String("removeindex")},
                   {'r', QLatin1String("removepoint")},
                   {'r', QLatin1String("removeprim")},
                   {'r', QLatin1String("removevalue")},
                   {'r', QLatin1String("renderstate")},
                   {'r', QLatin1String("reorder")},
                   {'r', QLatin1String("resample_linear")},
                   {'r', QLatin1String("resize")},
                   {'r', QLatin1String("resolvemissedray")},
                   {'r', QLatin1String("reverse")},
                   {'r', QLatin1String("rgbtohsv")},
                   {'r', QLatin1String("rgbtoxyz")},
                   {'r', QLatin1String("rint")},
                   {'r', QLatin1String("rotate")},
                   {'r', QLatin1String("rotate_x_to")},
                   {'r', QLatin1String("rstrip")},
                   {'s', QLatin1String("sample_bsdf")},
                   {'s', QLatin1String("sample_cauchy")},
                   {'s', QLatin1String("sample_circle_arc")},
                   {'s', QLatin1String("sample_circle_edge_uniform")},
                   {'s', QLatin1String("sample_circle_slice")},
                   {'s', QLatin1String("sample_circle_uniform")},
                   {'s', QLatin1String("sample_direction_cone")},
                   {'s', QLatin1String("sample_direction_uniform")},
                   {'s', QLatin1String("sample_discrete")},
                   {'s', QLatin1String("sample_exponential")},
                   {'s', QLatin1String("sample_geometry")},
                   {'s', QLatin1String("sample_hemisphere")},
                   {'s', QLatin1String("sample_hypersphere_cone")},
                   {'s', QLatin1String("sample_hypersphere_uniform")},
                   {'s', QLatin1String("sample_light")},
                   {'s', QLatin1String("sample_lognormal")},
                   {'s', QLatin1String("sample_lognormal_by_median")},
                   {'s', QLatin1String("sample_normal")},
                   {'s', QLatin1String("sample_orientation_cone")},
                   {'s', QLatin1String("sample_orientation_uniform")},
                   {'s', QLatin1String("sample_photon")},
                   {'s', QLatin1String("sample_sphere_cone")},
                   {'s', QLatin1String("sample_sphere_uniform")},
                   {'s', QLatin1String("sampledisk")},
                   {'s', QLatin1String("scale")},
                   {'s', QLatin1String("select")},
                   {'s', QLatin1String("sensor_panorama_create")},
                   {'s', QLatin1String("sensor_panorama_getcolor")},
                   {'s', QLatin1String("sensor_panorama_getcone")},
                   {'s', QLatin1String("sensor_panorama_getdepth")},
                   {'s', QLatin1String("sensor_save")},
                   {'s', QLatin1String("serialize")},
                   {'s', QLatin1String("set")},
                   {'s', QLatin1String("setagentclipnames")},
                   {'s', QLatin1String("setagentcliptimes")},
                   {'s', QLatin1String("setagentclipweights")},
                   {'s', QLatin1String("setagentcollisionlayer")},
                   {'s', QLatin1String("setagentcurrentlayer")},
                   {'s', QLatin1String("setagentlocaltransform")},
                   {'s', QLatin1String("setagentlocaltransforms")},
                   {'s', QLatin1String("setagentworldtransform")},
                   {'s', QLatin1String("setagentworldtransforms")},
                   {'s', QLatin1String("setattrib")},
                   {'s', QLatin1String("setattribtypeinfo")},
                   {'s', QLatin1String("setcomp")},
                   {'s', QLatin1String("setcurrentlight")},
                   {'s', QLatin1String("setdetailattrib")},
                   {'s', QLatin1String("setpointattrib")},
                   {'s', QLatin1String("setpointgroup")},
                   {'s', QLatin1String("setprimattrib")},
                   {'s', QLatin1String("setprimgroup")},
                   {'s', QLatin1String("setprimintrinsic")},
                   {'s', QLatin1String("setprimvertex")},
                   {'s', QLatin1String("setsamplestore")},
                   {'s', QLatin1String("setvertexattrib")},
                   {'s', QLatin1String("setvertexgroup")},
                   {'s', QLatin1String("setvertexpoint")},
                   {'s', QLatin1String("shadow")},
                   {'s', QLatin1String("shadow_light")},
                   {'s', QLatin1String("shadowmap")},
                   {'s', QLatin1String("shimport")},
                   {'s', QLatin1String("shl")},
                   {'s', QLatin1String("shr")},
                   {'s', QLatin1String("shrz")},
                   {'s', QLatin1String("sign")},
                   {'s', QLatin1String("simport")},
                   {'s', QLatin1String("sin")},
                   {'s', QLatin1String("sinh")},
                   {'s', QLatin1String("sleep")},
                   {'s', QLatin1String("slerp")},
                   {'s', QLatin1String("slice")},
                   {'s', QLatin1String("slideframe")},
                   {'s', QLatin1String("smooth")},
                   {'s', QLatin1String("smoothrotation")},
                   {'s', QLatin1String("snoise")},
                   {'s', QLatin1String("solvecubic")},
                   {'s', QLatin1String("solvepoly")},
                   {'s', QLatin1String("solvequadratic")},
                   {'s', QLatin1String("sort")},
                   {'s', QLatin1String("specular")},
                   {'s', QLatin1String("specularBRDF")},
                   {'s', QLatin1String("spline")},
                   {'s', QLatin1String("split")},
                   {'s', QLatin1String("splitpath")},
                   {'s', QLatin1String("sprintf")},
                   {'s', QLatin1String("sqrt")},
                   {'s', QLatin1String("startswith")},
                   {'s', QLatin1String("storelightexport")},
                   {'s', QLatin1String("strip")},
                   {'s', QLatin1String("strlen")},
                   {'s', QLatin1String("sum")},
                   {'s', QLatin1String("switch")},
                   {'s', QLatin1String("swizzle")},
                   {'t', QLatin1String("tan")},
                   {'t', QLatin1String("tanh")},
                   {'t', QLatin1String("tet_adjacent")},
                   {'t', QLatin1String("tet_faceindex")},
                   {'t', QLatin1String("teximport")},
                   {'t', QLatin1String("texprintf")},
                   {'t', QLatin1String("texture")},
                   {'t', QLatin1String("texture3d")},
                   {'t', QLatin1String("texture3dBox")},
                   {'t', QLatin1String("titlecase")},
                   {'t', QLatin1String("toNDC")},
                   {'t', QLatin1String("tolower")},
                   {'t', QLatin1String("toupper")},
                   {'t', QLatin1String("trace")},
                   {'t', QLatin1String("translate")},
                   {'t', QLatin1String("translucent")},
                   {'t', QLatin1String("transpose")},
                   {'t', QLatin1String("trunc")},
                   {'t', QLatin1String("tw_nspace")},
                   {'t', QLatin1String("tw_space")},
                   {'t', QLatin1String("tw_vspace")},
                   {'u', QLatin1String("uniqueval")},
                   {'u', QLatin1String("unpack_intfromsafefloat")},
                   {'u', QLatin1String("unserialize")},
                   {'u', QLatin1String("upush")},
                   {'u', QLatin1String("uvunwrap")},
                   {'v', QLatin1String("variance")},
                   {'v', QLatin1String("velocityname")},
                   {'v', QLatin1String("vertex")},
                   {'v', QLatin1String("vertexattrib")},
                   {'v', QLatin1String("vertexattribsize")},
                   {'v', QLatin1String("vertexattribtype")},
                   {'v', QLatin1String("vertexattribtypeinfo")},
                   {'v', QLatin1String("vertexhedge")},
                   {'v', QLatin1String("vertexindex")},
                   {'v', QLatin1String("vertexnext")},
                   {'v', QLatin1String("vertexpoint")},
                   {'v', QLatin1String("vertexprev")},
                   {'v', QLatin1String("vertexprim")},
                   {'v', QLatin1String("vertexprimindex")},
                   {'v', QLatin1String("vnoise")},
                   {'v', QLatin1String("volume")},
                   {'v', QLatin1String("volumegradient")},
                   {'v', QLatin1String("volumeindex")},
                   {'v', QLatin1String("volumeindexorigin")},
                   {'v', QLatin1String("volumeindextopos")},
                   {'v', QLatin1String("volumeindexv")},
                   {'v', QLatin1String("volumepostoindex")},
                   {'v', QLatin1String("volumeres")},
                   {'v', QLatin1String("volumesample")},
                   {'v', QLatin1String("volumesamplev")},
                   {'v', QLatin1String("vtransform")},
                   {'w', QLatin1String("warning")},
                   {'w', QLatin1String("wireblinn")},
                   {'w', QLatin1String("wirediffuse")},
                   {'w', QLatin1String("wnoise")},
                   {'w', QLatin1String("wo_nspace")},
                   {'w', QLatin1String("wo_space")},
                   {'w', QLatin1String("wo_vspace")},
                   {'w', QLatin1String("writepixel")},
                   {'w', QLatin1String("wt_nspace")},
                   {'w', QLatin1String("wt_space")},
                   {'w', QLatin1String("wt_vspace")},
                   {'x', QLatin1String("xnoise")},
                   {'x', QLatin1String("xnoised")},
                   {'x', QLatin1String("xyzdist")},
                   {'x', QLatin1String("xyztorgb")}};
    vex_other = {
        {('d'), QLatin1String("define")},  {('e'), QLatin1String("else")},   {('e'), QLatin1String("endif")},
        {('i'), QLatin1String("if")},      {('i'), QLatin1String("ifdef")},  {('i'), QLatin1String("ifndef")},
        {('i'), QLatin1String("include")}, {('p'), QLatin1String("pragma")}, {('u'), QLatin1String("undef")},
    };
}
void loadVEXData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                 QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                 QMultiHash<char, QLatin1String> &other)
{
    if (!vexDataInitialized)
    {
        initVEXData();
        vexDataInitialized = true;
    }
    types = vex_types;
    keywords = vex_keywords;
    builtin = vex_builtin;
    literals = vex_literals;
    other = vex_other;
}

/********************************************************/
/***   CMAKE DATA   ***************************************/
/********************************************************/
static bool cmakeDataInitialized = false;
static QMultiHash<char, QLatin1String> cmake_keywords;
static QMultiHash<char, QLatin1String> cmake_types;
static QMultiHash<char, QLatin1String> cmake_literals;
static QMultiHash<char, QLatin1String> cmake_builtin;
static QMultiHash<char, QLatin1String> cmake_other;
void initCMakeData()
{
    cmake_keywords = {{'b', QLatin1String("break")},
                      {'c', QLatin1String("cmake_host_system_information")},
                      {'c', QLatin1String("cmake_minimum_required")},
                      {'c', QLatin1String("cmake_parse_arguments")},
                      {'c', QLatin1String("cmake_policy")},
                      {'c', QLatin1String("configure_file")},
                      {'c', QLatin1String("continue")},
                      {'e', QLatin1String("elseif")},
                      {'e', QLatin1String("else")},
                      {'e', QLatin1String("endforeach")},
                      {'e', QLatin1String("endfunction")},
                      {'e', QLatin1String("endif")},
                      {'e', QLatin1String("endmacro")},
                      {'e', QLatin1String("endwhile")},
                      {'e', QLatin1String("execute_process")},
                      {'f', QLatin1String("file")},
                      {'f', QLatin1String("find_file")},
                      {'f', QLatin1String("find_library")},
                      {'f', QLatin1String("find_package")},
                      {'f', QLatin1String("find_path")},
                      {'f', QLatin1String("find_program")},
                      {'f', QLatin1String("foreach")},
                      {'f', QLatin1String("function")},
                      {'g', QLatin1String("get_cmake_property")},
                      {'g', QLatin1String("get_directory_property")},
                      {'g', QLatin1String("get_filename_component")},
                      {'g', QLatin1String("get_property")},
                      {'i', QLatin1String("if")},
                      {'i', QLatin1String("include")},
                      {'i', QLatin1String("include_guard")},
                      {'l', QLatin1String("list")},
                      {'m', QLatin1String("macro")},
                      {'m', QLatin1String("mark_as_advanced")},
                      {'m', QLatin1String("math")},
                      {'m', QLatin1String("message")},
                      {'o', QLatin1String("option")},
                      {'r', QLatin1String("return")},
                      {'s', QLatin1String("separate_arguments")},
                      {'s', QLatin1String("set_directory_properties")},
                      {'s', QLatin1String("set")},
                      {'s', QLatin1String("set_property")},
                      {'s', QLatin1String("site_name")},
                      {'s', QLatin1String("string")},
                      {'u', QLatin1String("unset")},
                      {'v', QLatin1String("variable_watch")},
                      {'w', QLatin1String("while")},
                      {'a', QLatin1String("add_compile_definitions")},
                      {'a', QLatin1String("add_compile_options")},
                      {'A', QLatin1String("ADD_COMPILE_OPTIONS")},
                      {'a', QLatin1String("add_custom_command")},
                      {'a', QLatin1String("add_custom_target")},
                      {'a', QLatin1String("add_definitions")},
                      {'a', QLatin1String("add_dependencies")},
                      {'a', QLatin1String("add_executable")},
                      {'a', QLatin1String("add_library")},
                      {'a', QLatin1String("add_link_options")},
                      {'a', QLatin1String("add_subdirectory")},
                      {'a', QLatin1String("add_test")},
                      {'a', QLatin1String("aux_source_directory")},
                      {'b', QLatin1String("build_command")},
                      {'c', QLatin1String("create_test_sourcelist")},
                      {'d', QLatin1String("define_property")},
                      {'e', QLatin1String("enable_language")},
                      {'e', QLatin1String("enable_testing")},
                      {'e', QLatin1String("export")},
                      {'f', QLatin1String("fltk_wrap_ui")},
                      {'g', QLatin1String("get_source_file_property")},
                      {'g', QLatin1String("get_target_property")},
                      {'g', QLatin1String("get_test_property")},
                      {'i', QLatin1String("include_directories")},
                      {'i', QLatin1String("include_external_msproject")},
                      {'i', QLatin1String("include_regular_expression")},
                      {'i', QLatin1String("install")},
                      {'l', QLatin1String("link_directories")},
                      {'l', QLatin1String("link_libraries")},
                      {'l', QLatin1String("load_cache")},
                      {'p', QLatin1String("project")},
                      {'q', QLatin1String("qt_wrap_cpp")},
                      {'q', QLatin1String("qt_wrap_ui")},
                      {'r', QLatin1String("remove_definitions")},
                      {'s', QLatin1String("set_source_files_properties")},
                      {'s', QLatin1String("set_target_properties")},
                      {'s', QLatin1String("set_tests_properties")},
                      {'s', QLatin1String("source_group")},
                      {'t', QLatin1String("target_compile_definitions")},
                      {'t', QLatin1String("target_compile_features")},
                      {'t', QLatin1String("target_compile_options")},
                      {'t', QLatin1String("target_include_directories")},
                      {'t', QLatin1String("target_link_directories")},
                      {'t', QLatin1String("target_link_libraries")},
                      {'t', QLatin1String("target_link_options")},
                      {'t', QLatin1String("target_sources")},
                      {'t', QLatin1String("try_compile")},
                      {'t', QLatin1String("try_run")},
                      {'c', QLatin1String("ctest_build")},
                      {'c', QLatin1String("ctest_configure")},
                      {'c', QLatin1String("ctest_coverage")},
                      {'c', QLatin1String("ctest_empty_binary_directory")},
                      {'c', QLatin1String("ctest_memcheck")},
                      {'c', QLatin1String("ctest_read_custom_files")},
                      {'c', QLatin1String("ctest_run_script")},
                      {'c', QLatin1String("ctest_sleep")},
                      {'c', QLatin1String("ctest_start")},
                      {'c', QLatin1String("ctest_submit")},
                      {'c', QLatin1String("ctest_test")},
                      {'c', QLatin1String("ctest_update")},
                      {'c', QLatin1String("ctest_upload")},
                      {'b', QLatin1String("build_name")},
                      {'e', QLatin1String("exec_program")},
                      {'e', QLatin1String("export_library_dependencies")},
                      {'i', QLatin1String("install_files")},
                      {'i', QLatin1String("install_programs")},
                      {'i', QLatin1String("install_targets")},
                      {'l', QLatin1String("load_command")},
                      {'m', QLatin1String("make_directory")},
                      {'o', QLatin1String("output_required_files")},
                      {'r', QLatin1String("remove")},
                      {'s', QLatin1String("subdir_depends")},
                      {'s', QLatin1String("subdirs")},
                      {'u', QLatin1String("use_mangled_mesa")},
                      {'u', QLatin1String("utility_source")},
                      {'v', QLatin1String("variable_requires")},
                      {'w', QLatin1String("write_file")},
                      {'q', QLatin1String("qt5_use_modules")},
                      {'q', QLatin1String("qt5_use_package")},
                      {'q', QLatin1String("qt5_wrap_cpp")},
                      {'a', QLatin1String("and")},
                      {'o', QLatin1String("or")},
                      {'n', QLatin1String("not")},
                      {'c', QLatin1String("command")},
                      {'p', QLatin1String("policy")},
                      {'t', QLatin1String("target")},
                      {'t', QLatin1String("test")},
                      {'e', QLatin1String("exists")},
                      {'i', QLatin1String("is_newer_than")},
                      {'i', QLatin1String("is_directory")},
                      {'i', QLatin1String("is_symlink")},
                      {'i', QLatin1String("is_absolute")},
                      {'m', QLatin1String("matches")},
                      {'l', QLatin1String("less")},
                      {'g', QLatin1String("greater")},
                      {'e', QLatin1String("equal")},
                      {'l', QLatin1String("less_equal")},
                      {'g', QLatin1String("greater_equal")},
                      {'s', QLatin1String("strless")},
                      {'s', QLatin1String("strgreater")},
                      {'s', QLatin1String("strequal")},
                      {'s', QLatin1String("strless_equal")},
                      {'s', QLatin1String("strgreater_equal")},
                      {'v', QLatin1String("version_less")},
                      {'v', QLatin1String("version_greater")},
                      {'v', QLatin1String("version_equal")},
                      {'v', QLatin1String("version_less_equal")},
                      {'v', QLatin1String("version_greater_equal")},
                      {'i', QLatin1String("in_list")},
                      {'d', QLatin1String("defined")}};
    cmake_types = {};
    cmake_literals = {{'o', QLatin1String("on")},   {'o', QLatin1String("off")},  {'O', QLatin1String("ON")},
                      {'O', QLatin1String("OFF")},  {'t', QLatin1String("true")}, {'f', QLatin1String("false")},
                      {'T', QLatin1String("TRUE")}, {'F', QLatin1String("FALSE")}};
    cmake_builtin = {{'A', QLatin1String("ALLOW_DUPLICATE_CUSTOM_TARGETS")},
                     {'A', QLatin1String("AUTOGEN_TARGETS_FOLDER")},
                     {'A', QLatin1String("AUTOMOC_TARGETS_FOLDER")},
                     {'D', QLatin1String("DEBUG_CONFIGURATIONS")},
                     {'D', QLatin1String("DISABLED_FEATURES")},
                     {'E', QLatin1String("ENABLED_FEATURES")},
                     {'E', QLatin1String("ENABLED_LANGUAGES")},
                     {'F', QLatin1String("FIND_LIBRARY_USE_LIB64_PATHS")},
                     {'F', QLatin1String("FIND_LIBRARY_USE_OPENBSD_VERSIONING")},
                     {'G', QLatin1String("GLOBAL_DEPENDS_DEBUG_MODE")},
                     {'G', QLatin1String("GLOBAL_DEPENDS_NO_CYCLES")},
                     {'I', QLatin1String("IN_TRY_COMPILE")},
                     {'P', QLatin1String("PACKAGES_FOUND")},
                     {'P', QLatin1String("PACKAGES_NOT_FOUND")},
                     {'J', QLatin1String("JOB_POOLS")},
                     {'P', QLatin1String("PREDEFINED_TARGETS_FOLDER")},
                     {'E', QLatin1String("ECLIPSE_EXTRA_NATURES")},
                     {'R', QLatin1String("REPORT_UNDEFINED_PROPERTIES")},
                     {'R', QLatin1String("RULE_LAUNCH_COMPILE")},
                     {'R', QLatin1String("RULE_LAUNCH_CUSTOM")},
                     {'R', QLatin1String("RULE_LAUNCH_LINK")},
                     {'R', QLatin1String("RULE_MESSAGES")},
                     {'T', QLatin1String("TARGET_ARCHIVES_MAY_BE_SHARED_LIBS")},
                     {'T', QLatin1String("TARGET_SUPPORTS_SHARED_LIBS")},
                     {'U', QLatin1String("USE_FOLDERS")},
                     {'A', QLatin1String("ADDITIONAL_MAKE_CLEAN_FILES")},
                     {'C', QLatin1String("CACHE_VARIABLES")},
                     {'C', QLatin1String("CLEAN_NO_CUSTOM")},
                     {'C', QLatin1String("CMAKE_CONFIGURE_DEPENDS")},
                     {'C', QLatin1String("COMPILE_DEFINITIONS")},
                     {'C', QLatin1String("COMPILE_OPTIONS")},
                     {'D', QLatin1String("DEFINITIONS")},
                     {'E', QLatin1String("EXCLUDE_FROM_ALL")},
                     {'I', QLatin1String("IMPLICIT_DEPENDS_INCLUDE_TRANSFORM")},
                     {'I', QLatin1String("INCLUDE_DIRECTORIES")},
                     {'I', QLatin1String("INCLUDE_REGULAR_EXPRESSION")},
                     {'I', QLatin1String("INTERPROCEDURAL_OPTIMIZATION")},
                     {'L', QLatin1String("LINK_DIRECTORIES")},
                     {'L', QLatin1String("LISTFILE_STACK")},
                     {'M', QLatin1String("MACROS")},
                     {'P', QLatin1String("PARENT_DIRECTORY")},
                     {'R', QLatin1String("RULE_LAUNCH_COMPILE")},
                     {'R', QLatin1String("RULE_LAUNCH_CUSTOM")},
                     {'R', QLatin1String("RULE_LAUNCH_LINK")},
                     {'T', QLatin1String("TEST_INCLUDE_FILE")},
                     {'V', QLatin1String("VARIABLES")},
                     {'A', QLatin1String("ALIASED_TARGET")},
                     {'A', QLatin1String("ARCHIVE_OUTPUT_DIRECTORY")},
                     {'A', QLatin1String("ARCHIVE_OUTPUT_NAME")},
                     {'A', QLatin1String("AUTOGEN_TARGET_DEPENDS")},
                     {'A', QLatin1String("AUTOMOC_MOC_OPTIONS")},
                     {'A', QLatin1String("AUTOMOC")},
                     {'A', QLatin1String("AUTOUIC")},
                     {'A', QLatin1String("AUTOUIC_OPTIONS")},
                     {'A', QLatin1String("AUTORCC")},
                     {'A', QLatin1String("AUTORCC_OPTIONS")},
                     {'B', QLatin1String("BUILD_WITH_INSTALL_RPATH")},
                     {'B', QLatin1String("BUNDLE_EXTENSION")},
                     {'B', QLatin1String("BUNDLE")},
                     {'C', QLatin1String("COMPATIBLE_INTERFACE_BOOL")},
                     {'C', QLatin1String("COMPATIBLE_INTERFACE_NUMBER_MAX")},
                     {'C', QLatin1String("COMPATIBLE_INTERFACE_NUMBER_MIN")},
                     {'C', QLatin1String("COMPATIBLE_INTERFACE_STRING")},
                     {'C', QLatin1String("COMPILE_DEFINITIONS")},
                     {'C', QLatin1String("COMPILE_FLAGS")},
                     {'C', QLatin1String("COMPILE_OPTIONS")},
                     {'D', QLatin1String("DEBUG_POSTFIX")},
                     {'D', QLatin1String("DEFINE_SYMBOL")},
                     {'E', QLatin1String("EchoString")},
                     {'E', QLatin1String("ENABLE_EXPORTS")},
                     {'E', QLatin1String("EXCLUDE_FROM_ALL")},
                     {'E', QLatin1String("EXCLUDE_FROM_DEFAULT_BUILD")},
                     {'E', QLatin1String("EXPORT_NAME")},
                     {'F', QLatin1String("FOLDER")},
                     {'F', QLatin1String("Fortran_FORMAT")},
                     {'F', QLatin1String("Fortran_MODULE_DIRECTORY")},
                     {'F', QLatin1String("FRAMEWORK")},
                     {'G', QLatin1String("GENERATOR_FILE_NAME")},
                     {'G', QLatin1String("GNUtoMS")},
                     {'H', QLatin1String("HAS_CXX")},
                     {'I', QLatin1String("IMPLICIT_DEPENDS_INCLUDE_TRANSFORM")},
                     {'I', QLatin1String("IMPORTED_CONFIGURATIONS")},
                     {'I', QLatin1String("IMPORTED_IMPLIB")},
                     {'I', QLatin1String("IMPORTED_LINK_DEPENDENT_LIBRARIES")},
                     {'I', QLatin1String("IMPORTED_LINK_INTERFACE_LANGUAGES")},
                     {'I', QLatin1String("IMPORTED_LINK_INTERFACE_LIBRARIES")},
                     {'I', QLatin1String("IMPORTED_LINK_INTERFACE_MULTIPLICITY")},
                     {'I', QLatin1String("IMPORTED_LOCATION")},
                     {'I', QLatin1String("IMPORTED_NO_SONAME")},
                     {'I', QLatin1String("IMPORTED")},
                     {'I', QLatin1String("IMPORTED_SONAME")},
                     {'I', QLatin1String("IMPORT_PREFIX")},
                     {'I', QLatin1String("IMPORT_SUFFIX")},
                     {'I', QLatin1String("INCLUDE_DIRECTORIES")},
                     {'I', QLatin1String("INSTALL_NAME_DIR")},
                     {'I', QLatin1String("INSTALL_RPATH")},
                     {'I', QLatin1String("INSTALL_RPATH_USE_LINK_PATH")},
                     {'I', QLatin1String("INTERFACE_AUTOUIC_OPTIONS")},
                     {'I', QLatin1String("INTERFACE_COMPILE_DEFINITIONS")},
                     {'I', QLatin1String("INTERFACE_COMPILE_OPTIONS")},
                     {'I', QLatin1String("INTERFACE_INCLUDE_DIRECTORIES")},
                     {'I', QLatin1String("INTERFACE_LINK_LIBRARIES")},
                     {'I', QLatin1String("INTERFACE_POSITION_INDEPENDENT_CODE")},
                     {'I', QLatin1String("INTERFACE_SYSTEM_INCLUDE_DIRECTORIES")},
                     {'I', QLatin1String("INTERPROCEDURAL_OPTIMIZATION")},
                     {'J', QLatin1String("JOB_POOL_COMPILE")},
                     {'J', QLatin1String("JOB_POOL_LINK")},
                     {'L', QLatin1String("LABELS")},
                     {'L', QLatin1String("LIBRARY_OUTPUT_DIRECTORY")},
                     {'L', QLatin1String("LIBRARY_OUTPUT_NAME")},
                     {'L', QLatin1String("LINK_DEPENDS_NO_SHARED")},
                     {'L', QLatin1String("LINK_DEPENDS")},
                     {'L', QLatin1String("LINKER_LANGUAGE")},
                     {'L', QLatin1String("LINK_FLAGS")},
                     {'L', QLatin1String("LINK_INTERFACE_LIBRARIES")},
                     {'L', QLatin1String("LINK_INTERFACE_MULTIPLICITY")},
                     {'L', QLatin1String("LINK_LIBRARIES")},
                     {'L', QLatin1String("LINK_SEARCH_END_STATIC")},
                     {'L', QLatin1String("LINK_SEARCH_START_STATIC")},
                     {'L', QLatin1String("LOCATION")},
                     {'M', QLatin1String("MACOSX_BUNDLE_INFO_PLIST")},
                     {'M', QLatin1String("MACOSX_BUNDLE")},
                     {'M', QLatin1String("MACOSX_FRAMEWORK_INFO_PLIST")},
                     {'M', QLatin1String("MACOSX_RPATH")},
                     //        {'N', QLatin1String("NAME")},
                     {'N', QLatin1String("NO_SONAME")},
                     {'N', QLatin1String("NO_SYSTEM_FROM_IMPORTED")},
                     {'O', QLatin1String("OSX_ARCHITECTURES")},
                     {'O', QLatin1String("OUTPUT_NAME")},
                     {'P', QLatin1String("PDB_NAME")},
                     {'P', QLatin1String("PDB_OUTPUT_DIRECTORY")},
                     {'P', QLatin1String("POSITION_INDEPENDENT_CODE")},
                     {'P', QLatin1String("POST_INSTALL_SCRIPT")},
                     {'P', QLatin1String("PREFIX")},
                     {'P', QLatin1String("PROPERTY")},
                     {'P', QLatin1String("PRE_INSTALL_SCRIPT")},
                     {'P', QLatin1String("PRIVATE_HEADER")},
                     {'P', QLatin1String("PROJECT_LABEL")},
                     {'P', QLatin1String("PUBLIC_HEADER")},
                     {'R', QLatin1String("RESOURCE")},
                     {'R', QLatin1String("RULE_LAUNCH_COMPILE")},
                     {'R', QLatin1String("RULE_LAUNCH_CUSTOM")},
                     {'R', QLatin1String("RULE_LAUNCH_LINK")},
                     {'R', QLatin1String("RUNTIME_OUTPUT_DIRECTORY")},
                     {'R', QLatin1String("RUNTIME_OUTPUT_NAME")},
                     {'S', QLatin1String("SKIP_BUILD_RPATH")},
                     {'S', QLatin1String("SOURCES")},
                     {'S', QLatin1String("SOVERSION")},
                     {'S', QLatin1String("STATIC_LIBRARY_FLAGS")},
                     {'S', QLatin1String("SUFFIX")},
                     {'T', QLatin1String("TARGET")},
                     {'T', QLatin1String("TYPE")},
                     {'V', QLatin1String("VERSION")},
                     {'V', QLatin1String("VISIBILITY_INLINES_HIDDEN")},
                     {'V', QLatin1String("VS_DOTNET_REFERENCES")},
                     {'V', QLatin1String("VS_DOTNET_TARGET_FRAMEWORK_VERSION")},
                     {'V', QLatin1String("VS_GLOBAL_KEYWORD")},
                     {'V', QLatin1String("VS_GLOBAL_PROJECT_TYPES")},
                     {'V', QLatin1String("VS_GLOBAL_ROOTNAMESPACE")},
                     {'V', QLatin1String("VS_KEYWORD")},
                     {'V', QLatin1String("VS_SCC_AUXPATH")},
                     {'V', QLatin1String("VS_SCC_LOCALPATH")},
                     {'V', QLatin1String("VS_SCC_PROJECTNAME")},
                     {'V', QLatin1String("VS_SCC_PROVIDER")},
                     {'V', QLatin1String("VS_WINRT_EXTENSIONS")},
                     {'V', QLatin1String("VS_WINRT_REFERENCES")},
                     {'W', QLatin1String("WIN32_EXECUTABLE")},
                     {'A', QLatin1String("ATTACHED_FILES_ON_FAIL")},
                     {'A', QLatin1String("ATTACHED_FILES")},
                     {'C', QLatin1String("COST")},
                     {'D', QLatin1String("DEPENDS")},
                     {'E', QLatin1String("ENVIRONMENT")},
                     {'F', QLatin1String("FAIL_REGULAR_EXPRESSION")},
                     {'L', QLatin1String("LABELS")},
                     {'M', QLatin1String("MEASUREMENT")},
                     {'P', QLatin1String("PASS_REGULAR_EXPRESSION")},
                     {'P', QLatin1String("PROCESSORS")},
                     {'R', QLatin1String("REQUIRED_FILES")},
                     {'R', QLatin1String("RESOURCE_LOCK")},
                     {'R', QLatin1String("RUN_SERIAL")},
                     {'S', QLatin1String("SKIP_RETURN_CODE")},
                     {'T', QLatin1String("TIMEOUT")},
                     {'W', QLatin1String("WILL_FAIL")},
                     {'W', QLatin1String("WORKING_DIRECTORY")},
                     {'A', QLatin1String("ABSTRACT")},
                     {'A', QLatin1String("AUTOUIC_OPTIONS")},
                     {'A', QLatin1String("AUTORCC_OPTIONS")},
                     {'C', QLatin1String("COMPILE_DEFINITIONS")},
                     {'C', QLatin1String("COMPILE_FLAGS")},
                     {'E', QLatin1String("EXTERNAL_OBJECT")},
                     {'F', QLatin1String("Fortran_FORMAT")},
                     {'G', QLatin1String("GENERATED")},
                     {'H', QLatin1String("HEADER_FILE_ONLY")},
                     {'K', QLatin1String("KEEP_EXTENSION")},
                     {'L', QLatin1String("LABELS")},
                     //        {'L', QLatin1String("LANGUAGE")},
                     {'L', QLatin1String("LOCATION")},
                     {'M', QLatin1String("MACOSX_PACKAGE_LOCATION")},
                     {'O', QLatin1String("OBJECT_DEPENDS")},
                     {'O', QLatin1String("OBJECT_OUTPUTS")},
                     {'S', QLatin1String("SYMBOLIC")},
                     {'W', QLatin1String("WRAP_EXCLUDE")},
                     {'A', QLatin1String("ADVANCED")},
                     {'H', QLatin1String("HELPSTRING")},
                     {'M', QLatin1String("MODIFIED")},
                     {'S', QLatin1String("STRINGS")},
                     {'T', QLatin1String("TYPE")},
                     {'V', QLatin1String("VALUE")}};
    cmake_other = {{'C', QLatin1String("CMAKE_ARGC")},
                   {'C', QLatin1String("CMAKE_ARGV0")},
                   {'C', QLatin1String("CMAKE_AR")},
                   {'C', QLatin1String("CMAKE_BINARY_DIR")},
                   {'C', QLatin1String("CMAKE_BUILD_TOOL")},
                   {'C', QLatin1String("CMAKE_CACHEFILE_DIR")},
                   {'C', QLatin1String("CMAKE_CACHE_MAJOR_VERSION")},
                   {'C', QLatin1String("CMAKE_CACHE_MINOR_VERSION")},
                   {'C', QLatin1String("CMAKE_CACHE_PATCH_VERSION")},
                   {'C', QLatin1String("CMAKE_CFG_INTDIR")},
                   {'C', QLatin1String("CMAKE_COMMAND")},
                   {'C', QLatin1String("CMAKE_CROSSCOMPILING")},
                   {'C', QLatin1String("CMAKE_CTEST_COMMAND")},
                   {'C', QLatin1String("CMAKE_CURRENT_BINARY_DIR")},
                   {'C', QLatin1String("CMAKE_CURRENT_LIST_DIR")},
                   {'C', QLatin1String("CMAKE_CURRENT_LIST_FILE")},
                   {'C', QLatin1String("CMAKE_CURRENT_LIST_LINE")},
                   {'C', QLatin1String("CMAKE_CURRENT_SOURCE_DIR")},
                   {'C', QLatin1String("CMAKE_DL_LIBS")},
                   {'C', QLatin1String("CMAKE_EDIT_COMMAND")},
                   {'C', QLatin1String("CMAKE_EXECUTABLE_SUFFIX")},
                   {'C', QLatin1String("CMAKE_EXTRA_GENERATOR")},
                   {'C', QLatin1String("CMAKE_EXTRA_SHARED_LIBRARY_SUFFIXES")},
                   {'C', QLatin1String("CMAKE_GENERATOR")},
                   {'C', QLatin1String("CMAKE_GENERATOR_TOOLSET")},
                   {'C', QLatin1String("CMAKE_HOME_DIRECTORY")},
                   {'C', QLatin1String("CMAKE_IMPORT_LIBRARY_PREFIX")},
                   {'C', QLatin1String("CMAKE_IMPORT_LIBRARY_SUFFIX")},
                   {'C', QLatin1String("CMAKE_JOB_POOL_COMPILE")},
                   {'C', QLatin1String("CMAKE_JOB_POOL_LINK")},
                   {'C', QLatin1String("CMAKE_LINK_LIBRARY_SUFFIX")},
                   {'C', QLatin1String("CMAKE_MAJOR_VERSION")},
                   {'C', QLatin1String("CMAKE_MAKE_PROGRAM")},
                   {'C', QLatin1String("CMAKE_MINIMUM_REQUIRED_VERSION")},
                   {'C', QLatin1String("CMAKE_MINOR_VERSION")},
                   {'C', QLatin1String("CMAKE_PARENT_LIST_FILE")},
                   {'C', QLatin1String("CMAKE_PATCH_VERSION")},
                   {'C', QLatin1String("CMAKE_PROJECT_NAME")},
                   {'C', QLatin1String("CMAKE_RANLIB")},
                   {'C', QLatin1String("CMAKE_ROOT")},
                   {'C', QLatin1String("CMAKE_SCRIPT_MODE_FILE")},
                   {'C', QLatin1String("CMAKE_SHARED_LIBRARY_PREFIX")},
                   {'C', QLatin1String("CMAKE_SHARED_LIBRARY_SUFFIX")},
                   {'C', QLatin1String("CMAKE_SHARED_MODULE_PREFIX")},
                   {'C', QLatin1String("CMAKE_SHARED_MODULE_SUFFIX")},
                   {'C', QLatin1String("CMAKE_SIZEOF_VOID_P")},
                   {'C', QLatin1String("CMAKE_SKIP_INSTALL_RULES")},
                   {'C', QLatin1String("CMAKE_SKIP_RPATH")},
                   {'C', QLatin1String("CMAKE_SOURCE_DIR")},
                   {'C', QLatin1String("CMAKE_STANDARD_LIBRARIES")},
                   {'C', QLatin1String("CMAKE_STATIC_LIBRARY_PREFIX")},
                   {'C', QLatin1String("CMAKE_STATIC_LIBRARY_SUFFIX")},
                   {'C', QLatin1String("CMAKE_TOOLCHAIN_FILE")},
                   {'C', QLatin1String("CMAKE_TWEAK_VERSION")},
                   {'C', QLatin1String("CMAKE_VERBOSE_MAKEFILE")},
                   {'C', QLatin1String("CMAKE_VERSION")},
                   {'C', QLatin1String("CMAKE_VS_DEVENV_COMMAND")},
                   {'C', QLatin1String("CMAKE_VS_INTEL_Fortran_PROJECT_VERSION")},
                   {'C', QLatin1String("CMAKE_VS_MSBUILD_COMMAND")},
                   {'C', QLatin1String("CMAKE_VS_MSDEV_COMMAND")},
                   {'C', QLatin1String("CMAKE_VS_PLATFORM_TOOLSET")},
                   {'C', QLatin1String("CMAKE_XCODE_PLATFORM_TOOLSET")},
                   {'P', QLatin1String("PROJECT_BINARY_DIR")},
                   //        {'P', QLatin1String("PROJECT_NAME")},
                   {'P', QLatin1String("PROJECT_SOURCE_DIR")},
                   {'P', QLatin1String("PROJECT_VERSION")},
                   {'P', QLatin1String("PROJECT_VERSION_MAJOR")},
                   {'P', QLatin1String("PROJECT_VERSION_MINOR")},
                   {'P', QLatin1String("PROJECT_VERSION_PATCH")},
                   {'P', QLatin1String("PROJECT_VERSION_TWEAK")},
                   {'B', QLatin1String("BUILD_SHARED_LIBS")},
                   {'C', QLatin1String("CMAKE_ABSOLUTE_DESTINATION_FILES")},
                   {'C', QLatin1String("CMAKE_APPBUNDLE_PATH")},
                   {'C', QLatin1String("CMAKE_AUTOMOC_RELAXED_MODE")},
                   {'C', QLatin1String("CMAKE_BACKWARDS_COMPATIBILITY")},
                   {'C', QLatin1String("CMAKE_BUILD_TYPE")},
                   {'C', QLatin1String("CMAKE_COLOR_MAKEFILE")},
                   {'C', QLatin1String("CMAKE_CONFIGURATION_TYPES")},
                   {'C', QLatin1String("CMAKE_DEBUG_TARGET_PROPERTIES")},
                   {'C', QLatin1String("CMAKE_ERROR_DEPRECATED")},
                   {'C', QLatin1String("CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION")},
                   {'C', QLatin1String("CMAKE_SYSROOT")},
                   {'C', QLatin1String("CMAKE_FIND_LIBRARY_PREFIXES")},
                   {'C', QLatin1String("CMAKE_FIND_LIBRARY_SUFFIXES")},
                   {'C', QLatin1String("CMAKE_FIND_NO_INSTALL_PREFIX")},
                   {'C', QLatin1String("CMAKE_FIND_PACKAGE_WARN_NO_MODULE")},
                   {'C', QLatin1String("CMAKE_FIND_ROOT_PATH")},
                   {'C', QLatin1String("CMAKE_FIND_ROOT_PATH_MODE_INCLUDE")},
                   {'C', QLatin1String("CMAKE_FIND_ROOT_PATH_MODE_LIBRARY")},
                   {'C', QLatin1String("CMAKE_FIND_ROOT_PATH_MODE_PACKAGE")},
                   {'C', QLatin1String("CMAKE_FIND_ROOT_PATH_MODE_PROGRAM")},
                   {'C', QLatin1String("CMAKE_FRAMEWORK_PATH")},
                   {'C', QLatin1String("CMAKE_IGNORE_PATH")},
                   {'C', QLatin1String("CMAKE_INCLUDE_PATH")},
                   {'C', QLatin1String("CMAKE_INCLUDE_DIRECTORIES_BEFORE")},
                   {'C', QLatin1String("CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE")},
                   {'C', QLatin1String("CMAKE_INSTALL_DEFAULT_COMPONENT_NAME")},
                   {'C', QLatin1String("CMAKE_INSTALL_PREFIX")},
                   {'C', QLatin1String("CMAKE_LIBRARY_PATH")},
                   {'C', QLatin1String("CMAKE_MFC_FLAG")},
                   {'C', QLatin1String("CMAKE_MODULE_PATH")},
                   {'C', QLatin1String("CMAKE_NOT_USING_CONFIG_FLAGS")},
                   {'C', QLatin1String("CMAKE_PREFIX_PATH")},
                   {'C', QLatin1String("CMAKE_PROGRAM_PATH")},
                   {'C', QLatin1String("CMAKE_SKIP_INSTALL_ALL_DEPENDENCY")},
                   {'C', QLatin1String("CMAKE_STAGING_PREFIX")},
                   {'C', QLatin1String("CMAKE_SYSTEM_IGNORE_PATH")},
                   {'C', QLatin1String("CMAKE_SYSTEM_INCLUDE_PATH")},
                   {'C', QLatin1String("CMAKE_SYSTEM_LIBRARY_PATH")},
                   {'C', QLatin1String("CMAKE_SYSTEM_PREFIX_PATH")},
                   {'C', QLatin1String("CMAKE_SYSTEM_PROGRAM_PATH")},
                   {'C', QLatin1String("CMAKE_USER_MAKE_RULES_OVERRIDE")},
                   {'C', QLatin1String("CMAKE_WARN_DEPRECATED")},
                   {'C', QLatin1String("CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION")},
                   {'A', QLatin1String("APPLE")},
                   {'B', QLatin1String("BORLAND")},
                   {'C', QLatin1String("CMAKE_CL_64")},
                   {'C', QLatin1String("CMAKE_COMPILER_2005")},
                   {'C', QLatin1String("CMAKE_HOST_APPLE")},
                   {'C', QLatin1String("CMAKE_HOST_SYSTEM_NAME")},
                   {'C', QLatin1String("CMAKE_HOST_SYSTEM_PROCESSOR")},
                   {'C', QLatin1String("CMAKE_HOST_SYSTEM")},
                   {'C', QLatin1String("CMAKE_HOST_SYSTEM_VERSION")},
                   {'C', QLatin1String("CMAKE_HOST_UNIX")},
                   {'C', QLatin1String("CMAKE_HOST_WIN32")},
                   {'C', QLatin1String("CMAKE_LIBRARY_ARCHITECTURE_REGEX")},
                   {'C', QLatin1String("CMAKE_LIBRARY_ARCHITECTURE")},
                   {'C', QLatin1String("CMAKE_OBJECT_PATH_MAX")},
                   {'C', QLatin1String("CMAKE_SYSTEM_NAME")},
                   {'C', QLatin1String("CMAKE_SYSTEM_PROCESSOR")},
                   {'C', QLatin1String("CMAKE_SYSTEM")},
                   {'C', QLatin1String("CMAKE_SYSTEM_VERSION")},
                   {'C', QLatin1String("CYGWIN")},
                   {'E', QLatin1String("ENV")},
                   {'M', QLatin1String("MSVC10")},
                   {'M', QLatin1String("MSVC11")},
                   {'M', QLatin1String("MSVC12")},
                   {'M', QLatin1String("MSVC60")},
                   {'M', QLatin1String("MSVC70")},
                   {'M', QLatin1String("MSVC71")},
                   {'M', QLatin1String("MSVC80")},
                   {'M', QLatin1String("MSVC90")},
                   {'M', QLatin1String("MSVC_IDE")},
                   {'M', QLatin1String("MSVC")},
                   {'M', QLatin1String("MSVC_VERSION")},
                   {'U', QLatin1String("UNIX")},
                   {'W', QLatin1String("WIN32")},
                   {'X', QLatin1String("XCODE_VERSION")},
                   {'C', QLatin1String("CMAKE_ARCHIVE_OUTPUT_DIRECTORY")},
                   {'C', QLatin1String("CMAKE_AUTOMOC_MOC_OPTIONS")},
                   {'C', QLatin1String("CMAKE_AUTOMOC")},
                   {'C', QLatin1String("CMAKE_AUTORCC")},
                   {'C', QLatin1String("CMAKE_AUTORCC_OPTIONS")},
                   {'C', QLatin1String("CMAKE_AUTOUIC")},
                   {'C', QLatin1String("CMAKE_AUTOUIC_OPTIONS")},
                   {'C', QLatin1String("CMAKE_BUILD_WITH_INSTALL_RPATH")},
                   {'C', QLatin1String("CMAKE_DEBUG_POSTFIX")},
                   {'C', QLatin1String("CMAKE_EXE_LINKER_FLAGS")},
                   {'C', QLatin1String("CMAKE_Fortran_FORMAT")},
                   {'C', QLatin1String("CMAKE_Fortran_MODULE_DIRECTORY")},
                   {'C', QLatin1String("CMAKE_GNUtoMS")},
                   {'C', QLatin1String("CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE")},
                   {'C', QLatin1String("CMAKE_INCLUDE_CURRENT_DIR")},
                   {'C', QLatin1String("CMAKE_INSTALL_NAME_DIR")},
                   {'C', QLatin1String("CMAKE_INSTALL_RPATH")},
                   {'C', QLatin1String("CMAKE_INSTALL_RPATH_USE_LINK_PATH")},
                   {'C', QLatin1String("CMAKE_LIBRARY_OUTPUT_DIRECTORY")},
                   {'C', QLatin1String("CMAKE_LIBRARY_PATH_FLAG")},
                   {'C', QLatin1String("CMAKE_LINK_DEF_FILE_FLAG")},
                   {'C', QLatin1String("CMAKE_LINK_DEPENDS_NO_SHARED")},
                   {'C', QLatin1String("CMAKE_LINK_INTERFACE_LIBRARIES")},
                   {'C', QLatin1String("CMAKE_LINK_LIBRARY_FILE_FLAG")},
                   {'C', QLatin1String("CMAKE_LINK_LIBRARY_FLAG")},
                   {'C', QLatin1String("CMAKE_MACOSX_BUNDLE")},
                   {'C', QLatin1String("CMAKE_MACOSX_RPATH")},
                   {'C', QLatin1String("CMAKE_MODULE_LINKER_FLAGS")},
                   {'C', QLatin1String("CMAKE_NO_BUILTIN_CHRPATH")},
                   {'C', QLatin1String("CMAKE_NO_SYSTEM_FROM_IMPORTED")},
                   {'C', QLatin1String("CMAKE_OSX_ARCHITECTURES")},
                   {'C', QLatin1String("CMAKE_OSX_DEPLOYMENT_TARGET")},
                   {'C', QLatin1String("CMAKE_OSX_SYSROOT")},
                   {'C', QLatin1String("CMAKE_PDB_OUTPUT_DIRECTORY")},
                   {'C', QLatin1String("CMAKE_POSITION_INDEPENDENT_CODE")},
                   {'C', QLatin1String("CMAKE_RUNTIME_OUTPUT_DIRECTORY")},
                   {'C', QLatin1String("CMAKE_SHARED_LINKER_FLAGS")},
                   {'C', QLatin1String("CMAKE_SKIP_BUILD_RPATH")},
                   {'C', QLatin1String("CMAKE_SKIP_INSTALL_RPATH")},
                   {'C', QLatin1String("CMAKE_STATIC_LINKER_FLAGS")},
                   {'C', QLatin1String("CMAKE_TRY_COMPILE_CONFIGURATION")},
                   {'C', QLatin1String("CMAKE_USE_RELATIVE_PATHS")},
                   {'C', QLatin1String("CMAKE_VISIBILITY_INLINES_HIDDEN")},
                   {'C', QLatin1String("CMAKE_WIN32_EXECUTABLE")},
                   {'E', QLatin1String("EXECUTABLE_OUTPUT_PATH")},
                   {'L', QLatin1String("LIBRARY_OUTPUT_PATH")},
                   {'C', QLatin1String("CMAKE_Fortran_MODDIR_DEFAULT")},
                   {'C', QLatin1String("CMAKE_Fortran_MODDIR_FLAG")},
                   {'C', QLatin1String("CMAKE_Fortran_MODOUT_FLAG")},
                   {'C', QLatin1String("CMAKE_INTERNAL_PLATFORM_ABI")},
                   {'C', QLatin1String("CPACK_ABSOLUTE_DESTINATION_FILES")},
                   {'C', QLatin1String("CPACK_COMPONENT_INCLUDE_TOPLEVEL_DIRECTORY")},
                   {'C', QLatin1String("CPACK_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION")},
                   {'C', QLatin1String("CPACK_INCLUDE_TOPLEVEL_DIRECTORY")},
                   {'C', QLatin1String("CPACK_INSTALL_SCRIPT")},
                   {'C', QLatin1String("CPACK_PACKAGING_INSTALL_PREFIX")},
                   {'C', QLatin1String("CPACK_SET_DESTDIR")},
                   {'C', QLatin1String("CPACK_WARN_ON_ABSOLUTE_INSTALL_DESTINATION")}};
}

void loadCMakeData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                   QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                   QMultiHash<char, QLatin1String> &other)
{
    if (!cmakeDataInitialized)
    {
        initCMakeData();
        cmakeDataInitialized = true;
    }
    types = cmake_types;
    keywords = cmake_keywords;
    builtin = cmake_builtin;
    literals = cmake_literals;
    other = cmake_other;
}

/********************************************************/
/***   MAKE DATA   **************************************/
/********************************************************/
static bool makeDataInitialized = false;
static QMultiHash<char, QLatin1String> make_keywords;
static QMultiHash<char, QLatin1String> make_types;
static QMultiHash<char, QLatin1String> make_literals;
static QMultiHash<char, QLatin1String> make_builtin;
static QMultiHash<char, QLatin1String> make_other;
void initMakeData()
{
    make_keywords = {
        {'i', QLatin1String("include")},  {'d', QLatin1String("define")},   {'e', QLatin1String("else")},
        {'e', QLatin1String("endef")},    {'e', QLatin1String("endif")},    {'e', QLatin1String("export")},
        {'i', QLatin1String("ifn?def")},  {'i', QLatin1String("ifn?eq")},   {'i', QLatin1String("include")},
        {'o', QLatin1String("override")}, {'p', QLatin1String("private")},  {'s', QLatin1String("sinclude")},
        {'u', QLatin1String("undefine")}, {'u', QLatin1String("unexport")}, {'v', QLatin1String("vpath")}};
    make_types = {
        {'a', QLatin1String("addsuffix")},  {'a', QLatin1String("abspath")},   {'a', QLatin1String("and")},
        {'a', QLatin1String("ar")},         {'b', QLatin1String("basename")},  {'c', QLatin1String("call")},
        {'d', QLatin1String("dir")},        {'e', QLatin1String("error")},     {'e', QLatin1String("eval")},
        {'f', QLatin1String("file")},       {'f', QLatin1String("filter")},    {'f', QLatin1String("find")},
        {'f', QLatin1String("findstring")}, {'f', QLatin1String("firstword")}, {'f', QLatin1String("flavor")},
        {'f', QLatin1String("foreach")},    {'g', QLatin1String("guile")},     {'i', QLatin1String("if")},
        {'i', QLatin1String("info")},       {'i', QLatin1String("install")},   {'j', QLatin1String("join")},
        {'l', QLatin1String("lastword")},   {'l', QLatin1String("load")},      {'n', QLatin1String("notdir")},
        {'o', QLatin1String("or")},         {'o', QLatin1String("origin")},    {'p', QLatin1String("patsubst")},
        {'r', QLatin1String("ranlib")},     {'r', QLatin1String("realpath")},  {'r', QLatin1String("rm")},
        {'s', QLatin1String("shell")},      {'s', QLatin1String("sort")},      {'s', QLatin1String("strip")},
        {'s', QLatin1String("subst")},      {'s', QLatin1String("suffix")},    {'v', QLatin1String("value")},
        {'w', QLatin1String("warning")},    {'w', QLatin1String("wildcard")},  {'w', QLatin1String("word")}};
    make_literals = {
        {'t', QLatin1String("true")},
        {'f', QLatin1String("false")},
    };
    make_builtin = {};
    make_other = {
        {'C', QLatin1String("CFLAGS")},
        {'L', QLatin1String("LIBS")},
        {'P', QLatin1String("PREFIX")},
    };
}

void loadMakeData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                  QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                  QMultiHash<char, QLatin1String> &other)
{
    if (!makeDataInitialized)
    {
        initMakeData();
        makeDataInitialized = true;
    }
    types = make_types;
    keywords = make_keywords;
    builtin = make_builtin;
    literals = make_literals;
    other = make_other;
}
