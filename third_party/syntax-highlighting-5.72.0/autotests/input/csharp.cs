// this is a single-line comment
// NOTE ### FIXME TODO

/* this is a
   multi-line comment
*/

/* this is another
 * multiline comment
 * NOTE ### FIXME TODO
 */

/** this is another
 * multiline comment
 * NOTE ### FIXME TODO
 */

// simple assignment
var i = 5;

var j = // assignment missing, still should be nice highlighting
var k   // assignment missing, still should be nice highlighting

// test a string
var s = "Hello World!";

// arr is an array
var arr = new[] { 0, 1, 2, 3, 4 };

// expr is compiled as IEnumerable<Customer>
// or perhaps IQueryable<Customer>
var expr =
    from c in customers
    where c.City == "London"
    select c;

// compiled anonymous type
var anon = new { Country = "Germany", Inhabitants = 80000000 };
