#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
This file is part of **python-openzwave** project https://github.com/OpenZWave/python-openzwave.
    :platform: Unix, Windows, MacOS X

.. moduleauthor:: bibi21000 aka Sébastien GALLET <bibi21000@gmail.com>

License : GPL(v3)

**python-openzwave** is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

**python-openzwave** is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with python-openzwave. If not, see http://www.gnu.org/licenses.

Build process :
- ask user what to do (zmq way in pip)
- or parametrizes it
    --dev : use local sources and cythonize way (for python-openzwave devs, ...)
    --embed : use local sources and cpp file (for third parties packagers, ...)
    --git : download openzwave from git (for geeks)
    --shared : use pkgconfig and cython (for debian devs and common users)
    --pybind : use pybind alternative (not tested)
    --auto (default) : try static, shared and cython, fails if it can't
"""

from os import path
import os
import sys

from setuptools import Extension
from setuptools.command.sdist import sdist

try: import pkgconfig
except ImportError: pkgconfig = None

import pybind11

from pyozw_version import pyozw_version

class ExtBuilder(object):
    """
This class detects settings and generates the configuration to build the C++
code of "python-openzwave".
    """

    def __init__(self):
        """

        """

        self.libopenzwave_dict = { "name": "libopenzwave",
                                   "define_macros": [ ( 'PYOZW_LIB_VERSION', pyozw_version ) ],
                                   "sources": [ ],
                                   "include_dirs": [ ],
                                   "libraries": [ ],
                                   "extra_compile_args": [ ],
                                   "extra_link_args": [ ],
                                   "extra_objects": [ ],
                                   "language": "c++"
                                 }

        self.libopenzwave_cflags = os.environ.get("PYOZW_BUILD_LIB_CFLAGS", "")
        self.libopenzwave_link_flags = os.environ.get("PYOZW_BUILD_LIB_LINK_FLAGS", "")

        monolytic_ozw_dir = os.environ.get("PYOZW_BUILD_MONOLYTIC_OZW_DIR", "")
        self.monolytic_ozw_dir = (path.normpath(monolytic_ozw_dir) if (len(monolytic_ozw_dir) > 0) else "")

        self._configure_pybind11_build()

    def _configure_pybind11_build(self):
        """
Configures the Python compiler settings for a pybind11-based build.
        """

        self._set_default_compiler_flags()

        ext_dict = self.libopenzwave_dict

        ext_dict['include_dirs'].append(pybind11.get_include())
        ext_dict['include_dirs'].append("lib")

        ext_dict['sources'] += [ "lib/LibZWaveException.cpp",
                                 "lib/Driver.cpp",
                                 "lib/Group.cpp",
                                 "lib/Log.cpp",
                                 "lib/Options.cpp",
                                 "lib/Manager.cpp",
                                 "lib/Notification.cpp",
                                 "lib/Node.cpp",
                                 "lib/Values.cpp",
                                 "lib/libopenzwave.cpp"
                               ]

    def _set_default_compiler_flags(self):
        """
Sets generic and platform-specific compiler flags.
        """

        ext_dict = self.libopenzwave_dict
        is_default_build_env_used = True
        is_monolytic_build = (len(self.monolytic_ozw_dir) > 0)
        libopenzwave_cflags = self.libopenzwave_cflags
        libopenzwave_link_flags = self.libopenzwave_link_flags

        if (is_monolytic_build):
            ozw_path = path.join(self.monolytic_ozw_dir, "cpp", "src")

            ext_dict['include_dirs'] += [ ozw_path,
                                          path.join(ozw_path, "platform"),
                                          path.join(ozw_path, "value_classes")
                                        ]
        elif (len(libopenzwave_cflags) + len(libopenzwave_link_flags) < 1):
            if (pkgconfig is None): raise RuntimeError("No custom PYOZW_BUILD_LIB_CFLAGS is defined and pkgconfig is not installed for Python")

            libopenzwave_cflags = pkgconfig.cflags("libopenzwave")
            ext_dict['libraries'].append("openzwave")

        if (len(libopenzwave_cflags) > 0): ext_dict['extra_compile_args'] += libopenzwave_cflags.split(" ")
        if (len(libopenzwave_link_flags) > 0): ext_dict['extra_link_args'] += libopenzwave_link_flags.split(" ")

        if sys.platform.startswith("darwin"):
            is_default_build_env_used = False

            ext_dict['extra_compile_args'] += [ "-stdlib=libc++", "-mmacosx-version-min=10.7" ]
            ext_dict['extra_link_args'] += [ "-framework", "CoreFoundation", "-framework", "IOKit" ]

            if (is_monolytic_build):
                ext_dict['include_dirs'].append(path.join(self.monolytic_ozw_dir, "cpp", "build", "mac"))
                ext_dict['extra_objects'].append(path.join(self.monolytic_ozw_dir, "libopenzwave.a"))
        elif sys.platform.startswith("linux"):
            ext_dict['extra_compile_args'] += [ "-fvisibility=hidden" ]
        elif sys.platform.startswith("freebsd"):
            is_default_build_env_used = False

            ext_dict['extra_compile_args'] += [ "-fvisibility=hidden" ]

            if (is_monolytic_build):
                ext_dict['include_dirs'].append(path.join(self.monolytic_ozw_dir, "cpp", "build", "linux"))
                ext_dict['libraries'] += [ "udev", "stdc++" ]
                ext_dict['extra_objects'].append(path.join(self.monolytic_ozw_dir, "libopenzwave.a"))
        elif sys.platform.startswith("linux"):
            ext_dict['extra_compile_args'] += [ "-fvisibility=hidden" ]
        elif sys.platform.startswith("win"):
            is_default_build_env_used = False

            ext_dict['libraries'] += [ "setupapi", "msvcrt", "ws2_32", "dnsapi" ]

            if (is_monolytic_build):
                ext_dict['include_dirs'].append(path.join(self.monolytic_ozw_dir, "cpp", "build", "windows"))

                ext_dict['extra_objects'].append(path.join(self.monolytic_ozw_dir,
                                                           "cpp",
                                                           "build",
                                                           "windows",
                                                           "vs2010",
                                                           "Release",
                                                           "openzwave.lib"
                                                          )
                                                )

        if (is_default_build_env_used and is_monolytic_build):
            ext_dict['include_dirs'].append(path.join(self.monolytic_ozw_dir, "cpp", "build", "linux"))
            ext_dict['libraries'] += [ "udev", "stdc++", "resolv" ]
            ext_dict['extra_objects'].append(path.join(self.monolytic_ozw_dir, "libopenzwave.a"))

    @property
    def modules_list(self):
        """
Returns the extension modules to be compiled.

:return: (list) List of extension modules
        """

        return [ Extension(**self.libopenzwave_dict) ]

    @property
    def version(self):
        """
Returns the extension modules version to be compiled.

:return: (str) Extension modules version
        """

        return pyozw_version

class SDist(sdist):
    """
python.org: Smart sdist that finds anything supported by revision control.
    """

    def make_distribution(self):
        """
Generates the distribution set.
        """

        self.filelist.append("pyozw_setup.py")
        self.filelist.append("pyozw_version.py")

        sdist.make_distribution(self)

