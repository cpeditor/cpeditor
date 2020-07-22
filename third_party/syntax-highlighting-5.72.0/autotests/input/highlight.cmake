# This CMakeLists.txt doesn't do anything useful,
# but it shoudl demonstrate the cmake syntax highlighting
#
# Alex Turbov <i.zaufi@gmail.com>
#

#[[.rst:
Demo
----

This is an **RST** documentation.

::

    # Sample code block
    blah-blah

But we are about to test CMake here ;-)

#]]

cmake_policy(VERSION 3.11)

project(
    Demo VERSION 1.0
    DESCRIPTION "For unit testing purposes"
    # NOTE that particular languages is a separate style
    # to highlight "special" (well known values)
    LANGUAGES C CXX
  )

set(SOME_TRUE_OPTION TRUE)      # `true` value
# `false` value and "internal" variable
set(_ANOTHER_FALSE_OPTION OFF CACHE INTERNAL "Internal option")

#BEGIN Message block
message(FATAL_ERROR "Ordinal message do ${VARIABLE_EXPANSION}")
message(AUTHOR_WARNING "... standard variables have a dedicated style")
message(SEND_ERROR "e.g. ${PROJECT_DESCRIPTION} or ${CMAKE_COMMAND}")
message(
    STATUS [=[
        Raw messages do not do ${VARIABLES_EXPANSION} or \n
        escape symbols highlighting...
    ]=]
)
#END Message block

# ATTENTION Every command highlight only its own named keywords...
# Also, note aliased (most of the time imported) targets higlighted as well
add_library(Foo::foo IMPORTED GLOBAL)
set(KW_HL IMPORTED GLOBAL)      # `IMPORTED` and `GLOBAL` are not highlighted here!

# Properties are separate ("special value") style
set_target_properties(Foo::foo PROPERTIES LOCATION "${FOO_LIBRARY}")

# Generator expressions
target_compile_definitions(
    # NOTE Ok w/ CMake >= 3.11
    Foo::foo
    $<$<PLATFORM_ID:Windows>:WINDOWS_FOO>
    $<$<PLATFORM_ID:Linux>:
        LINUX_FOO
        $<$<BOOL:${_has_foo}>:SOME_FOO_PATH=${PROJECT_BINARY_DIR}/foo>
    >
  )

#[=======================================================================[.rst:
.. cmake:command:: my_fun

*RST* documentation ``can`` refer to :cmake:command:`any_commands` or
:cmake:variable:`variables`...

.. code-block:: cmake
   :caption: **Synopsys**

    my_fun([ANYTHING...])

#]=======================================================================]
function(my_fun)
    # TODO Add implementation
endfunction()

my_fun(
    # Custom functions do not highlight "standard" named args ...
    PUBLIC LOCATION PARENT_SCOPE
    # only some well-known values ...
    smth-NOTFOUND ON
    # and standard variables
    PROJECT_VERSION
    # or substitutions
    $ENV{HOME} OR ${_internal_var_is_grey}
  )

# I dont'recall exactly, but there was some bug with `if`...
if((A AND "${B}") OR C OR (var MATCHES "regex"))
    # Anyway... it is Ok nowadays ;-)

elseif(POLICY CMP066)
    add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cc)
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
            Qt5::Core
            $<$<BOOL:${HAS_FOO}>:Foo::foo>
      )

endif()

# In each function call below, all 3 named parameter lines should apply the same highlighting.
add_custom_command(
    COMMAND true
    COMMAND (true)
    COMMAND true
)
add_custom_target(TargetName
    WORKING_DIRECTORY somedir
    COMMAND (true)
    BYPRODUCTS somefile
)
execute_process(
    COMMAND true
    COMMAND (true)
    COMMAND true
)
add_test(
    NAME sometest
    COMMAND (true)
    WORKING_DIRECTORY somedir
)
