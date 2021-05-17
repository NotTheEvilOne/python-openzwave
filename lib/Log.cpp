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

#include <openzwave/platform/Log.h>

#include <pybind11/pybind11.h>

using namespace OpenZWave;
namespace py = pybind11;

/* Python wrapper */

void WrapLog(py::module m) {
    py::enum_<LogLevel>(m, "LogLevel")
        // Disable all logging
        .value("NoneValue", LogLevel_None)
        // These messages should always be shown
        .value("Always", LogLevel_Always)
        // A likely fatal issue in the library
        .value("Fatal", LogLevel_Fatal)
        // A serious issue with the library or the network
        .value("Error", LogLevel_Error)
        // A minor issue from which the library should be able to recover
        .value("Warning", LogLevel_Warning)
        // Something unexpected by the library about which the controlling application should be aware
        .value("Alert", LogLevel_Alert)
        // Everything's working fine...these messages provide streamlined feedback on each message
        .value("Info", LogLevel_Info)
        // Detailed information on the progress of each message
        .value("Detail", LogLevel_Detail)
        // Very detailed information on progress that will create a huge log file quickly. But this level (as others) can be queued and sent to the log only on an error or warning
        .value("Debug", LogLevel_Debug)
        // Will include low-level byte transfers from controller to buffer to application and back
        .value("StreamDetail", LogLevel_StreamDetail)
        // Used only within the log class (uses existing timestamp, etc.)
        .value("Internal", LogLevel_Internal)
    ;
};
