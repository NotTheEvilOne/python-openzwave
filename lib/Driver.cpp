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

#include <openzwave/Driver.h>

#include <pybind11/pybind11.h>

using namespace OpenZWave;
namespace py = pybind11;

/* Python wrapper */

void WrapDriver(py::module m) {
    py::class_<Driver, unique_ptr<Driver, py::nodelete>> driver(m, "Driver");

    py::enum_<Driver::ControllerInterface>(driver, "ControllerInterface")
        // Undefined
        .value("Unknown", Driver::ControllerInterface_Unknown)
        // Serial protocol
        .value("Serial", Driver::ControllerInterface_Serial)
        // Human interface device protocol
        .value("Hid", Driver::ControllerInterface_Hid)
    ;

    // Note: Update src-api/openzwave/controller.py as well
    py::class_<Driver::DriverData>(driver, "StatData")
        .def_readonly("SOFCnt", &Driver::DriverData::m_SOFCnt, "Number of SOF bytes received")
        .def_readonly("ACKWaiting", &Driver::DriverData::m_ACKWaiting, "Number of unsolicited messages while waiting for an ACK")
        .def_readonly("readAborts", &Driver::DriverData::m_readAborts, "Number of times read were aborted due to timeouts")
        .def_readonly("badChecksum", &Driver::DriverData::m_badChecksum, "Number of bad checksums")
        .def_readonly("readCnt", &Driver::DriverData::m_readCnt, "Number of messages successfully read")
        .def_readonly("writeCnt", &Driver::DriverData::m_writeCnt, "Number of messages successfully sent")
        .def_readonly("CANCnt", &Driver::DriverData::m_CANCnt, "Number of CAN bytes received")
        .def_readonly("NAKCnt", &Driver::DriverData::m_NAKCnt, "Number of NAK bytes received")
        .def_readonly("ACKCnt", &Driver::DriverData::m_ACKCnt, "Number of ACK bytes received")
        .def_readonly("OOFCnt", &Driver::DriverData::m_OOFCnt, "Number of bytes out of framing")
        .def_readonly("dropped", &Driver::DriverData::m_dropped, "Number of messages dropped & not delivered")
        .def_readonly("retries", &Driver::DriverData::m_retries, "Number of messages retransmitted")
        .def_readonly("callbacks", &Driver::DriverData::m_callbacks, "Number of unexpected callbacks")
        .def_readonly("badroutes", &Driver::DriverData::m_badroutes, "Number of failed messages due to bad route response")
        .def_readonly("noack", &Driver::DriverData::m_noack, "Number of no ACK returned errors")
        .def_readonly("netbusy", &Driver::DriverData::m_netbusy, "Number of network busy/failure messages")
        .def_readonly("nondelivery", &Driver::DriverData::m_nondelivery, "Number of messages not delivered to network")
        .def_readonly("routedbusy", &Driver::DriverData::m_routedbusy, "Number of messages received with routed busy status")
        .def_readonly("broadcastReadCnt", &Driver::DriverData::m_broadcastReadCnt, "Number of broadcasts read")
        .def_readonly("broadcastWriteCnt", &Driver::DriverData::m_broadcastWriteCnt, "Number of broadcasts sent")
    ;

    // Note: Update src-api/openzwave/controller.py as well
    py::enum_<Driver::ControllerCommand>(driver, "ControllerCommand")
        // No command.
        .value("NoneValue", Driver::ControllerCommand_None)
        // Add a new device or controller to the Z-Wave network.
        .value("AddDevice", Driver::ControllerCommand_AddDevice)
        // Add a new controller to the Z-Wave network. Used when old primary fails. Requires SUC.
        .value("CreateNewPrimary", Driver::ControllerCommand_CreateNewPrimary)
        // Receive Z-Wave network configuration information from another controller.
        .value("ReceiveConfiguration", Driver::ControllerCommand_ReceiveConfiguration)
        // Remove a device or controller from the Z-Wave network.
        .value("RemoveDevice", Driver::ControllerCommand_RemoveDevice)
        // Move a node to the controller's failed nodes list. This command will only work if the node cannot respond.
        .value("RemoveFailedNode", Driver::ControllerCommand_RemoveFailedNode)
        // Check whether a node is in the controller's failed nodes list.
        .value("HasNodeFailed", Driver::ControllerCommand_HasNodeFailed)
        // Replace a non-responding node with another. The node must be in the controller's list of failed nodes for this command to succeed.
        .value("ReplaceFailedNode", Driver::ControllerCommand_ReplaceFailedNode)
        // Make a different controller the primary.
        .value("TransferPrimaryRole", Driver::ControllerCommand_TransferPrimaryRole)
        // Request network information from the SUC/SIS.
        .value("RequestNetworkUpdate", Driver::ControllerCommand_RequestNetworkUpdate)
        // Get a node to rebuild its neighbour list.  This method also does RequestNodeNeighbors
        .value("RequestNodeNeighborUpdate", Driver::ControllerCommand_RequestNodeNeighborUpdate)
        // Assign a network return routes to a device.
        .value("AssignReturnRoute", Driver::ControllerCommand_AssignReturnRoute)
        // Delete all return routes from a device.
        .value("DeleteAllReturnRoutes", Driver::ControllerCommand_DeleteAllReturnRoutes)
        // Send a node information frame
        .value("SendNodeInformation", Driver::ControllerCommand_SendNodeInformation)
        // Send information from primary to secondary
        .value("ReplicationSend", Driver::ControllerCommand_ReplicationSend)
        // Create an id that tracks handheld button presses
        .value("CreateButton", Driver::ControllerCommand_CreateButton)
        // Delete id that tracks handheld button presses
        .value("DeleteButton", Driver::ControllerCommand_DeleteButton)
    ;

    // Note: Update src-api/openzwave/controller.py as well
    py::enum_<Driver::ControllerState>(driver, "ControllerState")
        // No command in progress.
        .value("Normal", Driver::ControllerState_Normal)
        // The command is starting.
        .value("Starting", Driver::ControllerState_Starting)
        // The command was cancelled.
        .value("Cancel", Driver::ControllerState_Cancel)
        // Command invocation had error(s) and was aborted
        .value("Error", Driver::ControllerState_Error)
        // Controller is waiting for a user action.
        .value("Waiting", Driver::ControllerState_Waiting)
        // Controller command is on a sleep queue wait for device.
        .value("Sleeping", Driver::ControllerState_Sleeping)
        // The controller is communicating with the other device to carry out the command.
        .value("InProgress", Driver::ControllerState_InProgress)
        // The command has completed successfully.
        .value("Completed", Driver::ControllerState_Completed)
        // The command has failed.
        .value("Failed", Driver::ControllerState_Failed)
        // Used only with ControllerCommand_HasNodeFailed to indicate that the controller thinks the node is OK.
        .value("NodeOK", Driver::ControllerState_NodeOK)
        // Used only with ControllerCommand_HasNodeFailed to indicate that the controller thinks the node has failed.
        .value("NodeFailed", Driver::ControllerState_NodeFailed)
    ;

    // Note: Update src-api/openzwave/controller.py as well
    py::enum_<Driver::ControllerError>(driver, "ControllerError")
        .value("NoneValue", Driver::ControllerError_None)
        // Button
        .value("ButtonNotFound", Driver::ControllerError_ButtonNotFound)
        // Button
        .value("NodeNotFound", Driver::ControllerError_NodeNotFound)
        // Button
        .value("NotBridge", Driver::ControllerError_NotBridge)
        // CreateNewPrimary
        .value("NotSUC", Driver::ControllerError_NotSUC)
        // CreateNewPrimary
        .value("NotSecondary", Driver::ControllerError_NotSecondary)
        // RemoveFailedNode, AddNodeToNetwork
        .value("NotPrimary", Driver::ControllerError_NotPrimary)
        // ReceiveConfiguration
        .value("IsPrimary", Driver::ControllerError_IsPrimary)
        // RemoveFailedNode
        .value("NotFound", Driver::ControllerError_NotFound)
        // RemoveFailedNode, RequestNetworkUpdate
        .value("Busy", Driver::ControllerError_Busy)
        // RemoveFailedNode, RequestNetworkUpdate
        .value("Failed", Driver::ControllerError_Failed)
        // RequestNetworkUpdate error
        .value("Disabled", Driver::ControllerError_Disabled)
        // RequestNetworkUpdate error
        .value("Overflow", Driver::ControllerError_Overflow)
    ;

    driver
        .def("isNetworkKeySet", &Driver::isNetworkKeySet)
    ;
};
