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

#include <openzwave/Group.h>

#include <pybind11/pybind11.h>

using namespace OpenZWave;
namespace py = pybind11;

/* Python wrapper */

void WrapGroup(py::module m) {
    py::class_<Group> group(m, "Group");

    py::class_<InstanceAssociation>(group, "InstanceAssociation")
        .def_readonly("node_id", &InstanceAssociation::m_nodeId)
        .def_readonly("instance", &InstanceAssociation::m_instance)
    ;
};
