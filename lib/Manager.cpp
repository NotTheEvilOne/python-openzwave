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

#include <thread>

#include <openzwave/Driver.h>
#include <openzwave/Group.h>
#include <openzwave/Manager.h>
#include <openzwave/Notification.h>
#include <openzwave/Options.h>
#include <openzwave/platform/Log.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "Manager.h"
#include "PythonConstants.h"

/* Drivers */

bool RemoveDriver(Manager *self, const string &_controllerPath) {
    py::gil_scoped_release pyOuterGil;

    bool result = self->RemoveDriver(_controllerPath);
    ClearCallbackBackgroundExecList();

    return result;
}

/* Statistics */

Node::NodeData GetNodeStatistics(Manager *self, const uint32 _homeId, const uint8 _nodeId) {
    Node::NodeData result;

    self->GetNodeStatistics(_homeId, _nodeId, &result);

    return result;
}

Driver::DriverData GetDriverStatistics(Manager *self, const uint32 _homeId) {
    Driver::DriverData result;

    self->GetDriverStatistics(_homeId, &result);

    return result;
}

/* Nodes */

py::set GetNodeNeighborsAsPySet(Manager *self, const uint32 _homeId, const uint8 _nodeId) {
    uint8_t** dbuf = (uint8_t**)malloc(sizeof(uint8_t) * NUM_NODE_BITFIELD_BYTES);
    py::set result;

    uint32_t count = self->GetNodeNeighbors(_homeId, _nodeId, dbuf);

    if (!count) {
        free(dbuf);
        return result;
    }

    uint8_t* p = dbuf[0];

    try {
        for(uint32_t i = 0; i < count; i++) {
            result.add(py::cast(p[0]));
            p++;
        }
    } catch (...) {
        free(dbuf);
        throw;
    }

    free(dbuf);

    return result;
}

py::object GetNodeClassInformation(Manager *self, const uint32 _homeId, const uint8 _nodeId, COMMAND_CLASS _commandClassId) {
    NodeClassInformation result;

    string className;
    uint8 classVersion;
    bool found = self->GetNodeClassInformation(_homeId, _nodeId, static_cast<uint8_t>(_commandClassId), &className, &classVersion);

    if (!found) {
        return py::none();
    }

    result.className = className;
    result.classVersion = classVersion;

    return py::cast(result);
}

Node::QueryStage GetNodeQueryStageCode(Manager *self, const uint32 _homeId, const uint8 _nodeId) {
    Node::QueryStage result;

    string queryStage = self->GetNodeQueryStage(_homeId, _nodeId);

    if (queryStage == "None") {
        result = Node::QueryStage::QueryStage_None;
    } else if (queryStage == "ProtocolInfo") {
        result = Node::QueryStage::QueryStage_ProtocolInfo;
    } else if (queryStage == "Probe") {
        result = Node::QueryStage::QueryStage_Probe;
    } else if (queryStage == "WakeUp") {
        result = Node::QueryStage::QueryStage_WakeUp;
    } else if (queryStage == "ManufacturerSpecific1") {
        result = Node::QueryStage::QueryStage_ManufacturerSpecific1;
    } else if (queryStage == "NodeInfo") {
        result = Node::QueryStage::QueryStage_NodeInfo;
    } else if (queryStage == "NodePlusInfo") {
        result = Node::QueryStage::QueryStage_NodePlusInfo;
    } else if (queryStage == "SecurityReport") {
        result = Node::QueryStage::QueryStage_SecurityReport;
    } else if (queryStage == "ManufacturerSpecific2") {
        result = Node::QueryStage::QueryStage_ManufacturerSpecific2;
    } else if (queryStage == "Versions") {
        result = Node::QueryStage::QueryStage_Versions;
    } else if (queryStage == "Instances") {
        result = Node::QueryStage::QueryStage_Instances;
    } else if (queryStage == "Static") {
        result = Node::QueryStage::QueryStage_Static;
    } else if (queryStage == "CacheLoad") {
        result = Node::QueryStage::QueryStage_CacheLoad;
    } else if (queryStage == "Associations") {
        result = Node::QueryStage::QueryStage_Associations;
    } else if (queryStage == "Neighbors") {
        result = Node::QueryStage::QueryStage_Neighbors;
    } else if (queryStage == "Session") {
        result = Node::QueryStage::QueryStage_Session;
    } else if (queryStage == "Dynamic") {
        result = Node::QueryStage::QueryStage_Dynamic;
    } else if (queryStage == "Configuration") {
        result = Node::QueryStage::QueryStage_Configuration;
    } else if (queryStage == "Complete") {
        result = Node::QueryStage::QueryStage_Complete;
    } else {
        throw runtime_error("Raised unsupported QueryStage");
    }

    return result;
}

/* ValueID */

ValueID::ValueType GetValueType(Manager *self, const ValueID &_id) {
    return _id.GetType();
}

bool GetValueAsBool(Manager *self, const ValueID &_id) {
    bool result;

    self->GetValueAsBool(_id, &result);

    return result;
}

uint8_t GetValueAsByte(Manager *self, const ValueID &_id) {
    uint8_t result;

    self->GetValueAsByte(_id, &result);

    return result;
}

float GetValueAsFloat(Manager *self, const ValueID &_id) {
    float result;

    self->GetValueAsFloat(_id, &result);

    return result;
}

int32_t GetValueAsInt(Manager *self, const ValueID &_id) {
    int32_t result;

    self->GetValueAsInt(_id, &result);

    return result;
}

int16_t GetValueAsShort(Manager *self, const ValueID &_id) {
    int16_t result;

    self->GetValueAsShort(_id, &result);

    return result;
}

uint8_t* GetValueAsRaw(Manager *self, const ValueID &_id) {
    uint8_t* result;
    uint8_t size;

    self->GetValueAsRaw(_id, &result, &size);

    return result;
}

string GetValueAsString(Manager *self, const ValueID &_id) {
    string result;

    self->GetValueAsString(_id, &result);

    return result;
}

string GetValueListSelectionItem(Manager *self, const ValueID &_id) {
    string result;

    self->GetValueListSelection(_id, &result);

    return result;
}

int32_t GetValueListSelectionValue(Manager *self, const ValueID &_id) {
    int32_t result;

    self->GetValueListSelection(_id, &result);

    return result;
}

vector<string> GetValueListItems(Manager *self, const ValueID &_id) {
    vector<string> result;

    self->GetValueListItems(_id, &result);

    return result;
}

vector<int32_t> GetValueListValues(Manager *self, const ValueID &_id) {
    vector<int32_t> result;

    self->GetValueListValues(_id, &result);

    return result;
}

py::object GetValue(Manager *self, const ValueID &_id) {
    ValueID::ValueType valueType = _id.GetType();

    switch(valueType) {
        case ValueID::ValueType::ValueType_Bool:
            return py::cast(GetValueAsBool(self, _id));
        case ValueID::ValueType::ValueType_Byte:
            return py::cast(GetValueAsByte(self, _id));
        case ValueID::ValueType::ValueType_Decimal:
            return py::cast(GetValueAsFloat(self, _id));
        case ValueID::ValueType::ValueType_Int:
            return py::cast(GetValueAsInt(self, _id));
        case ValueID::ValueType::ValueType_List:
            return py::cast(GetValueListSelectionItem(self, _id));
        case ValueID::ValueType::ValueType_Schedule:
            return py::cast(GetValueAsString(self, _id));
        case ValueID::ValueType::ValueType_Short:
            return py::cast(GetValueAsShort(self, _id));
        case ValueID::ValueType::ValueType_Button:
            return py::cast(GetValueAsBool(self, _id));
        case ValueID::ValueType::ValueType_Raw:
            return py::cast(GetValueAsRaw(self, _id));
        default:
            return py::cast(GetValueAsString(self, _id));
    }
}

ValueID::ValueGenre GetValueGenre(Manager *self, const ValueID &_id) {
    return _id.GetGenre();
}

COMMAND_CLASS GetValueCommandClass(Manager *self, const ValueID &_id) {
    return GetCommandClassForId(_id.GetCommandClassId());
}

uint8_t GetValueInstance(Manager *self, const ValueID &_id) {
    return _id.GetInstance();
}

