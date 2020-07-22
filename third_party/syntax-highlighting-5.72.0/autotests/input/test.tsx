// TypeScript React

/** @author Willy <willy@wmail.com>
  * @url https://reactjs.org/ **/

import React from 'react';
import { PhotoStory, VideoStory } from './stories';

function Story(props) {
  const SpecificStory = components[props.storyType];
  return <SpecificStory story={ props.story } attr2="&ref;" attr3="Hello\n" />;
}

function
<Tag attr1={ <tag/> function <noTag/> return class var 0x123 { } &noRef; hello() React.Component() } attr2="&ref;">
	/* no comment*/ function <tag/> return class var 0x123 &ref; hello() React.Component()
	.<tag/> anyWord <tag/>
	{ function <tag> return class var 0x123 hello() React.Component() }
</Tag>

<tag1> </tag1>
<tag1> </Tag$>
<Tag$> </tag>

<tag/*comment*/attr1/*comment*/= /*comment*/"value"/*comment*/attr2 /*comment*/attr3='a' key/*comment*/key2 />

// Detect Valid tags

/* comment */ <tag></tag>
{ /* comment
	*/ <Tag />
	word <noTag/> . <noTag/> } <noTag/>
return /* comment
	multiline */ <tag/> /* comment */ <Tag/>
&& /*comment*/ <Tag/>
& /*comment*/ <noTag/>

  <tag/>
{ <hello>Hello</hello> }
?<Tag />;
[ <tag />  ( <tag />
,<Tag/>    =<Tag/>
&&<tag/>   ||<tag/>
return <tag/> ;
default<tag/> ;
<Tag> <tag> <tag$/> </tag> return </Tag>

anyWord<noTag>
anyWord/*comment*/ <noTag/>
.<noTag>
&<notag> | <noTag/>
% /* comment*/ <noTag/>

// TODO: Fix this (comment before the tag name):
var x = </**/div></div>;

// Tag after ":"
annotation: <tag/>
annotation: text [ <tag/> ]
console.log("hello")

// Type assertion in tag
<C<number>/>
<C<number>> </C>
<C
<error />

// Non-ASCII tag name & attribute
<日本語></日本語>;
<Component 本本:本-本 aa本:本 aa:aa />
<aaaa:ñ />

<Namespace.DeepNamespace.Component />;
<Component { ... x } y
={2 } z />;

let k1 =
    <Comp a={10} b="hi" {...o} >
        hi hi hi!
    </Comp>;

let k2 =
    <Comp a={10} b="hi">
        <div> My Div </div>
        {(name: string) => <div> My name {name} </div>}
    </Comp>;

let k3 = <GenericComponent initialValues={{ x: "y" }} nextValues={a => ({ x: a.x })} />; // No Error

// OK
let k1 = <Comp a={10} b="hi"><></><Button /><AnotherButton /></Comp>;
let k2 = <Comp a={10} b="hi"><><Button /></><AnotherButton /></Comp>;
let k3 = <Comp a={10} b="hi"><><Button /><AnotherButton /></></Comp>;
let k4 = <SingleChildComp a={10} b="hi"><><Button /><AnotherButton /></></SingleChildComp>;
// OK
let k1 = <div> <h2> Hello </h2> <h1> world </h1></div>;
let k2 = <div> <h2> Hello </h2> {(user: any) => <h2>{user.name}</h2>}</div>;
let k3 = <div> {1} {"That is a number"} </div>;
let k4 = <Button> <h2> Hello </h2> </Button>;

// Empty tags
hello<>
hello<string>

<></>; // no whitespace
<    ></   >; // lots of whitespace
< /*starting wrap*/ ></ /*ending wrap*/>; // comments in the tags
<>hi</>; // text inside
<><span>hi</span><div>bye</div></>; // children
<><span>1</span><><span>2.1</span><span>2.2</span></><span>3</span></>; // nested fragments
<>#</>; // # would cause scanning error if not in jsxtext

// Tags after substitutions in templates
`aaa${<tag></tag>//comment
    /*comment*/<A/>}`

// Don't highlight tags within type declaration
type T12 = ReturnType<(<T>() => T)>;
type T13 = ReturnType<(<T extends U, U extends number[]>() => T)>;
type T14 = ReturnType<typeof f1>;
type T15 = ReturnType<(s: string) => void>;

// Don't highlight tags within variable declaration
let myIdentity: <T>(arg: T) => T <noTag/> = <Tag />;
var myIdentity: <U>(arg: U) => U = identity;
const myIdentity: {<T>(arg: T): T} = identity;

// Don't highlight tags within interfaces and classes
interface GenericIdentityFn {
    <T>(arg: T): T;
    <noTag />
}
class Handler {
    info: <T>(arg: T): T <noTag />;
    <tag> </tag>
}

// Highlight "<T extends" as a type assertion, not as a tag
// (the "cast" expression works and isn't supported in the TSX file)
const goodHighlighting = <T extends I>(
  arg: T
) => {
  const a = arg
  return a
}
<tag> text <T extends I/> </tag> // Here "<T extends I/>" is a tag

// Check character after tag name, do not highlight invalid tags
<noTag  ?
<noTag  ,
<noTag /* comment */ ?
<noTag#
<noTag/*comment*/#

// Conditionals expressions
let y = foo == null ? null : <Tag/>;
let x = (foo === null || foo === undefined) ?
    undefined :<tag>hello</tag>;

// Tagged template literals
tagFunc`
    Hello world!
    ${ <Tag />; 22 + "11" }`;
obj.something.tagFunc`Setting ${setting} is ${value}!`;
