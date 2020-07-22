#! /bin/env pike

/* This file is a syntax highlight test for Kate.
 * FIXME: Improve it to contain more (and more unusual) syntax examples.
 */


#define PIKE_ON_THE_WEB  /* Is this address correct? */  "http://pike.ida.liu.se/"


int main(int argc, array(string) args)
{
  // Write funny things with Pike :)
  write(`+("Command line arguments (%d of them): ", @map(args, `+, " ")) + "\n", argc);
                             
  write("\nVisit Pike site at %s\n\n", PIKE_ON_THE_WEB);

  for (int i = 1; i <= 3; i++)
    write(":" + ")" * i + "   ");

  write("\n" + ({"Bye", "bye"}) * "-" + "!\n");

  return 0;
}