uint8_t GetValueIndex(Manager *self, const ValueID &_id) {
    return _id.GetIndex();
}

uint8_t GetValueFloatPrecision(Manager *self, const ValueID &_id) {
    uint8_t result;

    self->GetValueFloatPrecision(_id, &result);

    return result;
}

/* SwitchPoints */

uint8 SetSwitchPoint(Manager *self, const ValueID &_id, SwitchPointData _data) {
    return self->SetSwitchPoint(_id, _data.hours, _data.minutes, _data.setback);
}

py::object GetSwitchPoint(Manager *self, const ValueID &_id, const uint8 _idx) {
    uint8_t o_hours;
    uint8_t o_minutes;
    int8_t o_setback;
    SwitchPointData result;

    bool found = self->GetSwitchPoint(_id, _idx, &o_hours, &o_minutes, &o_setback);

    if (!found) {
        return py::none();
    }

    result.hours = o_hours;
    result.minutes = o_minutes;
    result.setback = o_setback;

    return py::cast(result);
}

/* Associations */

py::set GetAssociationsAsPySet(Manager *self, const uint32 _homeId, const uint8 _nodeId, const uint8 _groupIdx) {
    py::set result;
    uint32_t size = self->GetMaxAssociations(_homeId, _nodeId, _groupIdx);

    uint8_t** dbuf = (uint8_t**)malloc(sizeof(uint8_t) * size);

    uint32_t count = self->GetAssociations(_homeId, _nodeId, _groupIdx, dbuf);

    if (!count) {
        free(dbuf);
        return result;
    }

    uint8_t* p = dbuf[0];

    try {
        for(uint32_t i = 0; i < count; i++) {
            result.add(py::cast(p[0]));
            p++;
        }
    } catch (...) {
        free(dbuf);
        throw;
    }

    free(dbuf);

    return result;
}

py::set GetAssociationsInstancesAsPySet(Manager *self, const uint32 _homeId, const uint8 _nodeId, const uint8 _groupIdx) {
    py::set result;
    uint32_t size = self->GetMaxAssociations(_homeId, _nodeId, _groupIdx);

    InstanceAssociation** dbuf = (InstanceAssociation**)malloc(sizeof(InstanceAssociation) * size);

    uint32_t count = self->GetAssociations(_homeId, _nodeId, _groupIdx, dbuf);

    if (!count) {
        free(dbuf);
        return result;
    }

    InstanceAssociation* p = dbuf[0];

    try {
        for(uint32_t i = 0; i < count; i++) {
            result.add(py::cast(p[0]));
            p++;
        }
    } catch (...) {
        free(dbuf);
        throw;
    }

    free(dbuf);

    return result;
}

/* Callbacks and notification watchers */

void ClearCallbackBackgroundExecList() {
    lock_guard<recursive_mutex> lock(PyOpenZWave::callbackBackgroundExecListLock);

    list<future<void>>::iterator it = PyOpenZWave::callbackBackgroundExecList.begin();

    while (it != PyOpenZWave::callbackBackgroundExecList.end()) {
        if ((!(*it).valid()) || (*it).wait_for(chrono::nanoseconds(0)) == future_status::ready) {
            it = PyOpenZWave::callbackBackgroundExecList.erase(it);
        } else {
            ++it;
        }
    }
}

void OnNotification(const Notification *_pNotification, void *_context) {
    shared_ptr<PyOpenZWave::CallbackThreadContext> callbackThreadContext = make_shared<PyOpenZWave::CallbackThreadContext>();
    PyOpenZWave::CallbackContext context = *static_cast<PyOpenZWave::CallbackContext*>(_context);
    unique_lock<recursive_mutex> deferredCallbackBackgroundExecListLock(PyOpenZWave::callbackBackgroundExecListLock, defer_lock);

    future<void> onNotificationExec = async(launch::async, [callbackThreadContext, context, &_pNotification]() {
        unique_lock<recursive_timed_mutex> managerSingletonLock(PyOpenZWave::managerSingletonLock);
        chrono::steady_clock::time_point managerUpdatedTime(PyOpenZWave::managerUpdatedTime);
        managerSingletonLock.unlock();

        try {
            py::gil_scoped_acquire pyGil;

            if (callbackThreadContext->isPythonCallable.load() && _pNotification) {
                managerSingletonLock.lock();

                if (managerUpdatedTime == PyOpenZWave::managerUpdatedTime) {
                    managerSingletonLock.unlock();

                    py::object pyNotification = py::cast(_pNotification);

                    py::dict kwargs = py::dict();

                    if (context.callbackContext && (!context.callbackContext.is_none())) {
                        kwargs[py::str("context")] = context.callbackContext;
                    }

                    callbackThreadContext->isPythonCalled = true;
                    context.callback(pyNotification, **kwargs);
                } else {
                    Log::Write(LogLevel_Error, "Python OpenZWave notification execution aborted as it has become invalid");
                }
            } else {
                Log::Write(LogLevel_Error, "Python OpenZWave notification execution aborted as it has been canceled");
            }
        } catch (...) {
            Log::Write(LogLevel_Error, "Python OpenZWave notification execution caused an unhandled exception");
        }
    });

    ClearCallbackBackgroundExecList();

    if (onNotificationExec.wait_for(chrono::milliseconds(OZW_PY_CALLBACK_WAIT_MS)) == future_status::timeout) {
        if (callbackThreadContext->isPythonCalled.load()) {
            Log::Write(LogLevel_Warning, "Python OpenZWave notification execution takes too long - Python caused deadlock possible");
        } else {
            callbackThreadContext->isPythonCallable = false;

            deferredCallbackBackgroundExecListLock.lock();
            PyOpenZWave::callbackBackgroundExecList.push_back(move(onNotificationExec));
        }
    }
}

PyOpenZWave::CallbackContext *GetWatcher(py::function watcher, py::object _context = py::none()) {
    unique_lock<recursive_timed_mutex> deferredLock(PyOpenZWave::watcherRegistryLock, defer_lock);
    PyOpenZWave::CallbackContext* result = nullptr;
    py::handle watcherCallbackFunction = py::detail::get_function(watcher.ptr());
    PyOpenZWave::CallbackContext* watcherContext;

    if (!deferredLock.try_lock_for(chrono::milliseconds(OZW_PY_EXECUTED_TIMEOUT_MS))) {
        throw runtime_error("Python OpenZWave watcher registry is already locked");
    }

    for(list<PyOpenZWave::CallbackContext*>::iterator it = PyOpenZWave::watcherRegistry.begin(); it != PyOpenZWave::watcherRegistry.end(); ++it) {
        watcherContext = *it;

        if (py::detail::get_function(watcherContext->callback.ptr()).is(watcherCallbackFunction) && watcherContext->callbackContext.is(_context)) {
            result = *it;
            break;
        }
    }

    return result;
}

bool AddWatcher(Manager *self, py::function _watcher, py::object _context) {
    unique_lock<recursive_timed_mutex> deferredLock(PyOpenZWave::watcherRegistryLock, defer_lock);
    chrono::steady_clock::time_point managerUpdatedTime(PyOpenZWave::managerUpdatedTime);

    if (!deferredLock.try_lock_for(chrono::milliseconds(OZW_PY_EXECUTED_TIMEOUT_MS))) {
        throw runtime_error("Python OpenZWave watcher registry is already locked");
    }

    if (!_watcher || _watcher.is_none()) {
        throw py::value_error("Python OpenZWave watcher object can not be None");
    }

    if (GetWatcher(_watcher, _context) != nullptr) {
        return true;
    }

    if (managerUpdatedTime != PyOpenZWave::managerUpdatedTime) {
        Log::Write(LogLevel_Error, "Python OpenZWave manager singleton changed - threading issue detected");
        return false;
    }

    PyOpenZWave::CallbackContext* watcherContext = new PyOpenZWave::CallbackContext(_watcher, _context);

    PyOpenZWave::watcherRegistry.push_back(watcherContext);

    return self->AddWatcher(OnNotification, static_cast<void*>(watcherContext));
}

