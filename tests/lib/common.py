# -*- coding: utf-8 -*-

"""
.. module:: tests

This file is part of **python-openzwave** project https://github.com/OpenZWave/python-openzwave.
    :platform: Unix, Windows, MacOS X
    :sinopsis: openzwave API

.. moduleauthor: bibi21000 aka Sébastien GALLET <bibi21000@gmail.com>

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

"""

import sys, os
import time
import unittest
import threading
import logging
import shutil
from nose.plugins.skip import SkipTest

import libopenzwave

from tests.common import SLEEP
from tests.common import TestPyZWave

class TestLib(TestPyZWave):
    """
    Parent test class for lib
    """

    SIGNAL_NETWORK_FAILED = 'NetworkFailed'
    SIGNAL_NETWORK_STARTED = 'NetworkStarted'
    SIGNAL_NETWORK_READY = 'NetworkReady'
    SIGNAL_NETWORK_STOPPED = 'NetworkStopped'
    SIGNAL_NETWORK_RESETTED = 'DriverResetted'
    SIGNAL_NETWORK_AWAKED = 'DriverAwaked'
    SIGNAL_DRIVER_FAILED = 'DriverFailed'
    SIGNAL_DRIVER_READY = 'DriverReady'
    SIGNAL_DRIVER_RESET = 'DriverReset'
    SIGNAL_DRIVER_REMOVED = 'DriverRemoved'
    SIGNAL_GROUP = 'Group'
    SIGNAL_NODE = 'Node'
    SIGNAL_NODE_ADDED = 'NodeAdded'
    SIGNAL_NODE_EVENT = 'NodeEvent'
    SIGNAL_NODE_NAMING = 'NodeNaming'
    SIGNAL_NODE_NEW = 'NodeNew'
    SIGNAL_NODE_PROTOCOL_INFO = 'NodeProtocolInfo'
    SIGNAL_NODE_READY = 'NodeReady'
    SIGNAL_NODE_REMOVED = 'NodeRemoved'
    SIGNAL_SCENE_EVENT = 'SceneEvent'
    SIGNAL_VALUE = 'Value'
    SIGNAL_VALUE_ADDED = 'ValueAdded'
    SIGNAL_VALUE_CHANGED = 'ValueChanged'
    SIGNAL_VALUE_REFRESHED = 'ValueRefreshed'
    SIGNAL_VALUE_REMOVED = 'ValueRemoved'
    SIGNAL_POLLING_ENABLED = 'PollingEnabled'
    SIGNAL_POLLING_DISABLED = 'PollingDisabled'
    SIGNAL_CREATE_BUTTON = 'CreateButton'
    SIGNAL_DELETE_BUTTON = 'DeleteButton'
    SIGNAL_BUTTON_ON = 'ButtonOn'
    SIGNAL_BUTTON_OFF = 'ButtonOff'
    SIGNAL_ESSENTIAL_NODE_QUERIES_COMPLETE = 'EssentialNodeQueriesComplete'
    SIGNAL_NODE_QUERIES_COMPLETE = 'NodeQueriesComplete'
    SIGNAL_AWAKE_NODES_QUERIED = 'AwakeNodesQueried'
    SIGNAL_ALL_NODES_QUERIED = 'AllNodesQueried'
    SIGNAL_ALL_NODES_QUERIED_SOME_DEAD = 'AllNodesQueriedSomeDead'
    SIGNAL_MSG_COMPLETE = 'MsgComplete'
    SIGNAL_NOTIFICATION = 'Notification'
    SIGNAL_CONTROLLER_COMMAND = 'ControllerCommand'
    SIGNAL_CONTROLLER_WAITING = 'ControllerWaiting'

    STATE_STOPPED = 0
    STATE_FAILED = 1
    STATE_RESETTED = 3
    STATE_STARTED = 5
    STATE_AWAKED = 7
    STATE_READY = 10

    @classmethod
    def setUpClass(self):
        super(TestPyZWave, self).setUpClass()
        self.options = None
        self.homeid = None
        self.ready = False
        self.manager = None
        self.nodes = {}
        self.driver_state = None
        self.driver_ready = None
        self.driver_removed = None
        self.driver_failed = None
        self.driver_reset = None
        self.network_state = None
        self.network_ready = None
        self.network_awake = None

    @classmethod
    def tearDownClass(self):
        self.stop_lib()
        super(TestPyZWave, self).tearDownClass()

    def start_lib(self):
        if self.options is None:
            self.options = libopenzwave.Options.create(config_path="openzwave/config", \
                user_path=self.userpath, cmd_line="--logging false")
            self.options.lock()
        if self.manager is None:
            self.homeid = None
            self.nodes = {}
            self.ready = False
            self.driver_state = None
            self.driver_ready = None
            self.driver_removed = None
            self.driver_failed = None
            self.driver_reset = None
            self.network_state = None
            self.network_ready = None
            self.network_awake = None
            self.manager = libopenzwave.Manager.create()
            self.manager.addWatcher(self.zwcallback)
            time.sleep(1.0)
            self.manager.addDriver(self.device)
            time.sleep(5.0)

    @classmethod
    def stop_lib(self):
        if self.manager is not None:
            print("Remove driver")
            self.manager.removeDriver(self.device)
            time.sleep(2.0)
            print("Remove watcher")
            self.manager.removeWatcher(self.zwcallback)
            time.sleep(2.0)
            print("Destroy manager")
            self.manager.destroy()
            time.sleep(1.0)
            self.manager = None
        if self.options is not None:
            libopenzwave.Options.destroy()
            print("Destroy options")
            time.sleep(1.0)
        self.options = None
        self.homeid = None
        self.ready = False
        self.manager = None
        self.driver_state = None
        self.driver_ready = None
        self.driver_removed = None
        self.driver_failed = None
        self.driver_reset = None
        self.network_state = None
        self.network_ready = None
        self.network_awake = None

    def zwcallback(self, notification):
        try:
            notify_type = str(notification.getType())[5:]
            print('zwcallback type=%s' % notify_type)
            if notify_type == self.SIGNAL_DRIVER_FAILED:
                self._handle_driver_failed(notification)
            elif notify_type == self.SIGNAL_DRIVER_READY:
                self._handle_driver_ready(notification)
            elif notify_type == self.SIGNAL_DRIVER_RESET:
                self._handle_driver_reset(notification)
            elif notify_type == self.SIGNAL_NODE_ADDED:
                self._handle_node_added(notification)
            elif notify_type == self.SIGNAL_NODE_EVENT:
                self._handle_node_event(notification)
            elif notify_type == self.SIGNAL_NODE_NAMING:
                self._handle_node_naming(notification)
            elif notify_type == self.SIGNAL_NODE_NEW:
                self._handle_node_new(notification)
            elif notify_type == self.SIGNAL_NODE_PROTOCOL_INFO:
                self._handle_node_protocol_info(notification)
            elif notify_type == self.SIGNAL_NODE_READY:
                self._handleNodeReady(notification)
            elif notify_type == self.SIGNAL_NODE_REMOVED:
                self._handle_node_removed(notification)
            elif notify_type == self.SIGNAL_GROUP:
                self._handle_group(notification)
            elif notify_type == self.SIGNAL_SCENE_EVENT:
                self._handle_scene_event(notification)
            elif notify_type == self.SIGNAL_VALUE_ADDED:
                self._handle_value_added(notification)
            elif notify_type == self.SIGNAL_VALUE_CHANGED:
                self._handle_value_changed(notification)
            elif notify_type == self.SIGNAL_VALUE_REFRESHED:
                self._handle_value_refreshed(notification)
            elif notify_type == self.SIGNAL_VALUE_REMOVED:
                self._handle_value_removed(notification)
            elif notify_type == self.SIGNAL_POLLING_DISABLED:
                self._handle_polling_disabled(notification)
            elif notify_type == self.SIGNAL_POLLING_ENABLED:
                self._handle_polling_enabled(notification)
            elif notify_type == self.SIGNAL_CREATE_BUTTON:
                self._handle_create_button(notification)
            elif notify_type == self.SIGNAL_DELETE_BUTTON:
                self._handle_delete_button(notification)
            elif notify_type == self.SIGNAL_BUTTON_ON:
                self._handle_button_on(notification)
            elif notify_type == self.SIGNAL_BUTTON_OFF:
                self._handle_button_off(notification)
            elif notify_type == self.SIGNAL_ALL_NODES_QUERIED:
                self._handle_all_nodes_queried(notification)
            elif notify_type == self.SIGNAL_ALL_NODES_QUERIED_SOME_DEAD:
                self._handle_all_nodes_queried_some_dead(notification)
            elif notify_type == self.SIGNAL_AWAKE_NODES_QUERIED:
                self._handle_awake_nodes_queried(notification)
            elif notify_type == self.SIGNAL_ESSENTIAL_NODE_QUERIES_COMPLETE:
                self._handle_essential_node_queries_complete(notification)
            elif notify_type == self.SIGNAL_NODE_QUERIES_COMPLETE:
                self._handle_node_queries_complete(notification)
            elif notify_type == self.SIGNAL_MSG_COMPLETE:
                self._handle_msg_complete(notification)
            elif notify_type == self.SIGNAL_NOTIFICATION:
                self._handle_notification(notification)
            elif notify_type == self.SIGNAL_DRIVER_REMOVED:
                self._handle_driver_removed(notification)
            elif notify_type == self.SIGNAL_CONTROLLER_COMMAND:
                self._handle_controller_command(notification)
            else:
                print('Skipping unhandled notification [%s]' % notification)
        except:
            import sys, traceback
            print('Error in manager callback : %s' % traceback.format_exception(*sys.exc_info()))

    def wait_for_ready(self):
        for i in range(0,20):
            if self.ready == True:
                break
            else:
                time.sleep(1.0)
        print("Network ready : %s"%self.ready)

    def wait_for_queue(self):
        for i in range(0,60):
            if self.manager.getSendQueueCount(self.homeid) <= 0:
                break
            else:
                time.sleep(0.5)

    def _handle_driver_failed(self, notification):
        self.driver_state = "DriverFailed"
        self.driver_failed = True
        print('Z-Wave Notification DriverFailed : %s' % notification)

    def _handle_driver_ready(self, notification):
        self.driver_state = "DriverReady"
        self.homeid = notification.getHomeId()
        self.driver_ready = True
        print('Z-Wave Notification DriverReady : %s' % notification)

    def _handle_driver_reset(self, notification):
        self.driver_state = "DriverReset"
        self.driver_reset = True
        print('Z-Wave Notification DriverReset : %s' % notification)

    def _handle_driver_removed(self, notification):
        print('Z-Wave Notification DriverRemoved : %s' % notification)
        self.driver_state = "DriverRemoved"
        self.driver_removed = True

    def _handle_group(self, notification):
        print('Z-Wave Notification Group : %s' % notification)

    def _handle_node(self, node):
        print('Z-Wave Notification Node : %s' % node)

    def _handle_node_added(self, notification):
        print('Z-Wave Notification NodeAdded : %s' % notification)
        self.nodes[notification.getNodeId()] = {}

    def _handle_scene_event(self, notification):
        print('Z-Wave Notification SceneEvent : %s' % notification)

    def _handle_node_event(self, notification):
        print('Z-Wave Notification NodeEvent : %s' % notification)

    def _handle_node_naming(self, notification):
        print('Z-Wave Notification NodeNaming : %s' % notification)
        self.nodes[notification.getNodeId()] = {'NodeNaming' : True }

    def _handle_node_new(self, notification):
        print('Z-Wave Notification NodeNew : %s' % notification)

    def _handle_node_protocol_info(self, notification):
        print('Z-Wave Notification NodeProtocolInfo : %s' % notification)

    def _handle_node_removed(self, notification):
        print('Z-Wave Notification NodeRemoved : %s' % notification)

    def _handle_essential_node_queries_complete(self, notification):
        print('Z-Wave Notification EssentialNodeQueriesComplete : %s' % notification)

    def _handle_node_queries_complete(self, notification):
        print('Z-Wave Notification NodeQueriesComplete : %s' % notification)

    def _handle_all_nodes_queried(self, notification):
        self.network_state = "AllNodesQueried"
        self.network_ready = True
        self.ready = True
        print('Z-Wave Notification AllNodesQueried : %s' % notification)

    def _handle_all_nodes_queried_some_dead(self, notification):
        self.network_state = "AllNodesQueriedSomeDead"
        self.network_ready = True
        self.ready = True
        print('Z-Wave Notification AllNodesQueriedSomeDead : %s' % notification)

    def _handle_awake_nodes_queried(self, notification):
        self.network_state = "AwakeNodesQueried"
        self.network_awake = True
        print('Z-Wave Notification AwakeNodesQueried : %s' % notification)

    def _handle_polling_disabled(self, notification):
        print('Z-Wave Notification PollingDisabled : %s' % notification)

    def _handle_polling_enabled(self, notification):
        print('Z-Wave Notification PollingEnabled : %s' % notification)

    def _handle_create_button(self, notification):
        print('Z-Wave Notification CreateButton : %s' % notification)

    def _handle_delete_button(self, notification):
        print('Z-Wave Notification DeleteButton : %s' % notification)

    def _handle_button_on(self, notification):
        print('Z-Wave Notification ButtonOn : %s' % notification)

    def _handle_button_off(self, notification):
        print('Z-Wave Notification ButtonOff : %s' % notification)

    def _handle_value_added(self, notification):
        print('Z-Wave Notification ValueAdded : %s' % notification)

    def _handle_value_changed(self, notification):
        print('Z-Wave Notification ValueChanged : %s' % notification)

    def _handle_value_refreshed(self, notification):
        print('Z-Wave Notification ValueRefreshed : %s' % notification)

    def _handle_value_removed(self, notification):
        print('Z-Wave Notification ValueRemoved : %s' % notification)

    def _handle_notification(self, notification):
        print('Z-Wave Notification : %s' % notification)

    def _handle_controller_command(self, notification):
        print('Z-Wave ControllerCommand : %s' % notification)

    def _handle_msg_complete(self, notification):
        print('Z-Wave Notification MsgComplete : %s' % notification)
