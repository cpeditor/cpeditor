// NOTE: This is a test file for kate's LPC syntax highlighting.

// This is a Single-Line Comment
/* This is a Multi-
                   Line Comment */

// This again is a Single-Line Comment which should end here /*

// And this is an evil single line comment \
   which should include this and the next line because of the \
   Do not use this style at home, kids.
// BEGIN region marker

// END of region marker

private void create()
{
// Some Preprocessor stuff:
#define SOME_STUFF if(foo("bar")) \
                   { \
                       bar("foo"); \
                   } // Preprocessor, Keyword, Preprocessor-String, Multiline

    // Some closures:
    #'create;
    #'?!;


    /* Some other Data Types: */

    int i = 1;            // Integer.
    float b = 2.34;       // Float.
    int c = 0b001;        // Binary
    int e = 0x019Beef;    // Hexadecimal
    int f = 0o2345;       // Octal
    string a = "Wheee";   // String
    string x = "Wheee\
     heee";               // Multi-Line String, again, don't use this.



    /* Some keywords: */
    if(1)
    {
        switch(2)
        {
            case 3:
                4;
                break;
        }
    }

    else
    {
        return 0;
    }
}

/*
WARNING: If the above function is part of a multi-line comment,
         it's buggy. The WARNING: itself should be a comment-keyword.
         That's not actually part of the language, but simply used
         to highlight important stuff inside comments like TODO: etc.
*/
