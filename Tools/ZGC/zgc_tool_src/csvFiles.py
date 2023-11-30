import csv
import os
from datetime import datetime
from settings import Settings
import pandas as pd
from hcicommandparse import ParseRecvCommand, ParseSendCommand


def get_nodes_addr(process_folder, all_packets_filename):
    # print(process_folder)
    all_packets_file_path = process_folder + all_packets_filename
    open_flag = 1
    cell_notsame = []
    # print(type(cell_notsame))

    node_info_file_path = process_folder + '/nodes_info.csv'

    try:
        addr_info = {}
        with open(node_info_file_path) as f:
            next(f)
            nodes_info = csv.reader(f, delimiter=',')
            for row in nodes_info:
                addr_info[row[0]] = []  #the address info in row 0
                for cell_num in range(len(row)):
                    if cell_num != 2:   #the devtype in row2
                        addr_info[row[0]].append(row[cell_num])

        packets_info = pd.read_csv(all_packets_file_path)
        packets_info = packets_info.dropna(subset=['DevAddr'])
        packets_info_short = packets_info[packets_info['AddrMode'] == '0x02']  # 短地址
        packets_info_long = packets_info[packets_info['AddrMode'] == '0x03']  # 短地址
        nodes_addr_short = list(packets_info_short['DevAddr'].unique())
        nodes_addr_long = list(packets_info_long['DevAddr'].unique())

        for addr_cell in addr_info:
            if addr_cell == 'ieee_addr':
                continue
            cell = []
            check_num = 0
            if addr_cell in nodes_addr_long:
                cell.append(addr_cell)
                for nwk_addr in addr_info[addr_cell]:
                    if nwk_addr in nodes_addr_short:
                        check_num += 1
                        cell.append(nwk_addr)
                        nodes_addr_short.remove(nwk_addr)

            if check_num > 0 and len(cell) != 0:
                nodes_addr_long.remove(addr_cell)
                cell_notsame.append(cell)
        if len(nodes_addr_short) != 0:
            for nodes in nodes_addr_short:
                # print('short addr:::')
                # print(nodes)
                if nodes != '0xfffc' and nodes != '0xfffd' and nodes != '0xfffe' and nodes != '0xffff':
                    cell = [nodes]
                    cell_notsame.append(cell)
        if len(nodes_addr_long) != 0:
            for nodes in nodes_addr_long:
                # print('long addr:::')
                # print(nodes)
                cell = [nodes]
                cell_notsame.append(cell)

    except FileNotFoundError:
        open_flag = 2
    except PermissionError:
        open_flag = 3
    except KeyError:
        open_flag = 4

    # print(cell_notsame)
    # print(type(cell_notsame))
    return open_flag, cell_notsame


def choose_different_node(process_folder, nodes_addr, all_packets_filename):
    all_packets_file_path = process_folder + all_packets_filename
    # ai_setting = Settings()
    open_flag = 1

    file_list = os.listdir(process_folder)
    for fileName in file_list:
        if 'all_packets' not in fileName and 'nodes_info' not in fileName:
            try:
                file_path_remove = process_folder + '/' + fileName
                os.remove(file_path_remove)
            except PermissionError:
                open_flag = 3
                return open_flag

    packets_info = pd.read_csv(all_packets_file_path, encoding='utf-8')
    for address in nodes_addr:
        str_name = address[0]
        # print(address[0])
        for cell in address[1:]:
            str_name += '-' + cell
        write_node_file_path = process_folder + '/' + str_name + '.csv'

        # print(all_packets_file_path)
        try:
            # print('process....')
            node_info = packets_info[packets_info['DevAddr'].isin(address)]
            # print('write....')
            node_info.to_csv(write_node_file_path, encoding='utf-8', index=False)
            # print('write finish....')
        except FileNotFoundError:
            open_flag = 2
            break
        except PermissionError:
            open_flag = 3
            break
        except KeyError:
            open_flag = 4
            break
        except UnicodeDecodeError:
            open_flag = 4
            break

    return open_flag


