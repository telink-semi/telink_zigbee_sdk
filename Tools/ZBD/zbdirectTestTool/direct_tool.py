from asyncio.windows_events import NULL
from pickle import TRUE
import sys
import struct
from PySide6 import QtWidgets
from PySide6.QtWidgets import QMessageBox
from PySide6.QtCore import QTimer
from PySide6.QtGui import QFont
from bletestwindow import Ui_MainWindow
from qtbluetooth_test import BLEtest
from datetime import datetime
from session_build import SessBuild
from commissioning import CommHandle
from commissioning import tlv_construct
from tunnel import TunnelHandle
from scanInfo_show import ScanLayout


def line_edit_str2int(input_s_orial):
    # print(input_s)
    send_list = []
    convert_cnt = 0
    input_s_1 = input_s_orial.replace(':','')
    input_s = input_s_1.replace('-','')
    if input_s != '':
        input_s = input_s.strip()  # 把前后的空格去掉
        while input_s != '':
            try:
                if len(input_s) < 2:
                    num = int(input_s[0:1], 16)
                else:
                    num = int(input_s[0:2], 16)
            except ValueError as e:
                # print(e)
                break
            input_s = input_s[2:].strip()  # 把前后的空格去掉
            # print(input_s)
            send_list.append(num)
            convert_cnt += 1

    return send_list, convert_cnt

def convert_str_to_128(uuidStr):
    # print(input_s)
    send_list = []
    convert_cnt = 0
    input_s = uuidStr.replace('-','')
    input_s = input_s.replace('{','')
    input_s = input_s.replace('}','')

    if input_s != '':
        input_s = input_s.strip()  # 把前后的空格去掉
        while input_s != '':
            try:
                if len(input_s) < 2:
                    num = int(input_s[0:1], 16)
                else:
                    num = int(input_s[0:2], 16)
            except ValueError:
                # print(e)
                break
            input_s = input_s[2:].strip()  # 把前后的空格去掉
            # print(input_s)
            send_list.append(num)
            convert_cnt += 1
    return send_list, convert_cnt    


class Blewindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        super(Blewindow, self).__init__()
        self.setupUi(self)  # 初始化UI到主窗口，主要是建立代码到UI之间的signal和slot
        self.setWindowTitle("Zigbee Direct Test Tool")
        self.scanInfoCnt = 0
        self.choose_attr = {}
        self.bleHand = BLEtest()
        self.session = SessBuild()
        self.comm = CommHandle()
        self.tunnel = TunnelHandle()
        self.tunnel.zvd_tunnel_send_cb(self.send_tunnel_data)
        self.layoutFn = ScanLayout()
        self.recvLastUuidStr = ''
        self.init()

    def parse_recv(self):
        self.recvLastUuidStr = ''
        self.checkRecvTimer.stop()

    def connect_timeout_cb(self):
        self.bleHand.start_disconnect()

    def send_tunnel_link_status_cb(self):
        self.tunnel.zvd_send_link_status()

    def init(self):
        self.checkRecvTimer = QTimer(self)
        self.checkRecvTimer.timeout.connect(self.parse_recv)

        self.checkConnTimer = QTimer(self)
        self.checkConnTimer.timeout.connect(self.connect_timeout_cb)

        self.sendTunnelLinkStatusTimer = QTimer(self)
        self.sendTunnelLinkStatusTimer.timeout.connect(self.send_tunnel_link_status_cb)
        self.window_init()

    def conn_finish_cb(self):
        # print('conn finish.........')
        # time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        # print(time)
        self.label_connState.setText('Connect State: TRUE(' + self.layoutFn.dev_choose['name'] +'  ' + self.layoutFn.dev_choose['address'] + ')')
        self.label_connState.setStyleSheet("color:green")
        self.bleHand.connState = True

        self.listWidget_gatt.clear()
        if self.bleHand.gattList:
            for info in self.bleHand.gattList:
                if 'services:' in info['gattStr']:
                    listItem = QtWidgets.QListWidgetItem()
                    listItem.setText(info['gattStr'] + info['valueStr'])
                    nameFont = QFont()
                    nameFont.setPointSize(12)
                    listItem.setFont(nameFont)
                    self.listWidget_gatt.addItem(listItem)
                else:
                    self.listWidget_gatt.addItem(info['gattStr'] + info['valueStr'])
        # print(self.bleHand.recvDataUuid.toString())
        # print(';;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;')
        self.checkConnTimer.stop()

    def recv_data_cb(self):
        # print('recv_data_cb.........')
    
        if self.recvLastUuidStr and self.recvLastUuidStr == self.bleHand.recvDataSerUuid.toString():
            return
        self.recvLastUuidStr = self.bleHand.recvDataSerUuid.toString()
        self.checkRecvTimer.start(5)
        
        recvDataDecry = False
        recv_data_str = ''
        if self.bleHand.recvDataSerUuid.toString() == self.session.securityServiceStr:
            for hvol in self.bleHand.recvDataContent:
                hhex = '%02x' % hvol
                recv_data_str += hhex + ' '
            dt = datetime.now()
            timestr = dt.strftime('%y-%m-%d %H:%M:%S.%f')
            self.textEdit_dataSecurity.append(timestr + ' recv<--:' + recv_data_str + '\n')

            self.session.recv_data_handle(self.bleHand.recvDataContent)
            if self.session.sessionFlag == True:
                self.label_sessionState.setText('Session Build State:TRUE' + self.session.sessionType)
                self.label_sessionState.setStyleSheet("color:green")  
                if self.session.sessionType == '(Basic level authorization)' and self.tunnel.zvdJoinState == True \
                    and self.tunnel.sendDevAnnce == False:
                    if self.tunnel.zvdNwkAddr != 0x0000:
                        self.tunnel.zvd_send_device_announce()
                    self.sendTunnelLinkStatusTimer.start(15000)
                    self.tunnel.sendDevAnnce = True
                self.session.sessionFlag = False  
        elif self.bleHand.recvDataSerUuid.toString() == self.comm.commServiceStr or \
            self.bleHand.recvDataSerUuid.toString() == self.tunnel.tunnelServiceStr:
            serviceUuid, serUuidLen = convert_str_to_128(self.bleHand.recvDataSerUuid.toString())
            charaUuid, chaUuidLen = convert_str_to_128(self.bleHand.recvDataUuid.toString())
            if serUuidLen != 16 and chaUuidLen != 16:
                QMessageBox.warning(self, 'Attr Read', "Fail to get service uuid or characteristic uuid!")
                return
            ret, recvData = self.session.decrypt_data(serviceUuid, charaUuid, self.bleHand.recvDataContent)
            if ret == 1:
                QMessageBox.warning(self, 'Attr Read', "Fail to decrypt the received data")
                return
            elif ret == 2:
                QMessageBox.warning(self, 'Attr Read', "The incoming framecounter verify failure!")
                return
            elif ret == 3:
                QMessageBox.warning(self, 'Attr Read', "Error length!")
                return
            recvDataDecry = True
            for hvol in recvData:
                hhex = '%02x' % hvol
                recv_data_str += hhex + ' '
            dt = datetime.now()
            timestr = dt.strftime('%y-%m-%d %H:%M:%S.%f')
            if self.bleHand.recvDataSerUuid.toString() == self.comm.commServiceStr:
                self.textEdit_dataComm.append(timestr + ' recv<--:' + recv_data_str + '\n')
            else:               
                self.textEdit_tunnelData.append(timestr + ' recv<--:' + recv_data_str + '\n')

                ret = self.tunnel.recv_data_handle(recvData)

                if self.session.sessionType == '(Basic level authorization)' and self.tunnel.zvdJoinState == True \
                    and self.tunnel.sendDevAnnce == False:
                    if self.tunnel.zvdNwkAddr != 0x0000:
                        self.tunnel.zvd_send_device_announce()
                    self.sendTunnelLinkStatusTimer.start(15000)
                    self.tunnel.sendDevAnnce = True
                
                if ret == 2:
                    QMessageBox.warning(self, 'Attr Read', "Recvive commissioning response status if failure!")
                elif ret == 3:
                    QMessageBox.warning(self, 'Attr Read', "ZVD joined the network successfully!")
                # elif ret == 0:
                #     print('Receive other network command.')

        rownum = 0
        if self.bleHand.gattList:
            for info in self.bleHand.gattList:
                if self.bleHand.recvDataUuid == info['uuid']:
                    if recvDataDecry == True:
                        info['valueStr'] = recv_data_str
                    self.listWidget_gatt.takeItem(rownum)
                    self.listWidget_gatt.insertItem(rownum, info['gattStr'] + info['valueStr'])
                    self.listWidget_gatt.setCurrentRow(rownum)
                    break
                rownum += 1
        self.bleHand.init_recv_info()
        # print('aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa')
            
    def disconnect_cb(self):
        # time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        # print(time)
        try:
            self.label_connState.setText('Connect State: FALSE')
            self.label_connState.setStyleSheet("color:red")
            self.bleHand.connState = False
            self.sendTunnelLinkStatusTimer.stop()

            self.label_sessionState.setText('Session Build State: FALSE')
            self.label_sessionState.setStyleSheet("color:red")
            QMessageBox.warning(self, 'BLE Connect State', "Device disconnected!")
            self.bleHand = BLEtest()
            self.session = SessBuild()
            self.tunnel = TunnelHandle()
            self.tunnel.zvd_tunnel_send_cb(self.send_tunnel_data)
        except Exception as e:
            # print(e)
            QMessageBox.warning(self, 'BLE Connect State', e.args[0])
    
    def service_error_cb(self, errorStr):
        QMessageBox.warning(self, 'Service Error', errorStr)

    def start_connect_ble(self): 
        # print('start conn:::::::')
        # time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        # print(time)
        if self.bleHand.connState == True:
            QMessageBox.warning(self, 'Connect BLE Device', "A BLE connection already exists!")

        if self.scanInfoCnt == 0:
            QMessageBox.warning(self, 'Connect BLE Device', "You should scan the devices first!")
        elif len(self.layoutFn.dev_choose) == 0:
            QMessageBox.warning(self, 'Connect BLE Device', "You should choose the device you want to connect!")
        else:
            # print(self.layoutFn.dev_choose)
            self.label_connState.setText('Connect State: FALSE  Connecting....')
            self.label_connState.setStyleSheet("color:red")
            self.bleHand.connState = False

            self.bleHand.start_connect(self.layoutFn.dev_choose['value'], self.conn_finish_cb, self.recv_data_cb, self.disconnect_cb, self.service_error_cb)
            self.layoutFn.close()
            self.checkConnTimer.start(30000)
      
    def ble_scan_finish(self, scanList):
        scanInfo = []
        scanInfoList = []

        self.label_connState.setText('Connect State: FALSE')
        self.label_connState.setStyleSheet("color:red")
        self.bleHand.connState = False
        
        for dev in scanList:
            devInfo = {}
            devInfo['address'] = dev.address().toString()
            devInfo['name'] = dev.name()
            devInfo['rssi'] = dev.rssi()
            devInfo['value'] = dev
            scanInfo.append(devInfo)
            self.scanInfoCnt += 1
            # print(devInfo)
        
        if scanInfo is not None:
            scanInfoList = sorted(scanInfo, key=lambda x:(x['rssi']), reverse=True)
            self.layoutFn.showInfo(scanInfoList)
            self.layoutFn.show()
        else:
            QMessageBox.warning(self, 'Ble scan', "Not find ble devices！")
        
    def start_ble_scan(self):
        self.bleHand.start_disconnect()
        self.listWidget_gatt.clear()
        self.label_connState.setText('Connect State: FALSE   Scanning....')
        self.label_connState.setStyleSheet("color:red")
        self.bleHand.connState = False
        self.scanInfoCnt = 0
        self.bleHand.scan_for_devices(self.ble_scan_finish)
    
    def choose_gatt_handle(self, item):
        # print("choose_gatt_handle")
        # print(item.text())
       
        try:
            index = self.listWidget_gatt.currentIndex().row()
            self.choose_attr = self.bleHand.gattList[index]
        except Exception as e:
            # print(e)
            QMessageBox.warning(self, 'Choose Attr', e.args[0])

    def ble_gatt_write(self):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'Attr Write', "No BLE connection!")
            return
        if self.choose_attr:
            sendStr = self.lineEdit_attrWrite.text()
            sendData, dataLen = line_edit_str2int(sendStr)
            # if dataLen == 0:
            #     QMessageBox.warning(self, 'Attr Write', "No data to write!")
            #     return
            
            if self.choose_attr['serviceuuid'].toString() == self.comm.commServiceStr or \
                self.choose_attr['serviceuuid'].toString() == self.tunnel.tunnelServiceStr:
                self.send_chara_encrypt_data(self.choose_attr['uuid'].toString(), sendData)
            else:
                payload = struct.pack("!%dB" % dataLen, *sendData)

                writeRet = self.bleHand.create_write_characteristic(self.choose_attr['uuid'], payload)
                if writeRet == 1:
                    QMessageBox.warning(self, 'Attr Write', "The charcateristic is not allow to write!")
                elif writeRet != 0:
                    QMessageBox.warning(self, 'Attr Write', "Failed to write charcateristic!")
        else:
            QMessageBox.warning(self, 'Attr Write', "Please select the characteristic you want to write!")
        
    def ble_gatt_read(self):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'Attr Read', "No BLE connection!")
            return
        if self.choose_attr:
            readRet = self.bleHand.creat_read_characteristic(self.choose_attr['uuid'])
            if readRet == 1:
                QMessageBox.warning(self, 'Attr Read', "The charcateristic is not allow to read!")
            elif readRet != 0:
                QMessageBox.warning(self, 'Attr Read', "Failed to read charcateristic!")
        else:
            QMessageBox.warning(self, 'Attr Read', "Please select the characteristic you want to read!")

    def notify_handle(self, enable):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'Attr Notification', "No BLE connection!")
            return
        if self.choose_attr:
            readRet = self.bleHand.creat_notification_characteristic(self.choose_attr['uuid'], enable)
            if readRet == 1:
                QMessageBox.warning(self, 'Attr Notification', "The charcateristic is not allow to handle!")
            elif readRet != 0:
                QMessageBox.warning(self, 'Attr Notification', "Failed to read charcateristic!")
        else:
            QMessageBox.warning(self, 'Attr Notification', "Please select the characteristic you want to handle!")

    def disconnect_ble(self):
        self.bleHand.start_disconnect()
        self.listWidget_gatt.clear()
        self.label_connState.setText('Connect State: FALSE')
        self.label_connState.setStyleSheet("color:red")
        self.bleHand.connState = False
        self.scanInfoCnt = 0
        return

    def enable_notify_handle(self):
        self.notify_handle(True)
        return

    def disable_notify_handle(self):
        self.notify_handle(False)
        return

    def session_security_data_send(self):
        if len(self.session.sendData):
            send_data_str = ''
            for hvol in self.session.sendData:
                hhex = '%02x' % hvol
                send_data_str += hhex + ' '
            dt = dt = datetime.now()
            timestr = dt.strftime('%y-%m-%d %H:%M:%S.%f')
            self.textEdit_dataSecurity.append(timestr + ' send-->:' + send_data_str + '\n')
            writeRet = self.bleHand.create_write_characteristic(self.choose_attr['uuid'], self.session.sendData)
            if writeRet == 1:
                QMessageBox.warning(self, 'Attr Write', "The charcateristic is not allow to write!")
            elif writeRet != 0:
                QMessageBox.warning(self, 'Attr Write', "Failed to write charcateristic!")

    def start_session_build(self):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'Session Start', "No BLE connection!")
            return
        self.session.keyNegoMethod = self.comboBox_keyMethod.currentIndex() + 1
        findCharaStr = ''
        if self.session.keyNegoMethod == 1: #ECDHE_CURVE25519_HASH_AESMMO128
            findCharaStr = self.session.securityCharaStr1
        else:
            findCharaStr = self.session.securityCharaStr2
        for info in self.bleHand.gattList:
            if info['uuid'].toString() == findCharaStr:
                self.choose_attr = info
                break
        else:
            QMessageBox.warning(self, 'Attr Write', "No security service to write!")
            return

        if self.comboBox_derived.currentIndex() == 0:
            self.session.whetherDerived = True
        else:
            self.session.whetherDerived = False
        
        self.session.presharedSecretType = self.comboBox_keyEnum.currentIndex()
        if self.session.presharedSecretType == 5:
            self.session.presharedSecretType = 0xff
        secretInfo_str = self.lineEdit_preKey.text()
        self.session.presharedSecret = []
        if secretInfo_str:
            self.session.presharedSecret, keyLen = line_edit_str2int(secretInfo_str)
            if keyLen != 16:
                QMessageBox.warning(self, 'Session Start', "Misformatted pre-shared secret key!")
                return
        networkKeySeqNum_str = self.lineEdit_keySeqNum.text()
        self.session.nwkSeqNumSetFlag = False
        if networkKeySeqNum_str:
            seqNum, seqNumLen= line_edit_str2int(networkKeySeqNum_str)
            if seqNumLen != 1:
                QMessageBox.warning(self, 'Session Start', "Misformatted network seqNum!")
                return
            else:
                self.session.nwkSeqNum = seqNum[0]
                self.session.nwkSeqNumSetFlag = True
        if self.checkBox_test.isChecked():
            self.session.testVector = True
        else:
            self.session.testVector = False
        self.session.session_start(self.session_security_data_send)

    def session_build_data_clear(self):
        self.textEdit_dataSecurity.clear()

    def commi_data_clear(self):
        self.textEdit_dataComm.clear()

    def tunnel_data_clear(self):
        self.textEdit_tunnelData.clear()

    def send_tunnel_data(self, tunnelData):
        self.send_chara_encrypt_data(self.tunnel.tunnelCharaStr, tunnelData)

    def tunnel_zvd_join(self):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'Tunnel ZVD Join', "No BLE connection!")
            return
        parentIeee_str = self.lineEdit_parentIeee.text()
        if parentIeee_str:
            self.tunnel.parentIeeeAddr, ieeeLen = line_edit_str2int(parentIeee_str)
            if ieeeLen != 8:
                QMessageBox.warning(self, 'Tunnel ZVD Join', "Misformatted parent ieee address to set!")
                return
        else:
            QMessageBox.warning(self, 'Tunnel ZVD Join', "You should set parent's ieee address!")
            return

        parentshort_str = self.lineEdit_parentshort.text()
        if parentshort_str:
            self.tunnel.parentNwkAddr, shortLen = line_edit_str2int(parentshort_str)
            if shortLen != 2:
                QMessageBox.warning(self, 'Tunnel ZVD Join', "Misformatted parent short address to set!")
                return
        else:
            QMessageBox.warning(self, 'Tunnel ZVD Join', "You should set parent's short address!")
            return

        zvdIeee_str = self.lineEdit_zvdIeee.text()
        if zvdIeee_str:
            self.tunnel.zvdIeeeAddr, ieeeLen = line_edit_str2int(zvdIeee_str)
            if ieeeLen != 8:
                QMessageBox.warning(self, 'Tunnel ZVD Join', "Misformatted ieee address to set!")
                return
        
        zvdNwkAddr_str = self.lineEdit_zvdNwkAddr.text()
        if zvdNwkAddr_str:
            self.tunnel.zvdNwkAddr, nwkAddrLen = line_edit_str2int(zvdNwkAddr_str)
            if nwkAddrLen != 2:
                QMessageBox.warning(self, 'Tunnel ZVD Join', "Misformatted nwk address to set!")
                return
        if not zvdIeee_str or not zvdNwkAddr_str:
            QMessageBox.warning(self, 'Tunnel ZVD Join', "Use the default parameter to join into the network!")
        self.tunnel.zvd_start_join_network()

    def tunnel_zvd_leave(self):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'Tunnel ZVD Join', "No BLE connection!")
            return
        if self.tunnel.zvdJoinState == True:
            self.tunnel.zvd_leave_network()
            self.tunnel.zvdJoinState = False
            self.sendTunnelLinkStatusTimer.stop()
        else:
            QMessageBox.warning(self, 'Tunnel ZVD Leave', "No ZVD device join into the network!")
  
    def tunnel_zvd_onoff_cmd(self):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'Tunnel ZVD Onoff', "No BLE connection!")
            return
        if self.tunnel.zvdJoinState == False:
            QMessageBox.warning(self, 'Tunnel ZVD Onoff', "No ZVD device join into the network!")
            return

        zvdNwkAddr_str = self.lineEdit_onoffAddr.text()
        if zvdNwkAddr_str:
            self.tunnel.sendShortAddr, nwkAddrLen = line_edit_str2int(zvdNwkAddr_str)
            if nwkAddrLen != 2:
                QMessageBox.warning(self, 'Tunnel ZVD Onoff', "Misformatted nwk address to set!")
                return
        else:
            QMessageBox.warning(self, 'Tunnel ZVD Onoff', "You should set the destination short address!")
            return

        self.tunnel.onoffCmdId = self.comboBox_commId.currentIndex()
        self.tunnel.send_onoff_cmd()

    def send_chara_encrypt_data(self, charaUuidStr, dataInput):
        for info in self.bleHand.gattList:
            # print(info['uuid'].toString())
            if charaUuidStr.lower() == info['uuid'].toString():
                self.choose_attr = info
                break
        else:
            QMessageBox.warning(self, 'Commission Write', "No characteristic to write!")
            return

        send_data_str = ''
        for hvol in dataInput:
            hhex = '%02x' % hvol
            send_data_str += hhex + ' '
        dt = datetime.now()
        timestr = dt.strftime('%y-%m-%d %H:%M:%S.%f')
        if self.choose_attr['serviceuuid'].toString() == self.comm.commServiceStr:
            self.textEdit_dataComm.append(timestr + ' send-->:' + send_data_str + '\n')
        elif self.choose_attr['serviceuuid'].toString() == self.tunnel.tunnelServiceStr:
            self.textEdit_tunnelData.append(timestr + ' send-->:' + send_data_str + '\n')

        serviceUuid = self.choose_attr['serviceuuid']
        charaUuid = self.choose_attr['uuid']

        serviceUuid, serUuidLen = convert_str_to_128(self.choose_attr['serviceuuid'].toString())
        charaUuid, chaUuidLen = convert_str_to_128(self.choose_attr['uuid'].toString())
        if serUuidLen != 16 and chaUuidLen != 16:
            QMessageBox.warning(self, 'Attr Write', "Fail to get service uuid or characteristic uuid!")
            return

        enRet, enData = self.session.encrypt_data(serviceUuid, charaUuid, dataInput)
        if enRet != 0:
            QMessageBox.warning(self, 'Attr Write', "Fail to encrypt the write data!")
            return
        # print("enData:%d" % len(enData))
        # print(enData)
        payload = struct.pack("!%dB" % len(enData), *enData)

        # print(self.choose_attr['uuid'].toString())
        writeRet = self.bleHand.create_write_characteristic(self.choose_attr['uuid'], payload)
        if writeRet == 1:
            QMessageBox.warning(self, 'Attr Write', "The charcateristic is not allow to write!")
        elif writeRet != 0:
            QMessageBox.warning(self, 'Attr Write', "Failed to write charcateristic!")        
    
    def commi_form_network(self):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'From Network', "No BLE connection!")
            return

        formData = []
        dataInputStr = self.lineEdit_adminKey.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.adminKeyLen:
                QMessageBox.warning(self, 'From Network', "Misformatted admin Key to set!")
                return
            else:
                self.session.adminKeyPre = []
                for i in range(16):
                    self.session.adminKeyPre.append(dataInput[i])  
                # print(f'adminKeyPre:{self.session.adminKeyPre}')
                formData.extend(tlv_construct(self.comm.adminKeyTlvId, dataInput, dataLen))
        
        dataInputStr = self.lineEdit_tcAddr.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.tcAddrLen:
                QMessageBox.warning(self, 'From Network', "Misformatted trust cdenter address to set!")
                return
            else:
                formData.extend(tlv_construct(self.comm.tcAddrTlvId, dataInput, dataLen))

        dataInputStr = self.lineEdit_exPanid.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.extPanIdLen:
                QMessageBox.warning(self, 'From Network', "Misformatted extPanId to set!")
                return
            else:
                formData.extend(tlv_construct(self.comm.extPanidTlvId, dataInput, dataLen))

        dataInputStr = self.lineEdit_shortPanid.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.panIdLen:
                QMessageBox.warning(self, 'From Network', "Misformatted panId to set!")
                return
            else:
                formData.extend(tlv_construct(self.comm.shortPanidTlvId, dataInput, dataLen))

        dataInputStr = self.lineEdit_nwkChannel.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.nwkChannelLen:
                QMessageBox.warning(self, 'From Network', "Misformatted network channel to set!")
                return
            else:
                formData.extend(tlv_construct(self.comm.nwkChannelTlvId, dataInput, dataLen))

        dataInputStr = self.lineEdit_nwkKey.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.nwkKeyLen:
                QMessageBox.warning(self, 'From Network', "Misformatted network key to set!")
                return
            else:
                formData.extend(tlv_construct(self.comm.nwkKeyTlvId, dataInput, dataLen))

        dataInputStr = self.lineEdit_linkkey.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.linkKeyLen-1:
                QMessageBox.warning(self, 'From Network', "Misformatted linkkey to set!")
                return
            else:
                dataAdd = []
                linkKeyType = self.comboBox_linkkType.currentIndex()
                linkKeyLife = self.comboBox_linkkLife.currentIndex()
                linkKeyAdd = linkKeyType + (linkKeyLife << 1)
                dataAdd.append(linkKeyAdd)
                dataAdd.extend(dataInput) 
                formData.extend(tlv_construct(self.comm.linkKeyTlvId, dataAdd, dataLen+1))

        dataInputStr = self.lineEdit_nwkAddr.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.nwkAddrLen:
                QMessageBox.warning(self, 'From Network', "Wrong nwk short address to set!")
                return
            else:
                formData.extend(tlv_construct(self.comm.nwkAddrTlvId, dataInput, dataLen))

        dataInputStr = self.lineEdit_updateId.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.nwkUpIdLen:
                QMessageBox.warning(self, 'From Network', "Wrong nwk update Id to set!")
                return
            else:
                formData.extend(tlv_construct(self.comm.nwkUpdateIdTlvId, dataInput, dataLen))

        dataInputStr = self.lineEdit_keySeqNum_2.text()
        if dataInputStr:
           QMessageBox.warning(self, 'From Network', "Invalid to set nwk active key seqNum!")
           return
        self.send_chara_encrypt_data(self.comm.formCharaStr, formData)
       
    def commi_join_network(self):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'Join Network', "No BLE connection!")
            return

        joinMethod = self.comboBox_joinMethod.currentIndex()
        methodDtat = []
        methodDtat.append(self.comboBox_joinMethod.currentIndex())
        joinData = []
        joinData.extend(tlv_construct(self.comm.joinMethodTlvId, methodDtat, 1))

        dataInputStr = self.lineEdit_exPanid.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.extPanIdLen:
                QMessageBox.warning(self, 'Join Network', "Misformatted extPanId to set!")
                return
            else:
                joinData.extend(tlv_construct(self.comm.extPanidTlvId, dataInput, dataLen))

        dataInputStr = self.lineEdit_nwkChannel.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.nwkChannelLen:
                QMessageBox.warning(self, 'Join Network', "Misformatted network channel to set!")
                return
            else:
                joinData.extend(tlv_construct(self.comm.nwkChannelTlvId, dataInput, dataLen))

        dataInputStr = self.lineEdit_linkkey.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.linkKeyLen-1:
                QMessageBox.warning(self, 'Join Network', "Misformatted linkkey to set!")
                return
            else:
                inputAdd = []
                linkKeyType = self.comboBox_linkkType.currentIndex()
                linkKeyLife = self.comboBox_linkkLife.currentIndex()
                inputAdd.append(linkKeyType + (linkKeyLife << 1))
                inputAdd.extend(dataInput)
                joinData.extend(tlv_construct(self.comm.linkKeyTlvId, inputAdd, 17))
        
        dataInputStr = self.lineEdit_tcAddr.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.tcAddrLen:
                QMessageBox.warning(self, 'Join Network', "Misformatted trust cdenter address to set!")
                return
            else:
                joinData.extend(tlv_construct(self.comm.tcAddrTlvId, dataInput, dataLen))

        dataInputStr = self.lineEdit_adminKey.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.adminKeyLen:
                QMessageBox.warning(self, 'Join Network', "Misformatted admin Key to set!")
                return
            else:
                self.session.adminKeyPre = []
                for i in range(16):
                    self.session.adminKeyPre.append(dataInput[i])
                joinData.extend(tlv_construct(self.comm.adminKeyTlvId, dataInput, dataLen))

        if joinMethod == 0x00: #Mac assocoation
            dataInputStr = self.lineEdit_nwkAddr.text()
            if dataInputStr:
                dataInput, dataLen = line_edit_str2int(dataInputStr)
                if dataLen != self.comm.nwkAddrLen:
                    QMessageBox.warning(self, 'Join-MAC Association', "Wrong nwk short address to set!")
                    return
                else:
                    joinData.extend(tlv_construct(self.comm.nwkAddrTlvId, dataInput, dataLen))      
        elif joinMethod == 0x01: #rejoin
            dataInputStr = self.lineEdit_exPanid.text()
            if not dataInputStr:
                 QMessageBox.warning(self, 'Join-Rejoin', "Extpanid is required to set!")    
                 return         

            dataInputStr = self.lineEdit_nwkKey.text()
            if dataInputStr:
                dataInput, dataLen = line_edit_str2int(dataInputStr)
                if dataLen != self.comm.nwkKeyLen:
                    QMessageBox.warning(self, 'Join-Rejoin', "Misformatted network key to set!")
                    return
                else:
                    joinData.extend(tlv_construct(self.comm.nwkKeyTlvId, dataInput, dataLen)) 
        elif joinMethod == 0x02: #out-of-band
            dataInputStr = self.lineEdit_exPanid.text()
            if not dataInputStr:
                 QMessageBox.warning(self, 'Join-Rejoin', "Extpanid is required to set!")    
                 return

            dataInputStr = self.lineEdit_shortPanid.text()
            if dataInputStr:
                dataInput, dataLen = line_edit_str2int(dataInputStr)
                if dataLen != self.comm.panIdLen:
                    QMessageBox.warning(self, 'Join-out of band commissioning', "Misformatted panId to set!")
                    return
                else:
                    joinData.extend(tlv_construct(self.comm.shortPanidTlvId, dataInput, dataLen)) 
            else:
                QMessageBox.warning(self, 'Join-out of band commissioning', "Short panId is required to set!")
            
            dataInputStr = self.lineEdit_nwkChannel.text()
            if not dataInputStr:
                QMessageBox.warning(self, 'Join-out of band commissioning', "Nwk channel is required to set!")
                return

            dataInputStr = self.lineEdit_nwkKey.text()
            if dataInputStr:
                dataInput, dataLen = line_edit_str2int(dataInputStr)
                if dataLen != self.comm.nwkKeyLen:
                    QMessageBox.warning(self, 'Join-out of band commissioning', "Misformatted network key to set!")
                    return
                else:
                    joinData.extend(tlv_construct(self.comm.nwkKeyTlvId, dataInput, dataLen)) 
            else:
                QMessageBox.warning(self, 'Join-out of band commissioning', "Nwk key is required to set!")
                return

            dataInputStr = self.lineEdit_nwkAddr.text()
            if dataInputStr:
                dataInput, dataLen = line_edit_str2int(dataInputStr)
                if dataLen != self.comm.nwkAddrLen:
                    QMessageBox.warning(self, 'Join-out of band commissioning', "Misformatted nwk short address to set!")
                    return
                else:
                    joinData.extend(tlv_construct(self.comm.nwkAddrTlvId, dataInput, dataLen))     
        
            dataInputStr = self.lineEdit_updateId.text()
            if dataInputStr:
                dataInput, dataLen = line_edit_str2int(dataInputStr)
                if dataLen != self.comm.nwkUpIdLen:
                    QMessageBox.warning(self, 'Join-out of band commissioning', "Misformatted nwk update Id to set!")
                    return
                else:
                    joinData.extend(tlv_construct(self.comm.nwkUpdateIdTlvId, dataInput, dataLen)) 

            dataInputStr = self.lineEdit_keySeqNum_2.text()
            if dataInputStr:
                dataInput, dataLen = line_edit_str2int(dataInputStr)
                if dataLen != self.comm.nwkActiveKeySeqLen:
                    QMessageBox.warning(self, 'Join-out of band commissioning', "Misformatted nwk key sequence number to set!")
                    return
                else:
                    joinData.extend(tlv_construct(self.comm.nwkActiveKeyNumTlvId, dataInput, dataLen)) 
        self.send_chara_encrypt_data(self.comm.joinCharaStr, joinData)

    def commi_permitjoin_time_set(self):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'Commission Permit Join', "No BLE connection!")
            return
        permitData = []
        dataInputStr = self.lineEdit_perjoinTime.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.permitJoinLen:
                QMessageBox.warning(self, 'Commission Permit Join', "Misformatted permit join parameter!")
                return
            else:
                permitData.extend(dataInput) 
        else:
            QMessageBox.warning(self, 'Commission Permit Join', "You should set permit join parameter!")
            return
        self.send_chara_encrypt_data(self.comm.permitCharaStr, permitData)

    def commi_leave_network(self):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'Commission Leave', "No BLE connection!")
            return

        leaveData = []
        dataInputStr = self.lineEdit_leaveRemove.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.removeChLen:
                QMessageBox.warning(self, 'Commission Leave', "Misformatted remove children parameter!")
                return
            else:
                leaveData.extend(dataInput) 
        else:
            QMessageBox.warning(self, 'Commission Leave', "You should set remove children parameter!")
            return

        dataInputStr = self.lineEdit_leaveRejoin.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.leaveRejoinLen:
                QMessageBox.warning(self, 'Commission Leave', "Misformatted rejoin parameter!")
                return
            else:
                leaveData.extend(dataInput) 
        else:
            QMessageBox.warning(self, 'Commission Leave', "You should set rejoin parameter!")
            return
        self.send_chara_encrypt_data(self.comm.leaveCharaStr, leaveData)

    def commi_manager_joiners(self):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'Manager Joiners', "No BLE connection!")
            return

        manaData = []
        manaTypeData = []
        manaTypeData.append(self.comboBox_manaType.currentIndex())
        manaType = self.comboBox_manaType.currentIndex()
        manaData.extend(tlv_construct(self.comm.manaJoinersCommTlvId, manaTypeData, 1)) 
        if manaType == 0x02 or manaType == 0x01: #Remove a joiner’s Provisional Link Key
            dataInputStr = self.lineEdit_manaIeee.text()
            if dataInputStr:
                dataInput, dataLen = line_edit_str2int(dataInputStr)
                if dataLen != self.comm.ieeeAddrLen:
                    QMessageBox.warning(self, 'Manager Joiners', "Misformatted ieee address!")
                    return
                else:
                    manaData.extend(tlv_construct(self.comm.manaIeeeAddeTlvId, dataInput, dataLen)) 
            else:
                QMessageBox.warning(self, 'Manager Joiners', "You should set ieee address!")
                return
        if manaType == 0x01: #Add a joiner’s Provisional Link Key
            dataInputStr = self.lineEdit_proLinkkey.text()
            if dataInputStr:
                dataInput, dataLen = line_edit_str2int(dataInputStr)
                if dataLen != self.comm.nwkKeyLen:
                    QMessageBox.warning(self, 'Manager Joiners', "Misformatted proviaional linkkey!")
                    return
                else:
                    manaData.extend(tlv_construct(self.comm.manaProviLinkkeyTlvId, dataInput, dataLen)) 
            else:
                QMessageBox.warning(self, 'Manager Joiners', "You should set provisional linkkey!")
                return
        self.send_chara_encrypt_data(self.comm.manaCharaStr, manaData)

    def commi_start_identify(self):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'Commission Identify', "No BLE connection!")
            return

        identifyData = []
        dataInputStr = self.lineEdit_identifyTime.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.identifyTimeLen:
                QMessageBox.warning(self, 'Commission Identify', "Misformatted identify time parameter!")
                return
            else:
                identifyData.extend(dataInput) 
        else:
            QMessageBox.warning(self, 'Commission Identify', "You should set identify time parameter!")
            return
        self.send_chara_encrypt_data(self.comm.identifyCharaStr, identifyData)

    def commi_start_findbind(self):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'Commission Finding&Binding', "No BLE connection!")
            return
        fbData = []
        dataInputStr = self.lineEdit_fbEndpoint.text()
        if dataInputStr:
            dataInput, dataLen = line_edit_str2int(dataInputStr)
            if dataLen != self.comm.endpointLen:
                QMessageBox.warning(self, 'Commission Finding&Binding', "Misformatted identify time parameter!")
                return
            else:
                fbData.extend(dataInput) 
        else:
            QMessageBox.warning(self, 'Commission Finding&Binding', "You should set identify time parameter!")
            return

        fbRole = self.comboBox_fbRole.currentIndex()
        fbData.append(fbRole)
        self.send_chara_encrypt_data(self.comm.fbCharaStr, fbData)

    def commi_status_read(self):
        if self.bleHand.connState == False:
            QMessageBox.warning(self, 'Commission Status Read', "No BLE connection!")
            return
        for info in self.bleHand.gattList:
            # print(info['uuid'].toString())
            if self.comm.commStaCharaStr.lower() == info['uuid'].toString():
                self.choose_attr = info
                break
        else:
            QMessageBox.warning(self, 'Commission Status Read', "No characteristic to read!")
            return

        readRet = self.bleHand.creat_read_characteristic(self.choose_attr['uuid'])
        if readRet == 1:
            QMessageBox.warning(self, 'Commission Status Read', "The charcateristic is not allow to read!")
        elif readRet != 0:
            QMessageBox.warning(self, 'Commission Status Read', "Failed to read charcateristic!")


    def window_init(self):
        self.label_connState.setText('Connect State: FALSE')
        self.label_connState.setStyleSheet("color:red")
        self.bleHand.connState = False

        self.label_sessionState.setText('Session Build State: FALSE')
        self.label_sessionState.setStyleSheet("color:red")

        self.pushButton_startScan.clicked.connect(self.start_ble_scan)

        self.pushButton_startConnect.clicked.connect(self.start_connect_ble)

        self.pushButton_disconnect.clicked.connect(self.disconnect_ble)

        self.pushButton_enNotify.clicked.connect(self.enable_notify_handle)

        self.pushButton_disNotify.clicked.connect(self.disable_notify_handle)

        self.listWidget_gatt.itemClicked.connect(self.choose_gatt_handle)

        self.pushButton_attrWrite.clicked.connect(self.ble_gatt_write)

        self.pushButton_attrRead.clicked.connect(self.ble_gatt_read)

        self.pushButton_startSession.clicked.connect(self.start_session_build)

        self.pushButton_secClear.clicked.connect(self.session_build_data_clear)

        self.pushButton_form.clicked.connect(self.commi_form_network)

        self.pushButton_join.clicked.connect(self.commi_join_network)

        self.pushButton_setPer.clicked.connect(self.commi_permitjoin_time_set)

        self.pushButton_leave.clicked.connect(self.commi_leave_network)

        self.pushButton_manaCom.clicked.connect(self.commi_manager_joiners)

        self.pushButton_identify.clicked.connect(self.commi_start_identify)

        self.pushButton_fbStart.clicked.connect(self.commi_start_findbind)

        self.pushButton_statusRead.clicked.connect(self.commi_status_read)

        self.pushButton_commClear.clicked.connect(self.commi_data_clear)

        self.pushButton_startJoin.clicked.connect(self.tunnel_zvd_join)

        self.pushButton_zvdLeave.clicked.connect(self.tunnel_zvd_leave)

        self.pushButton_onoffSend.clicked.connect(self.tunnel_zvd_onoff_cmd)

        self.pushButton_tunnalClear.clicked.connect(self.tunnel_data_clear)

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)  #初始化app
    myshow = Blewindow()                 #创建主窗口
    myshow.show()                           #显示窗口
    sys.exit(app.exec())                  #消息循环结束之后返回0，接着调用sys.exit(0)退出程序  
    input("please input any key to exit!") 
     