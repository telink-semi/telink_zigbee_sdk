import struct
import os
import copy
import serial
import serial.tools.list_ports
from PySide6 import QtWidgets
from PySide6.QtWidgets import QMessageBox, QFileDialog
from PySide6.QtCore import QTimer, QThread, Signal
from mainwindow import Ui_MainWindow

from datetime import datetime
from csvFiles import CsvFiles
from txtFiles import TxtFiles
from settings import Settings
from ringbuffer import RingBuffer
from hcicommandparse import parse_packet_detail_show, SetAutoBindPara, hci_mgmt_bind_req_send, hci_bind_req_send, convert_str2int


def crc8_calculate(datatype, length, data):
    crc8 = (datatype >> 0) & 0xff
    crc8 ^= (datatype >> 8) & 0xff
    crc8 ^= (length >> 0) & 0xff
    crc8 ^= (length >> 8) & 0xff

    for n in range(length):
        crc8 ^= data[n]

    return crc8


def simple_str2hex(input_s):
    result = []
    if input_s != '' and input_s[0:2] == '0x':
        input_s = input_s[2:].strip()  # 把前后的空格去掉
        while input_s != '':
            try:
                num = int(input_s[0:2], 16)
            except ValueError:
                break
            input_s = input_s[2:].strip()  # 把前后的空格去掉
            result.append(num)
    return result


def line_edit_str2int(input_len, input_s):
    # print(input_s)
    covert_result = True
    send_list = []
    convert_cnt = 0
    if input_s != '' and input_s[0:2] == '0x':
        input_s = input_s[2:].strip()  # 把前后的空格去掉
        if len(input_s) <= 2 * input_len :
            addStr = ''
            for n in range(2*input_len - len(input_s)):
                addStr += '0'
            input_s = addStr + input_s

            while input_s != '':
                try:
                    if len(input_s) < 2:
                        num = int(input_s[0:1], 16)
                    else:
                        num = int(input_s[0:2], 16)
                except ValueError:
                    covert_result = False
                    break
                input_s = input_s[2:].strip()  # 把前后的空格去掉
                # print(input_s)
                send_list.append(num)
                convert_cnt += 1
        else:
            covert_result = False
    else:
        covert_result = False

    if not covert_result or convert_cnt != input_len:
        # print('convert error!')
        send_list = []
        for i in range(input_len):
            send_list.append(0xcc)
    # print(send_list)

    return send_list


def line_edit_str2hex_result(input_s):
    covert_result = True
    convert_data = 0
    if input_s[0:2] == '0x':
        try:
            convert_data = int(input_s, 16)
        except:
            covert_result = False
    else:
        covert_result = False

    return covert_result, convert_data


