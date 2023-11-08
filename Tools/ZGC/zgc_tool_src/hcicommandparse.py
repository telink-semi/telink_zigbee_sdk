import struct

# import os

send_list = {}
ota_file_addr = ''


def get_ieee_addr(payload):
    ieee_addr = '%02x' % payload[0] + '%02x' % payload[1] + '%02x' % payload[2] + '%02x' % payload[3] + \
                '%02x' % payload[4] + '%02x' % payload[5] + '%02x' % payload[6] + '%02x' % payload[7]
    return ieee_addr


def get_data_str(bytes_data):
    data_str = ''
    for n in range(8):
        byte_str, = struct.unpack("!B", bytes_data[n:n + 1])
        data_str += byte_str
    return data_str


def get_node_dev_type(dev_type_id):
    if dev_type_id == 0:
        node_type = 'coordinator'
    elif dev_type_id == 1:
        node_type = 'router'
    elif dev_type_id == 2:
        node_type = 'end device'
    else:
        node_type = ''
    return node_type


def get_relation_type(relationship_id):
    if relationship_id == 0:
        ship_type = 'Parent'
    elif relationship_id == 1:
        ship_type = 'Child'
    elif relationship_id == 2:
        ship_type = 'Sibling'
    elif relationship_id == 3:
        ship_type = 'None'
    elif relationship_id == 4:
        ship_type = 'Prevision child'
    else:
        ship_type = 'Invalid'
    return ship_type


def convert_str2int(input_s):
    # print(input_s)
    covert_result = True
    input_int = []
    convert_cnt = 0

    input_s = input_s.strip()  # 把前后的空格去掉
    while input_s != '':
        try:
            num = int(input_s[0:2], 16)
        except ValueError:
            covert_result = False
            break
        input_s = input_s[2:].strip()  # 把前后的空格去掉
        # print(input_s)
        input_int.append(num)
        convert_cnt += 1

    if not covert_result:
        return None
    else:
        return input_int


def parse_packet_detail_show(ai_setting, data_str):
    data_int = convert_str2int(data_str)
    # print('data_int:')
    # print(data_int)
    parse_items = []
    if data_int is None or len(data_int) < ai_setting.command_length_min:
        return parse_items

    payload_len = data_int[ai_setting.packet_length_idx_low]
    commandid_high = data_int[ai_setting.packet_command_idx_high] << 8
    commandid_hex = commandid_high + data_int[ai_setting.packet_command_idx_low]

    if commandid_hex >= 0x8000:
        # parse_items = ['not support!']
        nodes_info = {}
        auto_bind = SetAutoBindPara()
        list1 = []
        command_str = ai_setting.get_command_id_str(commandid_hex)
        parse_items.append('hci commandid: 0x%04x' % commandid_hex + ' (' + command_str + ')')
        parse_items.append('payload len: %d' % payload_len)
        # print(parse_items)
        ota_file_path = ''
        command_info = ParseRecvCommand(ai_setting, ota_file_path, commandid_hex, payload_len,
                                        data_int[ai_setting.packet_payload_start_idx:-1], nodes_info, auto_bind, list1)
        parse_items.extend(command_info.payload_items)
    else:
        command_info = ParseSendCommand(ai_setting, commandid_hex, payload_len,
                                        data_int[ai_setting.packet_payload_start_idx:-1])
        parse_items.extend(command_info.parse_items)
    return parse_items


class SetAutoBindPara:
    def __init__(self):
        self.auto_bind_enable = False
        self.bind_srcep = 0x01
        self.bind_clusterid = 0x0006
        self.bind_dstep = 0x01

    def auto_bind_set_para(self, auto_bind_enable, bind_srcep, bind_clusterid, bind_dstep):
        self.auto_bind_enable = auto_bind_enable
        self.bind_srcep = bind_srcep
        self.bind_clusterid = bind_clusterid
        self.bind_dstep = bind_dstep

    def auto_bind_req(self, ieee_addr, dst_addr_mode, dst_ieee_addr):
        return hci_bind_req_send(ieee_addr, self.bind_srcep, self.bind_clusterid, dst_addr_mode, dst_ieee_addr,
                                 self.bind_dstep)


