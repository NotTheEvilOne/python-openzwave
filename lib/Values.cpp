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

#include <openzwave/value_classes/ValueID.h>

#include <pybind11/pybind11.h>

using namespace OpenZWave;
namespace py = pybind11;

/* Python wrapper */

void WrapValues(py::module m) {
    py::class_<ValueID, unique_ptr<ValueID, py::nodelete>> valueId(m, "ValueID");

    py::enum_<ValueID::ValueGenre>(valueId, "ValueGenre")
        // The 'level' as controlled by basic commands. Usually duplicated by another command class.
        .value("Basic", ValueID::ValueGenre_Basic)
        // Basic values an ordinary user would be interested in.
        .value("User", ValueID::ValueGenre_User)
        // Device-specific configuration parameters. These cannot be automatically discovered via Z-Wave, and are usually described in the user manual instead.
        .value("Config", ValueID::ValueGenre_Config)
        // Values of significance only to users who understand the Z-Wave protocol.
        .value("System", ValueID::ValueGenre_System)
        // A count of the number of genres defined. Not to be used as a genre itself.
        .value("Count", ValueID::ValueGenre_Count)
    ;

    py::enum_<ValueID::ValueType>(valueId, "ValueType")
        // Boolean, true or false
        .value("Bool", ValueID::ValueType_Bool)
        // 8-bit unsigned value
        .value("Byte", ValueID::ValueType_Byte)
        // Represents a non-integer value as a string, to avoid floating point accuracy issues.
        .value("Decimal", ValueID::ValueType_Decimal)
        // 32-bit signed value
        .value("Int", ValueID::ValueType_Int)
        // List from which one item can be selected
        .value("List", ValueID::ValueType_List)
        // Complex type used with the Climate Control Schedule command class
        .value("Schedule", ValueID::ValueType_Schedule)
        // 16-bit signed value
        .value("Short", ValueID::ValueType_Short)
        // Text string
        .value("String", ValueID::ValueType_String)
        // A write-only value that is the equivalent of pressing a button to send a command to a device
        .value("Button", ValueID::ValueType_Button)
        // Used as a list of Bytes
        .value("Raw", ValueID::ValueType_Raw)
    ;

    valueId
        .def("getHomeId", &ValueID::GetHomeId)
        .def("getNodeId", &ValueID::GetNodeId)
        .def("getGenre", &ValueID::GetGenre)
        .def("getCommandClassId", &ValueID::GetCommandClassId)
        .def("getInstance", &ValueID::GetInstance)
        .def("getIndex", &ValueID::GetIndex)
        .def("getType", &ValueID::GetType)
        .def("getId", &ValueID::GetId)
    ;
};
