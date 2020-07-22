/********
* Highlight testing module.
*
* Do not attempt to run this!
***********/
module highlighttest;
import X = null;

/++ Pragma directives. DDoc + DDoc embedded items. Special Tokens.
+
+ ---
+ // comment
+ #line 12 "hightlighttest.d"	/* block comment */
+ #line __LINE__ __FILE__	/++ embedded block comment +/
+
+ pragma /* */ (msg, "what?");
+ pragma(/++ +/ lib, "insane.a");
+ pragma(inline);
+ pragma(linkerDirective);
+ pragma(mangle);
+ pragma(startaddress);
+ pragma(D_Custom_Extension, "custom data");
+ pragma(foo 0);
+ ---
+/

/// version condition
version = X;
version = 1_2_3;
version (X) ;
version(linux) {}

/// linkage
extern
    (C) {}
extern :
;
extern (C++) {}
extern (C++, foo.bar.baz) {}
extern (D) {}
extern (Windows) {}
extern (Pascal) {}
extern (System) {}
extern (unknown) {}
extern (C,) {}
extern (C++, foo, bar) {}

/// alias & typedef
alias int.min minint;
typedef int myint;

int main(char[][] args) {
    /// statements
    if (1) {}
    else {}
    with (N) {x = B}
    
    /// attributes
    auto x = 1;
    static if (true) {}
    void (in X, out Y) {}	// NOTE: using in like this is rare, more common to use as an expression and no way to tell apart?
    
    /// deprecated
    deprecated void fct ();
    
    /// types
    void a;
    ushort u;
    int[uint] AA;
    class C;
    enum N : int { A = 5, B }
    typeof(u) u2;
    
    /// expressions
    x = cast(int) 55;
    void* p = null;
    p = cast(void*) new int;
    x = 1 in AA;	// NOTE: a THIRD use of in. How to detect??
    assert (true);
        
    /// libsymbols
    string s = "";
    throw new Exception;
    TypeInfo ti = typeid(int);
    
    /// tests
    debug {}
    debug (2) {}
    debug (DSymb) {}
    unittest {}
    
    /// scope (as attribute and as statement)
    scope struct S;
    scope (exit) {}
    scope 
     (success) {}	// NOTE: rules cannot match across new-lines
    scope (failure) {}
    
    /// Properties
    x = int.min;
    s = (5-3).stringof;
    
    /// strings
    s = r"raw string";
    s = x"00FF";
    s = \n \a;
    s = \u1234;
    s = \U12345678;
    s = \&amp; ;
    char c = 'a';
    s = "abc 012 \" \n \x12 \u1234 \U12345678";
    s = `BQString '"`;
    s = q{foo "bar" 123};
    s = q"FOO
foo
FOO";
    s = q"[foo [bar] q"[baz]"]";
    s = q"(foo (bar) q"(baz)")";
    s = q"<foo <bar> q"<baz>">";
    s = q"{foo {bar} q"{baz}"}";
    s = q"/foo/";
    s = q"!foo!";
    
    /// region markers
    //BEGIN x
    //END x
    
    /// DDoc
    /*******
    * DDoc
    *
    * Section:
    * New section.
    * $(I italic)
    *******/
    /+++++++
    + DDoc
    + /+
    + +/
    +++++++/
    
    // comments
    // FIXME NOTE
    /* comment */
    /+ comment /+ nested comment +/ +/
    
    /// brace folding
    {
    }
    
    /** normal text
    * ---
    * .x;
    * ..
    * ...
    * ....
    * .....
    * _._
    * _e1
    * ---
    */
    
    /// float and int literals
    int i;
    real r;
    ireal ir;
    r = .0;
    r = 0f;
    ir = 0e0i;
    ir = 0.fi;
    r = 0.0e0;
    r = 0xF.Fp0;
    r = 0x_._p0_;
    i = 5;
    i = -1;
    i = 0b10;
    i = 0070;
    i = 00;
    i = 0xF0;
    
    /// ranges
    int[] A;
    i = A[1];
    A = A[0..$];
    A = A[0..0];
    A = A[0..length];
    
    /// labels
    label:
    goto label;
    
    /// function, delegate
    creal function () fp = function creal() { return 0f+0fi; };
    void delegate (in int i, lazy int b) dg = delegate void (int, int) {}
    
    /// in, out, body
    // NOTE: highlighting in & out as statements here could be difficult
    float F ()
    in {}
    out (result) {}
    body {}
    
    /// try, catch, finally
    try {
        throw new Exception("oh no... ");
    } catch (Exception e) {
    } finally {
    }
    
    /// mixin
    mixin("return false;").stringof;
    
    /// templates
    macro; // what does this do?
    template Tp (T) {
        Tp t;
    }
    Tp!(int) y;
}
