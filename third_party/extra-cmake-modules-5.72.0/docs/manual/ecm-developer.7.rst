.. ecm-manual-description: ECM Developer Reference

ecm-developer(7)
****************

.. only:: html or latex

   .. contents::


Writing Modules
===============

The CMake 3 documentation (and `cmake-developer(7)`_ in particular) has a lot of
useful information about writing CMake modules, including a large section
devoted to find modules. This guide will only highlight things that are
particular to the Extra CMake Modules project.

Most of these are stylistic points. For example, the license header for a module
in ECM should look like::

  #=============================================================================
  # Copyright 20XX Your Name <your.email@example.com>
  #
  # Redistribution and use in source and binary forms, with or without
  # modification, are permitted provided that the following conditions
  # are met:
  #
  # 1. Redistributions of source code must retain the copyright
  #    notice, this list of conditions and the following disclaimer.
  # 2. Redistributions in binary form must reproduce the copyright
  #    notice, this list of conditions and the following disclaimer in the
  #    documentation and/or other materials provided with the distribution.
  # 3. The name of the author may not be used to endorse or promote products
  #    derived from this software without specific prior written permission.
  #
  # THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
  # IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  # OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  # IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
  # INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  # NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  # DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  # THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  # (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
  # THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Functions should be used instead of macros unless there is a good reason not to
(and that reason should be noted in a comment), and lowercase should be used for
macros, functions and commands.

4 spaces is the generally-recommended indent, although there are several files
that use 2 spaces; consistency within a file is more important than consistency
across files.

If in doubt, look at how other modules in Extra CMake Modules are written, and
follow the same pattern.


Find Modules
------------

A good template for find module documentation is::

  #.rst:
  # FindFoo
  # -------
  #
  # Finds the Foo library.
  #
  # This will define the following variables:
  #
  # ``Foo_FOUND``
  #     True if (the requested version of) Foo is available
  # ``Foo_VERSION``
  #     The version of Foo, if it is found
  # ``Foo_LIBRARIES``
  #     This can be passed to target_link_libraries() instead of the ``Foo::Foo``
  #     target
  # ``Foo_INCLUDE_DIRS``
  #     This should be passed to target_include_directories() if the target is not
  #     used for linking
  # ``Foo_DEFINITIONS``
  #     This should be passed to target_compile_options() if the target is not
  #     used for linking
  #
  # If ``Foo_FOUND`` is TRUE, it will also define the following imported target:
  #
  # ``Foo::Foo``
  #     The Foo library
  #
  # In general we recommend using the imported target, as it is easier to use.
  # Bear in mind, however, that if the target is in the link interface of an
  # exported library, it must be made available by the package config file.

Note the use of definition lists for the variables.

Because of the :module:`ECMUseFindModules` module, projects may easily make
local copies of find modules, and may install those copies with their own CMake
project config files. For this reason, find modules should include the full BSD
3-clause license::

  #=============================================================================
  # Copyright 20XX Your Name <your.email@example.com>
  #
  # Redistribution and use in source and binary forms, with or without
  # modification, are permitted provided that the following conditions
  # are met:
  #
  # 1. Redistributions of source code must retain the copyright
  #    notice, this list of conditions and the following disclaimer.
  # 2. Redistributions in binary form must reproduce the copyright
  #    notice, this list of conditions and the following disclaimer in the
  #    documentation and/or other materials provided with the distribution.
  # 3. The name of the author may not be used to endorse or promote products
  #    derived from this software without specific prior written permission.
  #
  # THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
  # IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  # OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  # IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
  # INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  # NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  # DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  # THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  # (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
  # THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  #=============================================================================

Find modules should always provide imported targets in addition to the
traditional variables (like ``Foo_LIBRARIES``, etc).

Unlike find modules shipped with CMake, if the module requires a specific CMake
version it is not enough to warn when the minimum required version is not high
enough: you should also produce an error when the actual CMake version being
used is not high enough. This can be done with:

.. code-block:: cmake

  if(CMAKE_VERSION VERSION_LESS 2.8.12)
      message(FATAL_ERROR "CMake 2.8.12 is required by FindFoo.cmake")
  endif()
  if(CMAKE_MINIMUM_REQUIRED_VERSION VERSION_LESS 2.8.12)
      message(AUTHOR_WARNING "Your project should require at least CMake 2.8.12 to use FindFoo.cmake")
  endif()

The :module:`ECMFindModuleHelpers` module has several useful functions and
macros. For example, it allows you to replace the above version check with:

.. code-block:: cmake

  ecm_find_package_version_check(Foo)

Components
~~~~~~~~~~

Using :module:`ECMFindModuleHelpers`, creating a find module for a library with
several inter-dependent components is reasonably straightforward. After the
documentation, you need to include the module and do the usual version check:

.. code-block:: cmake

  include(ECMFindModuleHelpers)
  ecm_find_package_version_check(Foo)

The important macros are ``ecm_find_package_parse_components`` and
``ecm_find_package_handle_library_components``.  These take a list of
components, and query other variables you provide to find out the information
they require.  The documentation for :module:`ECMFindModuleHelpers` provides
more information, but a simple setup might look like:

.. code-block:: cmake

  set(Foo_known_components Bar Baz)
  set(Foo_Bar_pkg_config "foo-bar")
  set(Foo_Bar_lib "bar")
  set(Foo_Bar_header "foo/bar.h")
  set(Foo_Bar_pkg_config "foo-baz")
  set(Foo_Baz_lib "baz")
  set(Foo_Baz_header "foo/baz.h")

If ``Baz`` depends on ``Bar``, for example, you can specify this with

.. code-block:: cmake

  set(Foo_Baz_component_deps "Bar")

Then call the macros:

.. code-block:: cmake

  ecm_find_package_parse_components(Foo
      RESULT_VAR Foo_components
      KNOWN_COMPONENTS ${Foo_known_components}
  )
  ecm_find_package_handle_library_components(Foo
      COMPONENTS ${Foo_components}
  )

Of course, if your components need unusual handling, you may want to replace
``ecm_find_package_handle_library_components`` with, for example, a ``foreach``
loop over the components (the body of which should implement most of what a
normal find module does, including setting ``Foo_<component>_FOUND``).

At this point, you should set ``Foo_VERSION`` using whatever information you
have available (such as from parsing header files).  Note that
``ecm_find_package_handle_library_components`` will set it to the version
reported by pkg-config of the first component found, but this depends on the
presence of pkg-config files, and the version of a component may not be the same
as the version of the whole package.  After that, finish off with

.. code-block:: cmake

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Foo
      FOUND_VAR
          Foo_FOUND
      REQUIRED_VARS
          Foo_LIBRARIES
      VERSION_VAR
          Foo_VERSION
      HANDLE_COMPONENTS
  )

  include(FeatureSummary)
  set_package_properties(Foo PROPERTIES
      URL "https://www.foo.example.com/"
      DESCRIPTION "A library for doing useful things")


Submitting Modules
==================

Proposed new modules should be submitted using the `KDE Review Board instance`_,
and be assigned to the ``buildsystem`` and ``extracmakemodules`` groups.  You
should be able to point to two separate projects that will make use of the
module.

The mailing list can be found at
https://mail.kde.org/mailman/listinfo/kde-buildsystem\ .


.. _KDE Review Board instance: https://git.reviewboard.kde.org/
.. _cmake-developer(7): https://www.cmake.org/cmake/help/git-master/manual/cmake-developer.7.html
