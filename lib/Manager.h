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

#ifndef __PY_MANAGER_H
#define __PY_MANAGER_H

#include <atomic>
#include <chrono>
#include <future>
#include <mutex>

#include <openzwave/Defs.h>

#include <pybind11/pybind11.h>

using namespace OpenZWave;
namespace py = pybind11;

namespace PyOpenZWave {
    struct CallbackContext {
        py::function callback;
        py::object callbackContext = py::none();

        CallbackContext() { }
        CallbackContext(py::function _callback, py::object _context): callback(_callback), callbackContext(_context) { }
    };

    struct CallbackThreadContext {
        atomic<bool> isPythonCallable;
        atomic<bool> isPythonCalled;

        CallbackThreadContext(): isPythonCallable(true), isPythonCalled(false) { }
        CallbackThreadContext(bool _isPythonCallable, bool _isPythonCalled): isPythonCallable(_isPythonCallable), isPythonCalled(_isPythonCalled) { }
    };

    //! This list holds futures that run longer than OZW_MANAGER_PY_CALLBACK_WAIT_MS
    list<future<void>> callbackBackgroundExecList;
    //! This lock protects the list of futures that run longer than OZW_MANAGER_PY_CALLBACK_WAIT_MS
    recursive_mutex callbackBackgroundExecListLock;
    //! The underlying OZW Manager singleton.
    py::object managerSingleton = py::none();
    //! This lock protects the OZW Manager singleton.
    recursive_timed_mutex managerSingletonLock;
    //! The managerUpdatedTime is used to validate the underlying OpenZWave manager
    chrono::steady_clock::time_point managerUpdatedTime;
    //! Registry of Python notification watchers
    list<CallbackContext*> watcherRegistry;
    //! This lock is used to protect the notification watchers registry
    recursive_timed_mutex watcherRegistryLock;
}

