Extra CMake Modules
*******************

Introduction
============

The Extra CMake Modules package, or ECM, adds to the modules provided by CMake,
including ones used by ``find_package()`` to find common software, ones that
can be used directly in ``CMakeLists.txt`` files to perform common tasks and
toolchain files that must be specified on the commandline by the user.

In addition, it provides common build settings used in software produced by the
KDE community.

While the driving force of this module is to reduce duplication in CMake scripts
across KDE software, it is intended to be useful for any software that uses the
CMake build system.


Usage
=====

To use ECM, add the following to your ``CMakeLists.txt``:

.. code-block:: cmake

  find_package(ECM REQUIRED NO_MODULE)
  set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

(note that you may want to append ``${ECM_MODULE_PATH}`` to
``CMAKE_MODULE_PATH`` rather than discarding the existing value).  You can then
just include the modules you require, or use ``find_package()`` as needed.  For
example:

.. code-block:: cmake

  include(ECMInstallIcons)

Developers of KDE software will often want to use the KDE standard settings
provided by ECM; they can do the following:

.. code-block:: cmake

  find_package(ECM REQUIRED NO_MODULE)
  set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
  include(KDEInstallDirs)
  include(KDECompilerSettings)
  include(KDECMakeSettings)

Note that any combination of the above includes can be used if you only want
some of the settings.  Some of the functionality of
:kde-module:`KDECMakeSettings` can also be selectively disabled.


Organization
------------

ECM provides four different types of modules.

* Core modules provide helpful macros for use in project CMake scripts.
  See :manual:`ecm-modules(7)` for more information.
* Find modules extend the functionality of CMake's ``find_package()`` command.
  See :manual:`ecm-find-modules(7)` for more information.
* KDE modules provide common settings for software produced by KDE; much of this
  may also be useful to other projects.  See :manual:`ecm-kde-modules(7)` for
  more information.
* Toolchain files change the build tools and targets used by CMake. See
  :manual:`ecm-toolchains(7)` for more information.

The ``${ECM_MODULE_DIR}``, ``${ECM_FIND_MODULE_DIR}`` and
``${ECM_KDE_MODULE_DIR}`` variables may be used instead of
``${ECM_MODULE_PATH}`` if you only need some of this functionality.


Development
===========

The :manual:`ecm-developer(7)` manual contains more information about
developing for Extra CMake Modules.


License
=======

All code is licensed under the `BSD 3-Clause license`_.

.. _BSD 3-Clause license: https://opensource.org/licenses/BSD-3-Clause


Links
=====

* Home page: https://commits.kde.org/extra-cmake-modules
* Documentation: https://api.kde.org/ecm/
* Mailing list: https://mail.kde.org/mailman/listinfo/kde-buildsystem
* IRC channel: #kde-devel on Freenode
* Git repository: https://commits.kde.org/extra-cmake-modules