bool RemoveWatcher(Manager *self, py::function _watcher, py::object _context) {
    unique_lock<recursive_timed_mutex> deferredWatcherRegistryLock(PyOpenZWave::watcherRegistryLock, defer_lock);
    chrono::steady_clock::time_point managerUpdatedTime(PyOpenZWave::managerUpdatedTime);
    bool result = false;
    py::handle watcherCallbackFunction = py::detail::get_function(_watcher.ptr());
    PyOpenZWave::CallbackContext* watcherContext;

    if (!deferredWatcherRegistryLock.try_lock_for(chrono::milliseconds(OZW_PY_EXECUTED_TIMEOUT_MS))) {
        throw runtime_error("Python OpenZWave watcher registry is already locked");
    }

    list<PyOpenZWave::CallbackContext*>::iterator it = PyOpenZWave::watcherRegistry.begin();

    while (it != PyOpenZWave::watcherRegistry.end()) {
        watcherContext = *it;

        if (py::detail::get_function(watcherContext->callback.ptr()).is(watcherCallbackFunction) && watcherContext->callbackContext.is(_context)) {
            if (managerUpdatedTime == PyOpenZWave::managerUpdatedTime) {
                py::gil_scoped_release pyOuterGil;

                result = self->RemoveWatcher(OnNotification, static_cast<void*>(watcherContext));

                PyOpenZWave::watcherRegistry.erase(it);
            }

            break;
        } else {
            ++it;
        }
    }

    return result;
}

/* Scenes */

py::set GetAllScenesAsPySet(Manager *self) {
    py::set result;
    uint32_t size = self->GetNumScenes();

    uint8_t** dbuf = (uint8_t**)malloc(sizeof(uint8_t) * size);

    uint32_t count = self->GetAllScenes(dbuf);

    if (!count) {
        free(dbuf);
        return result;
    }

    uint8_t* p = dbuf[0];

    try {
        for(uint32_t i = 0; i < count; i++) {
            result.add(py::cast(p[0]));
            p++;
        }
    } catch (...) {
        free(dbuf);
        throw;
    }

    free(dbuf);

    return result;
}

bool SceneGetValueAsBool(Manager *self, const uint8 &_sceneId, const ValueID &_id) {
    bool result;

    self->SceneGetValueAsBool(_sceneId, _id, &result);

    return result;
}

uint8_t SceneGetValueAsByte(Manager *self, const uint8 &_sceneId, const ValueID &_id) {
    uint8_t result;

    self->SceneGetValueAsByte(_sceneId, _id, &result);

    return result;
}

float SceneGetValueAsFloat(Manager *self, const uint8 &_sceneId, const ValueID &_id) {
    float result;

    self->SceneGetValueAsFloat(_sceneId, _id, &result);

    return result;
}

int32_t SceneGetValueAsInt(Manager *self, const uint8 &_sceneId, const ValueID &_id) {
    int32_t result;

    self->SceneGetValueAsInt(_sceneId, _id, &result);

    return result;
}

int16_t SceneGetValueAsShort(Manager *self, const uint8 &_sceneId, const ValueID &_id) {
    int16_t result;

    self->SceneGetValueAsShort(_sceneId, _id, &result);

    return result;
}

string SceneGetValueAsString(Manager *self, const uint8 &_sceneId, const ValueID &_id) {
    string result;

    self->SceneGetValueAsString(_sceneId, _id, &result);

    return result;
}

string SceneGetValueListSelectionItem(Manager *self, const uint8 &_sceneId, const ValueID &_id) {
    string result;

    self->SceneGetValueListSelection(_sceneId, _id, &result);

    return result;
}

int32_t SceneGetValueListSelectionValue(Manager *self, const uint8 &_sceneId, const ValueID &_id) {
    int32_t result;

    self->SceneGetValueListSelection(_sceneId, _id, &result);

    return result;
}

py::dict SceneGetValues(Manager *self, const uint8 &_sceneId) {
    vector<ValueID> values;
    py::dict result;

    if (!self->SceneGetValues(_sceneId, &values)) {
        return py::none();
    }

    py::object value;

    for(vector<ValueID>::iterator it = values.begin() ; it != values.end(); ++it) {
        ValueID valueID = *it;
        ValueID::ValueType valueType = valueID.GetType();

        switch(valueType) {
            case ValueID::ValueType::ValueType_Bool:
                value = py::cast(SceneGetValueAsBool(self, _sceneId, valueID));
            case ValueID::ValueType::ValueType_Byte:
                value = py::cast(SceneGetValueAsByte(self, _sceneId, valueID));
            case ValueID::ValueType::ValueType_Decimal:
                value = py::cast(SceneGetValueAsFloat(self, _sceneId, valueID));
            case ValueID::ValueType::ValueType_Int:
                value = py::cast(SceneGetValueAsInt(self, _sceneId, valueID));
            case ValueID::ValueType::ValueType_List:
                value = py::cast(SceneGetValueListSelectionItem(self, _sceneId, valueID));
            case ValueID::ValueType::ValueType_Schedule:
                value = py::cast(SceneGetValueAsString(self, _sceneId, valueID));
            case ValueID::ValueType::ValueType_Short:
                value = py::cast(SceneGetValueAsShort(self, _sceneId, valueID));
            case ValueID::ValueType::ValueType_Button:
                value = py::cast(SceneGetValueAsBool(self, _sceneId, valueID));
            default:
                value = py::cast(SceneGetValueAsString(self, _sceneId, valueID));
        }

        result[py::int_(valueID.GetId())] = value;
    }

    return result;
}

/* Static functions */

py::object CreateManager() {
    unique_lock<recursive_timed_mutex> deferredManagerSingletonLock(PyOpenZWave::managerSingletonLock, defer_lock);
    unique_lock<recursive_timed_mutex> deferredWatcherRegistryLock(PyOpenZWave::watcherRegistryLock, defer_lock);

    if (!deferredManagerSingletonLock.try_lock_for(chrono::milliseconds(OZW_PY_EXECUTED_TIMEOUT_MS))) {
        throw runtime_error("Python OpenZWave Manager singleton is already locked");
    }

    if (!deferredWatcherRegistryLock.try_lock_for(chrono::milliseconds(OZW_PY_EXECUTED_TIMEOUT_MS))) {
        throw runtime_error("Python OpenZWave watcher registry is already locked");
    }

    if (PyOpenZWave::managerSingleton.is_none() || PyOpenZWave::managerSingleton.ref_count() < 2) {
        PyOpenZWave::managerSingleton = py::cast(Manager::Create());
        PyOpenZWave::managerUpdatedTime = chrono::steady_clock::now();
    } else {
        Log::Write(LogLevel_Warning, "Python OpenZWave Manager::Create() called more than once - continuing anyway");
    }

    return PyOpenZWave::managerSingleton;
}

void DestroyManager() {
    unique_lock<recursive_timed_mutex> deferredManagerSingletonLock(PyOpenZWave::managerSingletonLock, defer_lock);
    unique_lock<recursive_timed_mutex> deferredWatcherRegistryLock(PyOpenZWave::watcherRegistryLock, defer_lock);
    bool isWatcherRegistryLocked;

    if (!deferredManagerSingletonLock.try_lock_for(chrono::milliseconds(OZW_PY_EXECUTED_TIMEOUT_MS))) {
        throw runtime_error("Python OpenZWave Manager singleton is already locked");
    }

    isWatcherRegistryLocked = deferredWatcherRegistryLock.try_lock_for(chrono::milliseconds(OZW_PY_EXECUTED_TIMEOUT_MS));

    if (!isWatcherRegistryLocked) {
        Log::Write(LogLevel_Warning, "Python OpenZWave watcher registry is already locked - continuing anyway");
    }

    if (!PyOpenZWave::watcherRegistry.empty()) {
        list<PyOpenZWave::CallbackContext*>::iterator it = PyOpenZWave::watcherRegistry.begin();
        PyOpenZWave::CallbackContext* watcherContext;

        while (it != PyOpenZWave::watcherRegistry.end()) {
            it = PyOpenZWave::watcherRegistry.erase(it);
        }
    }

    if (!PyOpenZWave::managerSingleton.is_none()) {
        unique_lock<recursive_mutex> deferredCallbackBackgroundExecListLock(PyOpenZWave::callbackBackgroundExecListLock, defer_lock);

        // Two references maximum - one for PyOpenZWave::managerSingleton itself and one for the referencing variable
        if (PyOpenZWave::managerSingleton.ref_count() > 2) {
            Log::Write(LogLevel_Error, "Python OpenZWave Manager::Destroy() detected too many references: %i > 2", PyOpenZWave::managerSingleton.ref_count());
        } else {
            PyOpenZWave::managerSingleton = py::none();
            PyOpenZWave::managerUpdatedTime = chrono::steady_clock::now();

            Manager::Destroy();
        }

        deferredCallbackBackgroundExecListLock.lock();

        if (!PyOpenZWave::callbackBackgroundExecList.empty()) {
            PyOpenZWave::callbackBackgroundExecList.clear();
        }
    }
}

