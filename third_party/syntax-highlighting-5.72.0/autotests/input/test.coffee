# CoffeeScript test
# Source: https://coffeescript.org

### Multiline
  comment ###
import 'local-file.coffee'

# Assignment:
number   = 42
opposite = true

# Conditions:
number = -42 if opposite

# Functions:
square = (x) -> x * x

# Arrays:
list = [1, 2, 3, 4, 5]

# Objects:
math =
  root:   Math.sqrt
  square: square
  cube:   (x) -> x * square x

# Splats:
race = (winner, runners...) ->
  print winner, runners

# Existence:
alert "I knew it!" if elvis?

# Array comprehensions:
cubes = (math.cube num for num in list)

# Classes:
class Animal
  constructor: (@name) ->

  move: (meters) ->
    alert @name + " moved #{meters}m."

# Switch/When/Else:
switch day
  when "Mon" then go work
  when "Tue" then go relax
  when "Thu" then go iceFishing
  when "Fri", "Sat"
    if day is bingoDay
      go bingo
      go dancing
  when "Sun" then go church
  else go work

# Member objects:
Object.prop.  Object .Object Object
timelineEvent.class

# Embedded JavaScript Code
markdown = `function () {
  return \`In Markdown, write code like \\\`this\\\`\`;
}`

```
  // Comment
  var array = new Array(500);
  var name = "Willy";
  alert(`Hello ${name}!`);
```
` String.raw\`String ${x} \` `

` // Regular Expression after template
  const a = \`6\` / 2; /*comment*/
  \`template\` /regex/ `

` // Tagged template literals
  tagFunc\`Setting ${setting} is ${value}!\` `
