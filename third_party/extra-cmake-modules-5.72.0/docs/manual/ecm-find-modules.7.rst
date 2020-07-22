.. ecm-manual-description: ECM Find Modules Reference

ecm-find-modules(7)
*******************

.. only:: html or latex

   .. contents::

Introduction
============

Find modules are used by the CMake ``find_package`` command to search for
packages that do not provide their own CMake package config files. CMake
provides an extensive set of find modules, and Extra CMake Modules (ECM) adds
to that.

To use ECM's find modules, you need to tell CMake to find the ECM package, and
then add either ``${ECM_MODULE_PATH}`` or ``${ECM_FIND_MODULE_DIR}`` to the
``CMAKE_MODULE_PATH`` variable:

.. code-block:: cmake

  find_package(ECM REQUIRED NO_MODULE)
  set(CMAKE_MODULE_PATH ${ECM_FIND_MODULE_DIR})

Using ``${ECM_MODULE_PATH}`` will also make the modules intended for direct use
by CMake scripts available (see :manual:`ecm-modules(7)` and
:manual:`ecm-kde-modules(7)`).

You can also make local copies of find modules using the
``ecm_use_find_modules`` function from :module:`ECMUseFindModules`, which is
automatically included when ECM is found:

.. code-block:: cmake

  find_package(ECM REQUIRED NO_MODULE)
  ecm_use_find_modules(
      DIR "${CMAKE_BINARY_DIR}/cmake"
      MODULES FindEGL.cmake
  )
  set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/cmake")

This allows selective use of ECM's find modules, and the NO_OVERRIDE argument
can be used to ensure that if CMake ships its own version of that find module,
it will be used instead.


All Find Modules
================

.. toctree::
   :maxdepth: 1
   :glob:

   /find-module/*

.. only:: man

  See Also
  ========

  :manual:`ecm(7)`, :manual:`ecm-modules(7)`, :manual:`ecm-kde-modules(7)`

