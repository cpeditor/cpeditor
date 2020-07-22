// Test file for Pony syntax highlighting, released under MIT License

primitive Red   fun apply(): U32 => 0xFFFF0000

primitive Colours
  fun black(): U32 => 0xFF000000
  fun red(): U32 => 0xFFFF0000

primitive Black
primitive Blue

type Colour is (Black | Blue )

primitive ColourList
  fun tag apply(): Array[Colour] =>
    [Black; Blue]

for colour in ColourList().values() do
end

type EGLEvent is (U8, F32, F32)
(var code, var x, var y) = @getEvent[EGLEvent]()

primitive _XDisplayHandle
primitive _EGLDisplayHandle

let x_dpy = @XOpenDisplay[Pointer[_XDisplayHandle]](U32(0))
if x_dpy.is_null() then
  env.out.print("XOpenDisplay failed")
end

let e_dpy = @eglGetDisplay[Pointer[_EGLDisplayHandle]](x_dpy)
if e_dpy.is_null() then
  env.out.print("eglGetDisplay failed")
end

primitive _EGLConfigHandle
let a = Array[U16](8)
a.push(0x3040)
a.push(0b01011)
let config = Pointer[_EGLConfigHandle]
if @eglChooseConfig[U32](e_dpy, a, config, U32(1), Pointer[U32]) == 0 then
    env.out.print("eglChooseConfig failed")
end


actor Main
  new create(env: Env) =>
    // The no of arguments
    env.out.print(env.args.size().string())
    for value in env.args.values() do
      env.out.print(value)
    end
    // Access the arguments the first one will always be the the appication name
    try env.out.print(env.args(0)?) end

actor Main
  new create(env: Env) =>
    var options = Options(env)

    options
      .add("output", "o", StringArgument)

    env.out.print(options.has_argument())

    for option in options do
      match option
      | ("output", var arg: String) => _outputFileName = arg.string()
      | let err: ParseError =>
          err.report(env.out)
          env.out.print(
            """
            pony-embed [OPTIONS]
              --output     name   string output filename.
            """
          )
      end
    end

use "ponytest"

actor Main is TestList
  new create(env: Env) => PonyTest(env, this)
  new make() => None

  fun tag tests(test: PonyTest) =>
    test(_TestAddition)

class iso _TestAddition is UnitTest
  """
  Adding 2 numbers
  """
  fun name(): String => "u32/add"

  fun apply(h: TestHelper): TestResult =>
    h.expect_eq[U32](2 + 2, 4)

fun tag log(msg: String, verbose: Bool = false)
be fail() =>
be assert_failed(msg: String) =>
fun tag assert_true(actual: Bool, msg: String = "") ?
fun tag expect_true(actual: Bool, msg: String = ""): Bool
fun tag expect_eq[A: (Equatable[A] #read & Stringable)]
  (expect: A, actual: A, msg: String = ""): Bool
fun tag expect_eq[A: (Equatable[A] #unknown & Stringable)]
  (expect: A, actual: A, msg: String = ""): Bool

fun add(other: A): A
fun sub(other: A): A
fun mul(other: A): A
fun div(other: A): A
fun mod(other: A): A
fun eq(other: A): Bool
fun ne(other: A): Bool
fun lt(other: A): Bool
fun le(other: A): Bool
fun ge(other: A): Bool
fun gt(other: A): Bool
fun shl(other: A): A
fun shr(other: A): A
fun op_and(other:A): A
fun op_or(other: A): A
fun op_xor(othr: A): A

class Test
  fun alpha() =>
    """
    """

let dice: Array[U32] = [1; 2; 3
  4
  5
  6
]

actor Main
  fun foo(n:U32): {ref(U32): U32} =>
    var s: Array[U32] = Array[U32].init(n, 1)
    {ref(i:U32)(s): U32 =>
      try
        s(0) = s(0) + i
        s(0)
      else
        0
      end
    }

  new create(env:Env) =>
    var f = foo(5)
    env.out.print(f(10).string())
    env.out.print(f(20).string())

/* nested /* commentary */ */
// single comment

class A
class _A
x'
x''
x'.string()
'\uaaaa'
'\Ubbbbbb'
'\xcc'
'\''
'\n'
"\uaaaaa"
"\Ubbbbbbb"
"\xccc"
"\""
"\n"
34.4
34.4e43
43e4
0x3040
0xaF
0b01
3_43_4
0x0_4

fun create(): U32 => 0
fun iso create(): U32 => 0
fun \\ abc \\  iso create(): U32 => 0
class \\ packet, blah \\ iso Xyz
if \\ likely \\ a then
end

a.endnormal

print();print()

/* syntactically false: */

class _aA
class _a
class a
0b2332
0b
0x
0xgf
0f00
3.
.3
3.e3
3_
3__43_4
''
'\u'
'\ua'
'\uaaa'
'\uaaaaa'
'\uyyyy'
"\u"
"\ua"
"\uaaa"
"\uyyyy"
a'a

class badType
print();
