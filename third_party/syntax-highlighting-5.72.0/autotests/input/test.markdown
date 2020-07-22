---
Title:    A Sample Markdown Document
Author:   Fletcher T. Penney
Date:     February 9, 2011
Comment:  This is a comment intended to demonstrate 
          metadata that spans multiple lines.
---

# H1

## H2

### H3

Multi-line paragraph bla bla bla
bla bla bla.

Intentional line break  
via two spaces at line.

Formats: _italic_, **bold**, `monospace`, ~~strikeout~~

Bullet list:

* item1
* item2

Numbered list:

1. item 1
2. item 2

Task list:

- [x] Finish my changes
- [ ] Push my commits
- [ ] Open a pull request

A link as normal text: http://example.com

[link](http://kde.org)

Auto-link: <http://kate-editor.org>
Mail: <mailto:mark@kde.org>

This is [an example](http://example.com/ "Title") inline link.

See my [About](/about/) page for details.

This is [an example] [id] reference-style link.

[id]: http://example.com/  "Optional Title Here"
[foo]: http://example.com/ 'Optional Title Here'
[bar]: http://example.com/ (Optional Title Here)
[id2]: <http://example.com/>  "Optional Title Here"

![Alt text](/path/to/img.jpg)

![Alt text](/path/to/img.jpg "Optional title")

[![Alt text](https://url/to/img.svg)](https://link.com/example/)

    code 1
    code 2

normal text

> block quote _italic_
> more block quote

normal text

Title: some text

normal text

# Lists

1. item
   * This is a list item
     with multiple lines.
     ```
     code
     ```

     > This is a
     blockquote

     This is a new paragraph, which
     is part of the item in the
     previous list.

         indented code block

       Other paragraph.

     * subitem
       * subitem
         with multiple
            lines.

         New paragraph of
         the previous subitem.

             code block

          -----------------------

            Other paragraph of
            the subitem.

          30. numlist item
          31. numlist item

                 New paragraph of
                 the previous item.

                 > Blockquote

                  code block
  
              Other paragraph.

# Inline HTML

&copy;
<strong>bold</strong>
<code>code</code>

<table>
    <tr>
        <th>a</th>
        <th>b</th>
    </tr>
    <tr>
        <th><ul>
            <li>c</li>
            <li>d</li>
        </ul></th>
        <th>e</th>
    </tr>
</table>

normal text *italic*

# Code

`simple code`

``simple ` code``

```simple `` ` code ```

````simple`code````

normal text
    normal text
    normal text
        normal text

    code blocks are only allowed
    after an empty line.

* list
    list
    
    normal text
    
1. num-list
    num-list

    normal text

> quote
    quote
    
    code block

normal
<!-- comment -->
    <!-- code, not comment -->
    code

normal

  <!-- coment -->
    code

# Fenced code blocks (bug 356974)

## Bash

```bash
#!/usr/bin/env bash

for f in *; do
    echo "$f"
done
```

## C++

```cpp
#include <QtCore/QObject>

class Q : public QObject {
Q_OBJECT
public:
    Q();
}

Q::Q() :QObject(NULL) {
}
```

## C

```c
#include <stdio.h>
#include "stdint.h"
#define SOME_VAR 1

static uint64_t intWithSuffix = 42ull + ~1233L;
static int octNum = 07232;
```

## C#  ##

```csharp
var arr = new[] { 0, 1, 2, 3, 4 };
var anon = new { Country = "Germany", Inhabitants = 80000000 };
```

## CMake

```cmake
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project (hello_world)

set(QT_MIN_VERSION "5.6.0")
```

## CSS

```css
h1.main::hover {
    font-size: 100;
    color: green;
}
```

## Email

```email
From: Konqi Dragon <konqi@kde.org>
To: All <all@kde.og>
Subject: highlights
```

## Go

```go
package main

import "fmt"

func main() {
  fmt.Println("Hola, mundo.")
}
```

## Haskell

```haskell
module Main (main) where

main :: IO ()
main = putStrLn "Hello, World!"
```

## HTML

```html
<html>
  <body>
    <p class="main">Hello world!</p>
  </body>
</html>
```

## Java

```java
package fibsandlies;
import java.util.HashMap;

/**
 * This is an example of a Javadoc comment; Javadoc can compile documentation
 * from this text. Javadoc comments must immediately precede the class, method, or field being documented.
 */
public class FibCalculator extends Fibonacci implements Calculator {

    private static Map<Integer, Integer> memoized = new HashMap<Integer, Integer>();
    public static void main(String[] args) {
        memoized.put(1, 1);
        memoized.put(2, 1);
        System.out.println(fibonacci(12));
    }
}
```

## JavaScript

```js
function factorial(n) {
    if (n === 0) {
        return 1;  // 0! = 1
    }
    return n * factorial(n - 1);
}
```

## JavaScript React (JSX)

```jsx
<hello />
function Story(props) {
  const SpecificStory = components[props.storyType];
  return <SpecificStory story={ props.story } attr2="&ref;" attr3="Hello\n" />;
}
```

## JSON

```json
[{
    "hello": "world",
    "count": 1,
    "bool": true
}]
```

## Matlab

```matlab
% Numbers _____________________________________________
5, 5.5, .1, 0.1, 0.4e5, 1.2e-5, 5i, 5.3i, 6j, .345+3i
5', 5.5', .1', 0.1', 0.4e5', 1.2e-5', 5i', 5.3i', 6j', .345+3i'
```

## Markdown

```markdown
**bold** `code`
> quote

```

## Mustache/Handlebars

```handlebars
{{#movie}}
    <div>
        <h1>{{title}}</h1>
        <img src="{{poster}}" alt="{{title}}"/>
    </div>
    <div>
        Rating - {{ratings.critics_rating}}
    </div>
{{/movie}}
```

## Perl

```perl
my $var = shift;

$var =~ s/bla/foo/igs;
$var =~ s!bla!foo!igs;
$var =~ s#bla#foo#igs;
```

## PHP

```php
namespace Application\Controller;

use Zend\Mvc\Controller\AbstractActionController;

class IndexController extends AbstractActionController
{
}
```

## Python

```python
def addXToY(x, y):
    total = x + y
    print total
```

## QML

```qml
Text {
    id: hello
    width: 100
    text: "Hello world!"
}
```

## R

```r
# Declare function “f” with parameters “x”, “y“
# that returns a linear combination of x and y.
f <- function(x, y) {
  z <- 3 * x + 4 * y
  return(z)
}
```

## Raku

```raku
grammar Parser {
    rule  TOP  { I <love> <lang> }
    token love { '♥' | love }
    token lang { < Raku Perl Rust Go Python Ruby > }
}

say Parser.parse: 'I ♥ Raku';

say 1 #`««« blah TODO blah
here? »»» 2 ;
say 1 #` ( blah TODO blah
2 ;
rx- regex -
```

## Ruby

```ruby
require 'Config'
  def CGI::escape(string)
    string.gsub(/([^ a-zA-Z0-9_.-]+)/n) do
      '%' + $1.unpack('H2' * $1.size).join('%').upcase
    end.tr(' ', '+')
  end
```

## Rust

```rust
fn main() {
    println!("Hello world!");
}
```

## MySQL

```mysql
-- comment
CREATE TABLE stuff COLUMNS(col1 INT, col2 Varchar);
SELECT * FROM stuff WHERE id = 'string';
select * from stuff where id < 0.42;
Select col1, col2 From stuff Where stuff.col1 IS NOT NuLL;
```

## TypeScript

```ts
class Student {
    fullName: string;
    constructor(public firstName: string, public middleInitial: string, public lastName: string) {
        this.fullName = firstName + " " + middleInitial + " " + lastName;
    }
}
```

## XML

```xml
<tag attribute="3">
  <nested/>
</tag>
```

## reStructuredText

```rst
Section Header
==============

.. image:: /path/to/image.jpg

- A bullet list item
```

## Doxygen

```doxygen
a normal member taking two arguments and returning an integer value.
@param a an integer argument.
@param s a constant character pointer.
@see Javadoc_Test()
@see publicVar()
@return The test results
```

## No language specified

```
No language is specified, but it should be still rendered as code block.
```

# Fenced code blocks with more than 3 backticks or tildes

~~~
Fenced code block with 3 tildes.
~~~

`````````
Some implementations of Markdown/MultiMarkdown support more than 3 backticks or tildes.
```
The block ends with the same amount of backticks.
`````````

~~~~~~~~~~~~
Fenced code block with more tildes.
~~~~~~~~~~~~

## With languages

```````html
<html>
  <body>
    <p class="main">Hello world!</p>
  </body>
</html>
``` <!-- <= doesn't end here -->
```````

~~~bash
for f in *; do
    echo "$f"
done
~~~

~~~~~~~~~python
def addXToY(x, y):
    total = x + y
    print total
~~~ # <= doesn't end here
~~~~~~~~~

# Italic and bold text

**b** *i* __b__ _i_
***ib*** ___ib___
**_ib_** __*ib*__

* _italic_ **bold** ***ib*** ~~strikeout~~

> _italic_ **bold** ***ib*** ~~strikeout~~

## Bold
**bold*___text** normal
__bold_***text__ normal

## Italic
*italic _ text* normal
_italic * text_ normal

## Italic-Bold
***italic-bold ** italic-bold*** normal
***bold-italic text **bold-italic text*** normal
***bold*italic*** normal
**_bold-italic text_** normal

<!-- A non-space character is required after or before * or _ -->
_ normal text_ * normal text *
*italic text*normal text*
*italic text *italic text*
**bold text**normal text**
**bold text **bold text**

<!-- A delimiter is required after or before _ -->
aaa**bold text**aaaaa
aaa__normal text__aaaaa
пристаням_стремятся_

<!-- Escapes within bold or italic text -->
\*normal text*
_normal text\_
**italic text\**
***only bold\***
**bold\** bold**
