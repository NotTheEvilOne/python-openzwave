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

#include <openzwave/Manager.h>
#include <openzwave/Notification.h>

#include <pybind11/pybind11.h>

using namespace OpenZWave;
namespace py = pybind11;

/* Python wrapper */

void WrapNotification(py::module m) {
    py::class_<Notification, unique_ptr<Notification, py::nodelete>> notification(m, "Notification");

    // Note: Update src-api/openzwave/notification.py as well
    py::enum_<Notification::NotificationType> notificationType(notification, "Type");

    notificationType
        // A new node value has been added to OpenZWave's list. These notifications occur after a node has been discovered, and details of its command classes have been received. Each command class may generate one or more values depending on the complexity of the item being represented.
        .value("ValueAdded", Notification::Type_ValueAdded)
        // A node value has been removed from OpenZWave's list. This only occurs when a node is removed.
        .value("ValueRemoved", Notification::Type_ValueRemoved)
        // A node value has been updated from the Z-Wave network and it is different from the previous value.
        .value("ValueChanged", Notification::Type_ValueChanged)
        // A node value has been updated from the Z-Wave network.
        .value("ValueRefreshed", Notification::Type_ValueRefreshed)
        // The associations for the node have changed. The application should rebuild any group information it holds about the node.
        .value("Group", Notification::Type_Group)
        // A new node has been found (not already stored in zwcfg*.xml file)
        .value("NodeNew", Notification::Type_NodeNew)
        // A new node has been added to OpenZWave's list. This may be due to a device being added to the Z-Wave network, or because the application is initializing itself.
        .value("NodeAdded", Notification::Type_NodeAdded)
        // A node has been removed from OpenZWave's list. This may be due to a device being removed from the Z-Wave network, or because the application is closing.
        .value("NodeRemoved", Notification::Type_NodeRemoved)
        // Basic node information has been receievd, such as whether the node is a listening device, a routing device and its baud rate and basic, generic and specific types. It is after this notification that you can call Manager::GetNodeType to obtain a label containing the device description.
        .value("NodeProtocolInfo", Notification::Type_NodeProtocolInfo)
        // One of the node names has changed (name, manufacturer, product).
        .value("NodeNaming", Notification::Type_NodeNaming)
        // A node has triggered an event. This is commonly caused when a node sends a Basic_Set command to the controller. The event value is stored in the notification.
        .value("NodeEvent", Notification::Type_NodeEvent)
        // Polling of a node has been successfully turned off by a call to Manager::DisablePoll
        .value("PollingDisabled", Notification::Type_PollingDisabled)
        // Polling of a node has been successfully turned on by a call to Manager::EnablePoll
        .value("PollingEnabled", Notification::Type_PollingEnabled)
        // Scene Activation Set received
        .value("SceneEvent", Notification::Type_SceneEvent)
        // Handheld controller button event created
        .value("CreateButton", Notification::Type_CreateButton)
        // Handheld controller button event deleted
        .value("DeleteButton", Notification::Type_DeleteButton)
        // Handheld controller button on pressed event
        .value("ButtonOn", Notification::Type_ButtonOn)
        // Handheld controller button off pressed event
        .value("ButtonOff", Notification::Type_ButtonOff)
        // A driver for a PC Z-Wave controller has been added and is ready to use. The notification will contain the controller's Home ID, which is needed to call most of the Manager methods.
        .value("DriverReady", Notification::Type_DriverReady)
        // Driver failed to load
        .value("DriverFailed", Notification::Type_DriverFailed)
        // All nodes and values for this driver have been removed. This is sent instead of potentially hundreds of individual node and value notifications.
        .value("DriverReset", Notification::Type_DriverReset)
        // The queries on a node that are essential to its operation have been completed. The node can now handle incoming messages.
        .value("EssentialNodeQueriesComplete", Notification::Type_EssentialNodeQueriesComplete)
        // All the initialisation queries on a node have been completed.
        .value("NodeQueriesComplete", Notification::Type_NodeQueriesComplete)
        // All awake nodes have been queried, so client application can expected complete data for these nodes.
        .value("AwakeNodesQueried", Notification::Type_AwakeNodesQueried)
        // All nodes have been queried but some dead nodes found.
        .value("AllNodesQueriedSomeDead", Notification::Type_AllNodesQueriedSomeDead)
        // All nodes have been queried, so client application can expected complete data.
        .value("AllNodesQueried", Notification::Type_AllNodesQueried)
        // A manager notification report.
        .value("Notification", Notification::Type_Notification)
        // The Driver is being removed. (either due to Error or by request) Do Not Call Any Driver Related Methods after receiving this call.
        .value("DriverRemoved", Notification::Type_DriverRemoved)
        // When Controller Commands are executed, Notifications of Success/Failure etc are communicated via this Notification * Notification::GetEvent returns Driver::ControllerCommand and Notification::GetNotification returns Driver::ControllerState
        .value("ControllerCommand", Notification::Type_ControllerCommand)
        // The Device has been reset and thus removed from the NodeList in OZW
        .value("NodeReset", Notification::Type_NodeReset)
    ;

    py::enum_<Notification::NotificationCode>(notification, "NotificationCode")
        // Completed messages.
        .value("MsgComplete", Notification::Code_MsgComplete)
        // Messages that timeout will send a Notification with this code.
        .value("Timeout", Notification::Code_Timeout)
        // Report on NoOperation message sent completion.
        .value("NoOperation", Notification::Code_NoOperation)
        // Report when a sleeping node wakes.
        .value("Awake", Notification::Code_Awake)
        // Report when a node goes to sleep.
        .value("Sleep", Notification::Code_Sleep)
        // Report when a node is presumed dead.
        .value("Dead", Notification::Code_Dead)
        // Report when a node is revived.
        .value("Alive", Notification::Code_Alive)
    ;

    notification
        .def("getType", &Notification::GetType)
        .def("getHomeId", &Notification::GetHomeId)
        .def("getNodeId", &Notification::GetNodeId)
        .def("getValueID", &Notification::GetValueID)
        .def("getGroupIdx", &Notification::GetGroupIdx)
        .def("getEvent", &Notification::GetEvent)
        .def("getButtonId", &Notification::GetButtonId)
        .def("getSceneId", &Notification::GetSceneId)
        .def("getNotification", &Notification::GetNotification)
        .def("getByte", &Notification::GetByte)
        .def("getAsString", &Notification::GetAsString)
    ;
};
