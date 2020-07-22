// Sample YARA file for Syntax Highlighting
// Obtained from: https://yara.readthedocs.io/en/stable/writingrules.html

/*
    This is a multi-line comment ...
*/

rule silent_banker : banker
{
    meta:
        description = "This is just an example"
        threat_level = 3
        in_the_wild = true
    strings:
        $a = {6A 40 68 00 30 00 00 6A 14 8D 91}
        $b = {8D 4D B0 2B C1 83 C0 27 99 6A 4E 59 F7 F9}
        $c = "UVODFRYSIHLNWPEJXQZAKCBGMT"
    condition:
        $a or $b or $c
}

rule dummy
{
    condition:
       false
}

rule ExampleRule
{
    strings:
        $my_text_string = "text here"
        $my_hex_string = { E2 34 A1 C8 23 FB }

    condition:
        $my_text_string or $my_hex_string
}

// Hexadecimal strings

rule WildcardExample
{
    strings:
       $hex_string = { E2 34 ?? C8 A? FB }

    condition:
       $hex_string
}

rule JumpExample
{
        strings:
           $hex_string = { F4 23 [4-6] 62 B4 }

        condition:
           $hex_string
}

rule AlternativesExample
{
    strings:
       $hex_string = { F4 23 ( 62 B4 | 56 | 45 ?? 67 ) 45 }

    condition:
       $hex_string
}

// Text strings

rule CaseInsensitiveTextExample
{
    strings:
        $text_string = "foobar" nocase

    condition:
        $text_string
}

rule WideCharTextExample
{
    strings:
        $wide_and_ascii_string = "Borland" wide ascii

    condition:
       $wide_and_ascii_string
}

// XOR strings

rule XorExample1
{
    strings:
        $xor_string = "This program cannot" xor

    condition:
        $xor_string
}

rule XorExample2
{
    strings:
        $xor_string_00 = "This program cannot"
        $xor_string_01 = "Uihr!qsnfs`l!b`oonu"
        $xor_string_02 = "Vjkq\"rpmepco\"acllmv"
        // Repeat for every single byte XOR
    condition:
        any of them
}

rule XorExample3
{
    strings:
        $xor_string = "This program cannot" xor wide ascii
    condition:
        $xor_string
}

rule XorExample4
{
    strings:
        $xor_string_00 = "T\x00h\x00i\x00s\x00 \x00p\x00r\x00o\x00g\x00r\x00a\x00m\x00 \x00c\x00a\x00n\x00n\x00o\x00t\x00"
        $xor_string_01 = "U\x01i\x01h\x01r\x01!\x01q\x01s\x01n\x01f\x01s\x01`\x01l\x01!\x01b\x01`\x01o\x01o\x01n\x01u\x01"
        $xor_string_02 = "V\x02j\x02k\x02q\x02\"\x02r\x02p\x02m\x02e\x02p\x02c\x02o\x02\"\x02a\x02c\x02l\x02l\x02m\x02v\x02"
        // Repeat for every single byte XOR operation.
    condition:
        any of them
}

rule XorExample5
{
    strings:
        $xor_string = "This program cannot" xor(0x01-0xff)
    condition:
        $xor_string
}

// Base64 strings

rule Base64Example1
{
    strings:
        $a = "This program cannot" base64

    condition:
        $a
}

rule Base64Example2
{
    strings:
        $a = "This program cannot" base64("!@#$%^&*(){}[].,|ABCDEFGHIJ\x09LMNOPQRSTUVWXYZabcdefghijklmnopqrstu")

    condition:
        $a
}

// Regular expressions

rule RegExpExample1
{
    strings:
        $re1 = /md5: [0-9a-fA-F]{32}/
        $re2 = /state: (on|off)/

    condition:
        $re1 and $re2
}

// Conditions

rule Example
{
    strings:
        $a = "text1"
        $b = "text2"
        $c = "text3"
        $d = "text4"

    condition:
        ($a or $b) and ($c or $d)
}

rule CountExample
{
    strings:
        $a = "dummy1"
        $b = "dummy2"

    condition:
        #a == 6 and #b > 10
}


rule AtExample
{
    strings:
        $a = "dummy1"
        $b = "dummy2"

    condition:
        $a at 100 and $b at 200
}

rule InExample
{
    strings:
        $a = "dummy1"
        $b = "dummy2"

    condition:
        $a in (0..100) and $b in (100..filesize)
}

// File size

rule FileSizeExample
{
    condition:
       filesize > 200KB
}

// Executable entry point

rule EntryPointExample
{
    strings:
        $a = { 9C 50 66 A1 ?? ?? ?? 00 66 A9 ?? ?? 58 0F 85 }

    condition:
       $a in (entrypoint..entrypoint + 10)
}


// Accessing data at a given position

rule IsPE
{
  condition:
     // MZ signature at offset 0 and ...
     uint16(0) == 0x5A4D and
     // ... PE signature at offset stored in MZ header at 0x3C
     uint32(uint32(0x3C)) == 0x00004550
}

// Sets of strings

rule OfExample1
{
    strings:
        $a = "dummy1"
        $b = "dummy2"
        $c = "dummy3"

    condition:
        2 of ($a,$b,$c)
}

rule OfExample2
{
    strings:
        $foo1 = "foo1"
        $foo2 = "foo2"
        $foo3 = "foo3"

    condition:
        2 of ($foo*)  // equivalent to 2 of ($foo1,$foo2,$foo3)
}

rule OfExample3
{
    strings:
        $a = "dummy1"
        $b = "dummy2"
        $c = "dummy3"

    condition:
        1 of them // equivalent to 1 of ($*)
}

// Iterating over string occurrences

rule Occurrences
{
    strings:
        $a = "dummy1"
        $b = "dummy2"

    condition:
        for all i in (1,2,3) : ( @a[i] + 10 == @b[i] )
}

// Referencing other rules

rule Rule1
{
    strings:
        $a = "dummy1"

    condition:
        $a
}

rule Rule2
{
    strings:
        $a = "dummy2"

    condition:
        $a and Rule1
}

// Metadata

rule MetadataExample
{
    meta:
        my_identifier_1 = "Some string data"
        my_identifier_2 = 24
        my_identifier_3 = true

    strings:
        $my_text_string = "text here"
        $my_hex_string = { E2 34 A1 C8 23 FB }

    condition:
        $my_text_string or $my_hex_string
}

// External variables

rule ExternalVariableExample1
{
    condition:
       ext_var == 10
}

rule ExternalVariableExample2
{
    condition:
       bool_ext_var or filesize < int_ext_var
}

rule ExternalVariableExample3
{
    condition:
        string_ext_var contains "text"
}

rule ExternalVariableExample4
{
    condition:
        string_ext_var matches /[a-z]+/
}

rule ExternalVariableExample5
{
    condition:
        /* case insensitive single-line mode */
        string_ext_var matches /[a-z]+/is
}

// Including files

include "other.yar"
include "./includes/other.yar"
include "../includes/other.yar"
