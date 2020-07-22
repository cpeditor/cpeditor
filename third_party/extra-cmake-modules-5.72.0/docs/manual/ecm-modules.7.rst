.. ecm-manual-description: ECM Modules Reference

ecm-modules(7)
**************

.. only:: html or latex

   .. contents::

Introduction
============

Extra CMake Modules (ECM) provides various modules that provide useful functions
for CMake scripts. ECM actually provides three types of modules that can be
used from CMake scripts: those that extend the functionality of the
``find_package`` command are documented in :manual:`ecm-find-modules(7)`; those
that provide standard settings for software produced by the KDE community are
documented in :manual:`ecm-kde-modules(7)`.  The rest provide macros and
functions for general use by CMake scripts and are documented here.

To use these modules, you need to tell CMake to find the ECM package, and
then add either ``${ECM_MODULE_PATH}`` or ``${ECM_MODULE_DIR}`` to the
``CMAKE_MODULE_PATH`` variable:

.. code-block:: cmake

  find_package(ECM REQUIRED NO_MODULE)
  set(CMAKE_MODULE_PATH ${ECM_MODULE_DIR})

Using ``${ECM_MODULE_PATH}`` will also make the find modules and KDE modules
available.

Note that there are also toolchain modules, documented in
:manual:`ecm-toolchains(7)`, but these are used by users building the software
rather than developers writing CMake scripts.


All Modules
===========

.. toctree::
   :maxdepth: 1
   :glob:

   /module/*

.. only:: man

  See Also
  ========

  :manual:`ecm(7)`, :manual:`ecm-find-modules(7)`, :manual:`ecm-kde-modules(7)`

