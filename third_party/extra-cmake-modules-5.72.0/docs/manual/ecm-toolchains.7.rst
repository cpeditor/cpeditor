.. ecm-manual-description: ECM Toolchains Reference

ecm-toolchains(7)
*****************

.. only:: html or latex

   .. contents::

Introduction
============

Extra CMake Modules (ECM) provides some toolchain modules. Unlike normal
modules, these are not included directly in projects, but specified with
the ``CMAKE_TOOLCHAIN_FILE`` cache variable on the commandline.


All Modules
===========

.. toctree::
   :maxdepth: 1
   :glob:

   /toolchain/*

.. only:: man

  See Also
  ========

  :manual:`ecm(7)`

