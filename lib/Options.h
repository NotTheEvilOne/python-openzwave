/*
This file is part of **python-openzwave** project https://github.com/OpenZWave/python-openzwave.

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
*/

#ifndef __PY_OPTIONS_H
#define __PY_OPTIONS_H

#include <mutex>

#include <pybind11/pybind11.h>

#include <openzwave/Defs.h>

using namespace OpenZWave;
namespace py = pybind11;

namespace PyOpenZWave {
    //! The underlying OZW Options singleton.
    py::object optionsSingleton = py::none();
    //! This lock protects the OZW Options singleton.
    recursive_timed_mutex optionsSingletonLock;
}

#endif
