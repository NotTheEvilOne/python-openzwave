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

#ifndef __PY_NODE_H
#define __PY_NODE_H

#include <openzwave/Node.h>

using namespace OpenZWave;
namespace py = pybind11;

enum class SECURITY_FLAG {
    SecurityFlag_Security = Node::SecurityFlag_Security,
    SecurityFlag_Controller = Node::SecurityFlag_Controller,
    SecurityFlag_SpecificDevice = Node::SecurityFlag_SpecificDevice,
    SecurityFlag_RoutingSlave = Node::SecurityFlag_RoutingSlave,
    SecurityFlag_BeamCapability = Node::SecurityFlag_BeamCapability,
    SecurityFlag_Sensor250ms = Node::SecurityFlag_Sensor250ms,
    SecurityFlag_Sensor1000ms = Node::SecurityFlag_Sensor1000ms,
    SecurityFlag_OptionalFunctionality = Node::SecurityFlag_OptionalFunctionality
};

#endif