class ParseRecvCommand:
    def __init__(self, ai_setting, hci_ota_file_path, command_id, payload_len, payload, nodes_info, auto_bind,
                 bind_list):
        # self.nwk_add = ''
        self.nodes_info_change = False
        self.payload_items = []
        self.recv_status = ''
        self.description = ''
        self.recv_nwk_addr = 0xfffe
        self.addr_mode = 0xff
        self.recv_cluster_id = ''
        self.send_data = b''
        self.get_joined_nodes = []
        self.auto_bind_list = bind_list
        # self.get_joined_info_change = False
        self.get_joined_info_finish = False
        self.hci_ota_offset = 0
        self.hci_ota_file_path = hci_ota_file_path
        self.parse_recv_command(ai_setting, command_id, payload_len, payload, nodes_info, auto_bind)

    def parse_recv_command(self, ai_setting, command_id, payload_len, payload, nodes_info, auto_bind):
        # print(' command_id:%s' % hex(command_id))
        # print(payload)
        try:
            if command_id == 0x8000:  # acknowledge
                self.hci_acknowledge_handle(ai_setting, payload)
            elif command_id == 0x8010 or command_id == 0x8011:  # nwk_addr_rep ieee_addr_rsp
                self.hci_addr_rsp_handle(ai_setting, payload_len, payload, nodes_info)
            elif command_id == 0x8012:  # node_desc_rsp
                self.hci_node_desc_rsp_handle(ai_setting, payload, nodes_info)
            elif command_id == 0x8013:  # simple desc rsp
                self.hci_simple_desc_rsp_handle(ai_setting, payload)
            elif command_id == 0x8014:  # ACTIVE_EP_RSP
                self.hci_active_endpoint_rsp_handle(ai_setting, payload)
            elif command_id == 0x8015:  # match desc rsp
                self.hci_match_desc_rsp_handle(ai_setting, payload)
            elif command_id == 0x8020 or command_id == 0x8021:  # bind response
                self.hci_bind_unbind_rsp_handle(ai_setting, payload)
            elif command_id == 0x8030:  # mgmt_lqi_rsp
                self.hci_lqi_rsp_handle(ai_setting, payload, nodes_info)
            elif command_id == 0x8031:  # mgmt_bind_rsp
                self.hci_mgmt_bind_rsp_handle(ai_setting, payload, nodes_info)
            elif command_id == 0x8032:  # leave resp
                self.hci_mgmt_leave_rsp_handle(ai_setting, payload, nodes_info)
            elif command_id == 0x8033:  # MGMT_DIRECT_JOIN_RSP
                pass
            elif command_id == 0x8034:  # MGMT_PERMIT_JOIN_RSP
                self.hci_mgmt_mgmt_permitjoin_rsp_handle(ai_setting, payload)
            elif command_id == 0x8035:  # MGMT_NWK_UPDATE_NOTIFY
                self.hci_mgmt_update_notify_handle(ai_setting, payload)
            elif command_id == 0x8040:  # NODES_JOINED_GET_RSP
                self.hci_nodes_joined_get_rsp_handle(ai_setting, payload, nodes_info)
            elif command_id == 0x8041:  # NODES_TOGGLE_TEST_RSP
                pass
            elif command_id == 0x8042:  # TXRX_PERFORMANCE_TEST_RSP
                self.hci_txrx_performance_test_rsp_handle(payload)
            elif command_id == 0x8043:  # dev annce
                self.hci_dev_annce_handle(payload, nodes_info, auto_bind)
            elif command_id == 0x8044:  # AF_DATA_SEND_TEST_RSP
                self.hci_af_data_send_test_rsp_handle(ai_setting, payload)
            elif command_id == 0x8045:  # GET_OWN_INFO_RSP
                self.hci_get_own_info_rsp_handle(payload, nodes_info, auto_bind)
            elif command_id == 0x8100:  # ZCL_ATTR_READ_RSP
                self.hci_zcl_attr_read_rsp_handle(ai_setting, payload)
            elif command_id == 0x8101:  # ZCL_ATTR_WRITE_RSP
                self.hci_zcl_attr_write_rsp_handle(ai_setting, payload, payload_len)
            elif command_id == 0x8102:  # ZCL_CONFIG_REPORT_RSP
                self.hci_zcl_config_report_rsp_handle(ai_setting, payload)
            elif command_id == 0x8103:  # ZCL_READ_REPORT_CFG_RSP
                self.hci_zcl_read_report_cfg_rsp_handle(ai_setting, payload)
            elif command_id == 0x8104:  # ZBHCI_CMD_ZCL_REPORT_MSG_RCV
                self.hci_zcl_report_msg_rcv_handle(ai_setting, payload)
            elif command_id == 0x8105:  # ZBHCI_CMD_ZCL_DEFAULT_RSP_RCV
                self.hci_zcl_default_response_rcv_handle(ai_setting, payload)
            elif command_id == 0x8120:  # ZCL_GROUP_ADD_RSP
                self.hci_zcl_group_add_rsp_handle(ai_setting, payload)
            elif command_id == 0x8121:  # ZCL_GROUP_VIEW_RSP
                self.hci_zcl_group_view_rsp_handle(ai_setting, payload)
            elif command_id == 0x8122:  # ZCL_GROUP_GET_MEMBERSHIP_RSP
                self.hci_zcl_group_get_membership_rsp_handle(payload)
            elif command_id == 0x8123:  # ZCL_GROUP_REMOVE_RSP
                self.hci_zcl_group_remove_rsp_handle(ai_setting, payload)
            elif command_id == 0x8131:  # ZCL_IDENTIFY_QUERY_RSP
                self.hci_zcl_identify_query_rsp_handle(payload)
            elif command_id == 0x8160:  # ZCL_SCENE_ADD_RSP
                self.hci_zcl_scene_add_rsp_handle(ai_setting, payload)
            elif command_id == 0x8161:  # ZCL_SCENE_VIEW_RSP
                self.hci_zcl_scene_view_rsp_handle(ai_setting, payload)
            elif command_id == 0x8162:  # SCENE_REMOVE_RSP
                self.hci_zcl_scene_remove_rsp_handle(ai_setting, payload)
            elif command_id == 0x8163:  # ZCL_SCENE_REMOVE_ALL_RSP
                self.hci_zcl_scene_all_remove_rsp_handle(ai_setting, payload)
            elif command_id == 0x8164:  # ZCL_SCENE_STORE_RSP
                self.hci_zcl_scene_store_rsp_handle(ai_setting, payload)
            elif command_id == 0x8166:  # ZCL_SCENE_GET_MEMBERSHIP_RSP
                self.hci_zcl_scene_get_membership_rsp_handle(ai_setting, payload)
            elif command_id == 0x8200:  # CMD_DATA_CONFIRM
                self.hci_data_confirm_handle(ai_setting, payload)
            elif command_id == 0x8201:  # MAC_ADDR_IND
                self.hci_mac_addr_ind_handle(payload)
            elif command_id == 0x8202:  # NODE_LEAVE_IND
                self.hci_node_leave_ind_handle(payload, nodes_info)
            elif command_id == 0x8210:  # HCI_OTA_START_RESPONSE
                self.hci_ota_start_response_handle(ai_setting, payload)
            elif command_id == 0x8211:  # ZBHCI_CMD_OTA_BLOCK_REQUEST
                self.hci_ota_block_request_handle(payload)
            elif command_id == 0x8212:  # ZBHCI_CMD_OTA_END_STATUS
                self.hci_ota_end_handle(ai_setting, payload)
            elif command_id == 0x8220:  # 'ZBHCI_CMD_ZCL_ZDD_SET_STATE_RESPONSE',
                self.hci_zcl_zbd_ifstate_response_handle(ai_setting, payload)
            else:
                self.description = 'command not support!'
                self.payload_items = ['command not support!']
                # print('command_id:%04x' % command_id)
                # print('这些命令暂时不支持！')
        except IndexError:
            self.description = 'parsing length error!'
            self.payload_items = ['parsing length error!']
            self.recv_status = 'parsing length error!'
            # print('parsing length error!')
        except struct.error:
            self.description = 'Parsing failed!'
            self.payload_items = ['Parsing failed!']
            self.recv_status = 'parsing failed!'
        # pass

    def hci_acknowledge_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        # print(bytes_data)
        # print(len(bytes_data))
        msgtype, status, seq_num = struct.unpack("!H2B", bytes_data)
        self.recv_status = ai_setting.get_hci_msg_status_str(status)
        self.description = '\tmsgtype:' + hex(msgtype) + ' status:' + hex(status) + '(' + self.recv_status + ')'
        msgtype_str = ai_setting.get_command_id_str(msgtype)
        self.payload_items.append('\tmsgtype:' + hex(msgtype) + '(' + msgtype_str + ')')
        self.payload_items.append('\tstatus:' + hex(status) + '(' + self.recv_status + ')')

    def hci_addr_rsp_handle(self, ai_setting, payload_len, payload, nodes_info):
        bytes_data = bytearray(payload)
        src_addr, seq_num, status, ieee_addr, nwk_adds_interest = struct.unpack("!H2BQH", bytes_data[:14])
        self.recv_status = ai_setting.get_zdp_msg_status_str(status)
        self.description = 'seq_num:' + hex(seq_num) + ' status:' + hex(status) + '(' + self.recv_status + ')' + \
                           ' ieee_addr: 0x%016x' % ieee_addr + ' nwk_adds_interest:0x%04x' % nwk_adds_interest
        self.payload_items = ['\tsrc_addr:0x%04x' % src_addr,
                              '\tseq_num:' + hex(seq_num),
                              '\tstatus:' + hex(status) + '(' + self.recv_status + ')',
                              '\tieee_addr: 0x%016x' % ieee_addr,
                              '\tnwk_adds_interest: 0x%04x' % nwk_adds_interest]

        ptr = 14
        if payload_len > ptr:
            associate_cnt, = struct.unpack("!B", bytes_data[ptr:ptr + 1])
            self.description += ' associate_cnt:' + hex(associate_cnt)
            self.payload_items.append('\tassociate_cnt:' + hex(associate_cnt))
            ptr += 1
            if associate_cnt > 0:
                start_idx, = struct.unpack("!B", bytes_data[ptr:ptr + 1])
                self.description += ' start_idx:' + hex(start_idx) + ' node_list:'
                self.payload_items.append('\tstart_idx:' + hex(start_idx))
                self.payload_items.append('\tnode_list:')
                ptr += 1
                for num in range(associate_cnt):
                    node_addr, = struct.unpack("!H", bytes_data[ptr:ptr + 2])
                    ptr += 2
                    self.description += hex(node_addr) + ','
                    self.payload_items.append('\t\t0x%04x' % node_addr)

        if self.recv_status == 'ZDO_SUCCESS':
            self.nodes_info_change = True
            if ieee_addr not in nodes_info:
                nodes_info[ieee_addr] = {}
                nodes_info[ieee_addr]['used_nwk_addr'] = []
            nodes_info[ieee_addr]['nwk_addr'] = nwk_adds_interest
        self.recv_nwk_addr = src_addr
        self.addr_mode = 2

    def hci_node_desc_rsp_handle(self, ai_setting, payload, nodes_info):
        bytes_data = bytearray(payload)
        src_addr, seq_num, status, nwk_adds_interest = struct.unpack("!H2BH", bytes_data[:6])

        self.recv_status = ai_setting.get_zdp_msg_status_str(status)
        self.description = 'seq_num:' + hex(seq_num) + ' status:' + hex(status) + '(' + self.recv_status + ')' + \
                           ' nwk_adds_interest:0x%04x' % nwk_adds_interest

        self.payload_items = ['\tsrc_addr:0x%04x' % src_addr,
                              '\tseq_num:' + hex(seq_num),
                              '\tstatus:' + hex(status) + '(' + self.recv_status + ')',
                              '\tnwk_adds_interest:0x%04x' % nwk_adds_interest]
        ptr = 6
        if self.recv_status == 'ZDO_SUCCESS':
            node_type_id, = struct.unpack("!B", bytes_data[ptr:ptr + 1])
            ptr += 1
            node_type = get_node_dev_type(node_type_id & 0x03)
            self.description += ' node_type:' + node_type
            self.payload_items.append('\tnode_type:' + node_type)
            frequency_band, mac_capa_flags, manufacturer_code, maximum_buffer_size, maximum_incoming_transfer_size, \
            server_mask, maximum_outgoing_transfer_size, descriptor_capa_field = struct.unpack("!2BHB3HB",
                                                                                               bytes_data[ptr:])
            self.payload_items.append('\tfrequency_band:' + hex(frequency_band >> 3))
            self.payload_items.append('\tmac_capa_flags:' + hex(mac_capa_flags))
            self.payload_items.append('\tmanufacturer_code:' + hex(manufacturer_code))
            self.payload_items.append('\tmaximum_buffer_size:' + hex(maximum_buffer_size))
            self.payload_items.append('\tmaximum_incoming_transfer_size:' + hex(maximum_incoming_transfer_size))
            self.payload_items.append('\tserver_mask:0x%04x' % server_mask)
            self.payload_items.append('\tmaximum_outgoing_transfer_size:' + hex(maximum_outgoing_transfer_size))
            self.payload_items.append('\tdescriptor_capa_field:' + hex(descriptor_capa_field))

            for node in nodes_info:
                if nodes_info[node]['nwk_addr'] == nwk_adds_interest:
                    self.nodes_info_change = True
                    nodes_info[node]['dev_type'] = node_type
                    break
        self.recv_nwk_addr = src_addr
        self.addr_mode = 2

    def hci_simple_desc_rsp_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        src_addr, seq_num, status, nwk_adds_interest = struct.unpack("!H2BH", bytes_data[:6])

        self.recv_status = ai_setting.get_zdp_msg_status_str(status)
        self.description = 'seq_num:' + hex(seq_num) + ' status:' + hex(status) + '(' + self.recv_status + ')' + \
                           ' nwk_adds_interest:0x%04x' % nwk_adds_interest

        self.payload_items = ['\tsrc_addr:0x%04x' % src_addr,
                              '\tseq_num:' + hex(seq_num),
                              '\tstatus:' + hex(status) + '(' + self.recv_status + ')',
                              '\tnwk_adds_interest:0x%04x' % nwk_adds_interest]
        ptr = 6
        if self.recv_status == 'ZDO_SUCCESS':
            desc_len, endpoint, profile_id, dev_id, dev_ver, inclu_cnt = struct.unpack("!2B2H2B",
                                                                                       bytes_data[ptr:ptr + 8])
            self.description += ' simple_desc_len:' + hex(desc_len) + ' endpoint:' + hex(endpoint) + ' profile_id:' + \
                                hex(profile_id) + ' dev_id:' + hex(dev_id) + ' dev_version:' + hex(dev_ver) + \
                                ' incluster_cnt:' + hex(inclu_cnt)
            self.payload_items.extend(['\tsimple_desc_len:' + hex(desc_len),
                                       '\tendpoint:' + hex(endpoint),
                                       '\tprofile_id:0x%04x' % profile_id,
                                       '\tdev_id: 0x%04x' % dev_id,
                                       '\tdev_version:' + hex(dev_ver),
                                       '\tincluster_cnt:' + hex(inclu_cnt)])

            ptr += 8
            if inclu_cnt > 0:
                incluster_id_list = '\tincluster_list:'
                for a in range(inclu_cnt):
                    incluster_id, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
                    incluster_id_list += '0x%04x' % incluster_id + ' '
                    ptr += 2
                self.payload_items.append(incluster_id_list)

            outcluster_cnt, = struct.unpack("!B", bytes_data[ptr:ptr + 1])
            self.description += ' outcluster_cnt:' + hex(outcluster_cnt)
            self.payload_items.append('\toutcluster_cnt:' + hex(outcluster_cnt))
            ptr += 1
            if outcluster_cnt > 0:
                outcluster_id_list = '\toutcluster_id_list:'
                for a in range(outcluster_cnt):
                    outcluster_id, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
                    outcluster_id_list += '0x%04x' % outcluster_id + ' '
                    ptr += 2
                self.payload_items.append(outcluster_id_list)
        self.recv_nwk_addr = src_addr
        self.addr_mode = 2

    def hci_active_endpoint_rsp_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        src_addr, seq_num, status, nwk_adds_interest = struct.unpack("!H2BH", bytes_data[:6])

        self.recv_status = ai_setting.get_zdp_msg_status_str(status)
        self.description = 'seq_num:' + hex(seq_num) + ' status:' + hex(status) + '(' + self.recv_status + ')' + \
                           ' nwk_adds_interest:0x%04x' % nwk_adds_interest
        self.payload_items = ['\tsrc_addr:0x%04x' % src_addr,
                              '\tseq_num:' + hex(seq_num),
                              '\tstatus:' + hex(status) + '(' + self.recv_status + ')',
                              '\tnwk_adds_interest:0x%04x' % nwk_adds_interest]
        ptr = 6
        if self.recv_status == 'ZDO_SUCCESS':
            endpoint_cnt, = struct.unpack("!B", bytes_data[ptr:ptr + 1])
            ptr += 1
            self.description += ' endpoint_cnt:' + hex(endpoint_cnt)
            self.payload_items.append('\tendpoint_cnt:' + hex(endpoint_cnt))
            if endpoint_cnt > 0:
                self.description += ' endpoint list:'
                endpoint_list = '\tendpoint list:'
                for a in range(endpoint_cnt):
                    endpoint, = struct.unpack("!B", bytes_data[ptr:ptr + 1])
                    self.description += '0x%02x,' % endpoint
                    endpoint_list += '0x%02x ' % endpoint
                    ptr += 1
                self.payload_items.append(endpoint_list)
        self.recv_nwk_addr = src_addr
        self.addr_mode = 2

    def hci_match_desc_rsp_handle(self, ai_setting, payload):
        return self.hci_active_endpoint_rsp_handle(ai_setting, payload)

    def hci_bind_unbind_rsp_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        src_addr, seq_num, status = struct.unpack("!H2B", bytes_data)
        self.recv_status = ai_setting.get_zdp_msg_status_str(status)
        self.description = 'seq_num:' + hex(seq_num) + ' status:' + hex(status) + '(' + self.recv_status + ')'
        self.payload_items = ['\tsrc_addr:0x%04x' % src_addr,
                              '\tseq_num:' + hex(seq_num),
                              '\tstatus:' + hex(status) + '(' + self.recv_status + ')']
        self.recv_nwk_addr = src_addr
        self.addr_mode = 2

    def hci_lqi_rsp_handle(self, ai_setting, payload, nodes_info):
        bytes_data = bytearray(payload)
        src_addr, seq_num, status, neighbor_entries, start_idx, neighbor_table_listcount = struct.unpack("!H5B",
                                                                                                         bytes_data[:7])
        self.recv_status = ai_setting.get_zdp_msg_status_str(status)
        self.description = 'seq_num:' + hex(seq_num) + ' status:' + hex(status) + '(' + self.recv_status + ')' + \
                           ' neighbor_table_entries:' + hex(neighbor_entries) + ' start_idx:' + \
                           hex(start_idx) + ' neighbor_table_listcount:' + hex(neighbor_table_listcount)

        self.payload_items = ['\tsrc_addr:0x%04x' % src_addr,
                              '\tseq_num:' + hex(seq_num),
                              '\tstatus:' + hex(status) + '(' + self.recv_status + ')',
                              '\tneighbor_table_entries:' + hex(neighbor_entries),
                              '\tstart_idx:' + hex(start_idx),
                              '\tneighbor_table_listcount:' + hex(neighbor_table_listcount)]
        ptr = 7
        for a in range(neighbor_table_listcount):
            ext_pan_id, ieee_addr, nwk_addr, bit_field, depth, lqi = struct.unpack("!2Q2H2B", bytes_data[ptr:ptr + 22])
            ptr += 22
            self.description += ' ext_pan_id: 0x%016x' % ext_pan_id + ' ieee_addr:0x%016x' % ieee_addr + \
                                ' nwk_addr: 0x%04x' % nwk_addr + ' bit_field:0x%04x' % bit_field + ' depth:' + \
                                hex(depth) + ' lqi:' + hex(lqi)
            dev_type = get_node_dev_type((bit_field >> 8) & 0x03)
            relationship = get_relation_type((bit_field >> 12) & 0x07)
            self.payload_items.extend(['    list element:%d' % a,
                                       '\text_pan_id:0x%016x' % ext_pan_id,
                                       '\tieee_addr:0x%016x' % ieee_addr,
                                       '\tnwk_addr: 0x%04x' % nwk_addr,
                                       '\tbit_field:0x%04x' % bit_field,
                                       '\t    dev_type:' + dev_type,
                                       '\t    relationship:' + relationship,
                                       '\tdepth:' + hex(depth),
                                       '\tlqi:0x%02x' % lqi])
            if ieee_addr not in nodes_info:
                nodes_info[ieee_addr] = {}
                nodes_info[ieee_addr]['used_nwk_addr'] = []
            nodes_info[ieee_addr]['nwk_addr'] = nwk_addr
            nodes_info[ieee_addr]['dev_type'] = dev_type
            self.description += ' dev_type:' + dev_type
            self.nodes_info_change = True

        if neighbor_entries > start_idx + neighbor_table_listcount:
            self.send_data = hci_mgmt_lqi_req_send(src_addr, start_idx + neighbor_table_listcount)

        self.recv_nwk_addr = src_addr
        self.addr_mode = 2

    def hci_mgmt_bind_rsp_handle(self, ai_setting, payload, nodes_info):
        bytes_data = bytearray(payload)
        src_addr, seq_num, status, bind_table_entries, start_idx, bind_table_listcount = struct.unpack("!H5B",
                                                                                                       bytes_data[:7])
        self.recv_status = ai_setting.get_zdp_msg_status_str(status)
        self.description = 'seq_num:' + hex(seq_num) + ' status:' + hex(status) + '(' + \
                           self.recv_status + ')' + ' bind_table_entries:' + hex(bind_table_entries) + ' start_idx:' + \
                           hex(start_idx) + ' bind_table_listcount:' + hex(bind_table_listcount)
        self.payload_items = ['\tsrc_addr:0x%04x' % src_addr,
                              '\tseq_num:' + hex(seq_num),
                              '\tstatus:' + hex(status) + '(' + self.recv_status + ')',
                              '\tbind_table_entries:' + hex(bind_table_entries),
                              '\tstart_idx:' + hex(start_idx),
                              '\tbind_table_listcount:' + hex(bind_table_listcount)]
        ptr = 7
        if bind_table_listcount > 0:
            for a in range(bind_table_listcount):
                src_ieee_addr, src_endpoint, cluster_id, dest_addr_mode = struct.unpack("!QBHB",
                                                                                        bytes_data[ptr:ptr + 12])
                ptr += 12
                self.description += ' ieee source address:' + hex(src_ieee_addr) + ' src_endpoint:' + hex(
                    src_endpoint) + ' cluster_id: 0x%04x' % cluster_id + ' dest_addr_mode:' + hex(dest_addr_mode)

                dst_mode = ai_setting.get_dst_addr_mode(dest_addr_mode)
                self.payload_items.extend(['    List Element:%d' % a,
                                           '\tieee source address: 0x%016x' % src_ieee_addr,
                                           '\tsrc_endpoint: 0x%02x' % src_endpoint,
                                           '\tcluster_id: 0x%04x' % cluster_id,
                                           '\tdest_addr_mode:' + hex(dest_addr_mode) + '(' + dst_mode + ')'])
                if dest_addr_mode == 3:
                    dst_addr, dest_endpoint = struct.unpack("!QB", bytes_data[ptr: ptr + 9])
                    ptr += 9
                    self.description += ' dst_addr: 0x%016x' % dst_addr + ' dest_endpoint: 0x%02x' % dest_endpoint
                    self.payload_items.extend(['\tdst_addr: 0x%016x' % dst_addr,
                                               '\tdest_endpoint: 0x%02x' % dest_endpoint])
                else:
                    dst_addr, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
                    ptr += 2
                    self.description += ' dst_addr: 0x%04x' % dst_addr
                    self.payload_items.extend(['\tdst_addr: 0x%04x' % dst_addr])

                if dest_addr_mode == 3:
                    try:
                        nwk_addr = nodes_info[src_ieee_addr]['nwk_addr']
                        bind_dev_info = {'info': {'ieee_addr': src_ieee_addr, 'src_ep': src_endpoint,
                                                  'src_cluster': cluster_id, 'dst_addr': dst_addr,
                                                  'dst_ep': dest_endpoint}, 'nwk_addr': nwk_addr,
                                         'retry_cnt': 0}
                        for bind_cell in self.auto_bind_list:
                            if bind_cell['info'] == bind_dev_info['info']:
                                # print('delete listcell: nwkaddr:')
                                # print(hex(bind_cell['nwk_addr']))
                                self.auto_bind_list.remove(bind_cell)
                                break
                    except KeyError:
                        pass

        if bind_table_entries > start_idx + bind_table_listcount:
            self.send_data = hci_mgmt_bind_req_send(src_addr, start_idx + bind_table_listcount)
        self.recv_nwk_addr = src_addr
        self.addr_mode = 2

    def hci_mgmt_mgmt_permitjoin_rsp_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        src_addr, seq_num, status = struct.unpack("!H2B", bytes_data)
        self.recv_status = ai_setting.get_zdp_msg_status_str(status)
        self.description = 'seq_num:' + hex(seq_num) + ' status:' + hex(status) + '(' + self.recv_status + ')'
        self.recv_nwk_addr = src_addr
        self.addr_mode = 2
        self.payload_items = ['\tsrc_addr:0x%04x' % src_addr,
                              '\tseq_num:' + hex(seq_num),
                              '\tstatus:' + hex(status) + '(' + self.recv_status + ')']

    def hci_mgmt_update_notify_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        src_addr, seq_num, status = struct.unpack("!H2B", bytes_data[:4])
        # print('src_addr:0x%04x' % src_addr)
        self.recv_status = ai_setting.get_zdp_msg_status_str(status)
        self.description = 'seq_num:' + hex(seq_num) + ' status:' + hex(status) + '(' + self.recv_status + ')'
        self.payload_items = ['\tsrc_addr:0x%04x' % src_addr,
                              '\tseq_num:' + hex(seq_num),
                              '\tstatus:' + hex(status) + '(' + self.recv_status + ')']

        # print('\tstatus:' + hex(status))
        if self.recv_status == 'ZDO_SUCCESS':
            scan_channel, total_tran, fail_tran, scan_num = struct.unpack("!L2HB", bytes_data[4: 13])
            self.description = 'scanChannel:' + hex(scan_channel) + ' totalTrans:' + hex(total_tran) + \
                               ' failTrans:' + hex(fail_tran) + ' scanNum:' + hex(scan_num)
            self.payload_items.extend(['\tscanChannel:0x%08x' % scan_channel,
                                       '\ttotalTrans:0x%04x' % total_tran,
                                       '\tfailTrans:0x%04x' % fail_tran,
                                       '\tscanNum:0x%04x' % scan_num])
            ptr = 13
            if scan_num > 0:
                self.description += ' ed scan list:'
                scan_list = '\ted scan list:'
                for a in range(scan_num):
                    scan_ret, = struct.unpack("!B", bytes_data[ptr:ptr + 1])
                    self.description += '0x%02x ' % scan_ret
                    if a % 8 == 0:
                        scan_list += '\n\t\t'
                    scan_list += '0x%02x ' % scan_ret

                    ptr += 1
                self.payload_items.append(scan_list)

    def hci_mgmt_leave_rsp_handle(self, ai_setting, payload, nodes_info):
        bytes_data = bytearray(payload)
        src_addr, seq_num, status, ieee_addr, rejoin = struct.unpack("!H2BQB", bytes_data)
        self.recv_status = ai_setting.get_zdp_msg_status_str(status)
        self.description = 'seq_num:' + hex(seq_num) + ' status:' + hex(status) + '(' + self.recv_status + ')' + \
                           ' ieee_addr:0x%016x' % ieee_addr + ' rejoin:' + hex(rejoin)
        self.payload_items = ['\tsrc_addr:0x%04x' % src_addr,
                              '\tseq_num:' + hex(seq_num),
                              '\tstatus:' + hex(status) + '(' + self.recv_status + ')',
                              '\tieee_addr:0x%016x' % ieee_addr,
                              '\trejoin:' + hex(rejoin)]
        if ieee_addr in nodes_info and not rejoin:
            self.nodes_info_change = True
            nodes_info[ieee_addr]['used_nwk_addr'].append(nodes_info[ieee_addr]['nwk_addr'])
            nodes_info[ieee_addr]['nwk_addr'] = 0xfffe
            # del nodes_info[ieee_addr]
        self.recv_nwk_addr = src_addr
        self.addr_mode = 2

    def hci_nodes_joined_get_rsp_handle(self, ai_setting, payload, nodes_info):
        # nodes_info_change = False
        bytes_data = bytearray(payload)
        total_cnt, start_idx, list_count, status = struct.unpack("!2H2B", bytes_data[:6])
        self.recv_status = ai_setting.get_hci_msg_status_str(status)
        self.description = 'total_cnt:' + hex(total_cnt) + ' start_idx:' + hex(start_idx) + ' list_count:' + hex(
            list_count) + ' status:' + hex(status) + '(' + self.recv_status + ')'
        self.payload_items = ['\ttotal_cnt:' + hex(total_cnt),
                              '\tstart_idx:' + hex(start_idx),
                              '\tlist_count:' + hex(list_count),
                              '\tstatus:' + hex(start_idx) + '(' + self.recv_status + ')']
        ptr = 6
        # if start_idx == 0:
        #     self.get_joined_info_change = True
        # else:
        #     self.get_joined_info_change = False
        if self.recv_status == 'ZBHCI_MSG_STATUS_SUCCESS':
            for a in range(list_count):
                ext_addr, short_addr = struct.unpack("!QH", bytes_data[ptr:ptr + 10])
                ptr += 10
                self.description += ' ieee_addr:0x%016x' % ext_addr + ' short_addr:0x%04x' % short_addr
                self.payload_items.append('\tieee_addr:0x%016x' % ext_addr + ' short_addr:0x%04x' % short_addr)
                self.nodes_info_change = True
                self.get_joined_nodes.append(ext_addr)
                if ext_addr not in nodes_info:
                    nodes_info[ext_addr] = {}
                    nodes_info[ext_addr]['used_nwk_addr'] = []
                nodes_info[ext_addr]['nwk_addr'] = short_addr
            # print('total_cnt:%d' % total_cnt)
            # print('start_idx:%d' % start_idx)
            # print('list_count:%d' % list_count)
            # print('start_idx+ list_count:%d' % (start_idx + list_count))
            if total_cnt > start_idx + list_count:
                # print('total_cnt < start_idx + list_count')
                start_idx_next = start_idx + list_count
                self.send_data = hci_get_joined_nodes_req_send(start_idx_next)
            elif total_cnt == start_idx + list_count:
                self.get_joined_info_finish = True
        # return description, recv_status, nodes_info_change, payload_items

    def hci_txrx_performance_test_rsp_handle(self, payload):
        bytes_data = bytearray(payload)
        dst_addr, send_cnt, ack_cnt = struct.unpack("!3H", bytes_data)
        loss_rate = float((send_cnt - ack_cnt) * 100 / send_cnt)
        loss_rate_str = '%.2f' % loss_rate
        self.description = 'dst_addr:' + hex(dst_addr) + ' send_cnt:' + hex(send_cnt) + ' ack_cnt:' + hex(ack_cnt)
        self.payload_items = ['\tdst_addr:' + hex(dst_addr),
                              '\tsend_cnt:' + hex(send_cnt),
                              '\tack_cnt:' + hex(ack_cnt),
                              '\tloss_rate' + loss_rate_str]
        # return description, payload_items

    def hci_dev_annce_handle(self, payload, nodes_info, auto_bind):
        bytes_data = bytearray(payload)
        nwk_addr, ieee_addr, node_cap = struct.unpack("!HQB", bytes_data)

        node_dev_type = node_cap & 0x02
        # print('node_dev_type:%d' % node_dev_type)
        if node_dev_type == 2:
            node_type = 'Router'
        else:
            node_type = 'End device'

        self.nodes_info_change = True
        if ieee_addr not in nodes_info:
            nodes_info[ieee_addr] = {}
            nodes_info[ieee_addr]['used_nwk_addr'] = []
        else:
            if nodes_info[ieee_addr]['nwk_addr'] != 0xfffe and nodes_info[ieee_addr]['nwk_addr'] != nwk_addr:
                nodes_info[ieee_addr]['used_nwk_addr'].append(nodes_info[ieee_addr]['nwk_addr'])
        nodes_info[ieee_addr]['nwk_addr'] = nwk_addr
        nodes_info[ieee_addr]['dev_type'] = node_type

        self.description = 'nwk_addr: 0x%04x' % nwk_addr + ' ieee_addr:0x%016x' % ieee_addr + \
                           ' capability:' + hex(node_cap) + ' dev_type:' + node_type
        self.payload_items = ['\tnwk_addr: 0x%04x' % nwk_addr,
                              '\tieee_addr:0x%016x' % ieee_addr,
                              '\tcapability:' + hex(node_cap),
                              '\tdev_type:' + node_type]
        self.recv_nwk_addr = nwk_addr
        self.addr_mode = 2

        find_gw = False
        if auto_bind.auto_bind_enable:
            for node in nodes_info:
                # print(hex(nodes_info[node]['nwk_addr']))
                if nodes_info[node]['nwk_addr'] == 0x0:
                    find_gw = True
                    dst_ieee_addr = node
                    # print('first send bind req:')
                    self.send_data = auto_bind.auto_bind_req(ieee_addr, 0x03, dst_ieee_addr)
                    bind_dev_info = {'info': {'ieee_addr': ieee_addr, 'src_ep': auto_bind.bind_srcep,
                                              'src_cluster': auto_bind.bind_clusterid, 'dst_addr': dst_ieee_addr,
                                              'dst_ep': auto_bind.bind_dstep}, 'nwk_addr': nwk_addr, 'retry_cnt': 0,
                                     'type': 'bind_req'}
                    for bind_cell in self.auto_bind_list:
                        if bind_cell['info'] == bind_dev_info['info']:
                            # bind_cell['retry_cnt'] += 1
                            break
                    else:
                        self.auto_bind_list.append(bind_dev_info)
                    pass
            if not find_gw:
                # print('get_own_info_req')
                self.send_data = hci_get_own_info_req_send()
                send_list[0x0020] = [ieee_addr, nwk_addr]

    def hci_af_data_send_test_rsp_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        src_addr, src_endpoint, dst_endpoint, cluster_id, payload_len = struct.unpack("!HBBHH", bytes_data)
        cluster_str = ai_setting.get_zcl_cluster_str(cluster_id)
        self.description = 'src_addr:0x%04x' % src_addr + ' src_endpoint: 0x%02x' % src_endpoint + ' dst_endpoint:' + \
                           hex(dst_endpoint) + ' cluster_id: 0x%04x' % cluster_id + '(' + cluster_str + ')' + \
                           ' payload_len:' + hex(payload_len)

        self.payload_items = ['\tsrc_addr:0x%04x' % src_addr,
                              '\tsrc_endpoint: 0x%02x' % src_endpoint,
                              '\tdst_endpoint: 0x%02x' % dst_endpoint,
                              '\tcluster_id: 0x%04x' % cluster_id + '(' + cluster_str + ')',
                              '\tpayload_len:' + hex(payload_len)]
        self.recv_nwk_addr = src_addr
        self.recv_cluster_id = cluster_str

    def hci_get_own_info_rsp_handle(self, payload, nodes_info, auto_bind):
        bytes_data = bytearray(payload)
        dev_type, capability, onnetwork, panid, ext_panid, nwk_addr, ieee_addr = struct.unpack("!3BHQHQ", bytes_data)
        dev_type_str = get_node_dev_type(dev_type)
        self.description = 'dev_type:' + dev_type_str + ' capability' + hex(capability) + \
                           ' node_onnetwork:%x' % onnetwork + ' panid:' + hex(panid) + ' ext_panid:' + hex(ext_panid) \
                           + ' network_addr:' + hex(nwk_addr) + ' ieee_addr:0x%016x' % ieee_addr

        self.payload_items = ['\tdev_type:' + dev_type_str,
                              '\tcapability:' + hex(capability),
                              '\tnode_onnetwork:' + hex(onnetwork),
                              '\tpanid: 0x%04x' % panid,
                              '\text_panid: 0x%016x' % ext_panid,
                              '\tnetwork_addr: 0x%04x' % nwk_addr,
                              '\tieee_addr:0x%016x' % ieee_addr]

        # print('onnetwork' + str(onnetwork))
        if onnetwork:
            self.nodes_info_change = True
            if ieee_addr not in nodes_info:
                nodes_info[ieee_addr] = {}
                nodes_info[ieee_addr]['used_nwk_addr'] = []
            nodes_info[ieee_addr]['nwk_addr'] = nwk_addr
            nodes_info[ieee_addr]['dev_type'] = dev_type_str
            for data in send_list:
                if data == 0x0020:  # bind req
                    # print('0x0020 auto_bind_enable' + str(auto_bind.auto_bind_enable))
                    if auto_bind.auto_bind_enable:
                        # print('first send bind req:')
                        self.send_data = auto_bind.auto_bind_req(send_list[data][0], 0x03, ieee_addr)
                        bind_dev_info = {'info': {'ieee_addr': send_list[data][0], 'src_ep': auto_bind.bind_srcep,
                                                  'src_cluster': auto_bind.bind_clusterid, 'dst_addr': ieee_addr,
                                                  'dst_ep': auto_bind.bind_dstep}, 'nwk_addr': send_list[data][1],
                                         'retry_cnt': 0, 'type': 'bind_req'}
                        for bind_cell in self.auto_bind_list:
                            if bind_cell['info'] == bind_dev_info['info']:
                                # bind_cell['retry_cnt'] = 0
                                break
                        else:
                            self.auto_bind_list.append(bind_dev_info)
                    del send_list[data]
                    break
        # return description, nodes_info_change, send_data, payload_items

    def parse_zcl_cmd_header(self, ai_setting, bytes_data):
        src_addr, src_endpoint, dst_endpoint, cluster_id, seq_num = struct.unpack("!H2BHB", bytes_data[1:8])
        ptr = 7

        cluster_str = ai_setting.get_zcl_cluster_str(cluster_id)
        self.description = 'src_endpoint: 0x%02x' % src_endpoint + ' dst_endpoint:' + hex(
            dst_endpoint) + ' cluster_id:' + hex(cluster_id) + '(' + cluster_str + ')' + ' seq_num:' + hex(seq_num)
        self.payload_items = ['\tsrc_addr:0x%04x' % src_addr,
                              '\tsrc_endpoint: 0x%02x' % src_endpoint,
                              '\tdst_endpoint: 0x%02x' % dst_endpoint,
                              '\tcluster_id: 0x%04x' % cluster_id + '(' + cluster_str + ')',
                              '\tseq_num:' + hex(seq_num)]
        self.recv_cluster_id = cluster_str
        self.recv_nwk_addr = src_addr
        self.addr_mode = 2
        return ptr

    def parse_zcl_general_rsp_cmd_header(self, ai_setting, bytes_data):
        src_addr, src_endpoint, dst_endpoint, seq_num, cluster_id = struct.unpack("!H3BH", bytes_data[:7])
        ptr = 7
        cluster_str = ai_setting.get_zcl_cluster_str(cluster_id)
        self.description = 'src_endpoint: 0x%02x' % src_endpoint + 'dst_endpoint: 0x%02x' % dst_endpoint + ' seq_num:' + \
                           hex(seq_num) + ' cluster_id: 0x%04x' % cluster_id + '(' + cluster_str + ')'

        self.payload_items = ['\tsrc_addr:0x%04x' % src_addr,
                              '\tsrc_endpoint: 0x%02x' % src_endpoint,
                              '\tdst_endpoint: 0x%02x' % dst_endpoint,
                              '\tseq_num:' + hex(seq_num),
                              '\tcluster_id: 0x%04x' % cluster_id + '(' + cluster_str + ')']
        self.recv_cluster_id = cluster_str
        self.recv_nwk_addr = src_addr
        self.addr_mode = 2
        return ptr

    def parse_zcl_rsp_cmd_header(self, bytes_data):
        src_addr, src_endpoint, dst_endpoint, seq_num = struct.unpack("!H3B", bytes_data[:5])
        ptr = 5

        self.description = 'src_endpoint: 0x%02x' % src_endpoint + 'dst_endpoint: 0x%02x' % dst_endpoint + ' seq_num:' \
                           + hex(seq_num)
        self.payload_items = ['\tsrc_addr:0x%04x' % src_addr,
                              '\tsrc_endpoint: 0x%02x' % src_endpoint,
                              '\tdst_endpoint: 0x%02x' % dst_endpoint,
                              '\tseq_num:' + hex(seq_num)]
        self.recv_nwk_addr = src_addr
        self.addr_mode = 2
        return ptr

    def hci_zcl_attr_read_rsp_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.parse_zcl_general_rsp_cmd_header(ai_setting, bytes_data)

        num_attr, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
        ptr += 1
        if num_attr > 0:
            for a in range(num_attr):
                attr_id, status = struct.unpack("!HB", bytes_data[ptr:ptr + 3])
                status_str = ai_setting.get_zcl_msg_status_str(status)
                self.description += ' attr_id: 0x%04x' % attr_id + ' status:' + hex(status) + '(' + status_str + ')'
                self.payload_items.extend(['\tattr_id: 0x%04x' % attr_id,
                                           '\t    status:' + hex(status) + '(' + status_str + ')'])
                ptr += 3
                if status_str == 'ZCL_STA_SUCCESS':
                    data_type, = struct.unpack("!B", bytes_data[ptr:ptr + 1])
                    ptr += 1
                    self.description += ' data_type:%02x' % data_type
                    data_len = ai_setting.get_attr_size(data_type, payload[ptr:])
                    self.description += ' data_len:%02x' % data_len

                    data_int = struct.unpack("!%dB" % data_len, bytes_data[ptr:ptr + data_len])
                    data_s = '0x'
                    for data in data_int:
                        data_s += '%02x' % data
                    ptr += data_len
                    data_type_str = ai_setting.get_zcl_data_type(data_type)
                    self.payload_items.extend(['\t    data_type: ' + hex(data_type) + '(' + data_type_str + ')',
                                               '\t    data_len:' + hex(data_len),
                                               '\t    data:' + data_s])
                self.recv_status += status_str

    def hci_zcl_attr_write_rsp_handle(self, ai_setting, payload, payload_len):
        bytes_data = bytearray(payload)
        ptr = self.parse_zcl_general_rsp_cmd_header(ai_setting, bytes_data)
        while payload_len > ptr:
            # print('payload_len:%d,ptr:%d' % (payload_len, ptr))
            status, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
            ptr += 1
            self.recv_status = ai_setting.get_zcl_msg_status_str(status)
            self.description += ' status:' + hex(status) + '(' + self.recv_status + ')'
            if payload_len == ptr:
                break
            attribute_id, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
            self.description += ' attribute_id: 0x%04x' % attribute_id
            ptr += 2
            self.payload_items.extend(['\tstatus:' + hex(status) + '(' + self.recv_status + ')',
                                       '\tattribute_id: 0x%04x' % attribute_id])

    def hci_zcl_config_report_rsp_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.parse_zcl_general_rsp_cmd_header(ai_setting, bytes_data)

        num_attr, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
        ptr += 1
        self.description += ' num_attr:' + hex(num_attr)
        self.payload_items.append('\tnum_attr:' + hex(num_attr))
        for n in range(num_attr):
            status, direction, attribute_id = struct.unpack("!BBH", bytes_data[ptr: ptr + 4])
            item_recv_status = ai_setting.get_zcl_msg_status_str(status)
            self.recv_status += item_recv_status
            self.description += ' status:' + hex(status) + '(' + item_recv_status + ')' + ' direction:%02x' % direction \
                                + ' attribute_id: 0x%04x' % attribute_id
            ptr += 4
            if direction == 0:
                dir_str = 'reported'
            else:
                dir_str = 'received'
            self.payload_items.extend(['\tstatus:' + hex(status) + '(' + item_recv_status + ')',
                                       '\tdirection:' + hex(direction) + '(' + dir_str + ')',
                                       '\tattribute_id: 0x%04x' % attribute_id])

    def hci_zcl_read_report_cfg_rsp_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.parse_zcl_general_rsp_cmd_header(ai_setting, bytes_data)
        num_attr, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
        self.description += ' num_attr:' + hex(num_attr)
        ptr += 1
        self.payload_items.append('\tnum_attr:' + hex(num_attr))
        for a in range(num_attr):
            status, direction, attribute_id = struct.unpack("!BBH", bytes_data[ptr:ptr + 4])
            item_recv_status = ai_setting.get_zcl_msg_status_str(status)
            self.recv_status += item_recv_status
            self.description += ' status:' + hex(status) + '(' + item_recv_status + ')' + ' direction:' + hex(
                direction) + ' attribute_id: 0x%04x' % attribute_id

            if direction == 0:
                dir_str = 'reported'
            else:
                dir_str = 'received'
            self.payload_items.extend(['\tstatus:' + hex(status) + '(' + item_recv_status + ')',
                                       '\tdirection:' + hex(direction) + '(' + dir_str + ')',
                                       '\tattribute_id: 0x%04x' % attribute_id])
            ptr += 4
            if direction == 0:
                data_type, min_report_interval, max_report_interval = struct.unpack("!BHH", bytes_data[ptr:ptr + 5])
                ptr += 5
                data_type_str = ai_setting.get_zcl_data_type(data_type)
                self.description += ' data_type:' + hex(data_type) + ' min_report_interval:' + hex(
                    min_report_interval) + ' max_report_interval:' + hex(max_report_interval)
                self.payload_items.extend(['\tdata_type:' + hex(data_type) + ' (' + data_type_str + ')',
                                           '\tmin_report_interval:' + hex(min_report_interval),
                                           '\tmax_report_interval:' + hex(max_report_interval)])
            else:
                timeout, = struct.unpack("!H", bytes_data[ptr:ptr + 2])
                self.description += ' timeout:' + hex(timeout)
                ptr += 2
                self.payload_items.extend(['\ttimeout:' + hex(timeout)])

    def hci_zcl_report_msg_rcv_handle(self, ai_setting, payload):
        # print('hci_zcl_report_msg_rcv_handle')
        bytes_data = bytearray(payload)
        ptr = self.parse_zcl_general_rsp_cmd_header(ai_setting, bytes_data)
        # print('parse_zcl_general_rsp_cmd_header later')
        num_attr, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
        self.description += ' num_attr:%02x' % num_attr
        ptr += 1
        self.payload_items.append('\tnum_attr:' + hex(num_attr))
        for a in range(num_attr):
            attribute_id, data_type = struct.unpack("!HB", bytes_data[ptr: ptr + 3])
            ptr += 3
            data_len = ai_setting.get_attr_size(data_type, payload[ptr:])
            data_type_str = ai_setting.get_zcl_data_type(data_type)

            self.description += ' attribute_id: 0x%04x' % attribute_id + ' data_type:' + hex(data_type) + \
                                ' data_len:%02x' % data_len + ' data:'
            self.payload_items.extend(['\tattribute_id: 0x%04x' % attribute_id,
                                       '\tdata_type:' + hex(data_type) + '(' + data_type_str + ')',
                                       '\tdata_len:' + hex(data_len),
                                       '\tdata: '])
            if data_len > 0:
                data_str = '\t0x'
                for n in range(data_len):
                    data, = struct.unpack("!B", bytes_data[ptr:ptr + 1])
                    ptr += 1
                    self.description += hex(data) + ' '
                    data_str += '%02x' % data
                self.payload_items.append(data_str)
        # print('payload_items')
        # print(self.payload_items)

    def hci_zcl_default_response_rcv_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.parse_zcl_general_rsp_cmd_header(ai_setting, bytes_data)
        command_id, status = struct.unpack("!2B", bytes_data[ptr: ptr + 2])
        ptr += 2
        self.recv_status = ai_setting.get_zcl_msg_status_str(status)
        self.description += ' command_id:' + hex(command_id) + ' status:' + hex(status) + '(' + self.recv_status + ')'
        self.payload_items.extend(['\tcommand_id:' + hex(command_id),
                                   '\tstatus:' + hex(status) + '(' + self.recv_status + ')'])

    def hci_zcl_group_add_rsp_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.parse_zcl_rsp_cmd_header(bytes_data)

        status, group_id = struct.unpack("!BH", bytes_data[ptr: ptr + 3])
        self.recv_status = ai_setting.get_zcl_msg_status_str(status)
        self.description += ' status:' + hex(status) + '(' + self.recv_status + ')' + ' group_id: 0x%04x' % group_id
        self.payload_items.extend(['\tstatus:' + hex(status) + '(' + self.recv_status + ')',
                                   '\tgroup_id: 0x%04x' % group_id])

    def hci_zcl_group_view_rsp_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.parse_zcl_rsp_cmd_header(bytes_data)

        status, group_id = struct.unpack("!BH", bytes_data[ptr: ptr + 3])
        self.recv_status = ai_setting.get_zcl_msg_status_str(status)
        self.description += ' status:' + hex(status) + '(' + self.recv_status + ')' + ' group_id: 0x%04x' % group_id
        ptr += 3
        self.payload_items.extend(['\tstatus:' + hex(status) + '(' + self.recv_status + ')',
                                   '\tgroup_id: 0x%04x' % group_id])
        if self.recv_status == 'ZCL_STA_SUCCESS':
            group_name_len, = struct.unpack("!B", bytes_data[ptr:ptr + 1])
            ptr += 1
            try:
                group_name = bytes(bytes_data[ptr:ptr + group_name_len]).decode('ascii')
            except UnicodeDecodeError:
                group_name = 'trans ascii fail!'
            self.description += ' group_name_len:%02x' % group_name_len + ' group_name:' + group_name
            self.payload_items.extend(['\tgroup_name_len:' + hex(group_name_len),
                                       '\tgroup_name:' + group_name])

    def hci_zcl_group_get_membership_rsp_handle(self, payload):
        bytes_data = bytearray(payload)
        ptr = self.parse_zcl_rsp_cmd_header(bytes_data)

        capacity, group_cnt = struct.unpack("!2B", bytes_data[ptr: ptr + 2])
        self.description += ' capacity:' + hex(capacity) + ' group_cnt:' + hex(group_cnt)
        ptr += 2
        self.payload_items.extend(['\tcapacity:' + hex(capacity),
                                   '\tgroup_cnt:' + hex(group_cnt)])
        for a in range(group_cnt):
            group_id, = struct.unpack("!H", bytes_data[ptr:ptr + 2])
            self.description += ' group_id: 0x%04x' % group_id
            ptr += 2
            self.payload_items.extend(['\tgroup_id: 0x%04x' % group_id])

    def hci_zcl_group_remove_rsp_handle(self, ai_setting, payload):
        return self.hci_zcl_group_add_rsp_handle(ai_setting, payload)

    def hci_zcl_identify_query_rsp_handle(self, payload):
        bytes_data = bytearray(payload)
        ptr = self.parse_zcl_rsp_cmd_header(bytes_data)

        timeout, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
        self.description += ' timeout:' + hex(timeout)
        self.payload_items.extend(['\ttimeout:' + hex(timeout) + '(s)'])

    def hci_zcl_zbd_ifstate_response_handle(self, payload):
        bytes_data = bytearray(payload)
        ptr = self.parse_zcl_rsp_cmd_header(bytes_data)

        preState, currState = struct.unpack("!2B", bytes_data[ptr: ptr + 2])
        self.description += ' preState:' + hex(preState)
        self.description += ' currState:' + hex(currState)
        self.payload_items.extend(['\tpreState:' + hex(preState) + ',' + 'currState:' + hex(currState)])

    def hci_zcl_scene_add_rsp_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.parse_zcl_rsp_cmd_header(bytes_data)

        status, group_id, scene_id = struct.unpack("!BHB", bytes_data[ptr: ptr + 4])
        self.recv_status = ai_setting.get_zcl_msg_status_str(status)
        self.description += ' status:' + hex(status) + '(' + self.recv_status + ')' + ' group_id: 0x%04x' % group_id + \
                            ' scene_id:' + hex(scene_id)
        self.payload_items.extend(['\tstatus:' + hex(status) + '(' + self.recv_status + ')',
                                   '\tgroup_id: 0x%04x' % group_id,
                                   '\tscene_id:' + hex(scene_id)])
        return ptr

    def hci_zcl_scene_view_rsp_handle(self, ai_setting, payload):
        self.hci_zcl_scene_add_rsp_handle(ai_setting, payload)

    def hci_zcl_scene_remove_rsp_handle(self, ai_setting, payload):
        self.hci_zcl_scene_add_rsp_handle(ai_setting, payload)

    def hci_zcl_scene_all_remove_rsp_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.parse_zcl_rsp_cmd_header(bytes_data)

        status, group_id = struct.unpack("!BH", bytes_data[ptr: ptr + 3])
        self.recv_status = ai_setting.get_zcl_msg_status_str(status)
        self.description += ' status:' + hex(status) + '(' + self.recv_status + ')' + ' group_id: 0x%04x' % group_id
        self.payload_items.extend(['\tstatus:' + hex(status) + '(' + self.recv_status + ')',
                                   '\tgroup_id: 0x%04x' % group_id])

    def hci_zcl_scene_store_rsp_handle(self, ai_setting, payload):
        return self.hci_zcl_scene_add_rsp_handle(ai_setting, payload)

    def hci_zcl_scene_get_membership_rsp_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.parse_zcl_rsp_cmd_header(bytes_data)

        status, capacity, group_id = struct.unpack("!2BH", bytes_data[ptr: ptr + 4])
        ptr += 4
        self.recv_status = ai_setting.get_zcl_msg_status_str(status)
        self.description += ' status:' + hex(status) + '(' + self.recv_status + ')' + ' capacity:' + hex(capacity) + \
                            ' group_id: 0x%04x' % group_id
        self.payload_items.extend(['\tstatus:' + hex(status) + '(' + self.recv_status + ')',
                                   '\tcapacity:' + hex(capacity),
                                   '\tgroup_id: 0x%04x' % group_id])
        if self.recv_status == 'ZCL_STA_SUCCESS':
            scene_cnt, = struct.unpack("!B", bytes_data[ptr:ptr + 1])
            ptr += 1
            self.description += ' scene_cnt:%02x' % scene_cnt
            self.payload_items.extend(['\tscene_cnt:' + hex(scene_cnt)])
            if scene_cnt > 0:
                self.description += ' scene_list:'
                self.payload_items.extend(['\tscene_list:'])
                scene_list = ''
                for a in range(scene_cnt):
                    scene, = struct.unpack("!B", bytes_data[ptr:ptr + 1])
                    ptr += 1
                    self.description += hex(scene) + ' '
                    scene_list += '0x%02x ' % scene
                self.payload_items.extend(['\t' + scene_list])

    def hci_ota_start_response_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        flash_base_addr, total_size, file_offset, status = struct.unpack("!3LB", bytes_data)
        self.description = 'flash_base_addr:' + hex(flash_base_addr) + ' total_size:' + hex(total_size) + \
                           ' file_offset:' + hex(file_offset) + ' status:' + hex(status)
        self.recv_status = ai_setting.get_hci_ota_status_str(status)

        self.payload_items = ['\tflash_base_addr:' + hex(flash_base_addr),
                              '\ttotal_size:' + hex(total_size),
                              '\tfile_offset:' + hex(file_offset),
                              '\tstatus:' + hex(status) + '(' + self.recv_status + ')']

        # return description, recv_status, payload_items

    def hci_ota_block_request_handle(self, payload):
        bytes_data = bytearray(payload)
        file_offset, block_len = struct.unpack("!LB", bytes_data)
        self.description = 'file_offset:' + hex(file_offset) + ' block_len:' + hex(block_len)
        block_rsp_len = block_len

        # print('file_offset:')
        # print(file_offset)
        # print(block_len)
        # print('self.hci_ota_file_path:')
        # print(self.hci_ota_file_path)
        if self.hci_ota_file_path != '':
            with open(self.hci_ota_file_path, mode='rb')as f:
                bin_data = f.read()
            try:
                self.hci_ota_offset = file_offset
                block_data = bin_data[file_offset: file_offset + block_len]
                status = 0  # success
            except ValueError:
                block_rsp_len = 0
                block_data = []
                status = 0x03  # ZBHCI_OTA_INCORRECT_OFFSET
        else:
            block_rsp_len = 0
            block_data = []
            status = 0x05  # 'ZBHCI_OTA_NOT_IN_PROGRESS'
        self.payload_items = ['\tfile_offset:' + hex(file_offset),
                              '\tblock_len:' + hex(block_len)]

        self.send_data = hci_ota_block_response_send(status, file_offset, block_rsp_len, block_data)
        # return description, send_data, payload_items

    def hci_ota_end_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        total_size, file_offset, status = struct.unpack("!2LB", bytes_data)
        self.recv_status = ai_setting.get_hci_ota_status_str(status)
        self.description = 'total_size:' + hex(total_size) + ' file_offset:' + hex(file_offset) + ' status:' \
                           + self.recv_status
        self.payload_items = ['\ttotal_size:' + hex(total_size),
                              '\tfile_offset:' + hex(file_offset),
                              '\tstatus:' + hex(status) + '(' + self.recv_status + ')']
        self.hci_ota_offset = file_offset
        # return description, recv_status, payload_items

    def hci_data_confirm_handle(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = 0
        dst_mode, = struct.unpack("!B", bytes_data[ptr:ptr + 1])
        ptr += 1
        addr_mode_str = ai_setting.get_conf_dst_addrmode(dst_mode)
        self.description = 'dstAddr_mode: 0x%02x' % dst_mode + "(" + addr_mode_str + ")"
        self.payload_items = ['\tdstAddr_mode:0x%02x' % dst_mode + "(" + addr_mode_str + ")"]
        dst_endpoint = 0xfe
        dst_addr = 0xfffe

        if dst_mode == 0:  # APS_DSTADDR_EP_NOTPRESETNT
            src_endpoint, = struct.unpack("!B", bytes_data[ptr:ptr + 1])
            ptr += 1
            self.description += ' src_endpoint: 0x%02x' % src_endpoint
            self.payload_items.extend(['\tsrc_endpoint: 0x%02x' % src_endpoint])
        elif dst_mode == 1:  # APS_SHORT_GROUPADDR_NOEP
            dst_addr, src_endpoint = struct.unpack("!HB", bytes_data[ptr:ptr + 3])
            ptr += 3
            self.description += ' dst_addr: 0x%04x' % dst_addr + ' src_endpoint: 0x%02x' % src_endpoint
            self.payload_items.extend(['\tdst_addr: 0x%04x' % dst_addr,
                                       '\tsrc_endpoint: 0x%02x' % src_endpoint])
        elif dst_mode == 2:  # APS_SHORT_DSTADDR_WITHEP
            dst_addr, src_endpoint, dst_endpoint = struct.unpack("!HBB", bytes_data[ptr:ptr + 4])
            ptr += 4
            self.description += 'dst_addr: 0x%04x' % dst_addr + ' src_endpoint: 0x%02x' % src_endpoint + \
                                ' dst_endpoint: 0x%02x' % dst_endpoint
            self.payload_items.extend(['\tdst_addr: 0x%04x' % dst_addr,
                                       '\tsrc_endpoint: 0x%02x' % src_endpoint,
                                       '\tdst_endpoint: 0x%02x' % dst_endpoint])
        elif dst_mode == 3:  # APS_LONG_DSTADDR_WITHEP
            dst_addr, src_endpoint, dst_endpoint = struct.unpack("!QBB", bytes_data[ptr:ptr + 10])
            ptr += 10
            self.description += ' dst_addr: 0x%016x' % dst_addr + ' src_endpoint: 0x%02x' % src_endpoint + \
                                ' dst_endpoint: 0x%02x' % dst_endpoint
            self.payload_items.extend(['\tdst_addr: 0x%016x' % dst_addr,
                                       '\tsrc_endpoint: 0x%02x' % src_endpoint,
                                       '\tdst_endpoint: 0x%02x' % dst_endpoint])
        self.addr_mode = dst_mode

        cluster_id, status, aps_cnt = struct.unpack("!H2B", bytes_data[ptr: ptr+4])
        if dst_endpoint == 0:
            cluster_str = ai_setting.get_zdp_cluster_str(cluster_id)
        else:
            cluster_str = ai_setting.get_zcl_cluster_str(cluster_id)
        self.recv_status = ai_setting.get_confirm_status_str(status)
        self.description += ' cluster_id:' + hex(cluster_id) + '(' + cluster_str + ')' + ' status:' + hex(status) + \
                            '(' + self.recv_status + ')' + ' aps_cnt:' + hex(aps_cnt)

        self.payload_items.extend(['\tcluster_id: 0x%04x' % cluster_id + '(' + cluster_str + ')',
                                   '\tstatus:' + hex(status) + '(' + self.recv_status + ')',
                                   '\taps_cnt:' + hex(aps_cnt)])
        self.recv_nwk_addr = dst_addr
        self.addr_mode = dst_mode
        self.recv_cluster_id = cluster_str

    def hci_mac_addr_ind_handle(self, payload):
        bytes_data = bytearray(payload)
        ieee_addr, = struct.unpack("!Q", bytes_data)
        self.description = 'ieee_addr:0x%016x' % ieee_addr
        self.payload_items = ['\tieee_addr:0x%016x' % ieee_addr]

    def hci_node_leave_ind_handle(self, payload, nodes_info):
        bytes_data = bytearray(payload)
        total_cnt, ieee_addr = struct.unpack("!HQ", bytes_data)
        self.description = 'total_cnt:' + hex(total_cnt) + ' ieee_addr:0x%016x' % ieee_addr
        self.payload_items = ['\ttotal_cnt:' + hex(total_cnt),
                              '\tieee_addr:0x%016x' % ieee_addr]
        if ieee_addr in nodes_info:
            self.nodes_info_change = True
            nodes_info[ieee_addr]['used_nwk_addr'].append(nodes_info[ieee_addr]['nwk_addr'])
            nodes_info[ieee_addr]['nwk_addr'] = 0xfffe
            # del nodes_info[ieee_addr]
        self.recv_nwk_addr = ieee_addr
        self.addr_mode = 3


class ParseSendCommand:
    def __init__(self, ai_setting, command_id, payload_len, payload):
        # self.nwk_add = ''
        self.nodes_info_change = False
        self.parse_items = []
        self.description = ''
        self.send_status = ''
        self.send_dst_addr = 0xfffe
        self.addr_mode = 0xff
        self.parse_send_command(ai_setting, command_id, payload_len, payload)

    def parse_send_command(self, ai_setting, command_id, payload_len, payload):
        # print('parse_send_command command_id:%s' % hex(command_id))
        # print(payload)

        command_str = ai_setting.get_command_id_str(command_id)
        self.parse_items.append('hci commandid: 0x%04x' % command_id + ' (' + command_str + ')')
        self.parse_items.append('payload len: %d' % payload_len)
        # print(self.parse_items)
        try:
            if command_id == 0x0000:  # ZBHCI_CMD_BDB_COMMISSION_FORMATION
                self.hci_bdb_commission_formation_req_parse(payload)
            elif command_id == 0x0001:  # BDB_COMMISSION_STEER
                self.hci_bdb_commission_steer_req_parse(payload)
            elif command_id == 0x0002:  # BDB_COMMISSION_TOUCHLINK
                self.hci_bdb_commission_touchlink_req_parse(payload)
            elif command_id == 0x0005:  # ZBHCI_CMD_BDB_FACTORY_RESET
                self.hci_bdb_bdb_factory_reset_parse(payload)
            elif command_id == 0x0006:  # ZBHCI_CMD_BDB_PRE_INSTALL_CODE
                self.hci_bdb_bdb_preinstall_code_parse(payload)
            elif command_id == 0x0007:  # 'BDB_CHANNEL_SET',
                self.hci_bdb_bdb_channel_set_parse(payload)
            elif command_id == 0x0008:  # 'BDB_DONGLE_WORKING_MODE_SET',
                self.hci_bdb_bdb_workingmode_set_parse(payload)
            elif command_id == 0x0009:  # 'BDB_NODE_DELETE',
                self.hci_bdb_bdb_node_delete_parse(payload)
            elif command_id == 0x000a:  # ZBHCI_CMD_BDB_TX_POWER_SET
                self.hci_bdb_bdb_txpower_set_parse(payload)
            elif command_id == 0x0010:  # 'DISCOVERY_NWK_ADDR_REQ',
                self.hci_nwk_addr_req_parse(payload)
            elif command_id == 0x0011:  # 'DISCOVERY_IEEE_ADDR_REQ',
                self.hci_ieee_addr_req_parse(payload)
            elif command_id == 0x0012:  # 'DISCOVERY_NODE_DESC_REQ',
                self.hci_node_desc_req_parse(payload)
            elif command_id == 0x0013:  # 'DISCOVERY_SIMPLE_DESC_REQ',
                self.hci_simple_desc_req_parse(payload)
            elif command_id == 0x0014:  # 'DISCOVERY_MATCH_DESC_REQ',
                self.hci_match_desc_req_parse(payload)
            elif command_id == 0x0015:  # 'DISCOVERY_ACTIVE_EP_REQ',
                self.hci_active_endpoint_req_parse(payload)
            elif command_id == 0x0020:  # 'BINDING_REQ',
                self.hci_bind_req_parse(ai_setting, payload)
            elif command_id == 0x0021:  # 'UNBINDING_REQ',
                self.hci_unbind_req_parse(ai_setting, payload)
            elif command_id == 0x0030:  # 'MGMT_LQI_REQ',
                self.hci_mgmt_lqi_req_parse(payload)
            elif command_id == 0x0031:  # 'MGMT_BIND_REQ',
                self.hci_mgmt_bind_req_parse(payload)
            elif command_id == 0x0032:  # 'MGMT_LEAVE_REQ',
                self.hci_mgmt_leave_req_parse(payload)
            elif command_id == 0x0033:  # 'MGMT_DIRECT_JOIN_REQ',
                self.hci_direct_join_req_parse(payload)
            elif command_id == 0x0034:  # 'MGMT_PERMIT_JOIN_REQ',
                self.hci_mgmt_permit_join_req_parse(payload)
            elif command_id == 0x0035:  # 'MGMT_NWK_UPDATE_REQ',
                self.hci_mgmt_nwkupdate_req_parse(payload)
            elif command_id == 0x0040:  # 'NODES_JOINED_GET_REQ',
                self.hci_nodes_joined_get_parse(payload)
            elif command_id == 0x0041:  # 'NODES_TOGGLE_TEST_REQ',
                self.hci_nodes_toggle_test_req_parse(payload)
            elif command_id == 0x0042:  # 'TXRX_PERFORMANCE_TEST_REQ',
                self.hci_txrx_performance_test_req_parse(payload)
            elif command_id == 0x0044:  # 'AF_DATA_SEND_TEST_REQ',
                self.hci_af_data_send_test_parse(ai_setting, payload)
            elif command_id == 0x0045:  # 'GET_OWN_NWK_INFO_REQ',
                self.hci_get_own_info_parse(payload)
            elif command_id == 0x0100:  # 'ZCL_ATTR_READ',
                self.hci_zcl_attr_read_parse(ai_setting, payload)
            elif command_id == 0x0101:  # 'ZCL_ATTR_WRITE',
                self.hci_zcl_attr_write_parse(ai_setting, payload_len, payload)
            elif command_id == 0x0102:  # 'ZCL_CONFIG_REPORT',
                self.hci_zcl_config_report_parse(ai_setting, payload)
            elif command_id == 0x0103:  # 'ZCL_READ_REPORT_CFG',
                self.hci_zcl_read_report_cfg_parse(ai_setting, payload)
            elif command_id == 0x0110:  # 'ZCL_BASIC_RESET',
                self.hci_zcl_basic_reset_parse(ai_setting, payload)
            elif command_id == 0x0120:  # 'ZCL_GROUP_ADD',
                self.hci_zcl_group_add_parse(ai_setting, payload)
            elif command_id == 0x0121:  # 'ZCL_GROUP_VIEW',
                self.hci_zcl_group_view_parse(ai_setting, payload)
            elif command_id == 0x0122:  # 'ZCL_GROUP_GET_MEMBERSHIP',
                self.hci_zcl_group_get_memnbership_parse(ai_setting, payload)
            elif command_id == 0x0123:  # 'ZCL_GROUP_REMOVE',
                self.hci_zcl_group_remove_parse(ai_setting, payload)
            elif command_id == 0x0124:  # 'ZCL_GROUP_REMOVE_ALL',
                self.hci_zcl_group_remove_all_parse(ai_setting, payload)
            elif command_id == 0x0125:  # 'ZCL_GROUP_ADD_IF_IDENTIFY',
                self.hci_zcl_group_add_if_identify_parse(ai_setting, payload)
            elif command_id == 0x0130:  # 'ZCL_IDENTIFY',
                self.hci_zcl_identify_parse(ai_setting, payload)
            elif command_id == 0x0131:  # 'ZCL_IDENTIFY_QUERY',
                self.hci_zcl_identify_query_parse(ai_setting, payload)
            elif command_id == 0x0140:  # 'ZCL_ONOFF_ON',
                self.hci_zcl_onoff_on_parse(ai_setting, payload)
            elif command_id == 0x0141:  # 'ZCL_ONOFF_OFF',
                self.hci_zcl_onoff_off_parse(ai_setting, payload)
            elif command_id == 0x0142:  # 'ZCL_ONOFF_TOGGLE',
                self.hci_zcl_onoff_toggle_parse(ai_setting, payload)
            elif command_id == 0x0150:  # 'ZCL_LEVEL_MOVE2LEVEL',
                self.hci_zcl_level_move2level_parse(ai_setting, payload)
            elif command_id == 0x0151:  # 'ZCL_LEVEL_MOVE',
                self.hci_zcl_level_move_parse(ai_setting, payload)
            elif command_id == 0x0152:  # 'ZCL_LEVEL_STEP',
                self.hci_zcl_level_step_parse(ai_setting, payload)
            elif command_id == 0x0153:  # 'ZCL_LEVEL_STOP',
                self.hci_zcl_level_stop_parse(ai_setting, payload)
            elif command_id == 0x0154:  # 'ZCL_LEVEL_MOVE2LEVEL_WITHONOFF',
                self.hci_zcl_level_move2level_withonoff_parse(ai_setting, payload)
            elif command_id == 0x0155:  # 'ZCL_LEVEL_MOVE_WITHONOFF',
                self.hci_zcl_level_move_withonoff_parse(ai_setting, payload)
            elif command_id == 0x0156:  # 'ZCL_LEVEL_STEP_WITHONOFF',
                self.hci_zcl_level_step_withonoff_parse(ai_setting, payload)
            elif command_id == 0x0157:  # 'ZCL_LEVEL_STOP_WITHONOFF',
                self.hci_zcl_level_stop_withonoff_parse(ai_setting, payload)
            elif command_id == 0x0160:  # 'ZCL_SCENE_ADD',
                self.hci_zcl_scene_add_parse(ai_setting, payload)
            elif command_id == 0x0161:  # 'ZCL_SCENE_VIEW',
                self.hci_zcl_scene_view_parse(ai_setting, payload)
            elif command_id == 0x0162:  # 'ZCL_SCENE_REMOVE',
                self.hci_zcl_scene_remove_parse(ai_setting, payload)
            elif command_id == 0x0163:  # 'ZCL_SCENE_REMOVE_ALL',
                self.hci_zcl_scene_remove_all_parse(ai_setting, payload)
            elif command_id == 0x0164:  # 'ZCL_SCENE_STORE',
                self.hci_zcl_scene_store_parse(ai_setting, payload)
            elif command_id == 0x0165:  # 'ZCL_SCENE_RECALL',
                self.hci_zcl_scene_recall_parse(ai_setting, payload)
            elif command_id == 0x0166:  # 'ZCL_SCENE_GET_MEMBERSHIP',
                self.hci_zcl_scene_get_membership_parse(ai_setting, payload)
            elif command_id == 0x0170:  # 'ZCL_COLOR_MOVE2HUE',
                self.hci_zcl_color_move2hue_parse(ai_setting, payload)
            elif command_id == 0x0171:  # 'ZCL_COLOR_MOVE2COLOR',
                self.hci_zcl_color_move2color_parse(ai_setting, payload)
            elif command_id == 0x0172:  # 'ZCL_COLOR_MOVE2SAT',
                self.hci_zcl_color_move2sat_parse(ai_setting, payload)
            elif command_id == 0x0173:  # 'ZCL_COLOR_MOVE2TEMP',
                self.hci_zcl_color_move2temp_parse(ai_setting, payload)
            elif command_id == 0x0180:  # 'ZCL_IAS_ZONE',
                self.hci_zcl_ias_zone_parse(ai_setting, payload)
            elif command_id == 0x0190:  # 'ZCL_OTA_IMAGE_NOTIFY',
                self.hci_zcl_ota_image_notify_parse(ai_setting, payload)
            elif command_id == 0x0210:  # 'HCI_OTA_START_REQUEST',
                self.hci_ota_start_req_parse(payload)
            elif command_id == 0x0211:  # 'ZBHCI_CMD_OTA_BLOCK_RESPONSE',
                self.hci_ota_block_response_parse(ai_setting, payload)
            elif command_id == 0x0220:  # 'ZBHCI_CMD_ZCL_ZDD_SET_STATE',
                self.hci_zcl_zbd_ifstate_set_parse(ai_setting, payload)
            elif command_id == 0x0221:  # 'ZBHCI_CMD_ZCL_ZDD_SET_ANJ_TIME',
                self.hci_zcl_zbd_anj_timeout_set_parse(ai_setting, payload)
            else:
                self.parse_items.append('command not support!')
                self.description = 'command not support!'
                # print('command_id:%04x' % command_id)
                # print('这些命令暂时不支持！')
        except IndexError:
            self.description = 'parsing length error!'
            self.parse_items.append('parsing length error!')
            # print('parsing length error!')
        except struct.error:
            self.description = 'Parsing failed!'
            self.parse_items.append('Parsing failed!')
        # pass

    def hci_bdb_commission_formation_req_parse(self, payload):
        pass

    def hci_bdb_commission_steer_req_parse(self, payload):
        pass

    def hci_bdb_commission_touchlink_req_parse(self, payload):
        bytes_data = bytearray(payload)
        role, = struct.unpack("!B", bytes_data)
        if role == 1:
            role_str = 'initiator'
        else:
            role_str = 'target'
        self.description += 'touchlink role:%d' % role + '(' + role_str + ')'
        self.parse_items.append('\ttouchlink role:%d' % role + '(' + role_str + ')')

    def hci_bdb_bdb_factory_reset_parse(self, payload):
        pass

    def hci_bdb_bdb_preinstall_code_parse(self, payload):
        bytes_data = bytearray(payload)
        ieee_addr, = struct.unpack("!Q", bytes_data[:8])
        self.description += 'ieee_addr: 0x%016x'% ieee_addr
        self.parse_items.append('\tieee_addr: 0x%016x' % ieee_addr)
        install_code, = struct.unpack("!16s", bytes_data[8:])
        install_code_str = '0x'
        for a in range(16):
            hvol = install_code[a]
            hhex = '%02x' % hvol
            install_code_str += hhex
        self.description += ' install_code: ' + install_code_str
        self.parse_items.append('\tinstall_code: ' + install_code_str)

    def hci_bdb_bdb_channel_set_parse(self, payload):
        bytes_data = bytearray(payload)
        channel, = struct.unpack("!B", bytes_data)
        self.description += 'channel: %d' % channel
        self.parse_items.append('\tchannel: %d' % channel)

    def hci_bdb_bdb_workingmode_set_parse(self, payload):
        bytes_data = bytearray(payload)
        working_mode, = struct.unpack("!B", bytes_data)
        self.description += 'working_mode: %d' % working_mode
        self.parse_items.append('\tworking_mode: %d' % working_mode)

    def hci_bdb_bdb_node_delete_parse(self, payload):
        bytes_data = bytearray(payload)
        ieee_addr, = struct.unpack("!Q", bytes_data)
        self.description += 'ieee_addr: 0x%016x' % ieee_addr
        self.parse_items.append('\tieee_addr: 0x%016x' % ieee_addr)

    def hci_bdb_bdb_txpower_set_parse(self, payload):
        bytes_data = bytearray(payload)
        tx_power, = struct.unpack("!B", bytes_data)
        self.description += 'tx_power: ' + hex(tx_power)
        self.parse_items.append('\ttx_power: ' + hex(tx_power))

    def hci_nwk_addr_req_parse(self, payload):
        bytes_data = bytearray(payload)
        dst_addr, interest_addr, single, start_idx = struct.unpack("!HQ2B", bytes_data)
        self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
        self.parse_items.append('\tinterest_addr: 0x%016x' % interest_addr)
        self.parse_items.append('\tsingle: ' + hex(single))
        self.parse_items.append('\tstart_idx: ' + hex(start_idx))
        self.description += 'interest_addr: 0x%016x' % interest_addr + ' single: ' + hex(single) + ' start_idx: ' + \
                            hex(start_idx)
        self.send_dst_addr = dst_addr
        self.addr_mode = 2

    def hci_ieee_addr_req_parse(self, payload):
        bytes_data = bytearray(payload)
        dst_addr, interest_addr, single, start_idx = struct.unpack("!2H2B", bytes_data)
        self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
        self.parse_items.append('\tinterest_addr: 0x%04x' % interest_addr)
        self.parse_items.append('\tsingle: ' + hex(single))
        self.parse_items.append('\tstart_idx: ' + hex(start_idx))
        self.description += 'interest_addr: 0x%04x' % interest_addr + ' single: ' + hex(single) + ' start_idx: ' + \
                            hex(start_idx)
        self.send_dst_addr = dst_addr
        self.addr_mode = 2

    def hci_node_desc_req_parse(self, payload):
        bytes_data = bytearray(payload)
        dst_addr, interest_addr = struct.unpack("!2H", bytes_data)
        self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
        self.parse_items.append('\tinterest_addr: 0x%04x' % interest_addr)
        self.description += 'interest_addr: 0x%04x' % interest_addr
        self.send_dst_addr = dst_addr
        self.addr_mode = 2

    def hci_simple_desc_req_parse(self, payload):
        bytes_data = bytearray(payload)
        dst_addr, interest_addr, endpoint = struct.unpack("!2HB", bytes_data)
        self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
        self.parse_items.append('\tinterest_addr: 0x%04x' % interest_addr)
        self.parse_items.append('\tendpoint: ' + hex(endpoint))
        self.description += 'interest_addr: 0x%04x' % interest_addr + ' endpoint: ' + hex(endpoint)
        self.send_dst_addr = dst_addr
        self.addr_mode = 2

    def hci_match_desc_req_parse(self, payload):
        bytes_data = bytearray(payload)
        dst_addr, interest_addr, profile_id, incluster_cnt, outcluster_cnt = struct.unpack("!3H2B", bytes_data[:8])
        self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
        self.parse_items.append('\tinterest_addr: 0x%04x' % interest_addr)
        self.parse_items.append('\tprofile_id: 0x%04x' % profile_id)
        self.parse_items.append('\tincluster_cnt: ' + hex(incluster_cnt))
        self.description += 'interest_addr: 0x%04x' % interest_addr + ' profile_id: 0x%04x' % profile_id + \
                            ' incluster_cnt: ' + hex(incluster_cnt)
        ptr = 8
        if incluster_cnt > 0:
            incluster_list = ''
            for a in range(incluster_cnt):
                incluster_id, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
                # incluster_list += hex(incluster_id) + ' '
                incluster_list += '0x%04x ' % incluster_id
                ptr += 2
            self.parse_items.append('\tincluster_list: ' + incluster_list)
            self.description += ' incluster_list: ' + incluster_list
        self.parse_items.append('\toutcluster_cnt: ' + hex(outcluster_cnt))
        self.description += ' outcluster_cnt: ' + hex(outcluster_cnt)
        if outcluster_cnt > 0:
            outcluster_list = ''
            for a in range(outcluster_cnt):
                outcluster_id, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
                outcluster_list += '0x%04x ' % outcluster_id
                ptr += 2
            self.parse_items.append('\toutcluster_list: ' + outcluster_list)
            self.description += ' outcluster_list: ' + outcluster_list
        self.send_dst_addr = dst_addr
        self.addr_mode = 2

    def hci_active_endpoint_req_parse(self, payload):
        bytes_data = bytearray(payload)
        dst_addr, interest_addr = struct.unpack("!2H", bytes_data)
        self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
        self.parse_items.append('\tinterest_addr: 0x%04x' % interest_addr)
        self.description += 'interest_addr: 0x%04x' % interest_addr
        self.send_dst_addr = dst_addr
        self.addr_mode = 2

    def hci_bind_req_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        src_addr, src_ep, clister_id = struct.unpack("!QBH", bytes_data[:11])
        self.parse_items.append('\tsrc_addr: 0x%016x' % src_addr)
        self.parse_items.append('\tsrc_ep: 0x%02x' % src_ep)
        self.parse_items.append('\tclister_id: 0x%04x' % clister_id)
        self.description += 'src_addr: 0x%016x' % src_addr + ' src_ep: 0x%02x' % src_ep + \
                            ' clister_id: 0x%04x' % clister_id
        ptr = 11
        dst_addr_mode, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
        dst_mode = ai_setting.get_dst_addr_mode(dst_addr_mode)
        self.parse_items.append('\tdst_addr_mode: ' + hex(dst_addr_mode) + '(' + dst_mode + ')')
        self.description += ' dst_addr_mode: ' + hex(dst_addr_mode) + '(' + dst_mode + ')'
        ptr += 1
        if dst_addr_mode == 3:  # 长地址
            dst_addr, = struct.unpack("!Q", bytes_data[ptr: ptr + 8])
            ptr += 8
            self.parse_items.append('\tdst_addr: 0x%016x' % dst_addr)
            dst_ep, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
            self.parse_items.append('\tdst_ep:  0x%02x' % dst_ep)
            self.description += ' dst_addr: 0x%016x' % dst_addr + ' dst_ep:  0x%02x' % dst_ep
        else:
            dst_addr, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
            ptr += 2
            self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
            self.description += ' dst_addr: 0x%04x' % dst_addr
        self.send_dst_addr = src_addr
        self.addr_mode = 3

    def hci_unbind_req_parse(self, ai_setting, payload):
        self.hci_bind_req_parse(ai_setting, payload)

    def hci_mgmt_lqi_req_parse(self, payload):
        bytes_data = bytearray(payload)
        dst_addr, start_idx = struct.unpack("!HB", bytes_data)
        self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
        self.parse_items.append('\tstart_idx: ' + hex(start_idx))
        self.description += 'start_idx:' + hex(start_idx)
        self.send_dst_addr = dst_addr
        self.addr_mode = 2

    def hci_mgmt_bind_req_parse(self, payload):
        bytes_data = bytearray(payload)
        dst_addr, start_idx = struct.unpack("!HB", bytes_data)
        self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
        self.parse_items.append('\tstart_idx: ' + hex(start_idx))
        self.description += 'start_idx:' + hex(start_idx)
        self.send_dst_addr = dst_addr
        self.addr_mode = 2

    def hci_mgmt_leave_req_parse(self, payload):
        bytes_data = bytearray(payload)
        dst_addr, ieee_addr, rejoin, children_leave = struct.unpack("!HQ2B", bytes_data)
        self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
        self.parse_items.append('\tieee_addr: 0x%016x' % ieee_addr)
        self.parse_items.append('\trejoin: ' + hex(rejoin))
        self.parse_items.append('\tchildren_leave: ' + hex(children_leave))
        self.description += 'ieee_addr: 0x%016x' % ieee_addr + 'rejoin: ' + hex(rejoin) + 'children_leave: ' + \
                            hex(children_leave)
        self.send_dst_addr = dst_addr
        self.addr_mode = 2

    def hci_direct_join_req_parse(self, payload):
        pass

    def hci_mgmt_permit_join_req_parse(self, payload):
        bytes_data = bytearray(payload)
        dst_addr, duration, tc_significance = struct.unpack("!H2B", bytes_data)
        self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
        self.parse_items.append('\tduration: ' + hex(duration) + ' (s)')
        self.parse_items.append('\ttc_significance: ' + hex(tc_significance))
        self.description += 'duration: ' + hex(duration) + ' (s)' + ' tc_significance: ' + hex(tc_significance)
        self.send_dst_addr = dst_addr
        self.addr_mode = 2

    def hci_mgmt_nwkupdate_req_parse(self, payload):
        bytes_data = bytearray(payload)
        dst_addr, nwk_manager_addr, scan_channel, scan_duration, scan_cnt = struct.unpack("!2HL2B", bytes_data)
        self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
        self.parse_items.append('\tnwk_manager_addr: 0x%04x' % nwk_manager_addr)
        self.parse_items.append('\tscan_channel: 0x%08x' % scan_channel)

        if scan_duration <= 5:
            updata_type = '(ED scan)'
            str_follow = 'scan_cnt:'
        elif scan_duration == 0xfe:
            updata_type = '(channel change)'
            str_follow = 'update_id:'
        elif scan_duration == 0xff:
            str_follow = 'update_id:'
            updata_type = '(apsChannelMaskList and nwkManagerAddr change)'
        else:
            updata_type = '(invalid)'
            str_follow = 'scan_cnt:'
        self.parse_items.append('\tscan_duration: ' + hex(scan_duration) + updata_type)
        self.parse_items.append('\t' + str_follow + hex(scan_cnt))

        self.description += 'nwk_manager_addr: 0x%04x' % nwk_manager_addr + ' scan_channel: 0x%08x' % scan_channel \
                            + ' scan_duration: ' + hex(scan_duration) + updata_type + ' ' + str_follow + hex(scan_cnt)
        self.send_dst_addr = dst_addr
        self.addr_mode = 2

    def hci_nodes_joined_get_parse(self, payload):
        bytes_data = bytearray(payload)
        start_idx, = struct.unpack("!H", bytes_data)
        self.parse_items.append('\tstart_idx: ' + hex(start_idx))
        self.description += 'start_idx: ' + hex(start_idx)

    def hci_nodes_toggle_test_req_parse(self, payload):
        bytes_data = bytearray(payload)
        test_mode, interval = struct.unpack("!2B", bytes_data)
        if test_mode:
            test_mode_str = '(unicast)'
        else:
            test_mode_str = '(broadcast)'
        self.parse_items.append('\ttest_mode: ' + hex(test_mode) + test_mode_str)
        self.parse_items.append('\tinterval: ' + hex(interval))
        self.description += 'test_mode: ' + hex(test_mode) + test_mode_str + ' interval: ' + hex(interval)

    def hci_txrx_performance_test_req_parse(self, payload):
        bytes_data = bytearray(payload)
        dst_addr, src_ep, dst_ep, send_cnt, send_interval, tx_power = struct.unpack("!H2BH2B", bytes_data)
        self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
        self.parse_items.append('\tsrc_ep: 0x%02x' % src_ep)
        self.parse_items.append('\tdst_ep:  0x%02x' % dst_ep)
        self.parse_items.append('\tsend_cnt: ' + hex(send_cnt))
        self.parse_items.append('\tsend_interval: ' + hex(send_interval))
        self.parse_items.append('\ttx_power_idx: ' + hex(tx_power))
        self.description += 'src_ep: 0x%02x' % src_ep + ' dst_ep:  0x%02x' % dst_ep + ' send_cnt: ' + hex(send_cnt) + \
                            ' send_interval: ' + hex(send_interval) + ' tx_power_idx: ' + hex(tx_power)
        self.send_dst_addr = dst_addr
        self.addr_mode = 2

    def hci_af_data_send_test_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        dst_addr, src_ep, dst_ep, cluster_id, data_len = struct.unpack("!H2B2H", bytes_data[:8])
        self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
        self.parse_items.append('\tsrc_ep: 0x%02x' % src_ep)
        self.parse_items.append('\tdst_ep:  0x%02x' % dst_ep)
        cluster_id_str = ai_setting.get_zcl_cluster_str(cluster_id)
        self.parse_items.append('\tcluster_id:  0x%04x' % cluster_id + '(' + cluster_id_str + ')')
        self.parse_items.append('\tdata_len: ' + hex(data_len))
        ptr = 8
        data_list = '0x'
        for a in range(data_len):
            data_bytes, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
            ptr += 1
            data_list += '%02x' % data_bytes
        self.parse_items.append('\tdata: ' + data_list)
        self.description += 'src_ep: 0x%02x' % src_ep + ' dst_ep:  0x%02x' % dst_ep + \
                            ' cluster_id:  0x%04x' % cluster_id + '(' + cluster_id_str + ')' + \
                            ' data_len: ' + hex(data_len)
        self.send_dst_addr = dst_addr
        self.addr_mode = 2

    def hci_get_own_info_parse(self, payload):
        pass

    def hci_zcl_addr_resolve(self, ai_setting, bytes_data):
        dst_addr_mode, = struct.unpack("!B", bytes_data[:1])
        dst_mode = ai_setting.get_dst_addr_mode(dst_addr_mode)
        self.parse_items.append('\tdst_addr_mode: ' + hex(dst_addr_mode) + '(' + dst_mode + ')')
        self.description += 'dst_addr_mode: ' + hex(dst_addr_mode) + '(' + dst_mode + ')'
        ptr = 1
        if dst_mode == 'ieee' or dst_mode == 'ieee_no_ack':  # ieee
            dst_addr, = struct.unpack("!Q", bytes_data[ptr: ptr + 8])
            ptr += 8
            self.parse_items.append('\tdst_addr:0x%016x' % dst_addr)
            self.description += ' dst_addr:0x%016x' % dst_addr
            self.send_dst_addr = dst_addr
            self.addr_mode = 3
        elif dst_mode == 'group' or dst_mode == 'short' or dst_mode == 'broadcast' or dst_addr_mode == 'short_no_ack':
            dst_addr, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
            ptr += 2
            self.parse_items.append('\tdst_addr: 0x%04x' % dst_addr)
            self.description += ' dst_addr:0x%04x' % dst_addr
            self.send_dst_addr = dst_addr
            self.addr_mode = 2
        else:
            self.addr_mode = 0
        if dst_mode == 'group':
            self.addr_mode = 1
        src_ep, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
        ptr += 1
        self.parse_items.append('\tsrc_ep: 0x%02x' % src_ep)
        self.description += ' src_ep: 0x%02x' % src_ep

        if dst_mode == 'short' or dst_mode == 'ieee' or dst_mode == 'broadcast' or dst_mode == 'short_no_ack' or \
                dst_mode == 'ieee_no_ack':
            dst_ep, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
            ptr += 1
            self.parse_items.append('\tdst_ep:  0x%02x' % dst_ep)
            self.description += ' dst_ep: 0x%02x' % dst_ep
        return ptr

    def hci_zcl_attr_read_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        profile_id, direction, cluster_id, attr_cnt = struct.unpack("!HBHB", bytes_data[ptr: ptr + 6])
        ptr += 6
        self.parse_items.append('\tprofile_id:  0x%04x' % profile_id)
        if direction == 0:
            self.parse_items.append('\tdirection: ' + hex(direction) + '(to server)')
            self.description += ' direction: ' + hex(direction) + '(to server)'
        else:
            self.parse_items.append('\tdirection: ' + hex(direction) + '(to client)')
            self.description += ' direction: ' + hex(direction) + '(to client)'

        cluster_id_str = ai_setting.get_zcl_cluster_str(cluster_id)
        self.parse_items.append('\tcluster_id:  0x%04x' % cluster_id + '(' + cluster_id_str + ')')
        self.parse_items.append('\tattr_cnt:  %d' % attr_cnt)
        self.description += ' cluster_id:  0x%04x' % cluster_id + '(' + cluster_id_str + ')' + \
                            ' attr_cnt:  %d' % attr_cnt
        # print('hci_zcl_attr_read_parse:attr_cnt:%d' % attr_cnt)
        for num in range(attr_cnt):
            attr_id, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
            ptr += 2
            self.parse_items.append('\tattr_id: 0x%04x ' % attr_id)
            self.description += ' attr_id: 0x%04x ' % attr_id

    def hci_zcl_attr_write_parse(self, ai_setting, payload_len, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        profile_id, direction, cluster_id, attr_cnt = struct.unpack("!HBHB", bytes_data[ptr: ptr + 6])
        ptr += 6
        self.parse_items.append('\tprofile_id:  0x%04x' % profile_id)
        if direction == 0:
            self.parse_items.append('\tdirection: ' + hex(direction) + '(to server)')
            self.description += ' direction: ' + hex(direction) + '(to server)'
        else:
            self.parse_items.append('\tdirection: ' + hex(direction) + '(to client)')
            self.description += ' direction: ' + hex(direction) + '(to client)'
        cluster_id_str = ai_setting.get_zcl_cluster_str(cluster_id)
        self.parse_items.append('\tcluster_id:  0x%04x' % cluster_id + '(' + cluster_id_str + ')')
        self.parse_items.append('\tattr_cnt:  %d' % attr_cnt)
        self.description += ' cluster_id:  0x%04x' % cluster_id + '(' + cluster_id_str + ')' + \
                            ' attr_cnt:  %d' % attr_cnt
        for num in range(attr_cnt):
            attr_id, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
            ptr += 2
            self.parse_items.append('\tattr_id: 0x%04x ' % attr_id)
            self.description += ' attr_id: 0x%04x ' % attr_id
            data_type, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
            ptr += 1
            data_type_str = ai_setting.get_zcl_data_type(data_type)
            self.parse_items.append('\tdata_type: ' + hex(data_type) + '(' + data_type_str + ')')
            data_len = payload_len - ptr
            if data_len > 0:
                data_str = '0x'
                for a in range(data_len):
                    data, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
                    ptr += 1
                    data_str += '%02x' % data
                self.parse_items.append('\tdata: ' + data_str)

    def hci_zcl_config_report_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        profile_id, direction, cluster_id, attr_cnt = struct.unpack("!HBHB", bytes_data[ptr: ptr + 6])
        ptr += 6
        self.parse_items.append('\tprofile_id:  0x%04x' % profile_id)
        if direction == 0:
            self.parse_items.append('\tdirection: ' + hex(direction) + '(to server)')
            self.description += ' direction: ' + hex(direction) + '(to server)'
        else:
            self.parse_items.append('\tdirection: ' + hex(direction) + '(to client)')
            self.description += ' direction: ' + hex(direction) + '(to client)'
        cluster_id_str = ai_setting.get_zcl_cluster_str(cluster_id)
        self.parse_items.append('\tcluster_id:  0x%04x' % cluster_id + '(' + cluster_id_str + ')')
        self.parse_items.append('\tnum_attr: ' + hex(attr_cnt))
        self.description += ' cluster_id:  0x%04x' % cluster_id + '(' + cluster_id_str + ')' + \
                            ' attr_cnt:  %d' % attr_cnt
        for a in range(attr_cnt):
            report_dir, attr_id = struct.unpack("!BH", bytes_data[ptr: ptr + 3])
            ptr += 3
            if report_dir == 0:
                self.parse_items.append('\treport_dir: ' + hex(report_dir) + '(reported)')
                self.parse_items.append('\tattr_id: 0x%04x ' % attr_id)
                self.description += ' report_dir: ' + hex(report_dir) + '(reported)' + ' attr_id: 0x%04x ' % attr_id
                data_type, min_report_interval, max_report_interval = struct.unpack("!B2H", bytes_data[ptr: ptr + 5])
                ptr += 5
                data_type_str = ai_setting.get_zcl_data_type(data_type)
                self.parse_items.append('\tdata_type: ' + hex(data_type) + '(' + data_type_str + ')')
                self.parse_items.append('\tmin_report_interval: ' + hex(min_report_interval))
                self.parse_items.append('\tmax_report_interval: ' + hex(max_report_interval))

                # print('hci_zcl_config_report_parse, data_type:0x%x' % data_type)
                if ai_setting.is_analog_data_type(data_type):
                    data_len = ai_setting.get_data_type_len(data_type)
                    data_str = '0x'
                    for num in range(data_len):
                        data, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
                        ptr += 1
                        data_str += '%02x' % data
                    self.parse_items.append('\treportable_change: ' + data_str)
            else:
                self.parse_items.append('\treport_dir: ' + hex(report_dir) + '(received)')
                self.parse_items.append('\tattr_id: 0x%04x ' % attr_id)
                timeout_period, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
                self.parse_items.append('\ttimeout_period: ' + hex(timeout_period))

    def hci_zcl_read_report_cfg_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        profile_id, direction, cluster_id, attr_cnt = struct.unpack("!HBHB", bytes_data[ptr: ptr + 6])
        ptr += 6
        self.parse_items.append('\tprofile_id:  0x%04x' % profile_id)
        if direction == 0:
            self.parse_items.append('\tdirection: ' + hex(direction) + '(to server)')
            self.description += ' direction: ' + hex(direction) + '(to server)'
        else:
            self.parse_items.append('\tdirection: ' + hex(direction) + '(to client)')
            self.description += ' direction: ' + hex(direction) + '(to client)'
        cluster_id_str = ai_setting.get_zcl_cluster_str(cluster_id)
        self.parse_items.append('\tcluster_id:  0x%04x' % cluster_id + '(' + cluster_id_str + ')')
        self.parse_items.append('\tnum_attr: ' + hex(attr_cnt))
        self.description += ' cluster_id:  0x%04x' % cluster_id + '(' + cluster_id_str + ')' + \
                            ' attr_cnt:  %d' % attr_cnt
        for a in range(attr_cnt):
            report_dir, attr_id = struct.unpack("!HB", bytes_data[ptr: ptr + 3])
            ptr += 3
            if report_dir == 0:
                self.parse_items.append('\treport_dir: ' + hex(report_dir) + '(reported)')
            else:
                self.parse_items.append('\treport_dir: ' + hex(report_dir) + '(except report)')
            self.parse_items.append('\tattr_id: 0x%04x ' % attr_id)
            self.description += ' report_dir: ' + hex(report_dir) + ' attr_id: 0x%04x ' % attr_id

    def hci_zcl_basic_reset_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        self.hci_zcl_addr_resolve(ai_setting, bytes_data)

    def hci_zcl_group_add_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        group_id, group_name_len = struct.unpack("!HB", bytes_data[ptr: ptr + 3])
        ptr += 3
        self.parse_items.append('\tgroup_id: 0x%04x' % group_id)
        self.parse_items.append('\tgroup_name_len: %d' % group_name_len)
        self.description += ' group_id: 0x%04x' % group_id + ' group_name_len: %d' % group_name_len
        try:
            group_name = bytes(bytes_data[ptr:]).decode('ascii')
        except UnicodeDecodeError:
            group_name = 'trans ascii fail!'
        self.parse_items.append('\tgroup_name: ' + group_name)

    def hci_zcl_group_view_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        group_id, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
        ptr += 2
        self.parse_items.append('\tgroup_id: 0x%04x' % group_id)
        self.description += ' group_id: 0x%04x' % group_id

    def hci_zcl_group_get_memnbership_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        group_cnt, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
        ptr += 1
        self.parse_items.append('\tgroup_cnt: ' + hex(group_cnt))
        self.description += ' group_cnt: ' + hex(group_cnt)
        if group_cnt > 0:
            group_list = ''
            for a in range(group_cnt):
                group_id, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
                ptr += 2
                group_list += '0x%04x ' % group_id
            self.parse_items.append('\tgroup_list: ' + group_list)
            self.description += ' group_list: ' + group_list

    def hci_zcl_group_remove_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        group_id, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
        self.parse_items.append('\tgroup_id: 0x%04x' % group_id)
        self.description += ' group_id: 0x%04x' % group_id

    def hci_zcl_group_remove_all_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        self.hci_zcl_addr_resolve(ai_setting, bytes_data)

    def hci_zcl_group_add_if_identify_parse(self, ai_setting, payload):
        self.hci_zcl_group_add_parse(ai_setting, payload)

    def hci_zcl_identify_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        identify_time, = struct.unpack("!H", bytes_data[ptr: ptr + 2])
        self.parse_items.append('\tidentify_time: ' + hex(identify_time) + '(s)')
        self.description += ' identify_time: ' + hex(identify_time) + '(s)'

    def hci_zcl_identify_query_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        self.hci_zcl_addr_resolve(ai_setting, bytes_data)

    def hci_zcl_onoff_on_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        self.hci_zcl_addr_resolve(ai_setting, bytes_data)

    def hci_zcl_onoff_off_parse(self, ai_setting, payload):
        self.hci_zcl_onoff_on_parse(ai_setting, payload)

    def hci_zcl_onoff_toggle_parse(self, ai_setting, payload):
        self.hci_zcl_onoff_on_parse(ai_setting, payload)

    def hci_zcl_level_move2level_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        move2level, transition_time = struct.unpack("!BH", bytes_data[ptr:])
        self.parse_items.append('\tmove2level: ' + hex(move2level))
        self.parse_items.append('\ttransition_time: ' + hex(transition_time) + ' (*0.1s)')
        self.description += ' move2level: ' + hex(move2level) + ' transition_time: ' + hex(transition_time) + ' (*0.1s)'

    def hci_zcl_level_move_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        move_mode, rate = struct.unpack("!2B", bytes_data[ptr:])
        if move_mode == 0:
            self.parse_items.append('\tmove_mode: ' + hex(move_mode) + '(up)')
            self.description += ' move_mode: ' + hex(move_mode) + '(up)'
        else:
            self.parse_items.append('\tmove_mode: ' + hex(move_mode) + '(down)')
            self.description += ' move_mode: ' + hex(move_mode) + '(down)'
        self.parse_items.append('\trate: ' + hex(rate))
        self.description += ' rate: ' + hex(rate)

    def hci_zcl_level_step_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        move_mode, step_size, transition_time = struct.unpack("!2BH", bytes_data[ptr:])
        if move_mode == 0:
            self.parse_items.append('\tmove_mode: ' + hex(move_mode) + '(up)')
            self.description += ' move_mode: ' + hex(move_mode) + '(up)'
        else:
            self.parse_items.append('\tmove_mode: ' + hex(move_mode) + '(down)')
            self.description += ' move_mode: ' + hex(move_mode) + '(down)'
        self.parse_items.append('\tstep_size: ' + hex(step_size))
        self.parse_items.append('\ttransition_time: ' + hex(transition_time) + ' (*0.1s)')
        self.description += ' step_size: ' + hex(step_size) + ' transition_time: ' + hex(transition_time) + ' (*0.1s)'

    def hci_zcl_level_stop_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        self.hci_zcl_addr_resolve(ai_setting, bytes_data)

    def hci_zcl_level_move2level_withonoff_parse(self, ai_setting, payload):
        self.hci_zcl_level_move2level_parse(ai_setting, payload)

    def hci_zcl_level_move_withonoff_parse(self, ai_setting, payload):
        self.hci_zcl_level_move_parse(ai_setting, payload)

    def hci_zcl_level_step_withonoff_parse(self, ai_setting, payload):
        self.hci_zcl_level_step_parse(ai_setting, payload)

    def hci_zcl_level_stop_withonoff_parse(self, ai_setting, payload):
        self.hci_zcl_level_stop_parse(ai_setting, payload)

    def hci_zcl_scene_add_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        group_id, scene_id, transition_time, scene_name_len = struct.unpack("!HBHB", bytes_data[ptr: ptr + 6])
        ptr += 6
        self.parse_items.append('\tgroup_id: 0x%04x' % group_id)
        self.parse_items.append('\tscene_id:  0x%02x' % scene_id)
        self.parse_items.append('\ttransition_time: ' + hex(transition_time) + ' (s)')
        self.parse_items.append('\tscene_name_len: ' + hex(scene_name_len))
        if scene_name_len > 0:
            try:
                scene_name = bytes(bytes_data[ptr:ptr + scene_name_len]).decode('ascii')
            except UnicodeDecodeError:
                scene_name = 'trans ascii fail!'
            self.parse_items.append('\tscene_name: ' + scene_name)
            ptr += scene_name_len

        extend_len, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
        ptr += 1
        self.parse_items.append('\textend_field_len: ' + hex(extend_len))
        if extend_len > 0:
            data_list = '0x'
            for a in range(extend_len):
                data, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
                ptr += 1
                data_list += '%02x' % data
            self.parse_items.append('\textend_field: ' + data_list)
        self.description += ' group_id: 0x%04x' % group_id + ' scene_id:  0x%02x' % scene_id + ' transition_time: ' + \
                            hex(transition_time) + ' (s)' + ' scene_name_len: ' + hex(scene_name_len) + \
                            ' extend_field_len: ' + hex(extend_len)

    def hci_zcl_scene_view_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        group_id, scene_id = struct.unpack("!HB", bytes_data[ptr:])
        self.parse_items.append('\tgroup_id: 0x%04x' % group_id)
        self.parse_items.append('\tscene_id:  0x%02x' % scene_id)
        self.description += ' group_id: 0x%04x' % group_id + ' scene_id:  0x%02x' % scene_id

    def hci_zcl_scene_remove_parse(self, ai_setting, payload):
        self.hci_zcl_scene_view_parse(ai_setting, payload)

    def hci_zcl_scene_remove_all_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        group_id, = struct.unpack("!H", bytes_data[ptr:])
        self.parse_items.append('\tgroup_id: 0x%04x' % group_id)
        self.description += ' group_id: 0x%04x' % group_id

    def hci_zcl_scene_store_parse(self, ai_setting, payload):
        self.hci_zcl_scene_view_parse(ai_setting, payload)

    def hci_zcl_scene_recall_parse(self, ai_setting, payload):
        self.hci_zcl_scene_view_parse(ai_setting, payload)

    def hci_zcl_scene_get_membership_parse(self, ai_setting, payload):
        self.hci_zcl_scene_remove_all_parse(ai_setting, payload)

    def hci_zcl_color_move2hue_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        hue, direction, transition_time = struct.unpack("!2BH", bytes_data[ptr:])
        self.parse_items.append('\thue: ' + hex(hue))
        if direction == 0:
            dir_str = '(Shortest distance)'
        elif direction == 1:
            dir_str = '(Longest distance)'
        elif direction == 2:
            dir_str = '(Up)'
        elif direction == 3:
            dir_str = '(Down)'
        else:
            dir_str = '(invalid)'
        self.parse_items.append('\tdirection: ' + hex(direction) + dir_str)
        self.parse_items.append('\ttransition_time: ' + hex(transition_time) + ' (*0.1s)')
        self.description += ' direction: ' + hex(direction) + dir_str + ' transition_time: ' + \
                            hex(transition_time) + ' (*0.1s)'

    def hci_zcl_color_move2color_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        color_x, color_y, transition_time = struct.unpack("!3H", bytes_data[ptr:])
        self.parse_items.append('\tcolorX: ' + hex(color_x))
        self.parse_items.append('\tcolorY: ' + hex(color_y))
        self.parse_items.append('\ttransition_time: ' + hex(transition_time) + ' (*0.1s)')
        self.description += ' colorX: ' + hex(color_x) + ' colorY: ' + hex(color_y) + ' transition_time: ' + \
                            hex(transition_time) + ' (*0.1s)'

    def hci_zcl_color_move2sat_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        saturation, transition_time = struct.unpack("!BH", bytes_data[ptr:])
        self.parse_items.append('\tsaturation: ' + hex(saturation))
        self.parse_items.append('\ttransition_time: ' + hex(transition_time) + ' (*0.1s)')
        self.description += ' saturation: ' + hex(saturation) + ' transition_time: ' + hex(transition_time) + ' (*0.1s)'

    def hci_zcl_color_move2temp_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        temperature, transition_time = struct.unpack("!2H", bytes_data[ptr:])
        self.parse_items.append('\ttemperature: ' + hex(temperature))
        self.parse_items.append('\ttransition_time: ' + hex(transition_time) + ' (*0.1s)')
        self.description += ' temperature: ' + hex(temperature) + ' transition_time: ' + hex(transition_time) + \
                            ' (*0.1s)'

    def hci_zcl_ias_zone_parse(self, ai_setting, payload):
        pass

    def hci_zcl_ota_image_notify_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        payload_type, query_jitter = struct.unpack("!2B", bytes_data[ptr:])
        self.parse_items.append('\tpayload_type: ' + hex(payload_type))
        self.parse_items.append('\tquery_jitter: ' + hex(query_jitter))
        self.description += ' payload_type: ' + hex(payload_type) + ' query_jitter: ' + hex(query_jitter)

    def hci_ota_start_req_parse(self, payload):
        bytes_data = bytearray(payload)
        ota_file_total_size, = struct.unpack("!L", bytes_data)
        self.parse_items.append('\tota_file_total_size: ' + hex(ota_file_total_size))
        self.description += 'ota_file_total_size: ' + hex(ota_file_total_size)

    def hci_ota_block_response_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        status, = struct.unpack("!B", bytes_data[:1])
        ptr = 1
        status_str = ai_setting.get_hci_ota_status_str(status)
        self.parse_items.append('\tstatus: ' + hex(status) + '(' + status_str + ')')
        self.description += 'status: ' + hex(status) + '(' + status_str + ')'
        if status_str == 'ZBHCI_OTA_SUCCESS':
            send_offset, block_len = struct.unpack("!LB", bytes_data[ptr:ptr + 5])
            self.parse_items.append('\tsend_offset:' + hex(send_offset))
            self.parse_items.append('\tblock_len:' + hex(block_len))
            self.description += ' send_offset:' + hex(send_offset) + ' block_len:' + hex(block_len)

    def hci_zcl_zbd_ifstate_set_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        if_state, = struct.unpack("!B", bytes_data[ptr: ptr + 1])
        self.parse_items.append('\tif_state: ' + hex(if_state))
        self.description += ' if_state: ' + hex(if_state)

    def hci_zcl_zbd_anj_timeout_set_parse(self, ai_setting, payload):
        bytes_data = bytearray(payload)
        ptr = self.hci_zcl_addr_resolve(ai_setting, bytes_data)
        anj_timeout0, anj_timeout1, anj_timeout2 = struct.unpack("!3B", bytes_data[ptr: ptr + 3])
        anj_timeout = (anj_timeout0 << 16) + (anj_timeout1 << 8) + anj_timeout2
        self.parse_items.append('\tanj_timeout: ' + hex(anj_timeout))
        self.description += ' anj_timeout: ' + hex(anj_timeout)


def crc8_calculate(datatype, length, data):
    crc8 = (datatype >> 0) & 0xff
    crc8 ^= (datatype >> 8) & 0xff
    crc8 ^= (length >> 0) & 0xff
    crc8 ^= (length >> 8) & 0xff

    for n in range(length):
        crc8 ^= data[n]

    return crc8


def hci_command_struct(command_id, payload_len, payload):
    command_start = 0x55
    command_crc = crc8_calculate(command_id, payload_len, payload)
    command_end = 0xAA
    if payload_len > 0:
        # payload_str = bytes(payload)
        send_data: bytes = struct.pack("!B2HB%dsB" % payload_len, command_start, command_id, payload_len,
                                       command_crc, payload, command_end)
    else:
        send_data: bytes = struct.pack("!B2H2B", command_start, command_id, payload_len, command_crc, command_end)
    # print(send_data)
    # print(len(send_data))
    # for data in send_data:
    # print(hex(data))

    return send_data


def hci_find_ieee_by_short(shortAddr):
    payload = struct.pack("!H", 0xffff)
    payload += struct.pack("!H", shortAddr)
    payload += struct.pack("!B", 0)
    payload += struct.pack("!B", 0)
    return hci_command_struct(0x0011, len(payload), payload)


def hci_get_joined_nodes_req_send(start_idx):
    payload = struct.pack("!H", start_idx)
    return hci_command_struct(0x0040, len(payload), payload)


def hci_get_own_info_req_send():
    return hci_command_struct(0x0045, 0, 'None')


def hci_bind_req_send(src_ieee_addr, src_endpoint, cluster, dst_addr_mode, dst_ieee_addr, dst_endpoint):
    payload = struct.pack("!QBHBQB", src_ieee_addr, src_endpoint, cluster, dst_addr_mode, dst_ieee_addr, dst_endpoint)
    payload_len = len(payload)
    return hci_command_struct(0x0020, payload_len, payload)


def hci_mgmt_lqi_req_send(dst_addr, start_idx):
    payload = struct.pack("!HB", dst_addr, start_idx)
    payload_len = len(payload)
    return hci_command_struct(0x0030, payload_len, payload)


def hci_mgmt_bind_req_send(dst_addr, start_idx):
    payload = struct.pack("!HB", dst_addr, start_idx)
    payload_len = len(payload)
    return hci_command_struct(0x0031, payload_len, payload)


def hci_ota_block_response_send(status, file_offset, block_len, block_data):
    if status == 0:  # success
        payload = struct.pack("!BLB%ds" % block_len, status, file_offset, block_len, block_data)
    else:
        payload = struct.pack("!BLB", status, file_offset, block_len)
    payload_len = len(payload)
    return hci_command_struct(0x0211, payload_len, payload)
