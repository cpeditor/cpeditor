// Comment NOTE
/* Comment FIXME ALERT
	*/

// Identifiers
hellóñαωΑΩµo!();
HellóñαωΑΩµ::Hello;
'hellóñαωΑΩµo

pub use self::PathParameters::*;
pub use symbol::{Ident, Symbol as Name};
use serialize::{self, Encoder, Decoder};
use std::u32;

#[derive(Clone, PartialEq, Eq, Hash, Copy)]

pub struct Lifetime {
	pub id: NodeId,
	pub span: Span,
	pub bounds: Vec<PathSegment>
}

impl fmt::Debug for Lifetime {
	fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
		write!(f, "lifetime({}: {})", self.id, pprust::lifetime_to_string(self))
	}
}
impl<'a> PartialEq<&'a str> for Path {
	fn eq(&self, string: &&'a str) -> bool {
		self.segments.len() == 1 && self.segments[0].identifier.name == *string
	}
}

enum TraitBoundModifier {
	None,
	Maybe,
}

union tests {
	a: i128,
	b: isize,
}

// Self vs self
trait T {
    type Item;
    // `Self` will be whatever type that implements `T`.
    fn new() -> Self;
    // `Self::Item` will be the type alias in the implementation.
    fn f(&self) -> Self::Item;
}

// Raw identifiers
extern crate foo;
fn main() {
    foo::r#try();
}

#[valid types]
fn types() {
	let num = 333_3_;
	let num_u8: u8 = 333u8;
	let num_u16: u16 = 333u16;
	let num_u32: u32 = 333u32;
	let num_u64: u64 = 333u64;
	let num_u128: u128 = 333u128;
	let num_usize: usize = 333usize;
	let num_float: f32 = 333.45f32;

	let binary = 0b1_010;
	let octal = 0o21535;
	let hexadecimal = 0x73A2_F;

	let char1: char = 'a';
	let char2: char = '\n';
	let char3: char = '\u{123_AF}';

	let byte1: u8 = b'a';
	let byte2: u8 = b'\x13';

	let string: str = "hello \n \r \u{123_________fd_} \
						bye";
	let byte_string: str = b"hello \t \0 \u{123} \b bye";
	let raw_string1: str = r"hello \t \b";
	let raw_string2: str = r####"hello \n "### bye"########;
	let raw_string3: str = br####"hello \n"####;

    // Invalid

	let invalid_binary= 0b1_015;
	let invalid_octal = 0o64_92;
	let invalid_hexadecimal = 0x7_3AY;

    let invalid_char1: char = '\y';
	let invalid_char2: char = '\324';
	let invalid_char3: char = '%%';
	let invalid_char4: char = '\n\dfd';
	let invalid_char5: char = 'aaaaa';
	let open_char: char = '&&&;

	let invalid_byte1: u8 = b'ab';
	let invalid_byte2: u8 = b'\b';
	let invalid_byte2: u8 = b'\u{123}';

	let invalid_string: str = "hello \b \u{_123} \u{1234567} \  bye";
}
