* This file is an example to test the syntax highlighting file fortran-fixed.xml
* (for fortran, fixed format)

c <-- this is a comment in the old fortran 77 style (fixed form)
c In the free form file, so we shouldn't use this kind of comments!
c But fortran 90 still understands fixed form, when parsing sources with
c the *.f extension.

* this 'c' shouldn't be highlighted as a comment!
 c

*     Prints the values of e ** (j * i * pi / 4) for i = 0, 1, 2, ..., 7
*         where j is the imaginary number sqrt(-1)

      PROGRAM CMPLXD
          IMPLICIT COMPLEX(X)
          PARAMETER (PI = 3.141592653589793, XJ = (0, 1))               this text should be highlighted as a comment because it is after the 72th column (ignored by the compiler)
          DO 1, I = 0, 7
              X = EXP(XJ * I * PI / 4)
              IF (AIMAG(X).LT.0) THEN
                  PRINT 2, 'e**(j*', I, '*pi/4) = ', ! I am an in-line comment
     *                     REAL(X), ' - j',-AIMAG(X) ! the character at the 6th column indicate a continuation line and should be highlighted as a continuation character
              ELSE
                  PRINT 2, 'e**(j*', I, '*pi/4) = ',
     *                     REAL(X), ' + j', AIMAG(X)
              END IF
              FORMAT (A, I1, A, F10.7, A, F9.7)
              CONTINUE
          ENDDO
          STOP
      END
