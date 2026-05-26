#!/usr/bin/env python
# --*--coding=utf-8--*--
# import sys
import time
import struct
from PySide6 import QtCore
from PySide6 import QtBluetooth as QtBt
from PySide6.QtWidgets import QMessageBox

class BLEtest:
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.agent = None
        self.controller = None
        self.timer = None
        self.service = None
        self.serviceUUID = list()
        self.gattList = list()
        self.nextServiceIdx = 0
        self.ServiceObject = []
        self.serviceObeList = []
        self.Service = None
        self.characteristicWrite = None
        self.characteristicRead_ = None
        self.notification = None
        self.connState = False
        self.descriptorReadUUID = list()
        self.descriptorWriteUUID = list()
        self.descriptorNotifyUUID = list()
        self.descriptorIndicaUUID = list()
        self.recvDataContent = list()
        self.recvDataUuid = QtBt.QLowEnergyCharacteristic().uuid()
        self.recvDataSerUuid = QtBt.QLowEnergyCharacteristic().uuid()
        self.local = QtBt.QBluetoothLocalDevice().allDevices()
        self.localConnect = QtBt.QBluetoothLocalDevice().connectedDevices()

    def display_status(self):
        # print("display_status..........................................")
        # print(self.agent.isActive(), self.agent.discoveredDevices())
        self.timer.stop()

    def agent_finished(self):
        # print('Agent finished')
        scanList = list()
        for info in self.agent.discoveredDevices():
            scanList.append(info)
        if self.scanFCb:
            self.scanFCb(scanList)

    def start_connect(self, devInfo, connCb, recvDataCb, disConnCb, serviceErrorCb):
        self.connFinCb = connCb
        self.recvDataCb = recvDataCb
        self.disConnCb = disConnCb
        self.serviceErrorCb = serviceErrorCb

        self.controller = QtBt.QLowEnergyController.createCentral(devInfo)
        self.controller.connected.connect(self.connect_Notify)
        self.controller.errorOccurred.connect(self.controller_error)
        self.controller.disconnected.connect(self.disconnect_Notify)
        self.controller.serviceDiscovered.connect(self.addService)
        self.controller.discoveryFinished.connect(self.dis_Finished)
        self.controller.connectToDevice()

    def start_disconnect(self):
        if self.controller:
            self.controller.disconnectFromDevice()

    def controller_error(self, e):
        error = ["NoError", "UnknownError", "UnknownRemoteDeviceError", "NetworkError", "InvalidBluetoothAdapterError",
                 "ConnectionError"]

        if e.value < 6:
            # print(error[e.value])
            if error[e.value] == 'ConnectionError':
                self.disConnCb()

    def connect_Notify(self, *args, **kwargs):
        # print(f'connect notification.')
        # print(f'args: {args}')
        # print(f'kwargs: {kwargs}')
        self.serviceUUID = list()
        self.gattList = list()
        self.descriptorReadUUID = list()
        self.descriptorWriteUUID = list()
        self.descriptorNotifyUUID = list()
        self.descriptorIndicaUUID = list()
        self.controller.discoverServices()

    def disconnect_Notify(self, *args, **kwargs):
        if self.disConnCb:
            self.disConnCb()

    def addService(self, uuid: QtBt.QBluetoothUuid):
        # print('Service discovered')
        self.serviceUUID.append(uuid)

    def dis_Finished(self):
        # print(f'Service discovered finish.')
        time.sleep(1)
        self.nextServiceIdx = 0
        try:
            uuid = self.serviceUUID[0]
            serObj = self.controller.createServiceObject(uuid)
            self.ServiceObject.append(serObj)

            if self.ServiceObject[0] is None:
                QMessageBox.warning(self, 'Connect Service', "Connect server fail!")
            else:
                gattInfo = {}
                gattInfo['gattStr'] = 'services: uuid:' + uuid.toString() + '  ' + self.ServiceObject[0].serviceName()
                gattInfo['uuid'] = uuid
                gattInfo['valueStr'] = ''
                self.gattList.append(gattInfo)
                self.ServiceObject[0].stateChanged.connect(self.state_Changed)
                self.ServiceObject[0].characteristicWritten.connect(self.characteristic_Written)
                self.ServiceObject[self.nextServiceIdx].characteristicChanged.connect(self.characteristic_Changed)
                self.ServiceObject[0].errorOccurred.connect(self.service_Error)
                self.ServiceObject[0].discoverDetails()
        except AttributeError as e:
            # print(e)
            QMessageBox.warning(self, 'Connect Service', e.args[0])    

    def characteristic_Written(self, info: QtBt.QLowEnergyCharacteristic, value):
        return

    def characteristic_Changed(self, info: QtBt.QLowEnergyCharacteristic, value):
        for gattInfo in self.gattList:
            if gattInfo['uuid'] == info.uuid():
                gattInfo['gattStr'] = '  characteristic: uuid:' + info.uuid().toString() + ' ' \
                + self.get_pro_type_str(info) + ' value:0x'
                gattInfo['valueStr'] = ''

                self.recvDataContent = struct.unpack("!%dB" % len(value.data()), value.data())
                for hvol in self.recvDataContent:
                    hhex = '%02x' % hvol
                    gattInfo['valueStr'] += hhex + ' '
                self.recvDataUuid = info.uuid()
                self.recvDataSerUuid = gattInfo['serviceuuid']
                if self.recvDataCb:
                    self.recvDataCb()
                return

    def service_Error(self, error):
        ServiceError = ["NoError", "OperationError", "CharacteristicWriteError", "DescriptorWriteError", "UnknownError",
                        "CharacteristicReadError", "DescriptorReadError"]
        if self.connFinCb:
            self.connFinCb()

    def discover_next_service(self):
        tryCnt = 0
        self.nextServiceIdx += 1
        try:
            for uuid in self.serviceUUID:
                # print('tryCnt:%d, nextServiceIdx;%d' % (tryCnt, self.nextServiceIdx))
                if tryCnt == self.nextServiceIdx:
                    serObj = self.controller.createServiceObject(uuid)
                    self.ServiceObject.append(serObj)
                    if self.ServiceObject[self.nextServiceIdx] is None:
                        QMessageBox.warning(self, 'Connect Service', "Connect server fail!")
                    else:
                        gattInfo = {}
                        gattInfo['gattStr'] = 'services: uuid:' + uuid.toString() + '  ' + self.ServiceObject[self.nextServiceIdx].serviceName()
                        gattInfo['uuid'] = uuid
                        gattInfo['valueStr'] = ''
                        self.gattList.append(gattInfo)
                        self.ServiceObject[self.nextServiceIdx].stateChanged.connect(self.state_Changed)
                        self.ServiceObject[self.nextServiceIdx].characteristicWritten.connect(self.characteristic_Written)
                        self.ServiceObject[self.nextServiceIdx].errorOccurred.connect(self.service_Error)
                        self.ServiceObject[self.nextServiceIdx].characteristicChanged.connect(self.characteristic_Changed)
                        self.ServiceObject[self.nextServiceIdx].discoverDetails()
                    break
                tryCnt +=1
        except AttributeError as e:
            # print(e)
            # print("AttributeError")
            QMessageBox.warning(self, 'Connect Service', e.args[0])

    def get_pro_type_str(self, ch):
        typeint = ch.properties()
        proStr = ''
        if typeint & QtBt.QLowEnergyCharacteristic.PropertyType.Broadcasting:
            proStr += 'Broadcasting '
        if typeint & QtBt.QLowEnergyCharacteristic.PropertyType.Read:
            self.descriptorReadUUID.append(ch)
            proStr += 'Read '
        if typeint & QtBt.QLowEnergyCharacteristic.PropertyType.WriteNoResponse:
            self.descriptorWriteUUID.append(ch)
            proStr += 'WriteNoResponse '
        if typeint & QtBt.QLowEnergyCharacteristic.PropertyType.Write:
            self.descriptorWriteUUID.append(ch)
            proStr += 'Write '
        if typeint & QtBt.QLowEnergyCharacteristic.PropertyType.Notify:
            self.descriptorNotifyUUID.append(ch)
            proStr += 'Notify '
        if typeint & QtBt.QLowEnergyCharacteristic.PropertyType.Indicate:
            self.descriptorIndicaUUID.append(ch)
            proStr += 'Indicate '
        if typeint & QtBt.QLowEnergyCharacteristic.PropertyType.WriteSigned:
            proStr += 'WriteSigned '
        if typeint & QtBt.QLowEnergyCharacteristic.PropertyType.ExtendedProperty:
            proStr += 'ExtendedProperty '
        if proStr is None:
            proStr += 'Unknown '
        return proStr

    def state_Changed(self, s):
        if s == QtBt.QLowEnergyService.DiscoveringService:
            # print(f"Discovering services...")
            return
        elif s == QtBt.QLowEnergyService.ServiceDiscovered:
            # print(f"Service discovered.")
            for ch in self.ServiceObject[self.nextServiceIdx].characteristics():
                gattInfo = {}
                gattInfo['gattStr'] = '  characteristic: uuid:' + ch.uuid().toString() + ' ' + self.get_pro_type_str(ch) + ' value:0x'

                gattInfo['valueStr'] = ''
                recvData = struct.unpack("!%dB" % len(ch.value().data()), ch.value().data())
                
                for hvol in recvData:
                    hhex = '%02x' % hvol
                    gattInfo['valueStr'] += hhex + ' '

                gattInfo['uuid'] = ch.uuid()
                gattInfo['serviceuuid'] = self.ServiceObject[self.nextServiceIdx].serviceUuid()
                self.gattList.append(gattInfo)

                if ch.properties() & QtBt.QLowEnergyCharacteristic.PropertyType.Read:
                    self.ServiceObject[self.nextServiceIdx].characteristicRead.connect(self.characteristic_Read_ret)

                for desc in ch.descriptors():
                    gattInfo = {}
                    gattInfo['gattStr'] = '      descriptors: uuid' + desc.uuid().toString() + 'value:0x'
                    gattInfo['uuid'] = desc.uuid()
                    gattInfo['serviceuuid'] = self.ServiceObject[self.nextServiceIdx].serviceUuid()
                    descData = struct.unpack("!%dB" % len(desc.value().data()), desc.value().data())
                    gattInfo['valueStr'] = ''
                    for hvol in descData:
                        hhex = '%02x' % hvol
                        gattInfo['valueStr'] += hhex + ' '
                    self.gattList.append(gattInfo)
                   
            if self.nextServiceIdx + 1 < len(self.serviceUUID):
                time.sleep(1)
                self.discover_next_service()
            else:
                if self.connFinCb:
                    self.connFinCb()

    def characteristic_Read_ret(self, info: QtBt.QLowEnergyCharacteristic, value):
        # print(f'characteristic_Read_ret')
        for gattInfo in self.gattList:
            if gattInfo['uuid'] == info.uuid():
                gattInfo['gattStr'] = '  characteristic: uuid:' + info.uuid().toString() \
                + ' ' + self.get_pro_type_str(info) + ' value:0x'
                gattInfo['valueStr'] = ''
                self.recvDataContent = struct.unpack("!%dB" % len(value.data()), value.data())
                for hvol in self.recvDataContent:
                    hhex = '%02x' % hvol
                    gattInfo['valueStr'] += hhex + ' '
                self.recvDataUuid = info.uuid()
                self.recvDataSerUuid = gattInfo['serviceuuid']
                if self.recvDataCb:
                    self.recvDataCb()
                return

    def init_recv_info(self):
        self.recvDataContent = []
        self.recvDataSerUuid = QtBt.QLowEnergyCharacteristic().uuid()
        self.recvDataUuid = QtBt.QLowEnergyCharacteristic().uuid()

    def creat_read_characteristic(self, uuid):  
        self.init_recv_info()
        for info in self.descriptorReadUUID:
            if info.uuid() == uuid:
                break
        else:
            return 1
        
        try:
            for gettInfo in self.gattList:
                if uuid == gettInfo['uuid']:
                    serviceUuid = gettInfo['serviceuuid']
                    # print('read service uuid:')
                    # print(serviceUuid.toString())
                    break
            else:
                return 2

            for serObj in self.ServiceObject:
                if serviceUuid == serObj.serviceUuid():
                    self.nextServiceIdx = self.ServiceObject.index(serObj)

            if self.ServiceObject[self.nextServiceIdx]:
                self.characteristicRead = self.ServiceObject[self.nextServiceIdx].characteristic(uuid)
                # print(f'isValid:{self.characteristicRead.isValid()}')
                if self.characteristicRead.isValid():
                    self.ServiceObject[self.nextServiceIdx].readCharacteristic(self.characteristicRead)
                else:
                    return 3
        except Exception as e:
            # print(e)
            return 4
        return 0

    def creat_notification_characteristic(self, uuid, enable):
        self.init_recv_info()
        try:
            for gettInfo in self.gattList:
                if uuid == gettInfo['uuid']:
                    serviceUuid = gettInfo['serviceuuid']
                    break
            else:
                return 2

            for serObj in self.ServiceObject:
                if serviceUuid == serObj.serviceUuid():
                    self.nextServiceIdx = self.ServiceObject.index(serObj)

            return self.create_read_notify(uuid, enable)
        except Exception as e:
            # print(e)
            return 4

    def create_write_characteristic(self, uuid, inputData):
        for info in self.descriptorWriteUUID:
            if info.uuid() == uuid:
                break
        else:
            return 1

        try:
            for gettInfo in self.gattList:
                if uuid == gettInfo['uuid']:
                    serviceUuid = gettInfo['serviceuuid']
                    break
            else:
                return 2
            
            for serObj in self.ServiceObject:
                if serviceUuid == serObj.serviceUuid():
                    self.nextServiceIdx = self.ServiceObject.index(serObj)

            if self.ServiceObject[self.nextServiceIdx]:
                self.characteristicWrite = self.ServiceObject[self.nextServiceIdx].characteristic(uuid)
                if self.characteristicWrite.isValid():
                    if self.characteristicWrite.properties() & QtBt.QLowEnergyCharacteristic.PropertyType.Write:
                        self.ServiceObject[self.nextServiceIdx].writeCharacteristic(self.characteristicWrite, bytes(inputData), QtBt.QLowEnergyService.WriteWithResponse)
                    elif self.characteristicWrite.properties() & QtBt.QLowEnergyCharacteristic.PropertyType.WriteNoResponse:
                        self.ServiceObject[self.nextServiceIdx].writeCharacteristic(self.characteristicWrite, bytes(inputData), QtBt.QLowEnergyService.WriteWithoutResponse)
                else:
                    return 3
        except Exception as e:
            return 4
        return 0

    def create_read_notify(self, uuid: QtBt.QBluetoothUuid, enable):
        notifyFlag = False
        if self.ServiceObject[self.nextServiceIdx]:
            self.characteristicRead_ = self.ServiceObject[self.nextServiceIdx].characteristic(uuid)

            if self.characteristicRead_.descriptors():
                self.notification = self.characteristicRead_.descriptors()[0]
                if not self.notification.isValid():
                    notifyFlag = False
                else:
                    notifyFlag = True

            if notifyFlag:
                valueSetStr = ''
                if enable == True:
                    # 写0x01,0x00启用监听服务
                    self.ServiceObject[self.nextServiceIdx].writeDescriptor(self.notification, bytes.fromhex('0100'))
                    valueSetStr = '01 00 '
                else:
                    # 写0x01,0x00启用监听服务
                    self.ServiceObject[self.nextServiceIdx].writeDescriptor(self.notification, bytes.fromhex('0000'))
                    valueSetStr = '00 00 '
                for info in self.gattList:
                    for desc in self.characteristicRead_.descriptors():
                        if info['uuid'] == desc.uuid():
                            if info['valueStr'] != valueSetStr:
                                info['valueStr'] = valueSetStr
                                if self.recvDataCb:
                                    self.recvDataCb()
                            break
            else:
                return 2
        return 0

    def agent_error(self, e):
        error = ["NoError", "InputOutputError", "PoweredOffError", "InvalidBluetoothAdapterError", "UnknownError"]
        if e.value < 4:
            QMessageBox.warning(self, 'Connect Service', error[e.value])
        else:
            QMessageBox.warning(self, 'Connect Service', error[4])

    def scan_for_devices(self, cb):
        self.scanFCb = cb
        self.agent = QtBt.QBluetoothDeviceDiscoveryAgent()
        self.agent.setLowEnergyDiscoveryTimeout(3000)

        self.agent.errorOccurred.connect(self.agent_error)
        self.agent.finished.connect(self.agent_finished)

        self.timer = QtCore.QTimer(self.agent)
        self.timer.start(5)
        self.timer.timeout.connect(self.display_status)

        self.addr = QtBt.QBluetoothSocket().localAddress().toString()

        self.agent.start(self.agent.DiscoveryMethod.LowEnergyMethod)
