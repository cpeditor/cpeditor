#! shebang
#! no-shebang

/* comment */

function fun()
{
    var boo = { 'key': [ 1, 2.0, 3.0e1, 004, 0x5 ] };
}

class MyClass; // reserved keywords

// Member objects: text after "."
object.property instanceof Number;
iden1.iden2  . iden3.class class;

var escapes = "aa\b\n\0a\"a\x12a\32a\u{123}a\$\%\ \#\y\aaa\
aaa";
var octal = 0o124;
var bin = 0b1010;

日本語().ლಠ益ಠლ.ñá = 42;
δ /No-Regex/

// Only highlight valid regular expressions, of a single line, after strings
// See: https://github.com/microsoft/TypeScript-TmLanguage/issues/786
"text" /No-Regex
"text" /Regex[:)]*/;
const a = "6" / 2; /*comment*/ const b = 5;
console.log("4" / "2"); // 2
// Single quote
const a = '6' / 2; /*comment*/ const b = 5;
console.log('4' / '2'); // 2
// Template
const a = `6` / 2; /*comment*/ const b = 5;
console.log(`4` / `2`); // 2

// Built-in
const os = require('os');
JSON.stringify("hello");
console.error("hello");
Math.LOG10E;
Number.MAX_SAFE_INTEGER;
String.raw`raw text \.\n${}`

// Tagged template literals
tagFunc`
    Hello world!
    ${ alert("Hello!"); }`;
obj.something.tagFunc`Setting ${setting} is ${value + 5}!`;