class Pyside6Serial(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        super(Pyside6Serial, self).__init__()
        self.setupUi(self)  # 初始化UI到主窗口，主要是建立代码到UI之间的signal和slot
        self.setWindowTitle("ZGC TOOL V1.3")
        self.serial = serial.Serial()
        self.portisopen = 0
        self.port_com = ''
        self.ai_setting = Settings()
        self.ring_buffer = RingBuffer()
        self.CsvFiles = CsvFiles()
        self.txtFiles = TxtFiles()
        self.autoBindPro = SetAutoBindPara()
        self.choose_node_ieee = ''
        self.choose_node_nwk = ''
        self.get_joined_list = ''
        self.auto_bind_list = []
        self.auto_bind_fail_list = []
        self.recv_interval = 5
        self.recv_interval_pre = 5
        self.clearRecvFlag = 0
        self.advList = {}
        self.ble_choose_mac = []
        self.blehandle = 0
        self.bleHci_enable = False
        self.init()

    def init(self):
        self.serial_init()
        self.bdb_init()
        self.nodes_mgmt_init()
        self.mgmt_init()
        self.zcl_general_init()
        self.zcl_group_init()
        self.zcl_onoff_init()
        self.zcl_level_init()
        self.zcl_color_init()
        self.zcl_identify_init()
        self.zcl_scene_init()
        self.zcl_zbd_init()
        self.ota_init()
        self.af_test_init()
        self.hci_ota_init()
        self.command_analyze_init()

        self.ble_hci_init()

    def serial_init(self):
        self.port_detect()

        # 打开串口按钮
        self.pushButton_openPort.clicked.connect(self.port_open)

        # 关闭串口按钮
        self.pushButton_closePort.clicked.connect(self.port_close)

        # 发送数据按钮
        self.pushButton_send.clicked.connect(self.input_data_send)

        # 定时监测有效串口
        self.portDetectTimer = QTimer(self)
        self.portDetectTimer.timeout.connect(self.port_detect)
        self.portDetectTimer.start(5000)

        # 定时器接收数据
        self.recvTimer = QTimer(self)
        self.recvTimer.timeout.connect(self.serial_data_receive)

        # 定时器解析数据
        self.showTimer = QTimer(self)
        self.showTimer.timeout.connect(self.get_ring_buffer_data)

        # 定时获取节点地址
        self.getNwkAddrTimer = QTimer(self)
        self.getNwkAddrTimer.timeout.connect(self.get_nwkaddr_by_ieee)

        # 定时发送数据
        self.sendTimer = QTimer(self)
        self.sendTimer.timeout.connect(self.input_data_send)
        self.checkBox_timesend.stateChanged.connect(self.input_data_send_timer)

        self.bindReqSendTimer = QTimer(self)
        self.bindReqSendTimer.timeout.connect(self.auto_bind_req_send)

        # 清除发送窗口
        self.pushButton_clear.clicked.connect(self.command_data_clear)

        # 解析接收窗口的数据
        self.listWidget_commandData.itemClicked.connect(self.command_entry_parse)
        self.listWidget_commandData.currentItemChanged.connect(self.command_entry_parse)

        # ble广播接收窗口的数据
        self.listWidget_AdvList.itemClicked.connect(self.ble_mac_get)

    def port_detect(self):
        # 检测所有存在的串口，将信息存储在字典中
        com_dict = {}
        port_list = list(serial.tools.list_ports.comports())
        port_name_current = self.comboBox_portName.currentText()
        self.comboBox_portName.clear()
        for port in port_list:
            if 'USB' not in port.description:
                continue
            com_dict["%s" % port[0]] = "%s" % port[1]
            self.comboBox_portName.addItem(port[0])

        if self.portisopen == 1:
            for port in port_list:
                # print(self.port_com)
                # print(port[0])
                if self.port_com == port[0]:
                    break
            else:
                # print('the serial port has lost!!')
                self.port_close()
        self.comboBox_portName.setCurrentText(port_name_current)

    def recv_interval_change(self):
        try:
            self.recv_interval = int(self.lineEdit_recvInterval.text())
        except ValueError:
            self.recv_interval = 50
            self.lineEdit_recvInterval.setText('%d' % self.recv_interval)

        # print("self.recv_interval_pre:%d, self.recv_interval:%d\n" % (self.recv_interval_pre, self.recv_interval))
        if self.recv_interval_pre != self.recv_interval:
            self.recv_interval_pre = self.recv_interval
            self.recvTimer.start(self.recv_interval)  # 5ms
            self.showTimer.start(self.recv_interval)

    def port_open(self):
        self.serial.port = self.comboBox_portName.currentText()
        self.serial.baudrate = int(self.comboBox_baudrate.currentText())
        self.serial.bytesize = int(self.comboBox_databits.currentText())
        self.serial.stopbits = int(self.comboBox_stopbits.currentText())
        self.serial.parity = self.comboBox_parity.currentText()[0:1]
        self.serial.timeout = 5
        try:
            self.recv_interval = int(self.lineEdit_recvInterval.text())
        except ValueError:
            self.recv_interval = 5
            self.lineEdit_recvInterval.setText('%d' % self.recv_interval)
        # print("self.recv_interval:%d\n" % self.recv_interval)
        try:
            self.serial.open()
        except serial.SerialException:
            QMessageBox.critical(self, 'port error', "Can not open！")
        else:
            self.portisopen = 1
            self.port_com = self.serial.port
            self.label_portState.setText(self.serial.port + ' Opened')
            self.recvTimer.start(self.recv_interval)  # 5ms
            self.showTimer.start(self.recv_interval)
            self.pushButton_openPort.setEnabled(False)
            self.pushButton_closePort.setEnabled(True)
            self.lineEdit_recvInterval.textChanged.connect(self.recv_interval_change)
            self.lineEdit_srcEp.textChanged.connect(self.auto_bind_para_change)
            self.lineEdit_clusterId.textChanged.connect(self.auto_bind_para_change)
            self.lineEdit_dstEp.textChanged.connect(self.auto_bind_para_change)
            self.checkBox_autobind.stateChanged.connect(self.auto_bind_para_change)
            self.auto_bind_para_change()
            if not self.checkBox_bleHci.isChecked():
                self.bleHci_enable = False
            else:
                self.bleHci_enable = True
            if not self.checkBox_thread.isChecked():
                self.get_joined_nodes()

    def auto_bind_para_change(self):
        if self.checkBox_autobind.isChecked():
            src_ep_s = self.lineEdit_srcEp.text()
            ret, src_ep = line_edit_str2hex_result(src_ep_s)
            if not ret or src_ep == 0 or src_ep > 0xfe:
                self.label_bindConfigState.setText('(para invalid)')
                return
            cluster_id_s = self.lineEdit_clusterId.text()
            ret, cluster_id = line_edit_str2hex_result(cluster_id_s)
            if not ret or cluster_id == 0xffff:
                self.label_bindConfigState.setText('(para invalid)')
                return
            dst_ep_s = self.lineEdit_dstEp.text()
            ret, dst_ep = line_edit_str2hex_result(dst_ep_s)
            if not ret or dst_ep == 0 or dst_ep > 0xfe:
                self.label_bindConfigState.setText('(para invalid)')
                return
            self.label_bindConfigState.clear()
            self.autoBindPro.auto_bind_set_para(True, src_ep, cluster_id, dst_ep)
        else:
            self.label_bindConfigState.clear()
            self.autoBindPro.auto_bind_set_para(False, 0, 0, 0)

    def port_close(self):
        if self.portisopen:
            self.portisopen = 0
            self.port_com = ''
            self.recvTimer.stop()
            self.showTimer.stop()
            self.getNwkAddrTimer.stop()
            self.serial.close()
            self.label_portState.setText(' Closed')
            self.pushButton_openPort.setEnabled(True)
            self.pushButton_closePort.setEnabled(False)
            self.port_detect()
            self.bindReqSendTimer.stop()
        else:
            return

    def input_data_send(self):
        if self.portisopen:
            input_s = self.plainTextEdit_sendData.toPlainText()
            # print(type(input_s))
            # print(input_s)
            if input != '':
                if not self.checkBox_thread.isChecked():  # hex发送
                    input_s = input_s.strip()
                    # print(input_s)
                    send_list = []
                    while input_s != '':
                        try:
                            num = int(input_s[0:2], 16)
                        except ValueError:
                            QMessageBox.warning(self, 'wrong data', 'Input hex with space(eg:AA 55)!')
                            return None
                        input_s = input_s[2:].strip()  # 把前后的空格去掉
                        # print(input_s)
                        send_list.append(num)
                    send_data = bytes(send_list)
                else:  # ascii 发送
                    send_data = (input_s + '\n').encode('utf-8')
                    # print('ascii 发送::')
                    # print(send_data)
                self.send_serial_data(send_data)
        else:
            QMessageBox.warning(self, 'port warning', "Please open the port！")
            if self.checkBox_timesend.isChecked():
                self.checkBox_timesend.setChecked(False)
            return
        
    def ble_event_data_parse(self, recv_data):
        try:
            if recv_data[1] == 0x3e and recv_data[3] == 0x01: #LE_mATA LE_CONNECTION COMPLETE
                if recv_data[2] ==0x13 and recv_data[4] == 0x00: #status
                    connHandle = recv_data[5] + (recv_data[6] << 8)
                    self.blehandle = connHandle
                    role = recv_data[7]
                    addrType = recv_data[8]
                    ieee_addr_s = ''
                    for a in range(6):
                        ieee_addr_s += ":%02x"%recv_data[14-a]
                    
                    conn_interval = recv_data[15] + (recv_data[16] << 8)
                    conn_interval = conn_interval * 5
                    conn_interval = conn_interval/4
                    conn_latency = recv_data[17] + (recv_data[18] << 8)
                    conn_sup = recv_data[19] + (recv_data[20] << 8)
                    conn_sup = conn_sup * 10
                    self.label_bleState.setStyleSheet("color:red")
                    self.label_bleState.setText('  BLE connected: ' + '      Handle:0x%04x'%connHandle + 
                                                '\n     Peer addr type:0x%x'%role +  '      Peer addr' + ieee_addr_s +
                                                '\n     conn interval:%dms'%conn_interval + '    conn latency:%d'%conn_latency + '   supervision Timeout:%dms'%conn_sup)
            elif recv_data[5] == 0x06 and recv_data[6] == 0x04 and recv_data[3] == 0x00:  #disconnect
                if recv_data[4] == 0x01:
                    # print('disconnect')
                    self.label_bleState.setStyleSheet("color:red")
                    self.label_bleState.setText('BLE disconnected.')
                    self.blehandle = 0
            elif recv_data[1] == 0x05 and recv_data[2] == 0x04:
                self.label_bleState.setStyleSheet("color:red")
                self.label_bleState.setText('BLE disconnected.')
                self.blehandle = 0
            elif recv_data[1] == 0x3e and recv_data[4] == 0x01:
                # print('len:%d'%len(recv_data))
                # print('length:%d'%recv_data[2])
                pack_len_idx = 0
                pack_len = 0
                # print(recv_data)
                # print("firstLen:%d"%pack_len)
                while 1:
                    if (len(recv_data) - pack_len_idx) >= 13:
                        # print('firstdata:%d'%(recv_data[pack_len_idx]))
                        if recv_data[pack_len_idx] == 0x04 and recv_data[pack_len_idx+1] == 0x3e:
                            pack_len = recv_data[pack_len_idx+2]+3
                        else:
                            break
                        if (pack_len + 3) <= (len(recv_data) - pack_len_idx):
                            pack_data = recv_data[pack_len_idx:(pack_len_idx+ pack_len)]
                            bleMac = pack_data[7:13]
                            if bleMac not in self.advList:
                                self.advList[bleMac] = {}
                                self.advList[bleMac]['rssi'] = pack_data[-1]
                                if pack_data[13] >3 and pack_data[15] == 0x09:
                                    self.advList[bleMac]['name'] = pack_data[16: 16+pack_data[14]]
                                else:
                                    self.advList[bleMac]['name'] = "no name"
                            pack_len_idx += pack_len
                        else:
                            break
                    else:
                        break
        except IndexError:
            print('index error....')
            return

    def serial_data_receive(self):
        #print("waiting data...")
        try:
            num = self.serial.inWaiting()
        except:
            # print('data receive error!!')
            self.port_close()
            return None
        if num > 0:
            recv_data = self.serial.read(num)
            pktLen = len(recv_data)
        # print('recv num:%d' % num)
            
            if pktLen > 0:            
                if not self.checkBox_thread.isChecked():  # hex发送
                    if self.bleHci_enable is True:
                        if recv_data[0] ==0x04 and recv_data[1] != 0xff:
                            #print('recv ble data...')
                            #print(recv_data)
                            dt = datetime.now()
                            timestr = dt.strftime('%y-%m-%d %H:%M:%S.%f')
                            
                            self.ble_event_data_parse(recv_data)
                            recv_data_str = ''
                            for a in range(pktLen):
                                hhex = '%02x' % recv_data[a]
                                recv_data_str += hhex + ' '

                            #print("recv_data_str")
                            #print(recv_data_str)
                            self.listWidget_commandData.addItem(timestr + ' recv<--:' + recv_data_str)
                        else:
                            self.ring_buffer.ring_buffer_write(recv_data)
                    else:
                        self.ring_buffer.ring_buffer_write(recv_data)
                else:
                    # self.listWidget_commandData.addItem(recv_data.decode('utf-8')) #ascii
                    if self.checkBox_autoClear.isChecked():
                        if self.listWidget_commandData.count() > 500:
                            self.listWidget_commandData.clear()
                    dt = datetime.now()
                    timestr = dt.strftime('%y-%m-%d %H:%M:%S.%f')
                    # self.listWidget_commandData.addItem(timestr + ' recv<--:')
                    #print(recv_data)
                    recv_data_str = ''
                    try:
                        recv_data_str = recv_data.decode('utf-8')
                        self.listWidget_commandData.addItem('【' + timestr + '】' + recv_data_str)
                    except UnicodeDecodeError:
                        pass

                    write_result = self.txtFiles.write_file(timestr, recv_data_str, 'recv<--')
                    if write_result != 1:
                        QMessageBox.warning(self, 'write file warning', "Do not open the file when writing！")
        if not self.checkBox_bleHci.isChecked():
            self.bleHci_enable = False
        else:
            self.bleHci_enable = True
        if self.checkBox_scroll.isChecked():
            self.listWidget_commandData.scrollToBottom()
        # # 升级过程中不允许更改文件地址和开始ota，如果升级一半出错，得重启工具
        # if self.progressBar_hciOta.text() != '0%' and self.progressBar_hciOta.text() != '100%':
        #     self.pushButton_hciOtaFileChoose.setEnabled(False)
        #     self.pushButton_hciOtaStart.setEnabled(False)
        # else:
        #     self.pushButton_hciOtaFileChoose.setEnabled(True)
        #     self.pushButton_hciOtaStart.setEnabled(True)

    def command_data_clear(self):
        self.listWidget_commandData.clear()
        self.listWidget_commandDataParse.clear()

    def command_entry_parse(self, item):
        # print('enter recv_data_parse!')
        self.listWidget_commandDataParse.clear()
        if not self.checkBox_thread.isChecked():
            try:
                get_data = item.text()
            except AttributeError:
                return
            # print(get_data)
            get_real_data = get_data[33:]
            # print('get_real_data:')
            # print(get_real_data)
            parse_items = parse_packet_detail_show(self.ai_setting, get_real_data)
            self.listWidget_commandDataParse.addItems(parse_items)

    def ble_mac_get(self, item):
        try:
            get_data = item.text()
        except AttributeError:
            return
        get_blemac_s = get_data[6:18]
        # print('get_blemac_s:')
        # print(get_blemac_s)
        self.ble_choose_mac = convert_str2int(get_blemac_s)
        self.ble_choose_mac.reverse()

    def input_data_send_timer(self):
        if self.checkBox_timesend.isChecked():
            # print('send time ischeck')
            try:
                send_interval = int(self.lineEdit_sendInterval.text())
            except ValueError:
                send_interval = 1000
                self.lineEdit_sendInterval.setText('%d' % send_interval)

            # print('send_interval:%d' % send_interval)
            if send_interval < 10:
                send_interval = 10
                self.lineEdit_sendInterval.setText('%d' % send_interval)
            self.sendTimer.start(send_interval)
            self.lineEdit_sendInterval.setEnabled(False)
        else:
            self.sendTimer.stop()
            self.lineEdit_sendInterval.setEnabled(True)

    def get_nwkaddr_by_ieee(self):
        # print("get_nwkaddr_by_ieee,self.get_joined_list:" + str(len(self.get_joined_list)))
        find_to_search = False
        for node in self.get_joined_list:
            if self.get_joined_list[node] == 0xfffe:
                find_to_search = True
                payload = struct.pack("!H", 0xffff)
                # payload += struct.pack("!%dB" % len(node), *node)
                payload += struct.pack("!Q", node)
                payload += struct.pack("!B", 0)
                payload += struct.pack("!B", 0)
                self.send_hci_command(0x0010, len(payload), payload)  # nwk_addr_req
                del self.get_joined_list[node]
                break
        if not find_to_search:
            self.getJoindNodeListWidget.clear()
            index = 0
            # print("CsvFiles.joined_nodes_info:" + str(len(self.CsvFiles.joined_nodes_info)))
            for node in self.CsvFiles.joined_nodes_info:
                index += 1
                if node in self.CsvFiles.nodes_info:
                    self.getJoindNodeListWidget.addItem('%04d:' % index + '  ' + '0x%016x' % node +
                                                        ' (0x%04x)' % self.CsvFiles.nodes_info[node]['nwk_addr'])
            self.CsvFiles.joined_nodes_info = {}
            self.getNwkAddrTimer.stop()

    def auto_bind_req_send(self):
        if len(self.auto_bind_list) != 0:
            for cell in self.auto_bind_fail_list:
                if cell['info'] == self.auto_bind_list[0]['info']:
                    self.auto_bind_fail_list.remove(cell)
                    break

            if self.auto_bind_list[0]['type'] == 'bind_req':
                data_send = hci_mgmt_bind_req_send(self.auto_bind_list[0]['nwk_addr'], 0)  # 发送mgmt_bind
                self.send_serial_data(data_send)
                self.auto_bind_list[0]['type'] = 'mgmt_req'
                self.auto_bind_list[0]['retry_cnt'] += 1
            elif self.auto_bind_list[0]['type'] == 'mgmt_req':
                if self.auto_bind_list[0]['retry_cnt'] >= self.ai_setting.auto_bind_retry_max:
                    for cell in self.auto_bind_fail_list:
                        if cell['info'] == self.auto_bind_list[0]['info']:
                            break
                    else:
                        self.auto_bind_fail_list.append(self.auto_bind_list[0])
                    del self.auto_bind_list[0]
                else:
                    data_send = hci_bind_req_send(self.auto_bind_list[0]['info']['ieee_addr'],
                                                  self.auto_bind_list[0]['info']['src_ep'],
                                                  self.auto_bind_list[0]['info']['src_cluster'],
                                                  0x03, self.auto_bind_list[0]['info']['dst_addr'],
                                                  self.auto_bind_list[0]['info']['dst_ep'])  # 发送bind_req
                    self.send_serial_data(data_send)
                    self.auto_bind_list[0]['type'] = 'bind_req'
        else:
            if len(self.auto_bind_fail_list):
                fail_str = 'auto bind fail list:(total number: %d)\n' % len(self.auto_bind_fail_list)
                for cell in self.auto_bind_fail_list:
                    fail_str += '   src_ieee:' + hex(cell['info']['ieee_addr']) + ' src_ep:' + \
                                hex(cell['info']['src_ep']) + ' cluster: 0x%04x' % cell['info']['src_cluster']\
                                + ' dst_ieee:' + hex(cell['info']['dst_addr']) + \
                                ' dst_ep:' + hex(cell['info']['dst_ep']) + '\n'

                self.label_bindFailList.setText(fail_str)
            else:
                self.label_bindFailList.clear()
            self.bindReqSendTimer.stop()

    def get_ring_buffer_data(self):
        while self.ring_buffer.valid_data_length >= self.ai_setting.command_length_min:
            recv_packet = []
            recv_result, recv_len = self.ring_buffer.ring_buffer_get_packet(recv_packet)
            if not recv_result:
                self.clearRecvFlag = self.clearRecvFlag + 1
                # print('no wait the other data.clearRecvFlag:%d,recv_interval:%d' % (self.clearRecvFlag ,self.recv_interval))
                if self.clearRecvFlag * self.recv_interval > 500: #more than 500ms
                    self.clearRecvFlag = 0
                    self.ring_buffer.ring_buffer_clear_all_data()
                else:
                    return
            else:
            #   print(recv_packet)
                self.clearRecvFlag = 0
                recv_data_str = ''
                for i in range(recv_len):
                    hvol = recv_packet[i]
                    hhex = '%02x' % hvol
                    recv_data_str += hhex + ' '

                dt = datetime.now()
                timestr = dt.strftime('%y-%m-%d %H:%M:%S.%f')
                # QApplication.processEvents() # 可以实现一边执行耗时程序，一边刷新页面的功能
                self.CsvFiles.set_auto_bind_parameters(self.autoBindPro, self.auto_bind_list)
                write_result = self.CsvFiles.command_parsing_record(self.ai_setting, dt, recv_packet, recv_data_str)
                # print('write_result:%d' % write_result)
                if write_result == 1:
                    if self.checkBox_autoClear.isChecked():
                        if self.listWidget_commandData.count() > 2000:
                            self.listWidget_commandData.clear()
                    self.listWidget_commandData.addItem(timestr + ' recv<--:' + recv_data_str)
                    # print('total count:%d' % self.listWidget_commandData.count())
                    index = 0
                    has_no_nwkaddr = False
                    if self.CsvFiles.get_joined_info_finish:
                        if len(self.CsvFiles.joined_nodes_info):
                            # print("self.CsvFiles.joined_nodes_info:" + str(len(self.CsvFiles.joined_nodes_info)))
                            for node in self.CsvFiles.joined_nodes_info:
                                index += 1
                                self.getJoindNodeListWidget.addItem('%04d:' % index + '  ' + '0x%016x' % node +
                                                                    ' (0x%04x)' % self.CsvFiles.joined_nodes_info[node])
                                if self.CsvFiles.joined_nodes_info[node] == 0xfffe:
                                    has_no_nwkaddr = True
                                    self.get_joined_list = copy.deepcopy(self.CsvFiles.joined_nodes_info)

                        if has_no_nwkaddr:
                            self.getNwkAddrTimer.start(500)
                        else:
                            self.CsvFiles.joined_nodes_info = {}
                    self.CsvFiles.get_joined_info_finish = False

                    if len(self.auto_bind_list) != 0 and not self.bindReqSendTimer.isActive():
                        self.bindReqSendTimer.start(1000)
                elif write_result != 2:
                    QMessageBox.warning(self, 'write file warning', "Do not open the file when writing！")
                # print('self.CsvFiles.send_data len:%d' % len(self.CsvFiles.send_data))
                if len(self.CsvFiles.send_data):
                    self.send_serial_data(self.CsvFiles.send_data)

                if self.CsvFiles.hci_ota_offset != 0:
                    percent = int((self.CsvFiles.hci_ota_offset * 100) / self.CsvFiles.hci_ota_file_size)
                    if percent >= 100:
                        percent = 100
                    self.progressBar_hciOta.setValue(percent)

    def bdb_init(self):
        self.pushButton_BDBsetCh.clicked.connect(self.set_working_channel)
        self.pushButton_BDBstartNet.clicked.connect(self.start_network)
        self.pushButton_BDBfactoryRst.clicked.connect(self.factory_reset)
        self.pushButton_BDBpreinstallCode.clicked.connect(self.preinstall_code)
        self.pushButton_BDBdongleMode.clicked.connect(self.working_mode_set)
        self.pushButton_BDBnodeDelete.clicked.connect(self.node_delete)
        self.pushButton_BDBsetTXPower.clicked.connect(self.set_tx_power)
        self.pushButton_getLocalDevInfo.clicked.connect(self.get_locak_device_info)

    def set_working_channel(self):
        channel = int(self.comboBox_channelList.currentText())
        # print(channel)
        # print(type(channel))
        payload = struct.pack("!B", channel)
        # print(payload)
        self.send_hci_command(0x0007, len(payload), payload)

    def start_network(self):
        self.send_hci_command(0x0001, 0, 'None')

    def factory_reset(self):
        self.send_hci_command(0x0005, 0, 'None')

    def preinstall_code(self):
        dst_ieee_addr_s = self.lineEdit_dstAddr.text()
        # print(type(dst_ieee_addr_s))
        # print(dst_ieee_addr_s)
        ieee_addr = line_edit_str2int(8, dst_ieee_addr_s)

        install_code_s = self.lineEdit_installcode.text()
        # print(install_code_s)
        install_code = line_edit_str2int(16, install_code_s)

        payload = struct.pack("!%dB" % len(ieee_addr), *ieee_addr)
        payload += struct.pack("!%dB" % len(install_code), *install_code)

        self.send_hci_command(0x0006, len(payload), payload)

    def working_mode_set(self):
        mode_int = self.comboBox_mode.currentIndex()
        payload = struct.pack("!B", mode_int)
        self.send_hci_command(0x0008, len(payload), payload)

    def node_delete(self):
        dst_ieee_addr_s = self.lineEdit_macAddr.text()
        ieee_addr = line_edit_str2int(8, dst_ieee_addr_s)
        payload = struct.pack("!%dB" % len(ieee_addr), *ieee_addr)
        self.send_hci_command(0x0009, len(payload), payload)

    def set_tx_power(self):
        tx_power_s = self.lineEdit_TxIndex.text()
        tx_power = line_edit_str2int(1, tx_power_s)
        payload = struct.pack("!%dB" % len(tx_power), *tx_power)
        self.send_hci_command(0x000a, len(payload), payload)

    def get_locak_device_info(self):
        self.send_hci_command(0x0045, 0, 'None')

    def nodes_mgmt_init(self):
        self.pushButton_getJoinedNodes.clicked.connect(self.get_joined_nodes)
        self.pushButton_on.clicked.connect(self.chosen_nodes_on)
        self.pushButton_off.clicked.connect(self.chosen_nodes_off)
        self.pushButton_leave.clicked.connect(self.chosen_nodes_leave)
        self.get_choose_node_ieee_addr()

    def get_joined_nodes(self):
        start_idx = 0
        payload = struct.pack("!H", start_idx)
        self.getJoindNodeListWidget.clear()
        self.send_hci_command(0x0040, len(payload), payload)

    def get_choose_node_ieee_addr(self):  # get joined nodes info parse
        self.getJoindNodeListWidget.itemClicked.connect(self.get_joined_nodes_parse)
        self.getJoindNodeListWidget.currentItemChanged.connect(self.get_joined_nodes_parse)

    def get_joined_nodes_parse(self, item):
        try:
            get_data = item.text()
        except AttributeError:
            return
        node_ieee_info_str = get_data[7:25]
        self.choose_node_ieee = node_ieee_info_str
        self.choose_node_nwk = get_data[27:33]

    def chosen_nodes_on(self):
        dst_mode_s = 'ieee'
        dst_addr_s = self.choose_node_ieee
        src_ep_s = '0x01'
        dst_ep_s = '0x01'
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)
        self.send_hci_command(0x0140, len(payload), payload)

    def chosen_nodes_off(self):
        dst_mode_s = 'ieee'
        dst_addr_s = self.choose_node_ieee
        src_ep_s = '0x01'
        dst_ep_s = '0x01'
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)
        self.send_hci_command(0x0141, len(payload), payload)

    def chosen_nodes_leave(self):
        if self.choose_node_nwk != '0xfffe':
            dst_addr_s = self.choose_node_nwk
        else:
            dst_addr_s = '0xffff'
        dst_addr = line_edit_str2int(2, dst_addr_s)
        payload = struct.pack("!%dB" % len(dst_addr), *dst_addr)

        leave_ieee_addr_s = self.choose_node_ieee
        leave_ieee_addr = line_edit_str2int(8, leave_ieee_addr_s)
        payload += struct.pack("!%dB" % len(leave_ieee_addr), *leave_ieee_addr)

        rejoin = 0
        remove_child = 0
        payload += struct.pack("!2B", rejoin, remove_child)
        self.send_hci_command(0x0032, len(payload), payload)

    def mgmt_init(self):
        self.pushButton_permitJoin.clicked.connect(self.permit_join_req_send)
        self.pushButton_ieeeReq.clicked.connect(self.ieee_req_send)
        self.pushButton_nwkReq.clicked.connect(self.nwk_req_send)
        self.pushButton_activeReq.clicked.connect(self.active_req_send)
        self.pushButton_nodeDescReq.clicked.connect(self.node_desc_req_send)
        self.pushButton_simpleDescReq.clicked.connect(self.simple_desc_req_send)
        self.pushButton_matchDescReq.clicked.connect(self.match_desc_req_send)
        self.pushButton_bindReq.clicked.connect(self.bind_req_send)
        self.pushButton_unbindReq.clicked.connect(self.unbind_req_send)
        self.pushButton_lqiReq.clicked.connect(self.mgmt_liq_req_send)
        self.pushButton_mgmtBindReq.clicked.connect(self.mgmt_bind_req_send)
        self.pushButton_mgmtLeaveReq.clicked.connect(self.mgmt_leave_req_send)
        self.pushButton_nwkUpdateReq.clicked.connect(self.mgmt_nwk_update_req_send)

    def permit_join_req_send(self):
        # print('permit_join_req_send')
        dst_addr_s = self.lineEdit_permit_dstAddr.text()
        dst_addr = line_edit_str2int(2, dst_addr_s)
        payload = struct.pack("!%dB" % len(dst_addr), *dst_addr)
        # print(payload)
        interval_s = self.lineEdit_interval.text()
        interval = line_edit_str2int(1, interval_s)
        payload += struct.pack("!%dB" % len(interval), *interval)
        # print(payload)
        tc_sign = 1
        payload += struct.pack("!B", tc_sign)

        # print(payload)
        self.send_hci_command(0x0034, len(payload), payload)

    def ieee_req_send(self):
        dst_addr_s = self.lineEdit_ieeeReqdstAddr.text()
        dst_addr = line_edit_str2int(2, dst_addr_s)
        payload = struct.pack("!%dB" % len(dst_addr), *dst_addr)
        interest_addr_s = self.lineEdit_ieeeReqInterestAddr.text()
        interest_addr = line_edit_str2int(2, interest_addr_s)
        payload += struct.pack("!%dB" % len(interest_addr), *interest_addr)

        req_type = self.comboBox_ieeeReqType.currentIndex()
        payload += struct.pack("!B", req_type)

        start_idx_s = self.lineEdit_ieeeReqStartIdx.text()
        start_idx = line_edit_str2int(1, start_idx_s)
        payload += struct.pack("!%dB" % len(start_idx), *start_idx)
        self.send_hci_command(0x0011, len(payload), payload)

    def nwk_req_send(self):
        dst_addr_s = self.lineEdit_nwkReqDstAddr.text()
        dst_addr = line_edit_str2int(2, dst_addr_s)
        payload = struct.pack("!%dB" % len(dst_addr), *dst_addr)
        interest_addr_s = self.lineEdit_nwkReqInterestAddr.text()
        interest_addr = line_edit_str2int(8, interest_addr_s)
        payload += struct.pack("!%dB" % len(interest_addr), *interest_addr)

        req_type = self.comboBox_nwkReqReqtype.currentIndex()
        payload += struct.pack("!B", req_type)

        start_idx_s = self.lineEdit_nwkReqStartIdx.text()
        start_idx = line_edit_str2int(1, start_idx_s)
        payload += struct.pack("!%dB" % len(start_idx), *start_idx)
        self.send_hci_command(0x0010, len(payload), payload)

    def active_req_send(self):
        dst_addr_s = self.lineEdit_activeReqDstAddr.text()
        dst_addr = line_edit_str2int(2, dst_addr_s)
        payload = struct.pack("!%dB" % len(dst_addr), *dst_addr)
        interest_addr_s = self.lineEdit_activeReqInterestAddr.text()
        interest_addr = line_edit_str2int(2, interest_addr_s)
        payload += struct.pack("!%dB" % len(interest_addr), *interest_addr)
        self.send_hci_command(0x0015, len(payload), payload)

    def node_desc_req_send(self):
        dst_addr_s = self.lineEdit_nodeReqDstAddr.text()
        dst_addr = line_edit_str2int(2, dst_addr_s)
        payload = struct.pack("!%dB" % len(dst_addr), *dst_addr)
        interest_addr_s = self.lineEdit_nodeReqInterestAddr.text()
        interest_addr = line_edit_str2int(2, interest_addr_s)
        payload += struct.pack("!%dB" % len(interest_addr), *interest_addr)
        self.send_hci_command(0x0012, len(payload), payload)

    def simple_desc_req_send(self):
        dst_addr_s = self.lineEdit_simpleReqDstAddr.text()
        dst_addr = line_edit_str2int(2, dst_addr_s)
        payload = struct.pack("!%dB" % len(dst_addr), *dst_addr)
        interest_addr_s = self.lineEdit_simpleReqInterestAddr.text()
        interest_addr = line_edit_str2int(2, interest_addr_s)
        payload += struct.pack("!%dB" % len(interest_addr), *interest_addr)
        ep_s = self.lineEdit_simpleReqEndpoint.text()
        ep = line_edit_str2int(1, ep_s)
        payload += struct.pack("!%dB" % len(ep), *ep)
        self.send_hci_command(0x0013, len(payload), payload)

    def match_desc_req_send(self):
        dst_addr_s = self.lineEdit_matchReqDstAddr.text()
        dst_addr = line_edit_str2int(2, dst_addr_s)
        payload = struct.pack("!%dB" % len(dst_addr), *dst_addr)

        interest_addr_s = self.lineEdit_matchReqInterestAddr.text()
        interest_addr = line_edit_str2int(2, interest_addr_s)
        payload += struct.pack("!%dB" % len(interest_addr), *interest_addr)

        profile_id_s = self.lineEdit_matchReqProfileId.text()
        profile_id = line_edit_str2int(2, profile_id_s)
        payload += struct.pack("!%dB" % len(profile_id), *profile_id)

        incluster_id_s = self.lineEdit_matchReqInCluster.text()
        incluster_cnt = int((len(incluster_id_s) - 2) / 4)
        incluster_id = line_edit_str2int(incluster_cnt * 2, incluster_id_s)  # 2bytes in 1 cluster

        outcluster_id_s = self.lineEdit_matchReqOutCluster.text()
        # print(outcluster_id_s)
        outcluster_cnt = int((len(outcluster_id_s) - 2) / 4)
        outcluster_id = line_edit_str2int(outcluster_cnt * 2, outcluster_id_s)

        payload += struct.pack("!2B", incluster_cnt, outcluster_cnt)
        payload += struct.pack("!%dB" % len(incluster_id), *incluster_id)
        payload += struct.pack("!%dB" % len(outcluster_id), *outcluster_id)
        self.send_hci_command(0x0014, len(payload), payload)

    def bind_req_send(self):
        src_ieee_addr_s = self.lineEdit_bindReqSrcAddr.text()
        src_ieee_addr = line_edit_str2int(8, src_ieee_addr_s)
        payload = struct.pack("!%dB" % len(src_ieee_addr), *src_ieee_addr)

        src_ep_s = self.lineEdit_bindReqSrcEp.text()
        src_ep = line_edit_str2int(1, src_ep_s)
        payload += struct.pack("!%dB" % len(src_ep), *src_ep)

        cluster_id_s = self.lineEdit_bindReqClusterId.text()
        cluster_id = line_edit_str2int(2, cluster_id_s)
        payload += struct.pack("!%dB" % len(cluster_id), *cluster_id)

        dst_mode_s = self.comboBox_bindReqDstMode.currentText()
        dst_addr_s = self.lineEdit_bindReqDstAddr.text()
        if dst_mode_s == 'group':
            dst_mode = 1
            dst_addr = line_edit_str2int(2, dst_addr_s)
            payload += struct.pack("!B%dB" % len(dst_addr), dst_mode, *dst_addr)
        else:
            dst_mode = 3
            dst_addr = line_edit_str2int(8, dst_addr_s)
            payload += struct.pack("!B%dB" % len(dst_addr), dst_mode, *dst_addr)
            dst_ep_s = self.lineEdit_bindReqDstEp.text()
            dst_ep = line_edit_str2int(1, dst_ep_s)
            payload += struct.pack("!%dB" % len(dst_ep), *dst_ep)
        self.send_hci_command(0x0020, len(payload), payload)

    def unbind_req_send(self):
        src_ieee_addr_s = self.lineEdit_unbindReqSrcAddr.text()
        src_ieee_addr = line_edit_str2int(8, src_ieee_addr_s)
        payload = struct.pack("!%dB" % len(src_ieee_addr), *src_ieee_addr)

        src_ep_s = self.lineEdit_unbindReqSrcEp.text()
        src_ep = line_edit_str2int(1, src_ep_s)
        payload += struct.pack("!%dB" % len(src_ep), *src_ep)

        cluster_id_s = self.lineEdit_unbindReqClusterId.text()
        cluster_id = line_edit_str2int(2, cluster_id_s)
        payload += struct.pack("!%dB" % len(cluster_id), *cluster_id)

        dst_mode_s = self.comboBox_unbindReqDstMode.currentText()
        dst_addr_s = self.lineEdit_unbindReqDstAddr.text()
        if dst_mode_s == 'group':
            dst_mode = 1
            dst_addr = line_edit_str2int(2, dst_addr_s)
            payload += struct.pack("!B%dB" % len(dst_addr), dst_mode, *dst_addr)
        else:
            dst_mode = 3
            dst_addr = line_edit_str2int(8, dst_addr_s)
            payload += struct.pack("!B%dB" % len(dst_addr), dst_mode, *dst_addr)
            dst_ep_s = self.lineEdit_unbindReqDstEp.text()
            dst_ep = line_edit_str2int(1, dst_ep_s)
            payload += struct.pack("!%dB" % len(dst_ep), *dst_ep)
        self.send_hci_command(0x0021, len(payload), payload)

    def mgmt_liq_req_send(self):
        dst_addr_s = self.lineEdit_lqiReqDstAddr.text()
        dst_addr = line_edit_str2int(2, dst_addr_s)
        payload = struct.pack("!%dB" % len(dst_addr), *dst_addr)

        start_idx_s = self.lineEdit_lqiReqStartIdx.text()
        start_idx = line_edit_str2int(1, start_idx_s)
        payload += struct.pack("!%dB" % len(start_idx), *start_idx)
        self.send_hci_command(0x0030, len(payload), payload)

    def mgmt_bind_req_send(self):
        dst_addr_s = self.lineEdit_mgmtBindReqDstAddr.text()
        dst_addr = line_edit_str2int(2, dst_addr_s)
        payload = struct.pack("!%dB" % len(dst_addr), *dst_addr)

        start_idx_s = self.lineEdit_mgmtBindReqStartIdx.text()
        start_idx = line_edit_str2int(1, start_idx_s)
        payload += struct.pack("!%dB" % len(start_idx), *start_idx)
        self.send_hci_command(0x0031, len(payload), payload)

    def mgmt_leave_req_send(self):
        dst_addr_s = self.lineEdit_mgmtLeaveReqDstAddr.text()
        dst_addr = line_edit_str2int(2, dst_addr_s)
        payload = struct.pack("!%dB" % len(dst_addr), *dst_addr)

        leave_ieee_addr_s = self.lineEdit_leaveIeeeAddr.text()
        leave_ieee_addr = line_edit_str2int(8, leave_ieee_addr_s)
        payload += struct.pack("!%dB" % len(leave_ieee_addr), *leave_ieee_addr)

        rejoin_s = self.lineEdit_rejoin.text()
        rejoin = line_edit_str2int(1, rejoin_s)
        payload += struct.pack("!%dB" % len(rejoin), *rejoin)

        remove_child_s = self.lineEdit_removeChild.text()
        remove_child = line_edit_str2int(1, remove_child_s)
        payload += struct.pack("!%dB" % len(remove_child), *remove_child)
        self.send_hci_command(0x0032, len(payload), payload)

    def mgmt_nwk_update_req_send(self):
        dst_addr_s = self.lineEdit_nwkUpdateReqDstAddr.text()
        dst_addr = line_edit_str2int(2, dst_addr_s)
        payload = struct.pack("!%dB" % len(dst_addr), *dst_addr)

        nwk_manager_addr_s = self.lineEdit_nwkUpdateReqMgtAddr.text()
        nwk_manager_addr = line_edit_str2int(2, nwk_manager_addr_s)
        payload += struct.pack("!%dB" % len(nwk_manager_addr), *nwk_manager_addr)

        scan_channel_s = self.lineEdit_nwkUpdateReqChannel.text()
        scan_channel = line_edit_str2int(4, scan_channel_s)
        payload += struct.pack("!%dB" % len(scan_channel), *scan_channel)

        scan_duration_s = self.lineEdit_nwkUpdateReqDuration.text()
        scan_duration = line_edit_str2int(1, scan_duration_s)
        payload += struct.pack("!%dB" % len(scan_duration), *scan_duration)

        scan_count_s = self.lineEdit_nwkUpdateReqCount.text()
        scan_count = line_edit_str2int(1, scan_count_s)
        payload += struct.pack("!%dB" % len(scan_count), *scan_count)
        self.send_hci_command(0x0035, len(payload), payload)

    def zcl_general_init(self):
        self.pushButton_genRead.clicked.connect(self.zcl_general_attr_read)
        self.pushButton_writeAttr.clicked.connect(self.zcl_general_attr_write)
        self.pushButton_genConfig.clicked.connect(self.zcl_general_config_report)
        self.pushButton_readReportCfg.clicked.connect(self.zcl_general_read_report_config)
        self.pushButton_genReset.clicked.connect(self.zcl_basic_reset)

    def zcl_general_addr_handle(self, dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s, profile_id_s, direct, cluster_id_s):
        dst_mode = self.ai_setting.addr_mode[dst_mode_s]
        # print(dst_mode)
        payload = struct.pack("!B", dst_mode)
        if dst_mode_s == 'ieee' or dst_mode_s == 'ieee_no_ack':
            dst_addr = line_edit_str2int(8, dst_addr_s)
            payload += struct.pack("!%dB" % len(dst_addr), *dst_addr)
        elif dst_mode_s == 'short' or dst_mode_s == 'short_no_ack' or dst_mode_s == 'group' or \
                dst_mode_s == 'broadcast':
            dst_addr = line_edit_str2int(2, dst_addr_s)
            payload += struct.pack("!%dB" % len(dst_addr), *dst_addr)

        src_ep = line_edit_str2int(1, src_ep_s)
        payload += struct.pack("!%dB" % len(src_ep), *src_ep)

        if dst_mode_s == 'bound' or dst_mode_s == 'group' or dst_mode_s == 'bound_no_ack':
            pass
        else:
            dst_ep = line_edit_str2int(1, dst_ep_s)
            payload += struct.pack("!%dB" % len(dst_ep), *dst_ep)

        profile_id = line_edit_str2int(2, profile_id_s)
        payload += struct.pack("!%dB" % len(profile_id), *profile_id)

        if direct is not None:
            payload += struct.pack("!B", direct)

        if cluster_id_s is not None:
            cluster_id = line_edit_str2int(2, cluster_id_s)
            payload += struct.pack("!%dB" % len(cluster_id), *cluster_id)
        # print(payload)
        return payload

    def zcl_cluster_addr_handle(self, dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s):
        dst_mode = self.ai_setting.addr_mode[dst_mode_s]
        # print(dst_mode)
        payload = struct.pack("!B", dst_mode)
        if dst_mode_s == 'ieee' or dst_mode_s == 'ieee_no_ack':
            dst_addr = line_edit_str2int(8, dst_addr_s)
            payload += struct.pack("!%dB" % len(dst_addr), *dst_addr)
        elif dst_mode_s == 'short' or dst_mode_s == 'short_no_ack' or dst_mode_s == 'group' or \
                dst_mode_s == 'broadcast':
            dst_addr = line_edit_str2int(2, dst_addr_s)
            payload += struct.pack("!%dB" % len(dst_addr), *dst_addr)

        src_ep = line_edit_str2int(1, src_ep_s)
        payload += struct.pack("!%dB" % len(src_ep), *src_ep)

        if dst_mode_s == 'bound' or dst_mode_s == 'group' or dst_mode_s == 'bound_no_ack':
            pass
        else:
            dst_ep = line_edit_str2int(1, dst_ep_s)
            payload += struct.pack("!%dB" % len(dst_ep), *dst_ep)
        # print(payload)
        return payload

    def zcl_general_attr_read(self):
        dst_mode_s = self.comboBox_readAttrDstMode.currentText()
        dst_addr_s = self.lineEdit_readAttrDstAddr.text()
        src_ep_s = self.lineEdit_readAttrSrcEp.text()
        dst_ep_s = self.lineEdit_readAttrDstEp.text()
        profile_id_s = self.lineEdit_readAttrProfileId.text()
        direct = self.comboBox_readAttrDirect.currentIndex()
        cluster_id_s = self.lineEdit_readAttrClusterId.text()
        payload = self.zcl_general_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s, profile_id_s, direct,
                                               cluster_id_s)

        attr_id_s = self.lineEdit_readAttrAttrId.text()
        attr_id_cnt = int((len(attr_id_s) - 2) / 4)
        if (len(attr_id_s) - 2) > 0 and attr_id_cnt == 0:
            attr_id_cnt = 1
        attr_id = line_edit_str2int(attr_id_cnt * 2, attr_id_s)
        payload += struct.pack("!B%dB" % len(attr_id), attr_id_cnt, *attr_id)
        self.send_hci_command(0x0100, len(payload), payload)

    def zcl_general_attr_write(self):
        dst_mode_s = self.comboBox_writeAttDstMode.currentText()
        dst_addr_s = self.lineEdit_writeAttrDstAddr.text()
        src_ep_s = self.lineEdit_writeAttrSrcEp.text()
        dst_ep_s = self.lineEdit_writeAttrDstEp.text()
        profile_id_s = self.lineEdit_writeAttrProfileId.text()
        direct = self.comboBox_writeAttrDirect.currentIndex()
        cluster_id_s = self.lineEdit_writeAttrClusterId.text()
        payload = self.zcl_general_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s, profile_id_s, direct,
                                               cluster_id_s)

        attr_id_s = self.lineEdit_writeAttrAttrId.text()
        attr_id_cnt = int((len(attr_id_s) - 2) / 4)
        if (len(attr_id_s) - 2) > 0 and attr_id_cnt == 0:
            attr_id_cnt = 1
        attr_id = line_edit_str2int(attr_id_cnt * 2, attr_id_s)
        payload += struct.pack("!B%dB" % len(attr_id), attr_id_cnt, *attr_id)

        data_type_idx = self.comboBox_writeAttrDataType.currentIndex()
        # print(data_type_idx)
        data_type = self.ai_setting.data_type_list[data_type_idx]
        # print(data_type)
        data_s = self.lineEdit_writeAttrData.text()
        data_hex = simple_str2hex(data_s)
        data_len = self.ai_setting.get_attr_size(data_type, data_hex)
        # print(data_len)
        data = line_edit_str2int(data_len, data_s)
        payload += struct.pack("!B%dB" % len(data), data_type, *data)

        self.send_hci_command(0x0101, len(payload), payload)

    def zcl_general_config_report(self):
        dst_mode_s = self.comboBox_configReportDstMode.currentText()
        dst_addr_s = self.lineEdit_configReportDstAddr.text()
        src_ep_s = self.lineEdit_configReportSrcEp.text()
        dst_ep_s = self.lineEdit_configReportDstEp.text()
        profile_id_s = self.lineEdit_configReportProfileId.text()
        direct = self.comboBox_configReportDirect.currentIndex()
        cluster_id_s = self.lineEdit_configReportClusterId.text()
        payload = self.zcl_general_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s, profile_id_s, direct,
                                               cluster_id_s)

        report_dir = self.comboBox_configReportSendType.currentIndex()

        attr_id_cnt = 1
        attr_id_s = self.lineEdit_configReportAttrId.text()
        # attr_id_cnt = int((len(attr_id_s) - 2) / 4)
        attr_id = line_edit_str2int(attr_id_cnt * 2, attr_id_s)
        payload += struct.pack("!2B%dB" % len(attr_id), attr_id_cnt, report_dir, *attr_id)
        if report_dir == 0:
            data_type_idx = self.comboBox_configReportDataType.currentIndex()
            data_type = self.ai_setting.data_type_list[data_type_idx]
            payload += struct.pack("!B", data_type)

            min_interval_s = self.lineEdit_configReportMinInt.text()
            min_interval = line_edit_str2int(2, min_interval_s)
            payload += struct.pack("!%dB" % len(min_interval), *min_interval)

            max_interval_s = self.lineEdit_configReportMaxInt.text()
            max_interval = line_edit_str2int(2, max_interval_s)
            payload += struct.pack("!%dB" % len(max_interval), *max_interval)

            # print('zcl_general_config_report, data_type:0x%x' % data_type)
            if self.ai_setting.is_analog_data_type(data_type):
                date_len = self.ai_setting.get_data_type_len(data_type)
                data_s = self.lineEdit_configReportChange.text()
                data = line_edit_str2int(date_len, data_s)
                payload += struct.pack("!%dB" % len(data), *data)
        else:
            timeout_period_s = self.lineEdit_configReportTimeOut.text()
            timeout_period = line_edit_str2int(2, timeout_period_s)
            payload += struct.pack("!%dB" % len(timeout_period), *timeout_period)
        self.send_hci_command(0x0102, len(payload), payload)

    def zcl_general_read_report_config(self):
        dst_mode_s = self.comboBox_readReportCfgDstMode.currentText()
        dst_addr_s = self.lineEdit_readReportCfgDstAddr.text()
        src_ep_s = self.lineEdit_readReportCfgSrcEp.text()
        dst_ep_s = self.lineEdit_readReportCfgDstEp.text()
        profile_id_s = self.lineEdit_readReportCfgProfileId.text()
        direct = self.comboBox_readReportCfgDir.currentIndex()
        cluster_id_s = self.lineEdit_readReportCfgClusterId.text()
        payload = self.zcl_general_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s, profile_id_s, direct,
                                               cluster_id_s)

        report_dir = self.comboBox_readReportCfgType.currentIndex()

        attr_id_s = self.lineEdit_readReportCfgAttrId.text()
        attr_id_cnt = int((len(attr_id_s) - 2) / 4)
        if (len(attr_id_s) - 2) > 0 and attr_id_cnt == 0:
            attr_id_cnt = 1
        attr_id = line_edit_str2int(attr_id_cnt * 2, attr_id_s)
        payload += struct.pack("!2B%dB" % len(attr_id), attr_id_cnt, report_dir, *attr_id)
        self.send_hci_command(0x0103, len(payload), payload)

    def zcl_basic_reset(self):
        dst_mode_s = self.comboBox_resetDstMode.currentText()
        dst_addr_s = self.lineEdit_resetDstAddr.text()
        src_ep_s = self.lineEdit_resetSrcEp.text()
        dst_ep_s = self.lineEdit_resetDstEp.text()

        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)
        self.send_hci_command(0x0110, len(payload), payload)

    def zcl_group_init(self):
        self.pushButton_addGroup.clicked.connect(self.zcl_add_group)
        self.pushButton_viewGroup.clicked.connect(self.zcl_view_group)
        self.pushButton_getGroup.clicked.connect(self.zcl_get_group)
        self.pushButton_removeGroup.clicked.connect(self.zcl_remove_group)
        self.pushButton_removeAll.clicked.connect(self.zcl_remove_all_group)
        self.pushButton_addIfIdentify.clicked.connect(self.zcl_add_group_if_identify)

    def zcl_add_group(self):
        dst_mode_s = self.comboBox_addGroupDstmode.currentText()
        dst_addr_s = self.lineEdit_addGroupDstAddr.text()
        src_ep_s = self.lineEdit_addGroupSrcEp.text()
        dst_ep_s = self.lineEdit_addGroupDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        group_id_s = self.lineEdit_addGroupGroupId.text()
        group_id = line_edit_str2int(2, group_id_s)
        payload += struct.pack("!%dB" % len(group_id), *group_id)
        group_name_s = self.lineEdit_addGroupName.text()
        if group_name_s:
            group_name_len = int((len(group_name_s) - 2) / 2)
            group_name = line_edit_str2int(group_name_len, group_name_s)
            # print('group_name_len:%d' % group_name_len)
            payload += struct.pack("!B%dB" % len(group_name), group_name_len, *group_name)
        else:
            group_name_len = 0
            payload += struct.pack("!B", group_name_len)

        self.send_hci_command(0x0120, len(payload), payload)

    def zcl_view_group(self):
        dst_mode_s = self.comboBox_viewGroupDstMode.currentText()
        dst_addr_s = self.lineEdit_viewGroupDstAddr.text()
        src_ep_s = self.lineEdit_viewGroupSrcEp.text()
        dst_ep_s = self.lineEdit_viewGroupDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        group_id_s = self.lineEdit_viewGroupGroupId.text()
        group_id = line_edit_str2int(2, group_id_s)
        payload += struct.pack("!%dB" % len(group_id), *group_id)
        self.send_hci_command(0x0121, len(payload), payload)

    def zcl_get_group(self):
        dst_mode_s = self.comboBox_getGroupDstMode.currentText()
        dst_addr_s = self.lineEdit_getGroupDstAddr.text()
        src_ep_s = self.lineEdit_getGroupSrcEp.text()
        dst_ep_s = self.lineEdit_getGroupDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        group_list_s = self.lineEdit_getGroupList.text()
        # print(group_list_s)
        group_list_cnt = int((len(group_list_s) - 2) / 4)
        group_list = line_edit_str2int(group_list_cnt * 2, group_list_s)
        payload += struct.pack("!B%dB" % len(group_list), group_list_cnt, *group_list)
        self.send_hci_command(0x0122, len(payload), payload)

    def zcl_remove_group(self):
        dst_mode_s = self.comboBox_rmGroupDstMode.currentText()
        dst_addr_s = self.lineEdit_rmGroupDstAddr.text()
        src_ep_s = self.lineEdit_rmGroupSrcEp.text()
        dst_ep_s = self.lineEdit_rmGroupDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        group_id_s = self.lineEdit_rmGroupId.text()
        group_id = line_edit_str2int(2, group_id_s)
        payload += struct.pack("!%dB" % len(group_id), *group_id)
        self.send_hci_command(0x0123, len(payload), payload)

    def zcl_remove_all_group(self):
        dst_mode_s = self.comboBox_removeAllDstMode.currentText()
        dst_addr_s = self.lineEdit_removeAllDstAddr.text()
        src_ep_s = self.lineEdit_removeAllSrcEp.text()
        dst_ep_s = self.lineEdit_removeAllDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        self.send_hci_command(0x0124, len(payload), payload)

    def zcl_add_group_if_identify(self):
        dst_mode_s = self.comboBox_addIdenDstMode.currentText()
        dst_addr_s = self.lineEdit_addIdenDstAddr.text()
        src_ep_s = self.lineEdit_addIdenSrcEp.text()
        dst_ep_s = self.lineEdit_addIdenDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        group_id_s = self.lineEdit_addIdenGroupId.text()
        group_id = line_edit_str2int(2, group_id_s)
        payload += struct.pack("!%dB" % len(group_id), *group_id)
        group_name_s = self.lineEdit_addIdenName.text()
        if group_name_s:
            group_name_len = int((len(group_name_s) - 2) / 2)
            group_name = line_edit_str2int(group_name_len, group_name_s)
            payload += struct.pack("!B%dB" % len(group_name), group_name_len, *group_name)
        else:
            group_name_len = 0
            payload += struct.pack("!B", group_name_len)
        self.send_hci_command(0x0125, len(payload), payload)

    def zcl_onoff_init(self):
        self.pushButton_onOff.clicked.connect(self.zcl_onoff)

    def zcl_onoff(self):
        dst_mode_s = self.comboBox_onoffDstMode.currentText()
        dst_addr_s = self.lineEdit_onoffDstAddr.text()
        src_ep_s = self.lineEdit_onoffSrcEp.text()
        dst_ep_s = self.lineEdit_onoffDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        on_off = self.comboBox_onOff.currentText()
        if on_off == 'on':
            command_id = 0x0140
        elif on_off == 'off':
            command_id = 0x0141
        elif on_off == 'toggle':
            command_id = 0x0142
        else:
            command_id = 0xffff
        self.send_hci_command(command_id, len(payload), payload)

    def zcl_level_init(self):
        self.pushButton_moveToLevel.clicked.connect(self.zcl_level_move_to_level)
        self.pushButton_move.clicked.connect(self.zcl_level_move)
        self.pushButton_step.clicked.connect(self.zcl_level_step)
        self.pushButton_stop.clicked.connect(self.zcl_level_stop)

    def zcl_level_move_to_level(self):
        dst_mode_s = self.comboBox_toLevelDstMode.currentText()
        dst_addr_s = self.lineEdit_toLevelDstAddr.text()
        src_ep_s = self.lineEdit_toLevelSrcEp.text()
        dst_ep_s = self.lineEdit_toLevelDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        level_s = self.lineEdit_toLevelLevel.text()
        level = line_edit_str2int(1, level_s)
        payload += struct.pack("!%dB" % len(level), *level)

        transition_time_s = self.lineEdit_toLevelTime.text()
        transition_time = line_edit_str2int(2, transition_time_s)
        payload += struct.pack("!%dB" % len(transition_time), *transition_time)

        with_on_off = self.comboBox_withOnOff.currentText()
        if with_on_off == 'with onOff':
            command_id = 0x0154
        else:
            command_id = 0x0150
        self.send_hci_command(command_id, len(payload), payload)

    def zcl_level_move(self):
        dst_mode_s = self.comboBox_moveDstMode.currentText()
        dst_addr_s = self.lineEdit_moveDstAddr.text()
        src_ep_s = self.lineEdit_moveSrcEp.text()
        dst_ep_s = self.lineEdit_moveDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        move_mode_s = self.lineEdit_moveMode.text()
        move_mode = line_edit_str2int(1, move_mode_s)
        payload += struct.pack("!%dB" % len(move_mode), *move_mode)

        move_rate_s = self.lineEdit_moveRate.text()
        move_rate = line_edit_str2int(1, move_rate_s)
        payload += struct.pack("!%dB" % len(move_rate), *move_rate)

        with_on_off = self.comboBox_moveWithOnoff.currentText()
        if with_on_off == 'with onOff':
            command_id = 0x0155
        else:
            command_id = 0x0151
        self.send_hci_command(command_id, len(payload), payload)

    def zcl_level_step(self):
        dst_mode_s = self.comboBox_stepDstMode.currentText()
        dst_addr_s = self.lineEdit_stepDstAddr.text()
        src_ep_s = self.lineEdit_stepSrcEp.text()
        dst_ep_s = self.lineEdit_stepDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        move_mode_s = self.lineEdit_stepMode.text()
        move_mode = line_edit_str2int(1, move_mode_s)
        payload += struct.pack("!%dB" % len(move_mode), *move_mode)

        step_size_s = self.lineEdit_stepSize.text()
        step_size = line_edit_str2int(1, step_size_s)
        payload += struct.pack("!%dB" % len(step_size), *step_size)

        transition_time_s = self.lineEdit_stepTime.text()
        transition_time = line_edit_str2int(2, transition_time_s)
        payload += struct.pack("!%dB" % len(transition_time), *transition_time)

        with_on_off = self.comboBox_moveWithOnoff.currentText()
        if with_on_off == 'with onOff':
            command_id = 0x0156
        else:
            command_id = 0x0152
        self.send_hci_command(command_id, len(payload), payload)

    def zcl_level_stop(self):
        dst_mode_s = self.comboBox_stepDstMode.currentText()
        dst_addr_s = self.lineEdit_stepDstAddr.text()
        src_ep_s = self.lineEdit_stepSrcEp.text()
        dst_ep_s = self.lineEdit_stepDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        with_on_off = self.comboBox_moveWithOnoff.currentText()
        if with_on_off == 'with onOff':
            command_id = 0x0157
        else:
            command_id = 0x0153
        self.send_hci_command(command_id, len(payload), payload)

    def zcl_color_init(self):
        self.pushButton_moveHue.clicked.connect(self.zcl_color_move_to_hue)
        self.pushButton_color.clicked.connect(self.zcl_color_move_to_color)
        self.pushButton_saturation.clicked.connect(self.zcl_color_move_to_saturation)
        self.pushButton_temperature.clicked.connect(self.zcl_color_move_to_temperature)

    def zcl_color_move_to_hue(self):
        dst_mode_s = self.comboBox_moveHueDstMode.currentText()
        dst_addr_s = self.lineEdit_moveHueDstAddr.text()
        src_ep_s = self.lineEdit_moveHueSrcEp.text()
        dst_ep_s = self.lineEdit_moveHueDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        hue_s = self.lineEdit_moveHueHue.text()
        hue = line_edit_str2int(1, hue_s)
        payload += struct.pack("!%dB" % len(hue), *hue)

        direction_s = self.lineEdit_moveHueDir.text()
        direction = line_edit_str2int(1, direction_s)
        payload += struct.pack("!%dB" % len(direction), *direction)

        transition_time_s = self.lineEdit_moveHueTime.text()
        transition_time = line_edit_str2int(2, transition_time_s)
        payload += struct.pack("!%dB" % len(transition_time), *transition_time)
        self.send_hci_command(0x0170, len(payload), payload)

    def zcl_color_move_to_color(self):
        dst_mode_s = self.comboBox_moveColorDstMode.currentText()
        dst_addr_s = self.lineEdit_moveColorDstAddr.text()
        src_ep_s = self.lineEdit_moveColorSrcEp.text()
        dst_ep_s = self.lineEdit_moveColorDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        color_x_s = self.lineEdit_moveColorX.text()
        color_x = line_edit_str2int(2, color_x_s)
        payload += struct.pack("!%dB" % len(color_x), *color_x)

        color_y_s = self.lineEdit_moveColorY.text()
        color_y = line_edit_str2int(2, color_y_s)
        payload += struct.pack("!%dB" % len(color_y), *color_y)

        transition_time_s = self.lineEdit_moveColorTime.text()
        transition_time = line_edit_str2int(2, transition_time_s)
        payload += struct.pack("!%dB" % len(transition_time), *transition_time)
        self.send_hci_command(0x0171, len(payload), payload)

    def zcl_color_move_to_saturation(self):
        dst_mode_s = self.comboBox_moveSatDstMode.currentText()
        dst_addr_s = self.lineEdit_moveSatDstAddr.text()
        src_ep_s = self.lineEdit_moveSatSrcEp.text()
        dst_ep_s = self.lineEdit_moveSatDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        saturation_s = self.lineEdit_moveSatSat.text()
        saturation = line_edit_str2int(1, saturation_s)
        payload += struct.pack("!%dB" % len(saturation), *saturation)

        transition_time_s = self.lineEdit_moveSatTime.text()
        transition_time = line_edit_str2int(2, transition_time_s)
        payload += struct.pack("!%dB" % len(transition_time), *transition_time)
        self.send_hci_command(0x0172, len(payload), payload)

    def zcl_color_move_to_temperature(self):
        dst_mode_s = self.comboBox_moveTempDstMode.currentText()
        dst_addr_s = self.lineEdit_moveTempDstAddr.text()
        src_ep_s = self.lineEdit_moveTempSrcEp.text()
        dst_ep_s = self.lineEdit_moveTempDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        saturation_s = self.lineEdit_moveTempTemp.text()
        saturation = line_edit_str2int(2, saturation_s)
        payload += struct.pack("!%dB" % len(saturation), *saturation)

        transition_time_s = self.lineEdit_moveTempTime.text()
        transition_time = line_edit_str2int(2, transition_time_s)
        payload += struct.pack("!%dB" % len(transition_time), *transition_time)
        self.send_hci_command(0x0173, len(payload), payload)

    def zcl_identify_init(self):
        self.pushButton_identify.clicked.connect(self.zcl_identify)
        self.pushButton_identifyQuery.clicked.connect(self.zcl_identify_query)

    def zcl_identify(self):
        dst_mode_s = self.comboBox_identifyDstMode.currentText()
        dst_addr_s = self.lineEdit_identifyDstAddr.text()
        src_ep_s = self.lineEdit_identifySrcEp.text()
        dst_ep_s = self.lineEdit_identifyDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        identify_time_s = self.lineEdit_identifyTime.text()
        identify_time = line_edit_str2int(2, identify_time_s)
        payload += struct.pack("!%dB" % len(identify_time), *identify_time)
        self.send_hci_command(0x0130, len(payload), payload)

    def zcl_identify_query(self):
        dst_mode_s = self.comboBox_identifyQueryDstMode.currentText()
        dst_addr_s = self.lineEdit_identifyQueryDstAddr.text()
        src_ep_s = self.lineEdit_identifyQuerySrcEp.text()
        dst_ep_s = self.lineEdit_identifyQueryDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)
        self.send_hci_command(0x0131, len(payload), payload)

    def zcl_scene_init(self):
        self.pushButton_addScene.clicked.connect(self.zcl_scene_add_scene)
        self.pushButton_viewScene.clicked.connect(self.zcl_scene_view_scene)
        self.pushButton_removeScene.clicked.connect(self.zcl_scene_remove_scene)
        self.pushButton_sceneRemoveAll.clicked.connect(self.zcl_scene_remove_all)
        self.pushButton_storeScene.clicked.connect(self.zcl_scene_store_scene)
        self.pushButton_recallScene.clicked.connect(self.zcl_scene_recall_scene)
        self.pushButton_sceneGetMembership.clicked.connect(self.zcl_scene_get_membership)

    def zcl_zbd_init(self):
        self.pushButton_if_state_set.clicked.connect(self.zcl_zbd_set_interface_state)
        self.pushButton_anj_timeout.clicked.connect(self.zcl_zbd_set_anj_timeout)

    def zcl_scene_add_scene(self):
        dst_mode_s = self.comboBox_addSceneDstMode.currentText()
        dst_addr_s = self.lineEdit_addSceneDstAddr.text()
        src_ep_s = self.lineEdit_addSceneSrcEp.text()
        dst_ep_s = self.lineEdit_addSceneDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        group_id_s = self.lineEdit_addSceneGroupId.text()
        group_id = line_edit_str2int(2, group_id_s)
        payload += struct.pack("!%dB" % len(group_id), *group_id)

        scene_id_s = self.lineEdit_addSceneSceneId.text()
        scene_id = line_edit_str2int(1, scene_id_s)
        payload += struct.pack("!%dB" % len(scene_id), *scene_id)

        transition_time_s = self.lineEdit_addSceneTime.text()
        transition_time = line_edit_str2int(2, transition_time_s)
        payload += struct.pack("!%dB" % len(transition_time), *transition_time)

        scene_name_len_s = self.lineEdit_addSceneNameLen.text()
        if scene_name_len_s:
            # print('scene_name_len_s:')
            scene_name_len = line_edit_str2int(1, scene_name_len_s)
            # print(scene_name_len)
            payload += struct.pack("!%dB" % len(scene_name_len), *scene_name_len)

            scene_name_s = self.lineEdit_addSceneName.text()
            scene_name = line_edit_str2int(scene_name_len[0], scene_name_s)
            payload += struct.pack("!%dB" % len(scene_name), *scene_name)
        else:
            scene_name_len = 0
            payload += struct.pack("!B", scene_name_len)

        ext_len_s = self.lineEdit_addSceneExtLen.text()
        if ext_len_s:
            # print('ext_len_s:')
            # print(ext_len_s)
            ext_len = line_edit_str2int(1, ext_len_s)
            payload += struct.pack("!%dB" % len(ext_len), *ext_len)

            ext_field_s = self.lineEdit_addSceneExtField.text()
            ext_field = line_edit_str2int(ext_len[0], ext_field_s)
            payload += struct.pack("!%dB" % len(ext_field), *ext_field)
        else:
            ext_len = 0
            payload += struct.pack("!B", ext_len)

        self.send_hci_command(0x0160, len(payload), payload)

    def zcl_scene_view_scene(self):
        dst_mode_s = self.comboBox_viewSceneDstMode.currentText()
        dst_addr_s = self.lineEdit_viewSceneDstAddr.text()
        src_ep_s = self.lineEdit_viewSceneSrcEp.text()
        dst_ep_s = self.lineEdit_viewSceneDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        group_id_s = self.lineEdit_viewSceneGroupId.text()
        group_id = line_edit_str2int(2, group_id_s)
        payload += struct.pack("!%dB" % len(group_id), *group_id)

        scene_id_s = self.lineEdit_viewSceneId.text()
        scene_id = line_edit_str2int(1, scene_id_s)
        payload += struct.pack("!%dB" % len(scene_id), *scene_id)
        self.send_hci_command(0x0161, len(payload), payload)

    def zcl_scene_remove_scene(self):
        dst_mode_s = self.comboBox_removeSceneDstMode.currentText()
        dst_addr_s = self.lineEdit_removeSceneDstAddr.text()
        src_ep_s = self.lineEdit_removeSceneSrcEp.text()
        dst_ep_s = self.lineEdit_removeSceneDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        group_id_s = self.lineEdit_removeSceneGroupId.text()
        group_id = line_edit_str2int(2, group_id_s)
        payload += struct.pack("!%dB" % len(group_id), *group_id)

        scene_id_s = self.lineEdit_removeSceneId.text()
        scene_id = line_edit_str2int(1, scene_id_s)
        payload += struct.pack("!%dB" % len(scene_id), *scene_id)
        self.send_hci_command(0x0162, len(payload), payload)

    def zcl_scene_remove_all(self):
        dst_mode_s = self.comboBox_sceneRemoveAllDstMode.currentText()
        dst_addr_s = self.lineEdit_sceneRemoveAllDstAddr.text()
        src_ep_s = self.lineEdit_sceneRemoveAllSrcEp.text()
        dst_ep_s = self.lineEdit_sceneRemoveAllDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        group_id_s = self.lineEdit_sceneRemoveAllGroupId.text()
        group_id = line_edit_str2int(2, group_id_s)
        payload += struct.pack("!%dB" % len(group_id), *group_id)
        self.send_hci_command(0x0163, len(payload), payload)

    def zcl_scene_store_scene(self):
        dst_mode_s = self.comboBox_storeSceneDstMode.currentText()
        dst_addr_s = self.lineEdit_storeSceneDstAddr.text()
        src_ep_s = self.lineEdit_storeSceneSrcEp.text()
        dst_ep_s = self.lineEdit_storeSceneDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        group_id_s = self.lineEdit_storeSceneGroupId.text()
        group_id = line_edit_str2int(2, group_id_s)
        payload += struct.pack("!%dB" % len(group_id), *group_id)

        scene_id_s = self.lineEdit_storeSceneId.text()
        scene_id = line_edit_str2int(1, scene_id_s)
        payload += struct.pack("!%dB" % len(scene_id), *scene_id)
        self.send_hci_command(0x0164, len(payload), payload)

    def zcl_scene_recall_scene(self):
        dst_mode_s = self.comboBox_recallSceneDstMode.currentText()
        dst_addr_s = self.lineEdit_recallSceneDstAddr.text()
        src_ep_s = self.lineEdit_recallSceneSrcEp.text()
        dst_ep_s = self.lineEdit_recallSceneDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        group_id_s = self.lineEdit_recallSceneGroupId.text()
        group_id = line_edit_str2int(2, group_id_s)
        payload += struct.pack("!%dB" % len(group_id), *group_id)

        scene_id_s = self.lineEdit_recallSceneId.text()
        scene_id = line_edit_str2int(1, scene_id_s)
        payload += struct.pack("!%dB" % len(scene_id), *scene_id)
        self.send_hci_command(0x0165, len(payload), payload)

    def zcl_scene_get_membership(self):
        dst_mode_s = self.comboBox_getSceneDstMode.currentText()
        dst_addr_s = self.lineEdit_getSceneDstAddr.text()
        src_ep_s = self.lineEdit_getSceneSrcEp.text()
        dst_ep_s = self.lineEdit_getSceneDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        group_id_s = self.lineEdit_getSceneGroupId.text()
        group_id = line_edit_str2int(2, group_id_s)
        payload += struct.pack("!%dB" % len(group_id), *group_id)
        self.send_hci_command(0x0166, len(payload), payload)

    def zcl_zbd_set_interface_state(self):
        dst_mode_s = self.comboBox_ifSetDstMode.currentText()
        dst_addr_s = self.lineEdit_ifStateSetDstAddr.text()
        src_ep_s = self.lineEdit_ifStateSrcEp.text()
        dst_ep_s = self.lineEdit_ifStateDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        if_state_s = self.lineEdit_ifState.text()
        if_state = line_edit_str2int(1, if_state_s)
        payload += struct.pack("!%dB" % len(if_state), *if_state)
        self.send_hci_command(0x0220, len(payload), payload)

    def zcl_zbd_set_anj_timeout(self):
        dst_mode_s = self.comboBox_anjTimeDstMode.currentText()
        dst_addr_s = self.lineEdit_anjTimeDstAddr.text()
        src_ep_s = self.lineEdit_anjTimeSrcEp.text()
        dst_ep_s = self.lineEdit_anjTimeDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        anj_timeout_s = self.lineEdit_anjTime.text()
        anj_timeout = line_edit_str2int(3, anj_timeout_s)
        payload += struct.pack("!%dB" % len(anj_timeout), *anj_timeout)
        self.send_hci_command(0x0221, len(payload), payload)

    def ota_init(self):
        self.pushButton_imageNotify.clicked.connect(self.ota_image_notify_send)

    def ota_image_notify_send(self):
        dst_mode_s = self.comboBox_otaDstMode.currentText()
        dst_addr_s = self.lineEdit_otaDstAddr.text()
        src_ep_s = self.lineEdit_otaSrcEp.text()
        dst_ep_s = self.lineEdit_otaDstEp.text()
        payload = self.zcl_cluster_addr_handle(dst_mode_s, dst_addr_s, src_ep_s, dst_ep_s)

        dst_mode_s = self.comboBox_payloadType.currentIndex()
        payload += struct.pack("!B", dst_mode_s)
        jitter_str = self.lineEdit_jitter.text()
        jitter = line_edit_str2int(1, jitter_str)
        payload += struct.pack("!%dB" % len(jitter), *jitter)
        self.send_hci_command(0x0190, len(payload), payload)

    def af_test_init(self):
        self.pushButton_txrxTest.clicked.connect(self.af_txrx_test)
        self.pushButton_afDataSend.clicked.connect(self.af_data_send)
        self.pushButton_nodesToggleTest.clicked.connect(self.af_nodes_toggle_test)

    def af_txrx_test(self):
        dst_addr_s = self.lineEdit_txrxTestDstAddr.text()
        dst_addr = line_edit_str2int(2, dst_addr_s)
        payload = struct.pack("!%dB" % len(dst_addr), *dst_addr)

        src_ep_s = self.lineEdit_txrxTestSrcEp.text()
        src_ep = line_edit_str2int(1, src_ep_s)
        payload += struct.pack("!%dB" % len(src_ep), *src_ep)

        dst_ep_s = self.lineEdit_txrxTestDstEp.text()
        dst_ep = line_edit_str2int(1, dst_ep_s)
        payload += struct.pack("!%dB" % len(dst_ep), *dst_ep)

        send_count_s = self.lineEdit_txrxTestSendCount.text()
        send_count = line_edit_str2int(2, send_count_s)
        payload += struct.pack("!%dB" % len(send_count), *send_count)

        send_interval_s = self.lineEdit_txrxTestInterval.text()
        send_interval = line_edit_str2int(1, send_interval_s)
        payload += struct.pack("!%dB" % len(send_interval), *send_interval)

        send_tx_power_s = self.lineEdit_txrxTestTxPower.text()
        send_tx_power = line_edit_str2int(1, send_tx_power_s)
        payload += struct.pack("!%dB" % len(send_tx_power), *send_tx_power)
        self.send_hci_command(0x0042, len(payload), payload)

    def af_data_send(self):
        dst_addr_s = self.lineEdit_afDataSendDstAddr.text()
        dst_addr = line_edit_str2int(2, dst_addr_s)
        payload = struct.pack("!%dB" % len(dst_addr), *dst_addr)

        src_ep_s = self.lineEdit_afDataSendSrcEp.text()
        src_ep = line_edit_str2int(1, src_ep_s)
        payload += struct.pack("!%dB" % len(src_ep), *src_ep)

        dst_ep_s = self.lineEdit_afDataSendDstEp.text()
        dst_ep = line_edit_str2int(1, dst_ep_s)
        payload += struct.pack("!%dB" % len(dst_ep), *dst_ep)

        cluster_id_s = self.lineEdit_afDataSendClusterId.text()
        cluster_id = line_edit_str2int(2, cluster_id_s)
        payload += struct.pack("!%dB" % len(cluster_id), *cluster_id)

        data_send_s = self.lineEdit_afDataSendData.text()
        if data_send_s:
            data_len = int((len(data_send_s) - 2) / 2)
            payload += struct.pack("!H", data_len)
            data_send = line_edit_str2int(data_len, data_send_s)
            payload += struct.pack("!%dB" % len(data_send), *data_send)
        else:
            data_len = 0
            payload += struct.pack("!H", data_len)
        self.send_hci_command(0x0044, len(payload), payload)

    def af_nodes_toggle_test(self):
        dst_mode = self.comboBox_toggleTestMode.currentIndex()
        payload = struct.pack("!B", dst_mode)

        interval_s = self.lineEdit_toggleInterval.text()
        interval = line_edit_str2int(1, interval_s)
        payload += struct.pack("!%dB" % len(interval), *interval)
        self.send_hci_command(0x0041, len(payload), payload)

    def hci_ota_init(self):
        self.pushButton_hciOtaFileChoose.clicked.connect(self.choose_hci_ota_file)
        self.pushButton_hciOtaStart.clicked.connect(self.start_hci_ota)

    def choose_hci_ota_file(self):
        get_path, file_type = QFileDialog.getOpenFileName(self, 'choose file', '', "BIN(*.bin);; OTA(*.ota);; Zigbee(*.zigbee)")
        self.lineEdit_hciOtaFilePath.setText(get_path)

    def start_hci_ota(self):
        self.CsvFiles.hci_ota_offset = 0
        self.CsvFiles.hci_ota_file_size = 0
        hci_ota_path = self.lineEdit_hciOtaFilePath.text()
        if hci_ota_path:
            self.CsvFiles.hci_ota_file_path = hci_ota_path
            self.CsvFiles.hci_ota_local = self.checkBox_hciOta.isChecked()
            self.CsvFiles.hci_ota_file_size = os.path.getsize(hci_ota_path)
            # print('hci_ota_local:%d' % self.CsvFiles.hci_ota_local)
            # print('ota_file_size:%d' % self.CsvFiles.hci_ota_file_size)
            payload = struct.pack("!LB", self.CsvFiles.hci_ota_file_size, self.CsvFiles.hci_ota_local)
            payload_len = len(payload)
            self.send_hci_command(0x0210, payload_len, payload)
            self.progressBar_hciOta.setValue(0)
        else:
            QMessageBox.warning(self, 'hci ota warning', "Please choose the OTA file!")

    def command_analyze_init(self):
        self.pushButton_analyzePathChoose.clicked.connect(self.choose_analyze_folder)
        self.pushButton_startAnalyze.clicked.connect(self.analyze_differ_nodes)
        self.pushButton_startFilter.clicked.connect(self.start_filter_nodes)

    def ble_hci_init(self):
        self.pushButton_advParaSet.clicked.connect(self.set_ble_adv_parameters)
        self.pushButton_advDataSet.clicked.connect(self.set_ble_adv_data)
        self.pushButton_advStart.clicked.connect(self.set_ble_adv_start)
        self.pushButton_advStop.clicked.connect(self.set_ble_adv_stop)
        self.pushButton_scanParaSet.clicked.connect(self.set_ble_scan_para)
        self.pushButton_bleScanstart.clicked.connect(self.start_ble_scan)
        self.pushButton_bleScanstop.clicked.connect(self.stop_ble_scan)
        self.pushButton_bleConnect.clicked.connect(self.ble_connect_to_dev)
        self.pushButton_bleDisconnect.clicked.connect(self.ble_disconnect_to_dev)
        self.pushButton_getConnList.clicked.connect(self.ble_get_connect_list)

    def set_ble_adv_parameters(self):    
        adv_set_para_start = [0x01, 0x06, 0x20, 0x0f]
        send_data = struct.pack("!%dB" % len(adv_set_para_start), *adv_set_para_start)
        
        advMinInterval_s = self.lineEdit_minAdvInterval.text()
        advMinInterval = line_edit_str2int(2, advMinInterval_s)
        advMinInterval.reverse()
        send_data += struct.pack("!%dB" % len(advMinInterval), *advMinInterval)

        advMaxInterval_s = self.lineEdit_maxAdvInterval.text()
        advMaxInterval = line_edit_str2int(2, advMaxInterval_s)
        advMaxInterval.reverse()
        send_data += struct.pack("!%dB" % len(advMaxInterval), *advMaxInterval)

        adv_para_other = [0x00, 0x00, 0x00, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x07, 0x00]
        send_data += struct.pack("!%dB" % len(adv_para_other), *adv_para_other)
        self.send_ble_standard_data(send_data)

    def set_ble_adv_data(self):
        adv_set_advdata_start = [0x01, 0x08, 0x20]
        send_data = struct.pack("!%dB" % len(adv_set_advdata_start), *adv_set_advdata_start)

        advData_s = self.lineEdit_advData.text()
        # print(advData_s)
        advdata_hex = simple_str2hex(advData_s)
        # print(advdata_hex)
        # print(len(advdata_hex))
        send_data += struct.pack("!2B%dB" % len(advdata_hex), len(advdata_hex)+1, len(advdata_hex), *advdata_hex)
        self.send_ble_standard_data(send_data)

    def set_ble_adv_start(self):
        adv_start = [0x01, 0x0a, 0x20, 0x01, 0x01]
        send_data = struct.pack("!%dB" % len(adv_start), *adv_start)
        self.send_ble_standard_data(send_data)

    def set_ble_adv_stop(self):
        adv_stop = [0x01, 0x0a, 0x20, 0x01, 0x00]
        send_data = struct.pack("!%dB" % len(adv_stop), *adv_stop)
        self.send_ble_standard_data(send_data)

    def set_ble_scan_para(self):
        scan_para_start = [0x01, 0x0b, 0x20, 0x07, 0x01]
        send_data = struct.pack("!%dB" % len(scan_para_start), *scan_para_start)
        
        scanInterval_s = self.lineEdit_scanInterval.text()
        scanInterval = line_edit_str2int(2, scanInterval_s)
        scanInterval.reverse()
        send_data += struct.pack("!%dB" % len(scanInterval), *scanInterval)

        scanWindow_s = self.lineEdit_scanWindow.text()
        scanWindow = line_edit_str2int(2, scanWindow_s)
        scanWindow.reverse()
        send_data += struct.pack("!%dB" % len(scanWindow), *scanWindow)

        scan_para_other = [0x00, 0x00]
        send_data += struct.pack("!%dB" % len(scan_para_other), *scan_para_other)
        self.send_ble_standard_data(send_data)

    def start_ble_scan(self):
        start_scan = [0x01, 0x0c, 0x20, 0x02, 0x01, 0x00]
        send_data = struct.pack("!%dB" % len(start_scan), *start_scan)

        self.send_ble_standard_data(send_data)
        self.listWidget_AdvList.clear()
        self.ble_choose_mac = []
        self.advList = {}

    def stop_ble_scan(self):
        stop_scan = [0x01, 0x0c, 0x20, 0x02, 0x00, 0x00]
        send_data = struct.pack("!%dB" % len(stop_scan), *stop_scan)

        self.send_ble_standard_data(send_data)
        # print('stop scan .......')
        self.listWidget_AdvList.clear()
        self.ble_choose_mac = []
        for advNode in self.advList:
            # print(advNode)
            # print(self.advList[advNode]['rssi'])
            # print(self.advList[advNode]['name'])
            data_show_s = 'mac:0x'
            for i in range(6):
                hhex = '%02x' % advNode[5-i]
                data_show_s += hhex
            data_show_s +="  rssi:" + '%d' % (self.advList[advNode]['rssi'] - 256)
            if self.advList[advNode]['name'] != 'no name':
                data_show_s +="  name:"
                for a in self.advList[advNode]['name']:
                    if a >= 32 and a<127:
                        data_show_s +=chr(a)
                self.listWidget_AdvList.insertItem(0, data_show_s)
            else:
                self.listWidget_AdvList.addItem(data_show_s)

    def ble_connect_to_dev(self):
        connect_data_start = [0x01, 0x0d, 0x20, 0x19, 0xa0, 0x00, 0xa0, 0x00, 0x00, 0x00]
        send_data = struct.pack("!%dB" % len(connect_data_start), *connect_data_start)

        ble_mac_addr_s = self.lineEdit_bleMac.text()
        # print('ble_mac_addr_s')
        # print(ble_mac_addr_s)
        if ble_mac_addr_s:
            ieee_addr = line_edit_str2int(6, ble_mac_addr_s)
            ieee_addr.reverse()
            send_data += struct.pack("!%dB" % len(ieee_addr), *ieee_addr)
        else:
            if self.ble_choose_mac:
                send_data += struct.pack("!%dB" % len(self.ble_choose_mac), *(self.ble_choose_mac))
            else:
                QMessageBox.warning(self, 'BLE connect', "Choose or fill in the mac of the device！")
                return

        self.ble_choose_mac = []
        self.listWidget_AdvList.clearSelection()
        connect_data_end = [0x00, 0x19, 0x00, 0x27, 0x00, 0x02, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00]
        send_data += struct.pack("!%dB" % len(connect_data_end), *connect_data_end)
        self.send_ble_standard_data(send_data)

    def ble_disconnect_to_dev(self):
        disconn_start = [0x01, 0x06, 0x04, 0x03]
        send_data = struct.pack("!%dB" % len(disconn_start), *disconn_start)

        ble_handle_s = self.lineEdit_bleConnHandle.text()
        if ble_handle_s != '':
            ble_handle = line_edit_str2int(2, ble_handle_s)
            if ble_handle[0] == 0xcc and ble_handle[1] == 0xcc:
                QMessageBox.warning(self, 'BLE disconnect', "Invalid ble handle！")
            else:
                ble_handle.reverse()
                ble_handle.append(0x13)
                # print('ble handle.....')
                # print(ble_handle)
                send_data += struct.pack("!%dB" % len(ble_handle), *ble_handle)
                self.send_ble_standard_data(send_data)
        else:
            if self.blehandle == 0x00:
                QMessageBox.warning(self, 'BLE disconnect', "No BLE connection....！")
            else:
                disconn_data = []
                disconn_data.append(self.blehandle & 0xff)
                disconn_data.append((self.blehandle >> 8)&0xff)
                disconn_data.append(0x13)
                send_data += struct.pack("!%dB" % len(disconn_data), *disconn_data)
                self.send_ble_standard_data(send_data)

    def ble_get_connect_list(self):
        listInfo_s = 'Not support on windows.\n'
        self.label_bleState.setStyleSheet("color:red")        
        self.label_bleState.setText(listInfo_s)


    def analyze_differ_nodes(self):
        self.pushButton_startAnalyze.setEnabled(False)
        self.textEdit_nwkAddrShow.clear()
        self.textEdit_nwkAddrShow.setText('Getting。。。')
        file_path = self.lineEdit_analyzePath.text()
        self.analyzenodeswork = AnalyzeWorkThread(file_path, self.CsvFiles)
        self.analyzenodeswork.nodes_signals.connect(self.get_nodes_result_show)  # 信号连接槽函数
        self.analyzenodeswork.start()  # 开启线程

    def get_nodes_result_show(self, read_file_flag, analyze_result_str):
        # get_nodes_info = ''
        # print('analyze_result_str:')
        # print(analyze_result_str)
        if read_file_flag == 1:
            if len(analyze_result_str) == 0:
                QMessageBox.warning(self, 'get joined nodes address', "No communication data！")
        elif read_file_flag == 4:
            QMessageBox.warning(self, 'get joined nodes address', "Dab file, please check！")
        elif read_file_flag == 3:
            QMessageBox.warning(self, 'get joined nodes address', "Do not open file when writing！")
        elif read_file_flag == 2:
            QMessageBox.warning(self, 'get joined nodes address', "Please choose the right folder！")
        else:
            pass
        self.textEdit_nwkAddrShow.clear()
        self.textEdit_nwkAddrShow.setText(analyze_result_str)
        self.pushButton_startAnalyze.setEnabled(True)

    def choose_analyze_folder(self):
        default_dir = os.path.expanduser(os.getcwd()) + '/userdata'
        get_path = QFileDialog.getExistingDirectory(self, 'choose folder', default_dir)
        self.lineEdit_analyzePath.setText(get_path)

    def start_filter_nodes(self):
        folder_path = self.lineEdit_analyzePath.text()
        analyze_addr = self.lineEdit_analyzeAddr.text()
        analyze_interval = self.lineEdit_analyzeInterval.text()
        analyze_command = self.lineEdit_analyzeCommandId.text()
        self.pushButton_startFilter.setEnabled(False)
        self.pushButton_startFilter.setText('Getting。。。')
        self.processwork = ProcessWorkThread(self.CsvFiles, folder_path, analyze_addr, analyze_interval,
                                             analyze_command)
        self.processwork.process_result.connect(self.process_result_show)  # 信号连接槽函数
        self.processwork.start()  # 开启线程

    def process_result_show(self, read_file_flag, command_cnt_show):
        # print(read_file_flag)
        self.pushButton_startFilter.setText('Start Filter')
        self.pushButton_startFilter.setEnabled(True)
        if read_file_flag == 1:
            QMessageBox.warning(self, 'Result', 'total records number:' + str(command_cnt_show))
        elif read_file_flag == 2:
            QMessageBox.warning(self, 'Result', "No data for this device!")
        elif read_file_flag == 3:
            QMessageBox.warning(self, 'Result', "Do not open the file!")
        elif read_file_flag == 4:
            QMessageBox.warning(self, 'Result', "Bad file, please check！")
        elif read_file_flag == 5:
            QMessageBox.warning(self, 'Result', "Wrong input message！")
        else:
            pass

    def send_ble_standard_data(self, send_data):
        send_data_str = ''
        dt = datetime.now()
        timestr = dt.strftime('%y-%m-%d %H:%M:%S.%f')
        for hvol in send_data:
            hhex = '%02x' % hvol
            send_data_str += hhex + ' '
        self.listWidget_commandData.addItem(timestr + ' send-->:' + send_data_str)

        try:
            self.serial.write(send_data)
        except serial.serialutil.PortNotOpenError:
            QMessageBox.warning(self, 'port warning', "Port not open！")
        except serial.SerialTimeoutException:
            QMessageBox.warning(self, 'port warning', "Send timeout！")
        except serial.SerialException:
            QMessageBox.warning(self, 'port warning', "Port error！")

    def send_hci_command(self, command_id, payload_len, payload):
        command_start = self.ai_setting.command_start
        command_crc = crc8_calculate(command_id, payload_len, payload)
        command_end = self.ai_setting.command_end
        if payload_len > 0:
            # payload_str = bytes(payload)
            send_data: bytes = struct.pack("!B2HB%dsB" % payload_len, command_start, command_id, payload_len,
                                           command_crc, payload, command_end)
        else:
            send_data: bytes = struct.pack("!B2H2B", command_start, command_id, payload_len, command_crc, command_end)
        self.send_serial_data(send_data)

    def send_serial_data(self, send_data):
        if not self.checkBox_thread.isChecked():
            if len(send_data) > self.ai_setting.command_length_max:
                QMessageBox.warning(self, 'port warning',
                                    "Out of the max length:(%d)" % self.ai_setting.command_length_max)
                return
        try:
            self.parsing_send_command(send_data)
            if self.bleHci_enable is True:
                hcicmd = 0x01   #hci command
                hciOgd = 0x03
                hciOfd = 0xfd
                payloadLen = len(send_data)
                packet_data: bytes = struct.pack("!4B", hcicmd, hciOgd, hciOfd, payloadLen)
                packet_data += send_data
                self.serial.write(packet_data)
            else:
                self.serial.write(send_data)

        except serial.serialutil.PortNotOpenError:
            QMessageBox.warning(self, 'port warning', "Port not open！")
        except serial.SerialTimeoutException:
            QMessageBox.warning(self, 'port warning', "Send timeout！")
        except serial.SerialException:
            QMessageBox.warning(self, 'port warning', "Port error！")

    def send_serial_data_hci(self, ogf, ocf, send_data):
        if not self.checkBox_thread.isChecked():
            if len(send_data) > self.ai_setting.command_length_max:
                QMessageBox.warning(self, 'port warning',
                                    "Out of the max length:(%d)" % self.ai_setting.command_length_max)
                return
        try:
            if self.bluezhcid > 0:
                plen = len(send_data)  # + 7
                self.tl_hcitool.tl_hci_cmd_send(self.bluezhcid, ogf, ocf, plen, send_data)
            else:
                self.serial.write(send_data)
            self.parsing_send_command(send_data)
        except serial.serialutil.PortNotOpenError:
            QMessageBox.warning(self, 'port warning', "Port not open！")
        except serial.SerialTimeoutException:
            QMessageBox.warning(self, 'port warning', "Send timeout！")
        except serial.SerialException:
            QMessageBox.warning(self, 'port warning', "Port error！")

    def parsing_send_command(self, send_data):
        dt = datetime.now()
        timestr = dt.strftime('%y-%m-%d %H:%M:%S.%f')
        if not self.checkBox_thread.isChecked():
            send_data_str = ''
            for hvol in send_data:
                hhex = '%02x' % hvol
                send_data_str += hhex + ' '
            self.listWidget_commandData.addItem(timestr + ' send-->:' + send_data_str)
            write_result = self.CsvFiles.command_parsing_record(self.ai_setting, dt, send_data, send_data_str)
            if write_result != 1 and write_result != 2:
                QMessageBox.warning(self, 'write file warning', "Do not open the file when writing！")
        else:
            # print(str(send_data))
            send_data_str = send_data.decode('utf-8', 'ignore')
            self.listWidget_commandData.addItem('【' + timestr + '】' + send_data_str)
            write_result = self.txtFiles.write_file(timestr, send_data_str, 'send-->')
            if write_result != 1:
                QMessageBox.warning(self, 'write file warning', "Do not open the file when writing！")