py::object GetManager() {
    unique_lock<recursive_timed_mutex> deferredManagerSingletonLock(PyOpenZWave::managerSingletonLock, defer_lock);

    if (!deferredManagerSingletonLock.try_lock_for(chrono::milliseconds(OZW_PY_EXECUTED_TIMEOUT_MS))) {
        throw runtime_error("Python OpenZWave Manager singleton is already locked");
    }

    return PyOpenZWave::managerSingleton;
}

string GetPythonLibraryVersion() {
    char prependedText[] = "python-openzwave version ";
    char *result = new char[strlen(prependedText) + strlen(STR(PYOZW_LIB_VERSION))];
    sprintf(result, "%s%s", prependedText, STR(PYOZW_LIB_VERSION));

    return result;
}

string GetPythonLibraryVersionNumber() {
    return STR(PYOZW_LIB_VERSION);
}

COMMAND_CLASS GetCommandClassForId(uint16_t id) {
    return static_cast<COMMAND_CLASS>(id);
}

/* Python wrapper */

void WrapManager(py::module m) {
    py::class_<Manager, unique_ptr<Manager, py::nodelete>> manager(m, "Manager");

    py::enum_<COMMAND_CLASS>(manager, "COMMAND_CLASS")
        .value("ACTUATOR_MULTILEVEL", COMMAND_CLASS::ACTUATOR_MULTILEVEL)
        .value("ALARM", COMMAND_CLASS::ALARM)
        .value("ANTITHEFT", COMMAND_CLASS::ANTITHEFT)
        .value("APPLICATION_CAPABILITY", COMMAND_CLASS::APPLICATION_CAPABILITY)
        .value("APPLICATION_STATUS", COMMAND_CLASS::APPLICATION_STATUS)
        .value("AV_CONTENT_DIRECTORY_MD", COMMAND_CLASS::AV_CONTENT_DIRECTORY_MD)
        .value("AV_RENDERER_STATUS", COMMAND_CLASS::AV_RENDERER_STATUS)
        .value("AV_CONTENT_SEARCH_MD", COMMAND_CLASS::AV_CONTENT_SEARCH_MD)
        .value("AV_TAGGING_MD", COMMAND_CLASS::AV_TAGGING_MD)
        .value("ASSOCIATION", COMMAND_CLASS::ASSOCIATION)
        .value("ASSOCIATION_COMMAND_CONFIGURATION", COMMAND_CLASS::ASSOCIATION_COMMAND_CONFIGURATION)
        .value("ASSOCIATION_GRP_INFO", COMMAND_CLASS::ASSOCIATION_GRP_INFO)
        .value("BARRIER_OPERATOR", COMMAND_CLASS::BARRIER_OPERATOR)
        .value("BASIC", COMMAND_CLASS::BASIC)
        .value("BASIC_TARIFF_INFO", COMMAND_CLASS::BASIC_TARIFF_INFO)
        .value("BASIC_WINDOW_COVERING", COMMAND_CLASS::BASIC_WINDOW_COVERING)
        .value("BATTERY", COMMAND_CLASS::BATTERY)
        .value("CHIMNEY_FAN", COMMAND_CLASS::CHIMNEY_FAN)
        .value("CENTRAL_SCENE", COMMAND_CLASS::CENTRAL_SCENE)
        .value("CLIMATE_CONTROL_SCHEDULE", COMMAND_CLASS::CLIMATE_CONTROL_SCHEDULE)
        .value("CLOCK", COMMAND_CLASS::CLOCK)
        .value("COMPOSITE", COMMAND_CLASS::COMPOSITE)
        .value("CONFIGURATION", COMMAND_CLASS::CONFIGURATION)
        .value("CONTROLLER_REPLICATION", COMMAND_CLASS::CONTROLLER_REPLICATION)
        .value("CRC_16_ENCAP", COMMAND_CLASS::CRC_16_ENCAP)
        .value("DCP_CONFIG", COMMAND_CLASS::DCP_CONFIG)
        .value("DCP_MONITOR", COMMAND_CLASS::DCP_MONITOR)
        .value("DEVICE_RESET_LOCALLY", COMMAND_CLASS::DEVICE_RESET_LOCALLY)
        .value("DOOR_LOCK", COMMAND_CLASS::DOOR_LOCK)
        .value("DOOR_LOCK_LOGGING", COMMAND_CLASS::DOOR_LOCK_LOGGING)
        .value("ENERGY_PRODUCTION", COMMAND_CLASS::ENERGY_PRODUCTION)
        .value("ENTRY_CONTROL", COMMAND_CLASS::ENTRY_CONTROL)
        .value("FIRMWARE_UPDATE_MD", COMMAND_CLASS::FIRMWARE_UPDATE_MD)
        .value("GEOGRAPHIC_LOCATION", COMMAND_CLASS::GEOGRAPHIC_LOCATION)
        .value("GROUPING_NAME", COMMAND_CLASS::GROUPING_NAME)
        .value("HAIL", COMMAND_CLASS::HAIL)
        .value("HRV_CONTROL", COMMAND_CLASS::HRV_CONTROL)
        .value("HRV_STATUS", COMMAND_CLASS::HRV_STATUS)
        .value("HUMIDITY_CONTROL_MODE", COMMAND_CLASS::HUMIDITY_CONTROL_MODE)
        .value("HUMIDITY_CONTROL_OPERATING_STATE", COMMAND_CLASS::HUMIDITY_CONTROL_OPERATING_STATE)
        .value("HUMIDITY_CONTROL_SETPOINT", COMMAND_CLASS::HUMIDITY_CONTROL_SETPOINT)
        .value("INDICATOR", COMMAND_CLASS::INDICATOR)
        .value("IP_ASSOCIATION", COMMAND_CLASS::IP_ASSOCIATION)
        .value("IP_CONFIGURATION", COMMAND_CLASS::IP_CONFIGURATION)
        .value("IRRIGATION", COMMAND_CLASS::IRRIGATION)
        .value("KICK", COMMAND_CLASS::KICK)
        .value("LANGUAGE", COMMAND_CLASS::LANGUAGE)
        .value("LOCK", COMMAND_CLASS::LOCK)
        .value("MAILBOX", COMMAND_CLASS::MAILBOX)
        .value("MANUFACTURER_PROPRIETARY", COMMAND_CLASS::MANUFACTURER_PROPRIETARY)
        .value("MANUFACTURER_SPECIFIC", COMMAND_CLASS::MANUFACTURER_SPECIFIC)
        .value("MARK", COMMAND_CLASS::MARK)
        .value("METER", COMMAND_CLASS::METER)
        .value("METER_PULSE", COMMAND_CLASS::METER_PULSE)
        .value("METER_TBL_CONFIG", COMMAND_CLASS::METER_TBL_CONFIG)
        .value("METER_TBL_MONITOR", COMMAND_CLASS::METER_TBL_MONITOR)
        .value("METER_TBL_PUSH", COMMAND_CLASS::METER_TBL_PUSH)
        .value("MTP_WINDOW_COVERING", COMMAND_CLASS::MTP_WINDOW_COVERING)
        .value("MULTI_CHANNEL", COMMAND_CLASS::MULTI_CHANNEL)
        .value("MULTI_CHANNEL_ASSOCIATION", COMMAND_CLASS::MULTI_CHANNEL_ASSOCIATION)
        .value("MULTI_COMMAND", COMMAND_CLASS::MULTI_COMMAND)
        .value("NETWORK_MANAGEMENT_BASIC", COMMAND_CLASS::NETWORK_MANAGEMENT_BASIC)
        .value("NETWORK_MANAGEMENT_INCLUSION", COMMAND_CLASS::NETWORK_MANAGEMENT_INCLUSION)
        .value("NETWORK_MANAGEMENT_PRIMARY", COMMAND_CLASS::NETWORK_MANAGEMENT_PRIMARY)
        .value("NETWORK_MANAGEMENT_PROXY", COMMAND_CLASS::NETWORK_MANAGEMENT_PROXY)
        .value("NETWORK_STAT", COMMAND_CLASS::NETWORK_STAT)
        .value("NODE_NAMING", COMMAND_CLASS::NODE_NAMING)
        .value("NON_INTEROPERABLE", COMMAND_CLASS::NON_INTEROPERABLE)
        .value("NOTIFICATION", COMMAND_CLASS::NOTIFICATION)
        .value("NO_OPERATION", COMMAND_CLASS::NO_OPERATION)
        .value("POWERLEVEL", COMMAND_CLASS::POWERLEVEL)
        .value("PREPAYMENT", COMMAND_CLASS::PREPAYMENT)
        .value("PREPAYMENT_ENCAPSULATION", COMMAND_CLASS::PREPAYMENT_ENCAPSULATION)
        .value("PROPRIETARY", COMMAND_CLASS::PROPRIETARY)
        .value("PROTECTION", COMMAND_CLASS::PROTECTION)
        .value("RATE_TBL_CONFIG", COMMAND_CLASS::RATE_TBL_CONFIG)
        .value("RATE_TBL_MONITOR", COMMAND_CLASS::RATE_TBL_MONITOR)
        .value("REMOTE_ASSOCIATION", COMMAND_CLASS::REMOTE_ASSOCIATION)
        .value("REMOTE_ASSOCIATION_ACTIVATE", COMMAND_CLASS::REMOTE_ASSOCIATION_ACTIVATE)
        .value("SCENE_ACTIVATION", COMMAND_CLASS::SCENE_ACTIVATION)
        .value("SCENE_ACTUATOR_CONF", COMMAND_CLASS::SCENE_ACTUATOR_CONF)
        .value("SCENE_CONTROLLER_CONF", COMMAND_CLASS::SCENE_CONTROLLER_CONF)
        .value("SCHEDULE", COMMAND_CLASS::SCHEDULE)
        .value("SCHEDULE_ENTRY_LOCK", COMMAND_CLASS::SCHEDULE_ENTRY_LOCK)
        .value("SCREEN_ATTRIBUTES", COMMAND_CLASS::SCREEN_ATTRIBUTES)
        .value("SCREEN_MD", COMMAND_CLASS::SCREEN_MD)
        .value("SECURITY", COMMAND_CLASS::SECURITY)
        .value("SECURITY_SCHEME0_MARK", COMMAND_CLASS::SECURITY_SCHEME0_MARK)
        .value("SENSOR_ALARM", COMMAND_CLASS::SENSOR_ALARM)
        .value("SENSOR_BINARY", COMMAND_CLASS::SENSOR_BINARY)
        .value("SENSOR_CONFIGURATION", COMMAND_CLASS::SENSOR_CONFIGURATION)
        .value("SENSOR_MULTILEVEL", COMMAND_CLASS::SENSOR_MULTILEVEL)
        .value("SILENCE_ALARM", COMMAND_CLASS::SILENCE_ALARM)
        .value("SIMPLE_AV_CONTROL", COMMAND_CLASS::SIMPLE_AV_CONTROL)
        .value("SUPERVISION", COMMAND_CLASS::SUPERVISION)
        .value("SWITCH_ALL", COMMAND_CLASS::SWITCH_ALL)
        .value("SWITCH_BINARY", COMMAND_CLASS::SWITCH_BINARY)
        .value("SWITCH_COLOR", COMMAND_CLASS::SWITCH_COLOR)
        .value("SWITCH_MULTILEVEL", COMMAND_CLASS::SWITCH_MULTILEVEL)
        .value("SWITCH_TOGGLE_BINARY", COMMAND_CLASS::SWITCH_TOGGLE_BINARY)
        .value("SWITCH_TOGGLE_MULTILEVEL", COMMAND_CLASS::SWITCH_TOGGLE_MULTILEVEL)
        .value("TARIFF_TBL_CONFIG", COMMAND_CLASS::TARIFF_TBL_CONFIG)
        .value("TARIFF_TBL_MONITOR", COMMAND_CLASS::TARIFF_TBL_MONITOR)
        .value("THERMOSTAT_FAN_MODE", COMMAND_CLASS::THERMOSTAT_FAN_MODE)
        .value("THERMOSTAT_FAN_STATE", COMMAND_CLASS::THERMOSTAT_FAN_STATE)
        .value("THERMOSTAT_HEATING", COMMAND_CLASS::THERMOSTAT_HEATING)
        .value("THERMOSTAT_MODE", COMMAND_CLASS::THERMOSTAT_MODE)
        .value("THERMOSTAT_OPERATING_STATE", COMMAND_CLASS::THERMOSTAT_OPERATING_STATE)
        .value("THERMOSTAT_SETBACK", COMMAND_CLASS::THERMOSTAT_SETBACK)
        .value("THERMOSTAT_SETPOINT", COMMAND_CLASS::THERMOSTAT_SETPOINT)
        .value("TIME", COMMAND_CLASS::TIME)
        .value("TIME_PARAMETERS", COMMAND_CLASS::TIME_PARAMETERS)
        .value("TRANSPORT_SERVICE", COMMAND_CLASS::TRANSPORT_SERVICE)
        .value("USER_CODE", COMMAND_CLASS::USER_CODE)
        .value("VERSION", COMMAND_CLASS::VERSION)
        .value("WAKE_UP", COMMAND_CLASS::WAKE_UP)
        .value("WINDOW_COVERING", COMMAND_CLASS::WINDOW_COVERING)
        .value("ZIP", COMMAND_CLASS::ZIP)
        .value("ZIP_6LOWPAN", COMMAND_CLASS::ZIP_6LOWPAN)
        .value("ZIP_ADV_SERVICES", COMMAND_CLASS::ZIP_ADV_SERVICES)
        .value("ZIP_CLIENT", COMMAND_CLASS::ZIP_CLIENT)
        .value("ZIP_GATEWAY", COMMAND_CLASS::ZIP_GATEWAY)
        .value("ZIP_NAMING", COMMAND_CLASS::ZIP_NAMING)
        .value("ZIP_ND", COMMAND_CLASS::ZIP_ND)
        .value("ZIP_PORTAL", COMMAND_CLASS::ZIP_PORTAL)
        .value("ZIP_SERVER", COMMAND_CLASS::ZIP_SERVER)
        .value("ZWAVEPLUS_INFO", COMMAND_CLASS::ZWAVEPLUS_INFO)
    ;

    py::class_<NodeClassInformation>(manager, "NodeClassInformation")
        .def_readonly("className", &NodeClassInformation::className)
        .def_readonly("classVersion", &NodeClassInformation::classVersion)
    ;

        py::class_<SwitchPointData>(manager, "SwitchPointData")
        .def(py::init<const uint8_t, const uint8_t, const int8_t>())
        .def_readwrite("hours", &SwitchPointData::hours)
        .def_readwrite("minutes", &SwitchPointData::minutes)
        .def_readwrite("setback", &SwitchPointData::setback)
    ;

    manager
        // Configuration
        .def("writeConfig",
            &Manager::WriteConfig,
            "Saves the configuration of a PC Controller's Z-Wave network to the"
            " application's user data folder."
        )
        .def("getOptions", &Manager::GetOptions, py::return_value_policy::automatic_reference)
        // Drivers
        .def("addDriver",
            &Manager::AddDriver,
            py::arg("_controllerPath"),
            py::arg("_interface") = Driver::ControllerInterface::ControllerInterface_Serial,
            "Creates a new driver for a Z-Wave controller."
        )
        .def("removeDriver", &RemoveDriver, "Removes the driver for a Z-Wave controller, and closes the controller.")
        .def("getControllerNodeId", &Manager::GetControllerNodeId, "Get the node ID of the Z-Wave controller.")
        .def("getSUCNodeId", &Manager::GetSUCNodeId, "Get the node ID of the Static Update Controller.")
        .def("isPrimaryController", &Manager::IsPrimaryController, "Query if the controller is a primary controller.")
        .def("isStaticUpdateController", &Manager::IsStaticUpdateController, "Query if the controller is a static update controller (SUC).")
        .def("isBridgeController", &Manager::IsBridgeController, "Query if the controller is using the bridge controller library.")
        .def("getLibraryVersion", &Manager::GetLibraryVersion, "Get the version of the Z-Wave API library used by a controller.")
        .def("getLibraryTypeName", &Manager::GetLibraryTypeName, "Get a string containing the Z-Wave API library type used by a controller.")
        .def("getSendQueueCount", &Manager::GetSendQueueCount, "Get count of messages in the outgoing send queue.")
        .def("logDriverStatistics", &Manager::LogDriverStatistics, "Send current driver statistics to the log file.")
        .def("getDriverStatistics", &GetDriverStatistics, "Retrieve statistics from driver.")
        .def("getNodeStatistics", &GetNodeStatistics, "Retrieve statistics per node")
        .def("getControllerInterfaceType", &Manager::GetControllerInterfaceType, "Retrieve controller interface type, Unknown, Serial, Hid")
        .def("getControllerPath", &Manager::GetControllerPath, "Retrieve controller interface path, name or path used to open the controller hardware")
        // Network
        .def("testNetworkNode", &Manager::TestNetworkNode, "Test network node.")
        .def("testNetwork", &Manager::TestNetwork, "Test network.")
        .def("healNetworkNode", &Manager::HealNetworkNode, "Heal network node by requesting the node rediscover their neighbors.")
        .def("healNetwork", &Manager::HealNetwork, "Heal network by requesting nodes rediscover their neighbors.")
        // Polling
        .def("getPollInterval", &Manager::GetPollInterval, "Get the time period between polls of a nodes state")
        .def("setPollInterval", &Manager::SetPollInterval, "Set the time period between polls of a nodes state.")
        .def("enablePoll", &Manager::EnablePoll, "Enable the polling of a device's state.")
        .def("disablePoll", &Manager::DisablePoll, "Disable polling of a value.")
        .def("isPolled", &Manager::isPolled, "Check polling status of a value")
        .def("getPollIntensity",
            &Manager::GetPollIntensity,
            "Get the intensity with which this value is polled (0=none, 1=every"
            " time through the list, 2-every other time, etc)."
        )
        .def("setPollIntensity",
            &Manager::SetPollIntensity,
            "Set the frequency of polling (0=none, 1=every time through the set,"
            " 2-every other time, etc)."
        )
        // Node Information
        .def("refreshNodeInfo", &Manager::RefreshNodeInfo, "Trigger the fetching of fixed data about a node.")
        .def("requestNodeState", &Manager::RequestNodeState, "Trigger the fetching of just the dynamic value data for a node.")
        .def("requestNodeDynamic", &Manager::RequestNodeDynamic, "Trigger the fetching of fixed data about a node.")
        .def("isNodeListeningDevice", &Manager::IsNodeListeningDevice, "Get whether the node is a setening device that does not go to sleep")
        .def("isNodeFrequentListeningDevice",
            &Manager::IsNodeFrequentListeningDevice,
            "Get whether the node is a frequent setening device that goes to sleep but"
            " can be woken up by a beam. Useful to determine node and controller"
            " consistency."
        )
        .def("isNodeBeamingDevice", &Manager::IsNodeBeamingDevice, "Get whether the node is a beam capable device.")
        .def("isNodeRoutingDevice", &Manager::IsNodeRoutingDevice, "Get whether the node is a routing device that passes messages to other nodes")
        .def("isNodeSecurityDevice", &Manager::IsNodeSecurityDevice, "Get the security attribute for a node. True if node supports security features.")
        .def("isNodeZWavePlus", &Manager::IsNodeZWavePlus, "Get whether the node is a ZWave+ one")
        .def("getNodeMaxBaudRate", &Manager::GetNodeMaxBaudRate, "Get the maximum baud rate of a nodes communications")
        .def("getNodeVersion", &Manager::GetNodeVersion, "Get the version number of a node")
        .def("getNodeSecurity", &Manager::GetNodeSecurity, "Get the security byte for a node.  Bit meanings are still to be determined.")
        .def("getNodeBasic", &Manager::GetNodeBasic, "Get the basic type of a node.")
        .def("getNodeGeneric", &Manager::GetNodeGeneric, "Get the generic type of a node.")
        .def("getNodeSpecific", &Manager::GetNodeSpecific, "Get the specific type of a node.")
        .def("getNodeType", &Manager::GetNodeType, "Get a human-readable label describing the node")
        .def("getNodeDeviceType", &Manager::GetNodeDeviceType, "Get the node DeviceType as reported in the Z-Wave+ Info report.")
        .def("getNodeDeviceTypeString", &Manager::GetNodeDeviceTypeString, "Get the node DeviceType (string) as reported in the Z-Wave+ Info report.")
        .def("getNodeRole", &Manager::GetNodeRole, "Get the node role as reported in the Z-Wave+ Info report.")
        .def("getNodeRoleString", &Manager::GetNodeRoleString, "Get the node role (string) as reported in the Z-Wave+ Info report.")
        .def("getNodePlusType", &Manager::GetNodePlusType, "Get the node plus type as reported in the Z-Wave+ Info report.")
        .def("getNodePlusTypeString", &Manager::GetNodePlusTypeString, "Get the node plus type (string) as reported in the Z-Wave+ Info report.")
        .def("getNodeNeighbors", &GetNodeNeighborsAsPySet, "Get the bitmap of this node's neighbors.")
        .def("getNodeManufacturerName", &Manager::GetNodeManufacturerName, "Get the manufacturer name of a device")
        .def("getNodeProductName", &Manager::GetNodeProductName, "Get the product name of a device")
        .def("getNodeName", &Manager::GetNodeName, "Get the name of a node")
        .def("getNodeLocation", &Manager::GetNodeLocation, "Get the location of a node")
        .def("getNodeManufacturerId", &Manager::GetNodeManufacturerId, "Get the manufacturer ID of a device")
        .def("getNodeProductType", &Manager::GetNodeProductType, "Get the product type of a device")
        .def("getNodeProductId", &Manager::GetNodeProductId, "Get the product ID of a device")
        .def("setNodeManufacturerName", &Manager::SetNodeManufacturerName, "Set the manufacturer name of a device")
        .def("setNodeProductName", &Manager::SetNodeProductName, "Set the product name of a device")
        .def("setNodeName", &Manager::SetNodeName, "Set the name of a node")
        .def("setNodeLocation", &Manager::SetNodeLocation, "Set the location of a node")
        .def("setNodeOn", &Manager::SetNodeOn, "Turns a node on")
        .def("setNodeOff", &Manager::SetNodeOff, "Turns a node off")
        .def("setNodeLevel", &Manager::SetNodeLevel, "Sets the basic level of a node")
        .def("isNodeInfoReceived", &Manager::IsNodeInfoReceived, "Get whether the node information has been received")
        .def("getNodeClassInformation",
            &GetNodeClassInformation,
            py::arg("_homeId"),
            py::arg("_nodeId"),
            py::arg("_commandClassId"),
            "Helper method to return whether a particular class is available in a node"
        )
        .def("isNodeAwake", &Manager::IsNodeAwake, "Get whether the node is awake or asleep")
        .def("isNodeFailed", &Manager::IsNodeFailed, "Get whether the node is working or has failed")
        .def("getNodeQueryStage", &Manager::GetNodeQueryStage, "Get whether the node's query stage as a string")
        .def("getNodeQueryStageCode", &GetNodeQueryStageCode, "Get code value from a query stage description")
        // Values
        .def("getValueLabel", &Manager::GetValueLabel, "Gets the user-friendly label for the value")
        .def("setValueLabel", &Manager::SetValueLabel, "Sets the user-friendly label for the value")
        .def("getValueUnits", &Manager::GetValueUnits, "Gets the units that the value is measured in.")
        .def("setValueUnits", &Manager::SetValueUnits, "Sets the units that the value is measured in.")
        .def("getValueHelp", &Manager::GetValueHelp, "Gets a help string describing the value's purpose and usage.")
        .def("setValueHelp", &Manager::SetValueHelp, "Sets a help string describing the value's purpose and usage.")
        .def("getValueMin", &Manager::GetValueMin, "Gets the minimum that this value may contain.")
        .def("getValueMax", &Manager::GetValueMax, "Gets the maximum that this value may contain.")
        .def("isValueReadOnly", &Manager::IsValueReadOnly, "Test whether the value is read-only.")
        .def("isValueWriteOnly", &Manager::IsValueWriteOnly, "Test whether the value is write-only.")
        .def("isValueSet", &Manager::IsValueSet, "Test whether the value has been set.")
        .def("isValuePolled", &Manager::IsValuePolled, "Test whether the value is currently being polled.")
        .def("getValueGenre",
            &GetValueGenre,
            "Get the genre of the value.  The genre classifies a value to enable"
            " low-level system or configuration parameters to be filtered out"
            " by the application"
        )
        .def("getValueCommandClass",
            &GetValueCommandClass,
            "Get the command class of this value.  It is possible for there to be"
            " multiple instances of a command class, although currently it appears that"
            " only the SensorMultilevel command class ever does this.  Knowledge of"
            " instances and command classes is not required to use OpenZWave, but this"
            " information is exposed in case it is of interest."
        )
        .def("getValueInstance",
            &GetValueInstance,
            "Get the command class instance of this value.  It is possible for there to be"
            " multiple instances of a command class, although currently it appears that"
            " only the SensorMultilevel command class ever does this."
        )
        .def("getValueIndex",
            &GetValueIndex,
            "Get the value index.  The index is used to identify one of multiple"
            " values created and managed by a command class.  In the case of configurable"
            " parameters (handled by the configuration command class), the index is the"
            " same as the parameter ID."
        )
        .def("getValueType", &GetValueType, "Gets the type of the value")
        .def("getValue", &GetValue, "Gets a value.")
        .def("getValueAsBool", &GetValueAsBool, "Gets a value as a bool.")
        .def("getValueAsByte", &GetValueAsByte, "Gets a value as an 8-bit unsigned integer.")
        .def("getValueAsFloat", &GetValueAsFloat, "Gets a value as a float.")
        .def("getValueAsInt", &GetValueAsInt, "Gets a value as a 32-bit signed integer.")
        .def("getValueAsShort", &GetValueAsShort, "Gets a value as a 16-bit signed integer.")
        .def("getValueAsRaw", &GetValueAsRaw, "Gets a value as a collection of bytes.")
        .def("getValueAsString", &GetValueAsString, "Gets a value as a string.")
        .def("getValueListSelectionItem", &GetValueListSelectionItem, "Gets value of items from a list value")
        .def("getValueListSelectionStr", &GetValueListSelectionItem, "Gets value of items from a list value")
        .def("getValueListSelectionValue", &GetValueListSelectionValue, "Gets value of items from a list value")
        .def("getValueListSelectionNum", &GetValueListSelectionValue, "Gets value of items from a list value")
        .def("getValueListItems", &GetValueListItems, "Gets the list of items from a list value")
        .def("getValueListValues", &GetValueListValues, "Gets the list of values from a list value.")
        .def("setValue",
            (bool (Manager::*)(const ValueID&, bool)) &Manager::SetValue,
            "Sets the value of a device valueid."
        )
        .def("setValue",
            (bool (Manager::*)(const ValueID&, uint8_t)) &Manager::SetValue,
            "Sets the value of a device valueid."
        )
        .def("setValue",
            (bool (Manager::*)(const ValueID&, float)) &Manager::SetValue,
            "Sets the value of a device valueid."
        )
        .def("setValue",
            (bool (Manager::*)(const ValueID&, int32_t)) &Manager::SetValue,
            "Sets the value of a device valueid."
        )
        .def("setValue",
            (bool (Manager::*)(const ValueID&, int16_t)) &Manager::SetValue,
            "Sets the value of a device valueid."
        )
        .def("setValue",
            (bool (Manager::*)(const ValueID&, const uint8_t*, uint8_t)) &Manager::SetValue,
            "Sets the value of a device valueid."
        )
        .def("setValue",
            (bool (Manager::*)(const ValueID&, const string&)) &Manager::SetValue,
            "Sets the value of a device valueid."
        )
        .def("setValueListSelection", &Manager::SetValueListSelection)
        .def("refreshValue", &Manager::RefreshValue, "Refreshes the specified value from the Z-Wave network.")
        .def("setChangeVerified", &Manager::SetChangeVerified, "Sets a flag indicating whether value changes noted upon a refresh should be verified.")
        .def("getChangeVerified", &Manager::GetChangeVerified, "Determine if value changes upon a refresh should be verified.")
        .def("pressButton", &Manager::PressButton, "Starts an activity in a device.")
        .def("releaseButton", &Manager::ReleaseButton, "Stops an activity in a device.")
        .def("getValueFloatPrecision", &GetValueFloatPrecision, "Gets a float value's precision")
        // Climate Control
        .def("getNumSwitchPoints", &Manager::GetNumSwitchPoints, "Get the number of switch points defined in a schedule")
        .def("setSwitchPoint", &SetSwitchPoint, "Set a switch point in the schedule.")
        .def("setSwitchPoint", &Manager::SetSwitchPoint, "Set a switch point in the schedule.")
        .def("removeSwitchPoint", &Manager::RemoveSwitchPoint, "Remove a switch point from the schedule")
        .def("clearSwitchPoints", &Manager::ClearSwitchPoints, "Clears all switch points from the schedule")
        .def("getSwitchPoint", &GetSwitchPoint, "Gets switch point data from the schedule")
        // SwitchAll
        .def("switchAllOn",
            &Manager::SwitchAllOn,
            "Switch all devices on.  All devices that support the SwitchAll command class"
            " will be turned on."
        )
        .def("switchAllOff",
            &Manager::SwitchAllOff,
            "Switch all devices off.  All devices that support the SwitchAll command class"
            " will be turned off."
        )
        // Configuration Parameters
        .def("setConfigParam",
            &Manager::SetConfigParam,
            py::arg("_homeId"),
            py::arg("_nodeId"),
            py::arg("_param"),
            py::arg("_value"),
            py::arg("_size") = 2,
            "Set the value of a configurable parameter in a device. _size Is an optional"
            " number of bytes to be sent for the paramter _value. Defaults to 2."
        )
        .def("requestConfigParam", &Manager::RequestConfigParam, "Request the value of a configurable parameter from a device.")
        .def("requestAllConfigParams", &Manager::RequestAllConfigParams, "Request the values of all known configurable parameters from a device.")
        // Groups
        .def("getNumGroups", &Manager::GetNumGroups, "Gets the number of association groups reported by this node")
        .def("getAssociationsInstances", &GetAssociationsInstancesAsPySet, "Gets the associationsInstances for a group")
        .def("getAssociations", &GetAssociationsAsPySet, "Gets the associations for a group")
        .def("getMaxAssociations", &Manager::GetMaxAssociations, "Gets the maximum number of associations for a group.")
        .def("getGroupLabel", &Manager::GetGroupLabel, "Returns a label for the particular group of a node.")
        .def("addAssociation",
            &Manager::AddAssociation,
            py::arg("_homeId"),
            py::arg("_nodeId"),
            py::arg("_groupIdx"),
            py::arg("_targetNodeId"),
            py::arg("_instance") = 0x00,
            "Adds a node to an association group."
        )
        .def("removeAssociation",
            &Manager::RemoveAssociation,
            py::arg("_homeId"),
            py::arg("_nodeId"),
            py::arg("_groupIdx"),
            py::arg("_targetNodeId"),
            py::arg("_instance") = 0x00,
            "Removes a node from an association group."
        )
        .def("addWatcher",
            &AddWatcher,
            py::arg("_watcher"),
            py::arg("_context") = py::none(),
            "Add a notification watcher."
        )
        .def("removeWatcher",
            &RemoveWatcher,
            py::arg("_watcher"),
            py::arg("_context") = py::none(),
            "Remove a notification watcher."
        )
        // Controller Commands
        .def("resetController", &Manager::ResetController, "Hard Reset a PC Z-Wave Controller.")
        .def("softResetController", &Manager::SoftReset, "Soft Reset a PC Z-Wave Controller.")
        .def("cancelControllerCommand", &Manager::CancelControllerCommand, "Cancels any in-progress command running on a controller.")
        .def("addNode", &Manager::AddNode, "Start the Inclusion Process to add a Node to the Network.")
        .def("removeNode", &Manager::RemoveNode, "Remove a Device from the Z-Wave Network")
        .def("removeFailedNode",
            &Manager::RemoveFailedNode,
            "Move a node to the controller's failed nodes list. This command will only"
            " work if the node cannot respond."
        )
        .def("hasNodeFailed", &Manager::HasNodeFailed, "Check if the Controller Believes a Node has Failed.")
        .def("replaceFailedNode", &Manager::ReplaceFailedNode, "Replace a failed device with another.")
        .def("assignReturnRoute", &Manager::AssignReturnRoute, "Ask a Node to update its update its Return Route to the Controller")
        .def("requestNodeNeighborUpdate", &Manager::RequestNodeNeighborUpdate, "Ask a Node to update its Neighbor Tables")
        .def("requestNetworkUpdate", &Manager::RequestNetworkUpdate, "Update the controller with network information from the SUC/SIS.")
        .def("replicationSend", &Manager::ReplicationSend, "Send information from primary to secondary")
        .def("deleteAllReturnRoutes", &Manager::DeleteAllReturnRoutes, "Ask a Node to delete all Return Route.")
        .def("sendNodeInformation", &Manager::SendNodeInformation, "Create a new primary controller when old primary fails. Requires SUC.")
        .def("createNewPrimary", &Manager::CreateNewPrimary, "Create a new primary controller when old primary fails. Requires SUC.")
        .def("transferPrimaryRole", &Manager::TransferPrimaryRole, "Add a new controller to the network and make it the primary.")
        .def("receiveConfiguration", &Manager::ReceiveConfiguration, "Receive network configuration information from primary controller. Requires secondary.")
        .def("createButton", &Manager::CreateButton, "Create a handheld button id.")
        .def("deleteButton", &Manager::DeleteButton, "Delete a handheld button id.")
        // Scene commands
        .def("getNumScenes", &Manager::GetNumScenes, "Gets the number of scenes that have been defined")
        .def("getAllScenes", &GetAllScenesAsPySet, "Gets a list of all the SceneIds")
        .def("createScene", &Manager::CreateScene, "Create a Scene.")
        .def("removeAllScenes", &Manager::RemoveAllScenes, "Delete all scenes.")
        .def("removeScene", &Manager::RemoveScene, "Remove an existing Scene.")
        .def("addSceneValue",
            (bool (Manager::*)(uint8_t, const ValueID&, bool)) &Manager::AddSceneValue,
            "Add a ValueID of value to an existing scene."
        )
        .def("addSceneValue",
            (bool (Manager::*)(uint8_t, const ValueID&, uint8_t)) &Manager::AddSceneValue,
            "Add a ValueID of value to an existing scene."
        )
        .def("addSceneValue",
            (bool (Manager::*)(uint8_t, const ValueID&, float)) &Manager::AddSceneValue,
            "Add a ValueID of value to an existing scene."
        )
        .def("addSceneValue",
            (bool (Manager::*)(uint8_t, const ValueID&, int32_t)) &Manager::AddSceneValue,
            "Add a ValueID of value to an existing scene."
        )
        .def("addSceneValue",
            (bool (Manager::*)(uint8_t, const ValueID&, int16_t)) &Manager::AddSceneValue,
            "Add a ValueID of value to an existing scene."
        )
        .def("addSceneValue",
            (bool (Manager::*)(uint8_t, const ValueID&, const string&)) &Manager::AddSceneValue,
            "Add a ValueID of value to an existing scene."
        )
        .def("addSceneValueListSelection", (bool (Manager::*)(uint8_t, const ValueID&, const string&)) &Manager::AddSceneValueListSelection)
        .def("addSceneValueListSelection", (bool (Manager::*)(uint8_t, const ValueID&, int32_t)) &Manager::AddSceneValueListSelection)
        .def("removeSceneValue", &Manager::RemoveSceneValue, "Remove the Value ID from an existing scene.")
        .def("sceneGetValues", &SceneGetValues, "Retrieve the list of values from a scene.")
        .def("sceneGetValueAsBool", &Manager::SceneGetValueAsBool)
        .def("sceneGetValueAsByte", &Manager::SceneGetValueAsByte)
        .def("sceneGetValueAsFloat", &Manager::SceneGetValueAsFloat)
        .def("sceneGetValueAsInt", &Manager::SceneGetValueAsInt)
        .def("sceneGetValueAsShort", &Manager::SceneGetValueAsShort)
        .def("sceneGetValueAsString", &Manager::SceneGetValueAsString)
        .def("sceneGetValueListSelectionItem", &SceneGetValueListSelectionItem)
        .def("sceneGetValueListSelectionValue", &SceneGetValueListSelectionValue)
        .def("setSceneValue",
            (bool (Manager::*)(uint8_t, const ValueID&, bool)) &Manager::SetSceneValue,
            "Set a value to an existing scene's ValueID."
        )
        .def("setSceneValue",
            (bool (Manager::*)(uint8_t, const ValueID&, uint8_t)) &Manager::SetSceneValue,
            "Set a value to an existing scene's ValueID."
        )
        .def("setSceneValue",
            (bool (Manager::*)(uint8_t, const ValueID&, float)) &Manager::SetSceneValue,
            "Set a value to an existing scene's ValueID."
        )
        .def("setSceneValue",
            (bool (Manager::*)(uint8_t, const ValueID&, int32_t)) &Manager::SetSceneValue,
            "Set a value to an existing scene's ValueID."
        )
        .def("setSceneValue",
            (bool (Manager::*)(uint8_t, const ValueID&, int16_t)) &Manager::SetSceneValue,
            "Set a value to an existing scene's ValueID."
        )
        .def("setSceneValue",
            (bool (Manager::*)(uint8_t, const ValueID&, const string&)) &Manager::SetSceneValue,
            "Set a value to an existing scene's ValueID."
        )
        .def("setSceneValueListSelection",
            (bool (Manager::*)(uint8_t, const ValueID&, const string&)) &Manager::SetSceneValueListSelection
        )
        .def("setSceneValueListSelection",
            (bool (Manager::*)(uint8_t, const ValueID&, int32)) &Manager::SetSceneValueListSelection
        )
        .def("getSceneLabel", &Manager::GetSceneLabel, "Returns a label for the particular scene.")
        .def("setSceneLabel", &Manager::SetSceneLabel, "Sets a label for the particular scene.")
        .def("sceneExists", &Manager::SceneExists, "Check if a Scene ID is defined.")
        .def("activateScene", &Manager::ActivateScene, "Activate given scene to perform all its actions.")

        // Static methods
        .def_static("create", &CreateManager, "Creates the Manager singleton object.")
        .def_static("destroy", &DestroyManager, "Deletes the Manager and cleans up any associated objects.")
        .def_static("get", &GetManager)
        .def_static("getCommandClassForId", &GetCommandClassForId, "Returns the COMMAND_CLASS enum value for the given Z-Wave Command Class ID.")
        .def_static("getPythonLibraryVersion", &GetPythonLibraryVersion, "Get the version of the python library.")
        .def_static("getPythonLibraryVersionNumber", &GetPythonLibraryVersionNumber, "Get the python library version number")
        .def_static("getOzwLibraryVersion", &Manager::getVersionAsString, "Get a string containing the openzwave library version.")
        .def_static("getOzwLibraryLongVersion", &Manager::getVersionLongAsString, "Get a string containing the openzwave library version.")
        .def_static("getOzwLibraryVersionNumber", &Manager::getVersionAsString, "Get the openzwave library version number.")
    ;
}
