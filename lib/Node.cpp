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

#include <pybind11/pybind11.h>

#include "Node.h"

using namespace OpenZWave;
namespace py = pybind11;

/* Python wrapper */

void WrapNode(py::module m) {
    py::class_<Node> node(m, "Node");

    // Note: Update Manager::getNodeQueryStageCode as well
    py::enum_<Node::QueryStage>(node, "QueryStage")
        // Query process hasn't started for this node
        .value("NoneValue", Node::QueryStage_None)
        // Retrieve protocol information
        .value("ProtocolInfo", Node::QueryStage_ProtocolInfo)
        // Ping device to see if alive
        .value("Probe", Node::QueryStage_Probe)
        // Start wake up process if a sleeping node
        .value("WakeUp", Node::QueryStage_WakeUp)
        // Retrieve manufacturer name and product ids if ProtocolInfo lets us
        .value("ManufacturerSpecific1", Node::QueryStage_ManufacturerSpecific1)
        // Retrieve info about supported, controlled command classes
        .value("NodeInfo", Node::QueryStage_NodeInfo)
        // Retrieve ZWave+ info and update device classes
        .value("NodePlusInfo", Node::QueryStage_NodePlusInfo)
        // Retrieve a list of Command Classes that require Security
        .value("SecurityReport", Node::QueryStage_SecurityReport)
        // Retrieve manufacturer name and product ids
        .value("ManufacturerSpecific2", Node::QueryStage_ManufacturerSpecific2)
        // Retrieve version information
        .value("Versions", Node::QueryStage_Versions)
        // Retrieve information about multiple command class instances
        .value("Instances", Node::QueryStage_Instances)
        // Retrieve static information (doesn't change)
        .value("Static", Node::QueryStage_Static)
        // Ping a device upon restarting with cached config for the device
        .value("CacheLoad", Node::QueryStage_CacheLoad)
        // Retrieve information about associations
        .value("Associations", Node::QueryStage_Associations)
        // Retrieve node neighbor list
        .value("Neighbors", Node::QueryStage_Neighbors)
        // Retrieve session information (changes infrequently)
        .value("Session", Node::QueryStage_Session)
        // Retrieve dynamic information (changes frequently)
        .value("Dynamic", Node::QueryStage_Dynamic)
        // Retrieve configurable parameter information (only done on request)
        .value("Configuration", Node::QueryStage_Configuration)
        // Query process is completed for this node
        .value("Complete", Node::QueryStage_Complete)
    ;

    py::enum_<SECURITY_FLAG>(node, "SecurityFlag")
        .value("Security", SECURITY_FLAG::SecurityFlag_Security)
        .value("Controller", SECURITY_FLAG::SecurityFlag_Controller)
        .value("SpecificDevice", SECURITY_FLAG::SecurityFlag_SpecificDevice)
        .value("RoutingSlave", SECURITY_FLAG::SecurityFlag_RoutingSlave)
        .value("BeamCapability", SECURITY_FLAG::SecurityFlag_BeamCapability)
        .value("Sensor250ms", SECURITY_FLAG::SecurityFlag_Sensor250ms)
        .value("Sensor1000ms", SECURITY_FLAG::SecurityFlag_Sensor1000ms)
        .value("OptionalFunctionality", SECURITY_FLAG::SecurityFlag_OptionalFunctionality)
    ;

    py::class_<Node::CommandClassData>(node, "CommandClassData")
        .def_readonly("commandClassId", &Node::CommandClassData::m_commandClassId, "Num type of CommandClass id.")
        .def_readonly("sentCnt", &Node::CommandClassData::m_sentCnt, "Number of messages sent from this CommandClass.")
        .def_readonly("receivedCnt", &Node::CommandClassData::m_receivedCnt, "Number of messages received from this CommandClass.")
    ;

    py::class_<Node::NodeData>(node, "StatData")
        .def_readonly("sentCnt", &Node::NodeData::m_sentCnt, "Number of messages sent from this node.")
        .def_readonly("sentFailed", &Node::NodeData::m_sentFailed, "Number of sent messages failed")
        .def_readonly("retries", &Node::NodeData::m_retries, "Number of message retries")
        .def_readonly("receivedCnt", &Node::NodeData::m_receivedCnt, "Number of messages received from this node.")
        .def_readonly("receivedDups", &Node::NodeData::m_receivedDups, "Number of duplicated messages received")
        .def_readonly("receivedUnsolicited", &Node::NodeData::m_receivedUnsolicited, "Number of messages received unsolicited")
        .def_readonly("sentTS", &Node::NodeData::m_sentTS, "Last message sent time")
        .def_readonly("receivedTS", &Node::NodeData::m_receivedTS, "Last message received time")
        .def_readonly("lastRequestRTT", &Node::NodeData::m_lastRequestRTT, "Last message request RTT")
        .def_readonly("averageRequestRTT", &Node::NodeData::m_averageRequestRTT, "Average Request Round Trip Time (ms).")
        .def_readonly("lastResponseRTT", &Node::NodeData::m_lastResponseRTT, "Last message response RTT")
        .def_readonly("averageResponseRTT", &Node::NodeData::m_averageResponseRTT, "Average Reponse round trip time.")
        .def_readonly("quality", &Node::NodeData::m_quality, "Node quality measure")
        .def_readonly("lastReceivedMessage", &Node::NodeData::m_lastReceivedMessage, "Place to hold last received message")
        .def_readonly("ccData", &Node::NodeData::m_ccData, "List of statistic on each command_class")
    ;
};
