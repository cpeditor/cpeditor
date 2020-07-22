// This is a test file for the Katepart GLSL Syntax Highlighting.

normal text
// this is a single-line comment
normal text
/* this
is a multi-line 
comment */
normal text

some_symbol.some_member;
some_symbol.some_member_function();
some_function();

// this is a function
void main()
{
	float f = 1.4e3; // decimal float literal
	int i1 = 2884;   // decimal int literal
	int i2 = 0x44;   // hex int literal
	int i3 = 0456;   // octal int literal
}

// this is a structure
struct some_struct
{
	vec3 some_member_vector;
};

# this is 
#preprocessor code

// all keywords
break continue do for while
if else
true false
discard return
struct

// all basic types
float int void bool
mat2 mat3 mat4
vec2 vec3 vec4
ivec2 ivec3 ivec4
bvec2 bvec3 bvec4
sampler1D sampler2D sampler3D
samplerCube sampler1DShadow sampler1DShadow

// all type qualifiers
attribute const uniform varying
in out inout

// attensions:
// FIXME
// TODO
// BUG

// some of the std functions
radians degrees sin cos tan asin acos atan

// some of the std variables
gl_Position gl_PointSize gl_ClipVertex