// * = Not verified, use with care and might change without prior notice
enum class COMMAND_CLASS : uint16_t {
    ACTUATOR_MULTILEVEL = 0x78, //*
    ALARM = 0x71,
    ANTITHEFT = 0x5d,
    APPLICATION_CAPABILITY = 0x57,
    APPLICATION_STATUS = COMMAND_CLASS_APPLICATION_STATUS,
    AV_CONTENT_DIRECTORY_MD = 0x95, //*
    AV_RENDERER_STATUS = 0x96, //*
    AV_CONTENT_SEARCH_MD = 0x97, //*
    AV_TAGGING_MD = 0x99, //*
    ASSOCIATION = 0x85,
    ASSOCIATION_COMMAND_CONFIGURATION = 0x9b,
    ASSOCIATION_GRP_INFO = 0x59,
    BARRIER_OPERATOR = 0x66,
    BASIC = COMMAND_CLASS_BASIC,
    BASIC_TARIFF_INFO = 0x36,
    BASIC_WINDOW_COVERING = 0x50,
    BATTERY = 0x80,
    CHIMNEY_FAN = 0x2a, //*
    CENTRAL_SCENE = 0x5b,
    CLIMATE_CONTROL_SCHEDULE = 0x46,
    CLOCK = 0x81,
    COMPOSITE = 0x8d, //*
    CONFIGURATION = 0x70,
    CONTROLLER_REPLICATION = COMMAND_CLASS_CONTROLLER_REPLICATION,
    CRC_16_ENCAP = 0x56,
    DCP_CONFIG = 0x3a,
    DCP_MONITOR = 0x3b,
    DEVICE_RESET_LOCALLY = 0x5a,
    DOOR_LOCK = 0x62,
    DOOR_LOCK_LOGGING = 0x4c,
    ENERGY_PRODUCTION = 0x90,
    ENTRY_CONTROL = 0x6f,
    FIRMWARE_UPDATE_MD = 0x7a,
    GEOGRAPHIC_LOCATION = 0x8c,
    GROUPING_NAME = 0x7b,
    HAIL = COMMAND_CLASS_HAIL,
    HRV_CONTROL = 0x39,
    HRV_STATUS = 0x37,
    HUMIDITY_CONTROL_MODE = 0x6d,
    HUMIDITY_CONTROL_OPERATING_STATE = 0x6e,
    HUMIDITY_CONTROL_SETPOINT = 0x64,
    INDICATOR = 0x87,
    IP_ASSOCIATION = 0x5c,
    IP_CONFIGURATION = 0xe,
    IRRIGATION = 0x6b,
    KICK = 0x79, //*
    LANGUAGE = 0x89,
    LOCK = 0x76,
    MAILBOX = 0x69,
    MANUFACTURER_PROPRIETARY = 0x91,
    MANUFACTURER_SPECIFIC = 0x72,
    MARK = 0xef,
    METER = 0x32,
    METER_PULSE = 0x35,
    METER_TBL_CONFIG = 0x3c,
    METER_TBL_MONITOR = 0x3d,
    METER_TBL_PUSH = 0x3e,
    MTP_WINDOW_COVERING = 0x51,
    MULTI_CHANNEL = 0x60,
    MULTI_CHANNEL_ASSOCIATION = 0x8e,
    MULTI_COMMAND = 0x8f,
    NETWORK_MANAGEMENT_BASIC = 0x4d,
    NETWORK_MANAGEMENT_INCLUSION = 0x34,
    NETWORK_MANAGEMENT_PRIMARY = 0x54,
    NETWORK_MANAGEMENT_PROXY = 0x52,
    NETWORK_STAT = 0x83, //*
    NODE_NAMING = 0x77,
    NON_INTEROPERABLE = 0xf0,
    NOTIFICATION = 0x71,
    NO_OPERATION = 0x0,
    POWERLEVEL = 0x73,
    PREPAYMENT = 0x3f,
    PREPAYMENT_ENCAPSULATION = 0x41,
    PROPRIETARY = 0x88,
    PROTECTION = 0x75,
    RATE_TBL_CONFIG = 0x48,
    RATE_TBL_MONITOR = 0x49,
    REMOTE_ASSOCIATION = 0x7d,
    REMOTE_ASSOCIATION_ACTIVATE = 0x7c,
    SCENE_ACTIVATION = 0x2b,
    SCENE_ACTUATOR_CONF = 0x2c,
    SCENE_CONTROLLER_CONF = 0x2d,
    SCHEDULE = 0x53,
    SCHEDULE_ENTRY_LOCK = 0x4e,
    SCREEN_ATTRIBUTES = 0x93,
    SCREEN_MD = 0x92,
    SECURITY = 0x98,
    SECURITY_SCHEME0_MARK = 0xf100,
    SENSOR_ALARM = 0x9c,
    SENSOR_BINARY = 0x30,
    SENSOR_CONFIGURATION = 0x9e,
    SENSOR_MULTILEVEL = 0x31,
    SILENCE_ALARM = 0x9d,
    SIMPLE_AV_CONTROL = 0x94,
    SUPERVISION = 0x6c,
    SWITCH_ALL = 0x27,
    SWITCH_BINARY = 0x25,
    SWITCH_COLOR = 0x33,
    SWITCH_MULTILEVEL = 0x26,
    SWITCH_TOGGLE_BINARY = 0x28,
    SWITCH_TOGGLE_MULTILEVEL = 0x29,
    TARIFF_TBL_CONFIG = 0x4a,
    TARIFF_TBL_MONITOR = 0x4b,
    THERMOSTAT_FAN_MODE = 0x44,
    THERMOSTAT_FAN_STATE = 0x45,
    THERMOSTAT_HEATING = 0x38, //*
    THERMOSTAT_MODE = 0x40,
    THERMOSTAT_OPERATING_STATE = 0x42, //*
    THERMOSTAT_SETBACK = 0x47,
    THERMOSTAT_SETPOINT = 0x43,
    TIME = 0x8a,
    TIME_PARAMETERS = 0x8b,
    TRANSPORT_SERVICE = 0x55,
    USER_CODE = 0x63,
    VERSION = 0x86,
    WAKE_UP = 0x84,
    WINDOW_COVERING = 0x6a,
    ZIP = 0x23,
    ZIP_6LOWPAN = 0x4f,
    ZIP_ADV_SERVICES = 0x2f, //*
    ZIP_CLIENT = 0x2e, //*
    ZIP_GATEWAY = 0x5f,
    ZIP_NAMING = 0x68,
    ZIP_ND = 0x58,
    ZIP_PORTAL = 0x61,
    ZIP_SERVER = 0x24,
    ZWAVEPLUS_INFO = 0x5e
};

struct NodeClassInformation {
    string className;
    uint8 classVersion;
};

struct SwitchPointData {
    SwitchPointData() { }
    SwitchPointData(const uint8_t hours, const uint8_t minutes, const int8_t setback) : hours(hours), minutes(minutes), setback(setback) { }

    uint8_t hours;
    uint8_t minutes;
    int8_t setback;
};

/* Callbacks and notification watchers */

void ClearCallbackBackgroundExecList();

bool RemoveWatcher(Manager *self, py::function _watcher, py::object _context);

/* Static functions */

COMMAND_CLASS GetCommandClassForId(uint16_t id);

#endif
