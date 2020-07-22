! This file is an example to test the syntax highlighting file fortran-free.xml
! (for fortran, free format)

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!                      THIS IS AN EXAMPLE OF A MODULE                          !
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
module module_example

  ! use 'implicit none' when you want all variables to be declared
  implicit none

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! PUBLICS AND PRIVATES

  ! In fortran 90 you can define your own operator
  public :: operator(.norm.)
  public :: operator(+) ! <-- you can also overload the usual operators
  public :: factorial
  public :: example_fn

  private :: point3d_add
  private :: point3d_norm

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! USER-DEFINED TYPES...

  ! This is a definition to use in declarations of real variables,
  ! parameters, etc.
  integer, parameter, public :: kr = selected_real_kind(10)

  ! This is a user-defined type
  type, public :: point3d
    real(kind=kr) :: x, y, z
  end type point3d

  ! This type is useless: it is only an example of type definition!
  type, public :: example_type
    complex(kind=kr)            :: c ! <-- a complex number (two reals of kind kr)!
    real, dimension(-10:10)     :: & ! <-- this line does not end here!
      r1, r2 ! <-- this is the final part of the previous line
    real, pointer, dimension(:) :: pointer_to_array_of_real
    real, dimension(:), pointer :: array_of_pointer_to_real
  end type example_type

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! INTERFACES...

  ! Interface for the norm of a 3-D vector
  interface operator(.norm.)
    module procedure point3d_norm
  end interface

  ! Interface for the operator '+'
  interface operator(+)
    module procedure point3d_add
  end interface

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! SOME DECLARATIONS...

  ! A real number can be declared with the following line:
  real(kind=kr) :: real_var1
  ! But if you are not interested on the precision of floating point numbers,
  ! you can use simply:
  real :: real_var2

  ! An array can be declared in two ways:
  real(kind=kr), dimension(1:10, -4:5), private :: a, b, c
  real(kind=kr), private :: d(1:10, -4:5)

  ! This is a string with fixed lenght
  character(len=10) :: str_var

  ! This is an allocatable array, which can be a target of a pointer
  type(example_type), private, dimension(:), allocatable, target :: &
   many_examples

! Fortran 90 hasn't got its own preprocessor, it uses the C preprocessor!
#ifdef XXX

#endif

contains


  ! The sum of two points
  pure function point3d_add(a, b) result(rs)
    type(point3d) :: rs
    type(point3d), intent(in) :: a, b
    rs%x = a%x + b%x
    rs%y = a%y + b%y
    rs%z = a%z + b%z
  end function point3d_add


  ! The norm of a point
  pure function point3d_norm(a) result(rs)
    real(kind=kr) :: rs
    type(point3d), intent(in) :: a
    rs = sqrt(a%x * a%x + a%y * a%y + a%z * a%z)
  end function point3d_norm


  ! A simple recursive function
  recursive function factorial(i) result (rs)
    integer :: rs
    integer, intent(in) :: i
    if ( i <= 1 ) then
      rs = 1
    else
      rs = i * factorial(i - 1)
    end if
  end function factorial


  ! This is a useless function
  subroutine example_fn(int_arg, real_arg, str_arg)
    integer, intent(in) :: int_arg
    real(kind=kr), intent(out) :: real_arg
    character(len=*), intent(in) :: str_arg

    type(example_type), pointer :: p
    integer :: n, i, j
    logical :: flag

    flag = .true. ! .true. is not an operator!
    if ( flag .and. flag ) then ! .and. is a pre-defined operator
      print *, "blabla"
    end if

    ! Examples of inquiry functions: allocated, lbound, ubound.
    if ( .not. allocated(many_examples) ) then
      allocate( many_examples(10) )
    end if
    print *, "Lower bound = ", lbound(many_examples, 1)
    print *, "Upper bound = ", ubound(many_examples, 1)

    p => many_examples(5) ! <-- p is a pointer

    ! A strange way to calculate i*i: add the first i odd numbers
    i = 6
    j = 0
    do n = 1, i
      j = j + (2*n - 1)
    end do
    print *, "i*i = ", i*i, j

    real_arg = real(j) ! <-- here the highlighting is not very good:
    ! it is unable to distinguish between this and a definition like:
    !  real(kind=kr) :: a
    deallocate( many_examples )
  end subroutine example_fn

end module module_example


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!                         THIS IS THE MAIN PROGRAM                             !
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
program example
  use module_example

  ! this is another example of use of the 'implicit' keyword
  implicit double precision (a-h,o-z)

  real(kind=kr) :: var_out

  type(point3d) :: &
   a = point3d(0.0_kr, 1.0_kr, 2.0_kr), &
   b = point3d(4.0_kr, 5.0_kr, 6.0_kr)

  print *, "a + b = ", .norm. (a + b)
  print *, "factorial of 5 = ", factorial(5)

  call example_fn(1, var_out, "hello!")

end program example