def convert_datetime(time_data):
    try:
        return datetime.strptime(time_data, "%y-%m-%d %H:%M:%S:%f")
    except TypeError:
        pass


def delta_seconds(delta_time):
    return delta_time.total_seconds()


def re_calculate_delta(process_folder, nodes_addr):
    open_flag = 1
    for address in nodes_addr:
        str_name = address[0]
        for cell in address[1:]:
            str_name += '-' + cell

        cal_node_file_path = process_folder + '/' + str_name + '.csv'
        try:
            csv_reader = pd.read_csv(cal_node_file_path, encoding='utf-8')
        except FileNotFoundError:
            open_flag = 2
            break
        except PermissionError:
            open_flag = 3
            break
        # print(cal_node_file_path)
        csv_reader = csv_reader.dropna(subset=['Timestamp'])
        # print('dropna!!')
        cal_time = csv_reader['Timestamp'].agg(convert_datetime)

        # print('cal_time')
        if cal_time is not None:
            delta_time = cal_time.diff(periods=1)
            delta_time = delta_time.agg(delta_seconds)
            csv_reader['TimeDelta'] = delta_time
            csv_reader['TimeDelta'].fillna(value=0, inplace=True)  # 将NaN全设为0
            csv_reader.to_csv(cal_node_file_path, encoding='utf-8', index=False)
        else:
            open_flag = 4
    return open_flag


def calculate_loss_rate(process_folder, nodes_addr, all_packets_filename):
    open_flag = 1
    get_info = ''
    all_packets_file_path = process_folder + all_packets_filename
    all_static_file = process_folder + '/all_packets_statistics.csv'
    if os.path.exists(all_static_file):
        try:
            os.remove(all_static_file)
        except PermissionError:
            open_flag = 3
            return open_flag, get_info

    nodes_info = pd.read_csv(all_packets_file_path, encoding='utf-8')
    confirm_sta = nodes_info[nodes_info['CommandId'] == 'ZBHCI_CMD_DATA_CONFIRM']
    status_count = confirm_sta['Status'].value_counts()
    # print(status_count)
    # print('calculate_loss_rate')print
    # print(nodes_addr)
    diff_status = ['idx', 'node_addr', 'confirm_cnt', 'confirm_fail_cnt', 'loss_rate(%)']
    with open(all_static_file, 'a', encoding='utf-8', newline='')as cal_file:
        cal_write = csv.writer(cal_file)
        for cell in status_count.index:
            # cal_write.writerow([cell, str(status_count[cell])])
            diff_status.append(cell)
        cal_write.writerow(diff_status)

        line_idx = 0
        for address in nodes_addr:
            line_idx += 1
            str_name = address[0]
            for cell in address[1:]:
                str_name += '-' + cell
            cal_node_file_path = process_folder + '/' + str_name + '.csv'
            # print(cal_node_file_path)
            try:
                node_info = pd.read_csv(cal_node_file_path, encoding='utf-8')
                confirm_sta = node_info[node_info['CommandId'] == 'ZBHCI_CMD_DATA_CONFIRM']
                send_cnt = confirm_sta['Status'].count()
                send_success = confirm_sta[confirm_sta['Status'] == 'SUCCESS']['Status'].count()
                send_fail = send_cnt - send_success
                if send_cnt == 0:
                    # loss_rate = 0
                    loss_rate_str = '0'
                else:
                    loss_rate = float((send_cnt - send_success) * 100 / send_cnt)
                    loss_rate_str = '%.2f' % loss_rate

                send_status = [str(line_idx), str_name, send_cnt, send_fail, loss_rate_str]
                for cell in diff_status[5:]:
                    send_cca_fail = confirm_sta[confirm_sta['Status'] == cell]['Status'].count()
                    send_status.append(send_cca_fail)
                cal_write.writerow(send_status)
            except FileNotFoundError:
                open_flag = 2
                break
            except PermissionError:
                open_flag = 3
                break
            except KeyError:
                open_flag = 4
                break
    atatic_info = pd.read_csv(all_static_file, encoding='utf-8')
    total_status = [' ', 'total_cnt']
    fail_cnt = 0
    total_send = 0

    for cell in diff_status[2:]: #from confirm count
        if cell == 'confirm_cnt':
            total_send = atatic_info[cell].sum()
            total_status.append(total_send)
        elif cell == 'confirm_fail_cnt':
            fail_cnt = atatic_info[cell].sum()
            total_status.append(fail_cnt)
        elif cell == 'loss_rate(%)':
            if total_send == 0:
                loss_rate = float(0)
            else:
                loss_rate = float(fail_cnt * 100 / total_send)
            loss_rate = round(loss_rate, 2)
            total_status.append(loss_rate)
        else:
            total_status.append(atatic_info[cell].sum())

    with open(all_static_file, 'a', encoding='utf-8', newline='')as cal_file:
        cal_write = csv.writer(cal_file)
        cal_write.writerow(total_status)

    cal_info = pd.read_csv(all_static_file, encoding='utf-8')
    cell_len = []

    for cell in cal_info:
        if cell == 'node_addr':
            get_info += str(cell).center(33) + ' '
            cell_len.append(33)
        else:
            get_info += str(cell).center(len(str(cell))) + ' '
            cell_len.append(len(str(cell)))
    get_info += '\n'
    for item in cal_info.iterrows():    # read line by line
        idx = 0
        for cell in item[1]:
            get_info += str(cell).center(cell_len[idx]) + ' '
            idx += 1
        get_info += '\n'

    return open_flag, get_info


