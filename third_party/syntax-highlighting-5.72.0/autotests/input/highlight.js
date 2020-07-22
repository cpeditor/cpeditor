/* test.js - test for javascript.xml syntax file */
// Note: this script will not, and is not supposed to, compile in any js engine.

/* 
   NOTE: The words "todo", "fixme" and "note" should be rendered in a different style
   within comments, match should be caseless (to test for regexp insensitive attribute).
   The regex used for this rule is */
   String = /\b(?:fixme|todo|note)\b/
   /* Thus,  for example "Notebook" is not caught by
   this rule. (the "?:" in the subpattern is there to avoid the regex engine wasting time
   saving a backref, which is not used for anything. I do not know if the overhead of parsing
   that is greater than the time saved by not capturing the text...)
   The rule for catching these words is placed in a context "Comment common", which is used
   by both comment contexts (single line, multiline) using the new "IncludeRules" item.
*/

// test if regex support works - nice with new fallthrough prop in context:)
somestring.replace( /dooh/ , "bah!");
re=/foo/ig; // hehe

somestring.search(
       /^foo\w+\s\d{0,15}$/
                  );

        re =
        /dooh/;

// This is supposedly legal:
re = somebool ? /foo/ : /bar/;

// NOTE - Special case: an empty regex, not a comment.
// The rule uses a positive lookahead assertion to catch it: "//(?=;)".
re = //;
re = /a|b/;

/*
   Tests for the regex parser.
   It will parse classes, quanitfiers, special characters and regex operaters,
   as specified in the netscape documentation for javascript.
   Regexps are only parsed in their clean form, as the RegExp(string) constructor
   is using a quoted string.
   TODO: Find out if more regex feats should be supported.
         Consider using more itemDatas - assertion, quantifier are options.
*/

re = /^text\s+\d+\s*$/;
re = /a pattern with caret \(^\) in it/;
re = /(\d{0,4})\D/;
re = /[a-zA-Z_]+/;
re = /[^\d^]+/;
re = /\s+?\w+\.$/;
re = /\/\//;
re = /a|b/;

// the following are not regexps in E4X (=xml embedded into JavaScript)
var p = <p>Hello World</p>
var p = /</
var p = />/

// a test if #pop back from a comment will work
re = /*/foo/*/ /bar/;
//           ^ POP
//             ^ we got back after pop in comment, if there is regexp attribs here :-)

/*
   Some tests if the fallthrough works.
   The fallthrough happens if a regexp is not found in a possible (!) position,
   which is after "search(" or "replace(" or "=" or "?" or ":" in version 0.1 of the xml file
*/

var foo = 'bar';
//        ^ fallthrough!


somestring.replace( new RegExp("\\b\\w+\\b"), "word: $1");
//                  ^ fallthrough expected. ("new" whould be bold)


something.method =
    function ( a, b, c ) { /* ... */ }
//  ^ fallthrough ?!

something.other =
function ( d, e, f ) { /* ... */ }
// fallthrough expected at col 0 ("function" should be bold)

var ary = new Array(5);
//        ^ fallthrough ? (if keyword is correctly rendered)

var b = a ? 1 : 0;
//          ^   ^ fallthroughs. numbers must be rendered correctly.

var c = d ? true : false;

var conditinalstring = b ?
  "something" :
  "something else";
// guess...


/*
   Normal program flow...
*/

if (something)
  dostuff();
else
  dont();

  return;

try { bla() } catch (e) { alert("ERROR! : " + e) }

for (int i=0; i < j; i++)
  document.write("i is" + i + "<br>");

while (something)
{
  block();
  picky:
    if (!1)
      break;
    else
      continue;
}

with (a) {
  do {
    stuff( b ); // a.b if it exists
  } while (itmakessense);
}

switch (i) {
  case 0:
  f();
  break;
  default:
  break;
}

// Numerics
var a = 0xA;
var b = 0b1;
var c = 0o7;
var d = 1.1E+3;
var e = 1.E+3;
var f = .1E+3;
var g = 1E+3;
var h = 1.1;
var i = 1.;
var j = .1;
var k =  1;
// Bigint
const binBig = 0b101n;
const octBig = 0o567n;
const hexBig = 0xC0Bn;
const decBig = 123n;
// Invalid numbers
var l = 0xA1t;
var m = 0b0123;
var n = 0o29;