# 继承QThread，重写run方法
class AnalyzeWorkThread(QThread):
    nodes_signals = Signal(int, str)  # 定义信号对象,传递值为str类型，使用int，可以为int类型

    def __init__(self, folder_path, csv):  # 向线程中传递参数，以便在run方法中使用
        super(AnalyzeWorkThread, self).__init__()
        self.folder_path = folder_path
        self.csvFile = csv

    def run(self):  # 重写run方法
        # print(self.folder_path)
        read_file_flag, analyze_result_str = self.csvFile.pickup_diff_nodes_info(self.folder_path)
        self.nodes_signals.emit(read_file_flag, analyze_result_str)  # 发射信号，str类型数据，内容为需要传递的数据
        # self.write_signel.emit(read_file_flag)


# 继承QThread，重写run方法
class ProcessWorkThread(QThread):
    process_result = Signal(int, int)  # 定义信号对象,传递值为str类型，使用int，可以为int类型

    def __init__(self, csv, folder_path, analyze_addr, analyze_interval, analyze_command):  # 向线程中传递参数，以便在run方法中使用
        super(ProcessWorkThread, self).__init__()
        self.csvFile = csv
        self.folder_path = folder_path
        self.analyze_addr = analyze_addr
        self.analyze_interval = analyze_interval
        self.analyze_command = analyze_command

    def run(self):  # 重写run方法
        read_flag, command_cnt = self.csvFile.pick_out_command_by_addr_time(self.folder_path, self.analyze_addr,
                                                                            self.analyze_interval, self.analyze_command)
        self.process_result.emit(read_flag, command_cnt)  # 发射信号，str类型数据，内容为需要传递的数据
