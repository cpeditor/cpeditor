.. ecm-manual-description: ECM KDE Modules Reference

ecm-kde-modules(7)
******************

.. only:: html or latex

   .. contents::

Introduction
============

Extra CMake Modules (ECM) provides several modules that provide default settings
(like installation directories, compiler flags and other CMake options) aimed at
software produced by the KDE modules; these are documented here. ECM also
provides modules with more general functionality, documented in
:manual:`ecm-modules(7)`, and ones that extend the functionality of the
``find_package`` command, documented in :manual:`ecm-find-modules(7)`.

To use these modules, you need to tell CMake to find the ECM package, and
then add either ``${ECM_MODULE_PATH}`` or ``${ECM_KDE_MODULE_DIR}`` to the
``CMAKE_MODULE_PATH`` variable:

.. code-block:: cmake

  find_package(ECM REQUIRED NO_MODULE)
  set(CMAKE_MODULE_PATH ${ECM_MODULE_DIR})

Using ``${ECM_MODULE_PATH}`` will also make the other types of modules
available.

All KDE Modules
===============

.. toctree::
   :maxdepth: 1
   :glob:

   /kde-module/*

.. only:: man

  See Also
  ========

  :manual:`ecm(7)`, :manual:`ecm-modules(7)`, :manual:`ecm-find-modules(7)`

