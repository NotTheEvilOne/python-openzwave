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

#include <openzwave/Options.h>
#include <openzwave/platform/Log.h>

#include "PythonConstants.h"
#include "Options.h"

using namespace OpenZWave;
namespace py = pybind11;

bool GetOptionAsBool(Options *self, const string &_name) {
    bool result;

    self->GetOptionAsBool(_name, &result);

    return result;
}

int32_t GetOptionAsInt(Options *self, const string &_name) {
    int32_t result;

    self->GetOptionAsInt(_name, &result);

    return result;
}

string GetOptionAsString(Options *self, const string &_name) {
    string result;

    self->GetOptionAsString(_name, &result);

    return result;
}

py::object GetOption(Options *self, const string &_name) {
    Options::OptionType optionType = self->GetOptionType(_name);

    switch(optionType) {
        case Options::OptionType_Bool:
            return py::cast(GetOptionAsBool(self, _name));
        case Options::OptionType_Int:
            return py::cast(GetOptionAsInt(self, _name));
        case Options::OptionType_String:
            return py::cast(GetOptionAsString(self, _name));
        default:
            return py::cast(nullptr);
    }
}

/* Static functions */

py::object CreateOptions(const string &_configPath, const string &_userPath, const string &_commandLine) {
    unique_lock<recursive_timed_mutex> deferredOptionsSingletonLock(PyOpenZWave::optionsSingletonLock, defer_lock);

    if (!deferredOptionsSingletonLock.try_lock_for(chrono::milliseconds(OZW_PY_EXECUTED_TIMEOUT_MS))) {
        throw runtime_error("Python OpenZWave Options singleton is already locked");
    }

    if (PyOpenZWave::optionsSingleton.is_none() || PyOpenZWave::optionsSingleton.ref_count() < 2) {
        PyOpenZWave::optionsSingleton = py::cast(Options::Create(_configPath, _userPath, _commandLine));
    } else {
        Log::Write(LogLevel_Warning, "Python OpenZWave Options::Create() called more than once - continuing anyway");
    }

    return PyOpenZWave::optionsSingleton;
}

void DestroyOptions() {
    unique_lock<recursive_timed_mutex> deferredOptionsSingletonLock(PyOpenZWave::optionsSingletonLock, defer_lock);

    if (!deferredOptionsSingletonLock.try_lock_for(chrono::milliseconds(OZW_PY_EXECUTED_TIMEOUT_MS))) {
        throw runtime_error("Python OpenZWave Options singleton is already locked");
    }

    if (!PyOpenZWave::optionsSingleton.is_none()) {
        // Two references maximum - one for PyOpenZWave::optionsSingleton itself and one for the referencing variable
        if (PyOpenZWave::optionsSingleton.ref_count() > 2) {
            Log::Write(LogLevel_Error, "Python OpenZWave Options::Destroy() detected too many references: %i > 2", PyOpenZWave::optionsSingleton.ref_count());
        } else {
            PyOpenZWave::optionsSingleton = py::none();
            Options::Destroy();
        }
    }
}

py::object GetOptions() {
    unique_lock<recursive_timed_mutex> deferredOptionsSingletonLock(PyOpenZWave::optionsSingletonLock, defer_lock);

    if (!deferredOptionsSingletonLock.try_lock_for(chrono::milliseconds(OZW_PY_EXECUTED_TIMEOUT_MS))) {
        throw runtime_error("Python OpenZWave Options singleton is already locked");
    }

    return PyOpenZWave::optionsSingleton;
}

string GetConfigPath(Options *self) {
    return GetOptionAsString(self, "ConfigPath");
}

/* Python wrapper */

void WrapOptions(py::module m) {
    py::class_<Options, unique_ptr<Options, py::nodelete>> options(m, "Options");

    py::enum_<Options::OptionType>(options, "OptionType")
        .value("Invalid", Options::OptionType_Invalid)
        .value("Bool", Options::OptionType_Bool)
        .value("Int", Options::OptionType_Int)
        .value("String", Options::OptionType_String)
    ;

    options
        .def("lock", &Options::Lock, "Lock the options. Needed to start the manager")
        .def("areLocked", &Options::AreLocked, "Test whether the options have been locked.")
        .def("addOptionBool", &Options::AddOptionBool, "Add a boolean option.")
        .def("addOptionInt", &Options::AddOptionInt, "Add an integer option.")
        .def("addOptionString",
            &Options::AddOptionString,
            py::arg("_name"),
            py::arg("_default"),
            py::arg("_append") = false,
            "Add an option."
        )
        .def("addOption", &Options::AddOptionBool, "Add an option.")
        .def("addOption", &Options::AddOptionInt, "Add an option.")
        .def("addOption",
            &Options::AddOptionString,
            py::arg("_name"),
            py::arg("_default"),
            py::arg("_append") = false,
            "Add an option."
        )
        .def("getOptionAsBool", &GetOptionAsBool, "Retrieve boolean value of an option.")
        .def("getOptionAsInt", &GetOptionAsInt, "Retrieve integer value of an option.")
        .def("getOptionAsString", &GetOptionAsString, "Retrieve string value of an option.")
        .def("getOption", &GetOption, "Retrieve option of a value.")
        .def("getType", &Options::GetOptionType, "Get the type of value stored in an option.")
        .def("getConfigPath", &GetConfigPath, "Retrieve the config path. This directory hold the xml files.")

        // Static methods
        .def_static("create",
            &CreateOptions,
            // Reuse default "config/" directory for underlying required parameter _configPath
            py::arg("config_path") = "config/",
            py::arg("user_path") = "",
            py::arg("cmd_line") = ""
        )
        .def_static("destroy", &DestroyOptions)
        .def_static("get", &GetOptions)
    ;
};
