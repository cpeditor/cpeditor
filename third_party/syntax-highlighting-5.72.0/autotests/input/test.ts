/// <reference types="node" />

// TypeScript Test

class Student {
    fullName: string;
    constructor(public firstName: string, public middleInitial: string, public lastName: string) {
        this.fullName = firstName + " " + middleInitial + " " + lastName;
    }
}

interface Person {
    firstName: string;
    lastName: string;
}

function greeter(person : Person) {
    return "Hello, " + person.firstName + " " + person.lastName;
}

let user = new Student("Jane", "M.", "User");

// Built-in
document.body.innerHTML = greeter(user); to
JSON.stringify()
console.log("Hello world");
const os = require('os');
Math.LOG10E;
Number.MAX_SAFE_INTEGER;
String.raw`raw text \.\n${}`

import http = require("http");
import path = require("path");
import URL = url.URL;
import { Readable, Writable } from "stream";
import { isBuffer, isString, isObject } from "util";

const port = 8__88___8;
const baseUrl = new URL(`http://localhost:${port}/`);
const rootDir = path.dirname(__dirname);

const defaultBrowser = os.platform() === "win32" ? "edge" : "chrome";
let browser: "edge" | "chrome" | "none" = defaultBrowser;
let grep: string | undefined;

interface FileBasedTestConfiguration {
    [setting: string]: string;
}

function swapCase(s: string): string {
    return s.replace(/\w/g, (ch) => {
        const up = ch.toUpperCase();
        return ch === up ? ch.toLowerCase() : up;
    });
}

for (var i in pieces) {
    switch (true) {
        case /^\"?Accession\"?/.test(pieces[i]):
            numeration[0] = i;
            break;
    }
}

const enum ResponseCode {
    Success = 200,
    BadRequest = 400
}

// Substitutions
export function isInOpenInterval(start: number, end: number) {
    return tersible(a => a > start && a < end, () => `(${start}...${end})`)
}
const bar = `${x} ${y}`;

// readonly
function f1(mt: [number, number], rt: readonly [number, number]) {
}

function f2(ma: string[], ra: readonly string[], mt: [string, string], rt: readonly [string, string]) {
}

type ReadWrite<T> = { -readonly [P in keyof T] : T[P] };

// const assertion
let obj = { x: 10, y: [20, 30], z: { a: { b: 42 } } } as const;
let r2 = { x: 20, y: 10 } as const;
let r2 = {...p} as const;
let p1 = { x: 10, y: 20 as const };
let p2 = { 10 as const, 'foo' as const };

// Definite assignment operator
class Cl {
    one?: string;
    two!: string;
}
let x! = 1;

// Function with multiple arguments
const children = [].map<Set<Map<number, string>>>(element => {
    if (!this.identityProvider) {
        return element;
    }
    return element;
});

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
var l = 1__.e+3_22 | .2____e2 | 0o1_23 | 11__. ;

// Bigint
const binBig = 0b101n;
const octBig = 0o567n;
const hexBig = 0xC0Bn;
const decBig = 123n;

// Types
let a: null = null;
let b: number = 123;
let c: number = 123.456;
let d: string = `Geeks`;
let e: undefined = undefined;
let f: boolean = true;
let g: number = 0b111001; // Binary
let h: number = 0o436; // Octal
let i: number = 0xadf0d; // Hexa-Decimal
let j: bigint = 123n;

const query = query<[number], number>(`
        SELECT *
        FROM statistics
        WHERE unit_id = $1`)

function runQuery() {
    const query = createQuery<[number[]], Table<Columns>>(`
        some SQL here
    `)
    return database.execute(query)
}

aa: <sdf/> string ?<ssd/> string
 string // Don't highlight
aa: string assa |
   string
   string ;
   string

aa: { string
    string } // Don't highlight

aa: [ string
      string ]
aa: ( string
      string ) // Don't highlight
aa: string <string>
interface a {
    aa: /* comment
        */ string,
    bb: /* comment */
        number,
    cc: // comment
        void,
    dd:
        any,
}

null, <{[key]: () => any}> null
null, <{[key]: () =>{a: number}}> null

// Correctly highlighting regular expressions
dt = ((dt[0] * 1e9 + dt[1]) / 1e6).toFixed(3).replace(/\.?0+$/, '') + 'ms';
(a[0] / 2) / (2)

// Type guards
function isNumber(x: any): x is number {
    return typeof x === "number";
}

// Conditional expressions (highlight keywords before ":")
class C {
    w = () =>
        this.a() ? true : this.b() ? false : true;
    z = () =>
        this.b() ? hello : k;
}

function foo<T extends X>(arg: T): T extends B ? number : string {
  if (arg === "A") return <T extends B ? number : never>111;
  return <T extends B ? never : string>"returning a string";
}

let x = (foo === null || foo === undefined) ?
    undefined :
    foo.bar.baz();
let y = (foo === null || foo === undefined) ? /* comment */ undefined :
    foo.bar.baz();

// Operator ??
let x = foo ?? bar();

// Types and logical `&&` and `||` operators after `as` expression
Date as any || null;

//Assertions
const assert: (value: unknown) => asserts value = value => {}
declare function assertIsString(value: unknown): asserts value is string;
declare function assertIsArrayOfStrings(value: unknown): asserts value is string[];
declare function assertDefined<T>(value: T): asserts value is NonNullable<T>;
namespace Debug {
    export declare function assert(value: unknown, message?: string): asserts value;
    export declare function assertDefined<T>(value: T): asserts value is NonNullable<T>;
}

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

 [1, 2, 3].reduce<readonly number[]>((previous) => previous, []);

// Multiline arrow assignment
const h = {
  setSubProperty: <
      T extends A,
      K extends keyof T,
      J extends keyof T[K]
    >(
      property: K,
      key: J,
      value: T[K][J]
    ) =>  {}
}

// Private-Named Instance Fields
class Greeter {
    #name: string;
    constructor(name: string) {
        this.#name = name;
    }
    greet() {
        console.log(`hello ${this.#name}`);
    }
}
class Test {
    #NAME1: () => string;
    prop1 = x.#NAMEabc.#NAMEabc;
    prop2 = x.#NAMEabc.#NAMEabc();
    prop3 = this.#NAMEabc.#NAME();
}

// Type-only imports and exports
import type T from "mod";
import type { U, V } from "mod";
import type * as types from "mod";
import type from "mod";
import type = require("mod");
import type T = require("mod");

export { a as b, x };
export * from "AnotherModule";
export { k as m, l } from "AnotherModule";
export type { T };
export type { U } from "AnotherModule";

// 'awaited' type operator: https://github.com/microsoft/TypeScript/pull/35998
let v1: awaited number;
let v2: awaited Promise<number>;
function f7<U>() {
    let v0: awaited U;
    let v1: awaited Promise<U>;
}

// Tagged template literals
tagFunc`
    Hello world!
    ${ alert("Hello!"); }`;
obj.something.tagFunc`Setting ${setting} is ${value + 5}!`;
