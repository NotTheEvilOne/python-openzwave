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
"""

from setuptools import setup, find_packages

from pyozw_setup import ExtBuilder, SDist
from pyozw_version import pyozw_version

ext_builder = ExtBuilder()

setup(name = "python_openzwave",
      version = ext_builder.version,
      zip_safe = False,
      description = "python_openzwave is a python wrapper for the openzwave c++ library.",
      long_description = "A full API to map the ZWave network in Python objects. Look at examples at : https://github.com/OpenZWave/python-openzwave",

      keywords = [ "openzwave", "zwave" ],
      classifiers = [ "Topic :: Home Automation",
                      "Topic :: System :: Hardware",
                      "Topic :: System :: Hardware :: Hardware Drivers",
                      "Operating System :: MacOS :: MacOS X",
                      "Operating System :: Microsoft :: Windows",
                      "Operating System :: POSIX :: Linux",
                      "Operating System :: POSIX :: BSD",
                      "Programming Language :: C++",
                      "Programming Language :: Python :: 2",
                      "Programming Language :: Python :: 3",
                      "License :: OSI Approved :: GNU General Public License v3 or later (GPLv3+)",
                    ],

      author = "Sébastien GALLET aka bibi2100",
      author_email = "bibi21000@gmail.com",
      license = "GPLv3+",
      url = "https://github.com/OpenZWave/python-openzwave",
      download_url = "https://raw.githubusercontent.com/OpenZWave/python-openzwave/master/archives/python_openzwave-{0}.zip".format(pyozw_version),

      platforms = [ "any" ],

      install_requires = "pybind11",
      setup_requires = "pybind11",
      ext_modules = ext_builder.modules_list,

      package_dir = { "": "src" },
      packages = find_packages("src"),

      # Override build_py to first run builder.py over all PAS modules
      cmdclass = { "sdist": SDist }
)