class CsvFiles:
    def __init__(self):
        self.create_folder_path = ''
        self.all_packets_path = ''
        self.all_packets_file_name = '/all_packets.csv'
        self.nodes_info_file_name = '/nodes_info.csv'
        self.before_time = 0
        self.now_time = 0
        self.ota_file_size = 0
        self.nodes_info = {}
        self.joined_nodes_info = {}
        # self.get_joined_info_change = False
        self.get_joined_info_finish = False
        self.nodes_calculate = []
        self.send_data = b''
        self.hci_ota_offset = 0
        self.hci_ota_file_size = 0
        self.hci_ota_file_path = ''
        self.hci_ota_local = False
        self.auto_bind_list = []

    def create_folder(self):
        nowtime_strfile = datetime.now().strftime('%y-%m-%d-%H-%M-%S')
        is_exit = os.path.exists('./userdata')
        if not is_exit:
            os.mkdir('./' + 'userdata')
        self.create_folder_path = './userdata' + '/' + nowtime_strfile
        os.mkdir(self.create_folder_path)  # 创建文件夹
        self.all_packets_path = self.create_folder_path + self.all_packets_file_name  # 创建一个all_packets的文件
        with open(self.all_packets_path, 'w', encoding='utf-8', newline='') as f:
            csv_writer = csv.writer(f)
            csv_writer.writerow(
                ['Timestamp', 'TimeDelta', 'AddrMode', 'DevAddr', 'PacketInformation', 'CommandId', 'Status',
                 'Description'])
        return self.create_folder_path

    def set_auto_bind_parameters(self, auto_bind_class, auto_bind_list):
        self.auto_bind_class = auto_bind_class
        self.auto_bind_list = auto_bind_list

    def command_parsing_record(self, ai_setting, timenow, data, datastr):
        write_result = 1
        if not os.path.exists(self.all_packets_path):  # 防止文件或文件夹被误删
            self.create_folder()
        try:
            with open(self.all_packets_path, 'a', encoding='utf-8', newline='') as f:
                csv_writer = csv.writer(f)
                self.now_time = timenow
                time_insert = timenow.strftime("%y-%m-%d %H:%M:%S:%f")
                if self.before_time == 0:
                    delta_time = 0
                else:
                    delta_time = (self.now_time - self.before_time).total_seconds()
                self.before_time = self.now_time

                direct = ''
                payload_len = data[ai_setting.packet_length_idx_low]
                commandid_high = data[ai_setting.packet_command_idx_high] << 8
                commandid_hex = commandid_high + data[ai_setting.packet_command_idx_low]

                if commandid_hex in ai_setting.hci_command_list:
                    if commandid_hex >= 0x8000:
                        self.send_data = b''
                        command_info = ParseRecvCommand(ai_setting, self.hci_ota_file_path, commandid_hex, payload_len,
                                                        data[ai_setting.packet_payload_start_idx:-1], self.nodes_info,
                                                        self.auto_bind_class, self.auto_bind_list)
                        self.send_data = command_info.send_data
                        # command_addr = command_info.recv_nwk_addr
                        command_addr = command_info.recv_nwk_addr
                        # addr_str = '0x%04x' % command_info.recv_nwk_addr
                        command_status = command_info.recv_status
                        # self.get_joined_info_change = command_info.get_joined_info_change
                        if command_info.nodes_info_change:
                            self.save_node_info(self.create_folder_path)

                        # print("recv data parse:" + str(hex(commandid_hex)))
                        if commandid_hex == 0x8040:  # ZBHCI_CMD_NODES_JOINED_GET_RSP
                            # print("command_info.get_joined_nodes:" + str(len(command_info.get_joined_nodes)))
                            for get_nodes in command_info.get_joined_nodes:
                                # print('get_nodes:')
                                # print(hex(get_nodes))
                                if get_nodes in self.nodes_info:
                                    try:
                                        # print('nwk_addr:')
                                        # print(self.nodes_info[get_nodes]['nwk_addr'])
                                        self.joined_nodes_info[get_nodes] = self.nodes_info[get_nodes]['nwk_addr']
                                    except KeyError:
                                        self.joined_nodes_info[get_nodes] = 0xfffe
                                else:
                                    self.joined_nodes_info[get_nodes] = 0xfffe
                            # print("self.joined_nodes_info[get_nodes]:" + str(len(self.joined_nodes_info)))
                            self.get_joined_info_finish = command_info.get_joined_info_finish
                        if commandid_hex == 0x8211 or commandid_hex == 0x8212:  # ZBHCI_CMD_OTA_BLOCK_REQUEST
                            self.hci_ota_offset = command_info.hci_ota_offset
                    else:
                        command_info = ParseSendCommand(ai_setting, commandid_hex, payload_len,
                                                        data[ai_setting.packet_payload_start_idx:-1])
                        command_addr = command_info.send_dst_addr

                        command_status = command_info.send_status
                    # print(self.create_folder_path)
                    commandid_str = ai_setting.get_command_id_str(commandid_hex)
                    addr_str = ''
                    addrmode = ''
                    if command_info.addr_mode == 3:
                        addr_str = "0x%016x" % command_addr
                        addrmode = "0x%02x" % command_info.addr_mode
                    elif command_info.addr_mode == 0 or command_info.addr_mode == 1 or command_info.addr_mode == 2:
                        addr_str = "0x%04x" % command_addr
                        addrmode = "0x%02x" % command_info.addr_mode
                    # print('ParseRecvCommand')
                    # print(commandid_str)
                    # print(time_insert)
                    # print(delta_time)
                    # print(datastr)
                    # print(command_status)
                    # print(commandid_str)
                    # print(command_info.description)
                    csv_writer.writerow([time_insert, delta_time, addrmode, addr_str, datastr, commandid_str,
                                         command_status, command_info.description])
            # if command_addr in self.nodes_info:

                    # print('write success!!!!')
        except FileNotFoundError:
            self.create_folder()
        except PermissionError:
            write_result = 3
        # print('write_result111:%d' % write_result)
        return write_result

    def save_node_info(self, process_folder):
        nodes_info_file_path = process_folder + '/' + 'nodes_info.csv'
        with open(nodes_info_file_path, 'w', encoding='utf-8', newline='') as write_file:
            writer = csv.writer(write_file)
            writer.writerow(['ieee_addr', 'nwk_addr', 'dev_type'])
            # print(self.nodes_info)
            for info in self.nodes_info:
                # print('hex(info):::')
                # print('0x%016x' % info)
                # load_list = [hex(info)]
                load_list = ['0x%016x' % info]
                if 'nwk_addr' in self.nodes_info[info]:
                    load_list.append('0x%04x' % (self.nodes_info[info]['nwk_addr']))
                else:
                    load_list.append('0xfffe')
                if 'dev_type' in self.nodes_info[info]:
                    load_list.append(self.nodes_info[info]['dev_type'])
                else:
                    load_list.append('reserve')
                if 'used_nwk_addr' in self.nodes_info[info] and len(self.nodes_info[info]['used_nwk_addr']):
                    for used_cell in self.nodes_info[info]['used_nwk_addr']:
                        load_list.append('0x%04x' % used_cell)
                writer.writerow(load_list)

    def pick_out_command_by_addr_time(self, process_folder, nwk_addr, delta_time, command_id):
        file_read_result = 1
        self.nodes_calculate = []
        if process_folder == '':
            process_folder = self.create_folder_path
        if process_folder == '':
            return 2, 0   #no folder path
        find_total_cnt = 0
        if nwk_addr == '0xffff':
            file_list = os.listdir(process_folder)
            file_cnt = 0
            for file_name in file_list:
                # print("ZCL_REPORT_MSG_RCV:")
                if '0x' in file_name:
                    file_cnt += 1
                    file_read_result, find_cnt = self.pick_out_node_command(process_folder, file_name, delta_time,
                                                                            command_id)
                    if file_read_result != 1:
                        return file_read_result, find_total_cnt
                    find_total_cnt += find_cnt
            if file_cnt == 0:
                file_read_result = 2
        else:
            file_list = os.listdir(process_folder)
            for file_name in file_list:
                if nwk_addr in file_name:
                    # file_name = nwk_addr + '.csv'
                    file_read_result, find_total_cnt = self.pick_out_node_command(process_folder, file_name, delta_time,
                                                                                  command_id)
                    break
            else:
                file_read_result = 2
        if file_read_result == 1:
            write_file_path = process_folder + '/all_packets_statistics.csv'
            try:
                with open(write_file_path, 'a', encoding='utf-8', newline='') as write_file:
                    writer = csv.writer(write_file)
                    writer.writerow(['filename', 'commandID', 'total cnt', 'delta(s)', 'pick cnt'])
                    for row in self.nodes_calculate:
                        writer.writerow(row)
            except PermissionError:
                file_read_result = 3
        return file_read_result, find_total_cnt

    def pick_out_node_command(self, process_folder, file_name, delta_time, command_id):
        file_read_result = 1
        find_cnt = 0
        process_cnt = 0
        ai_setting = Settings()

        process_file_path = process_folder + '/' + file_name
        write_file_path = process_file_path

        # compare = 0
        if len(delta_time):
            if '>' in delta_time:
                compare = 1
            elif '=' in delta_time:
                compare = 2
            elif '<' in delta_time:
                compare = 3
            else:
                file_read_result = 5
                return file_read_result, find_cnt

            try:
                delta = float(delta_time[1:])
            except ValueError:
                file_read_result = 5
                return file_read_result, find_cnt
        else:
            compare = 0xff
            delta_time = '>=0'

        write_rows = []
        process_rows = []
        if command_id == '' or command_id == 'ALL_COMMANDS':
            command_id = 'ALL_COMMANDS'
        try:
            with open(process_file_path, 'r', encoding='utf-8') as read_file:
                reader_rows = csv.reader(read_file)
                try:
                    for row in reader_rows:
                        try:
                            if command_id == 'ALL_COMMANDS':
                                process_cnt += 1
                                if compare == 1:
                                    if float(row[ai_setting.delta_column]) > float(delta):
                                        write_rows.append(row)
                                        find_cnt += 1
                                elif compare == 2:
                                    if float(row[ai_setting.delta_column]) == float(delta):
                                        write_rows.append(row)
                                        find_cnt += 1
                                elif compare == 3:
                                    if float(row[ai_setting.delta_column]) < float(delta):
                                        write_rows.append(row)
                                        find_cnt += 1
                                elif compare == 0xff:
                                    write_rows.append(row)
                                    find_cnt += 1
                            else:
                                if row[ai_setting.command_column] == command_id:
                                    if compare != 0xff:
                                        row[ai_setting.time_column] = datetime.strptime(row[ai_setting.time_column],
                                                                                        "%y-%m-%d %H:%M:%S:%f")
                                        process_cnt += 1
                                        process_rows.append(row)
                                    else:
                                        write_rows.append(row)
                                        find_cnt += 1
                        except ValueError:
                            process_cnt -= 1
                            continue
                        except IndexError:
                            if command_id == 'ALL_COMMANDS':
                                process_cnt -= 1
                            break
                except UnicodeDecodeError:
                    file_read_result = 4

                if process_rows:
                    # print('process_rows exit,len:%d' % len(process_rows))
                    last_time = datetime.now()
                    for cell_row in process_rows:
                        if last_time > cell_row[ai_setting.time_column]:
                            cell_row[ai_setting.delta_column] = 0
                            # pass
                        else:
                            cell_row[ai_setting.delta_column] = (
                                    cell_row[ai_setting.time_column] - last_time).total_seconds()
                        last_time = cell_row[ai_setting.time_column]
                        cell_row[ai_setting.time_column] = cell_row[ai_setting.time_column].strftime(
                            "%y-%m-%d %H:%M:%S:%f")

                        if compare == 1 and float(cell_row[ai_setting.delta_column]) > float(delta):
                            write_rows.append(cell_row)
                            find_cnt += 1
                        elif compare == 2 and float(cell_row[ai_setting.delta_column]) == float(delta):
                            write_rows.append(cell_row)
                            find_cnt += 1
                        elif compare == 3 and float(cell_row[ai_setting.delta_column]) < float(delta):
                            write_rows.append(cell_row)
                            find_cnt += 1
        except FileNotFoundError:
            file_read_result = 2
        except PermissionError:
            file_read_result = 3

        if file_read_result == 1:
            try:
                with open(write_file_path, 'a', encoding='utf-8', newline='') as write_file:
                    writer = csv.writer(write_file)
                    writer.writerow(
                        [file_name + ' info as follows:'])
                    writer.writerow(['\tTotal ' + command_id + ' packets count: ' + str(process_cnt)])
                    writer.writerow(['\tInterval:' + str(delta_time) + 's ' + command_id +
                                     ' packets count: ' + str(find_cnt)])
                    if find_cnt > 0:
                        writer.writerows(write_rows)
                    self.nodes_calculate.append([file_name, command_id, process_cnt, delta_time, find_cnt])
            except FileNotFoundError:
                file_read_result = 2
                # print('file_read_result2:%d' % file_read_result)
            except PermissionError:
                file_read_result = 3

        return file_read_result, find_cnt

    def pickup_diff_nodes_info(self, process_folder):
        # print('self.all_packets_file_name:{}'.format(self.all_packets_file_name))
        if process_folder == '':
            process_folder = self.create_folder_path
        cal_info = ''
        # print('self.create_folder_path:{}'.format(self.create_folder_path))
        file_read_result, nodes_addrs = get_nodes_addr(process_folder, self.all_packets_file_name)
        if len(nodes_addrs):
            # print(f'get_nodes_addr,nodes_addrs:{nodes_addrs}')
            file_read_result = choose_different_node(process_folder, nodes_addrs, self.all_packets_file_name)
            # print('choose_different_node file_read_result:{}'.format(file_read_result))
            if file_read_result == 1:
                file_read_result = re_calculate_delta(process_folder, nodes_addrs)
                # print('re_calculate_delta file_read_result:{}'.format(file_read_result))
                if file_read_result == 1:
                    file_read_result, cal_info = calculate_loss_rate(process_folder, nodes_addrs, self.all_packets_file_name)

        return file_read_result, cal_info
