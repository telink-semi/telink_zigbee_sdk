class Settings:

    def __init__(self):
        # 屏幕设置
        self.ring_buff_length = 2500

        self.time_column = 0
        self.delta_column = 1
        self.payload_column = 2
        self.nwkaddr_column = 3
        self.command_column = 4
        self.status_column = 5
        self.desc_column = 6

        self.command_start = 0x55
        self.command_end = 0xaa
        self.command_length_min = 7
        self.command_length_max = 64
        self.packet_start_idx = 0
        self.packet_command_idx_high = 1
        self.packet_command_idx_low = 2
        self.packet_length_idx_high = 3
        self.packet_length_idx_low = 4
        self.packet_checksum_idx = 5
        self.packet_payload_start_idx = 6

        self.hci_msg_status = {
            0x00: 'ZBHCI_MSG_STATUS_SUCCESS',
            0x01: 'ZBHCI_MSG_STATUS_INCORRECT_PARAMETERS',
            0x02: 'ZBHCI_MSG_STATUS_UNHANDLED_COMMAND',
            0x03: 'ZBHCI_MSG_STATUS_BUSY',
            0x04: 'ZBHCI_MSG_STATUS_NO_MEMORY',
            0x05: 'ZBHCI_MSG_STATUS_STACK_ALREADY_STARTED',
            0x06: 'ZBHCI_MSG_STATUS_OTA_TIMEOUT',
            0x07: 'ZBHCI_MSG_STATUS_OTA_INPROCESS',
            0x08: 'ZBHCI_MSG_STATUS_OTA_SIZE_ERROR',
            0xe0: 'ZBHCI_MSG_STATUS_ERROR_START_CHAR',
            0xe1: 'ZBHCI_MSG_STATUS_MSG_OVERFLOW',
            0xe2: 'ZBHCI_MSG_STATUS_ERROR_END_CHAR',
            0xe3: 'ZBHCI_MSG_STATUS_BAD_MSG',
            0xe4: 'ZBHCI_MSG_STATUS_UART_EXCEPT',
            0xe5: 'ZBHCI_MSG_STATUS_CRC_ERROR'
        }

        self.hci_ota_status = {
            0x00: 'ZBHCI_OTA_SUCCESS',
            0x01: 'ZBHCI_OTA_GET_BLOCK_TIMEOUT',
            0x02: 'ZBHCI_OTA_IN_PROGRESS',
            0x03: 'ZBHCI_OTA_INCORRECT_OFFSET',
            0x04: 'ZBHCI_OTA_FILE_OVERSIZE',
            0x05: 'ZBHCI_OTA_NOT_IN_PROGRESS',
        }

        self.zcl_msg_status = {
            0x00: 'ZCL_STA_SUCCESS',
            0x01: 'ZCL_STA_FAILURE',
            0x7E: 'ZCL_STA_NOT_AUTHORIZED',
            0x80: 'ZCL_STA_MALFORMED_COMMAND',
            0x81: 'ZCL_STA_UNSUP_CLUSTER_COMMAND',
            0x82: 'ZCL_STA_UNSUP_GENERAL_COMMAND',
            0x83: 'ZCL_STA_UNSUP_MANU_CLUSTER_COMMAND',
            0x84: 'ZCL_STA_UNSUP_MANU_GENERAL_COMMAND',
            0x85: 'ZCL_STA_INVALID_FIELD',
            0x86: 'ZCL_STA_UNSUPPORTED_ATTRIBUTE',
            0x87: 'ZCL_STA_INVALID_VALUE',
            0x88: 'ZCL_STA_READ_ONLY',
            0x89: 'ZCL_STA_INSUFFICIENT_SPACE',
            0x8a: 'ZCL_STA_DUPLICATE_EXISTS',
            0x8b: 'ZCL_STA_NOT_FOUND',
            0x8c: 'ZCL_STA_UNREPORTABLE_ATTRIBUTE',
            0x8d: 'ZCL_STA_INVALID_DATA_TYPE',
            0x8e: 'ZCL_STA_INVALID_SELECTOR',
            0x8f: 'ZCL_STA_WRITE_ONLY',
            0x90: 'ZCL_STA_INCONSISTENT_STARTUP_STATE',
            0x91: 'ZCL_STA_DEFINED_OUT_OF_BAND',
            0x92: 'ZCL_STA_INCONSISTENT',
            0x93: 'ZCL_STA_ACTION_DENIED',
            0x94: 'ZCL_STA_TIMEOUT',
            0x95: 'ZCL_STA_ABORT',
            0x96: 'ZCL_STA_INVALID_IMAGE',
            0x97: 'ZCL_STA_WAIT_FOR_DATA',
            0x98: 'ZCL_STA_NO_IMAGE_AVAILABLE',
            0x99: 'ZCL_STA_REQUIRE_MORE_IMAGE',

            0xc0: 'ZCL_STA_HARDWARE_FAILURE',
            0xc1: 'ZCL_STA_SOFTWARE_FAILURE',
            0xc2: 'ZCL_STA_CALIBRATION_ERROR',

            0xff: 'ZCL_STA_CMD_HAS_RESP'
        }

        self.zdo_msg_status = {
            0x00: 'ZDO_SUCCESS',
            0x01: 'ZDO_INVALID_PARAMETERS',
            0x02: 'ZDO_RESPONSE_WAIT_TIMEOUT',
            0x04: 'ZDO_BUSY',
            0x05: 'ZDO_NO_NETWORKS',
            0x06: 'ZDO_EXTPANID_ALREADY_EXIST',
            0x07: 'ZDO_FAIL',
            0x08: 'ZDO_AUTHENTICATION_SUCCESS',
            0x09: 'ZDO_AUTHENTICATION_FAIL',
            0x0A: 'ZDO_SECURITY_SUCCESS',
            0x0B: 'ZDO_SECURITY_FAIL',
            0x0C: 'ZDO_SECURITY_NOT_SUPPORTED',
            0x0D: 'ZDO_CMD_COMPLETED',
            0x0E: 'ZDO_KEY_ESTABLISHMENT_NOT_SUPPORTED',

            0x60: 'ZDO_TC_NOT_AVAILABLE',

            0x80: 'ZDO_INVALID_REQUEST',
            0x81: 'ZDO_DEVICE_NOT_FOUND',
            0x82: 'ZDO_INVALID_EP',
            0x83: 'ZDO_NOT_ACTIVE',
            0x84: 'ZDO_NOT_SUPPORTED',
            0x85: 'ZDO_TIMEOUT',
            0x86: 'ZDO_NO_MATCH',

            0x88: 'ZDO_NO_ENTRY',
            0x89: 'ZDO_NO_DESCRIPTOR',
            0x8A: 'ZDO_INSUFFICIENT_SPACE',
            0x8B: 'ZDO_NOT_PERMITTED',
            0x8C: 'ZDO_TABLE_FULL',
            0x8D: 'ZDO_NOT_AUTHORIZED',
            0x8E: 'ZDO_DEVICE_BINDING_TABLE_FALL',
            0x8F: 'ZDO_NETWORK_STARTED',
            0x90: 'ZDO_NETWORK_LOST',
            0x91: 'ZDO_NETWORK_LEFT',
            0x92: 'ZDO_CHILD_JOINED',
            0x93: 'ZDO_CHILD_REMOVED',
            0x94: 'ZDO_USER_DESCRIPTOR_UPDATE',
            0x95: 'ZDO_STATIC_ADDRESS_CONFLICT'
        }

        self.send_confirm_status = {
            0x00: 'SUCCESS',
            0x01: 'RET_ERROR',
            0x06: 'RET_OUT_OF_RANGE',
            0x19: 'MAC_STA_BAD_STATE',
            0x1A: 'MAC_STA_NO_RESOURCES',
            0x1D: 'MAC_TX_ABORTED',
            0x20: 'MAC_STA_FRAME_PENDING',
            0x39: 'RET_BUF_FULL',
            0x89: 'ZCL_STA_INSUFFICIENT_SPACE',
            0xA7: 'APS_STATUS_NO_ACK',
            0xA8: 'APS_STATUS_NO_BOUND_DEVICE',
            0xA9: 'APS_STATUS_NO_SHORT_ADDRESS',
            0xAD: 'APS_STATUS_SECURITY_FAIL',
            0xB1: 'APS_STATUS_HANDLE_BACKING',
            0xB2: 'APS_STATUS_SHORT_ADDR_REQUESTING',
            0xB9: 'APS_STATUS_INTERNAL_BUF_FULL',
            0xC1: 'NWK_STATUS_INVALID_PARAMETER',
            0xC2: 'NWK_STATUS_INVALID_REQUEST',
            0xD0: 'NWK_STATUS_ROUTE_DISCOVERY_FAILED',
            0xD1: 'NWK_STATUS_ROUTE_ERROR',
            0xD4: 'NWK_STATUS_DECRYPT_ERROR',
            0xE1: 'MAC_STA_CHANNEL_ACCESS_FAILURE',
            0xE9: 'MAC_STA_NO_ACK',
            0xE5: 'MAC_STA_FRAME_TOO_LONG',
            0xF1: 'MAC_STA_TRANSACTION_OVERFLOW',
        }

        self.hci_command_list = {
            0x0001: 'ZBHCI_CMD_BDB_COMMISSION_FORMATION',
            0x0002: 'ZBHCI_CMD_BDB_COMMISSION_STEER',
            0x0003: 'ZBHCI_CMD_BDB_COMMISSION_TOUCHLINK',
            0x0004: 'ZBHCI_CMD_BDB_COMMISSION_FINDBIND',
            0x0005: 'ZBHCI_CMD_BDB_FACTORY_RESET',
            0x0006: 'ZBHCI_CMD_BDB_PRE_INSTALL_CODE',
            0x0007: 'ZBHCI_CMD_BDB_CHANNEL_SET',
            0x0008: 'ZBHCI_CMD_BDB_DONGLE_WORKING_MODE_SET',
            0x0009: 'ZBHCI_CMD_BDB_NODE_DELETE',
            0x000a: 'ZBHCI_CMD_BDB_TX_POWER_SET',
            0x8000: 'ZBHCI_CMD_ACKNOWLEDGE',

            0x0010: 'ZBHCI_CMD_DISCOVERY_NWK_ADDR_REQ',
            0x0011: 'ZBHCI_CMD_DISCOVERY_IEEE_ADDR_REQ',
            0x0012: 'ZBHCI_CMD_DISCOVERY_NODE_DESC_REQ',
            0x0013: 'ZBHCI_CMD_DISCOVERY_SIMPLE_DESC_REQ',
            0x0014: 'ZBHCI_CMD_DISCOVERY_MATCH_DESC_REQ',
            0x0015: 'ZBHCI_CMD_DISCOVERY_ACTIVE_EP_REQ',
            # 0x0016: 'ZBHCI_CMD_CMD_DISCOVERY_LEAVE_REQ',
            0x8010: 'ZBHCI_CMD_DISCOVERY_NWK_ADDR_RSP',
            0x8011: 'ZBHCI_CMD_DISCOVERY_IEEE_ADDR_RSP',
            0x8012: 'ZBHCI_CMD_DISCOVERY_NODE_DESC_RSP',
            0x8013: 'ZBHCI_CMD_DISCOVERY_SIMPLE_DESC_RSP',
            0x8014: 'ZBHCI_CMD_DISCOVERY_MATCH_DESC_RSP',
            0x8015: 'ZBHCI_CMD_DISCOVERY_ACTIVE_EP_RSP',

            0x0020: 'ZBHCI_CMD_BINDING_REQ',
            0x0021: 'ZBHCI_CMD_UNBINDING_REQ',
            0x8020: 'ZBHCI_CMD_BINDING_RSP',
            0x8021: 'ZBHCI_CMD_UNBINDING_RSP',

            0x0030: 'ZBHCI_CMD_MGMT_LQI_REQ',
            0x0031: 'ZBHCI_CMD_MGMT_BIND_REQ',
            0x0032: 'ZBHCI_CMD_MGMT_LEAVE_REQ',
            0x0033: 'ZBHCI_CMD_MGMT_DIRECT_JOIN_REQ',
            0x0034: 'ZBHCI_CMD_MGMT_PERMIT_JOIN_REQ',
            0x0035: 'ZBHCI_CMD_MGMT_NWK_UPDATE_REQ',
            0x8030: 'ZBHCI_CMD_MGMT_LQI_RSP',
            0x8031: 'ZBHCI_CMD_MGMT_BIND_RSP',
            0x8032: 'ZBHCI_CMD_MGMT_LEAVE_RSP',
            0x8033: 'ZBHCI_CMD_MGMT_DIRECT_JOIN_RSP',
            0x8034: 'ZBHCI_CMD_MGMT_PERMIT_JOIN_RSP',
            0x8035: 'ZBHCI_CMD_MGMT_NWK_UPDATE_RSP',

            0x0040: 'ZBHCI_CMD_NODES_JOINED_GET_REQ',
            0x0041: 'ZBHCI_CMD_NODES_TOGLE_TEST_REQ',
            0x0042: 'ZBHCI_CMD_TXRX_PERFORMANCE_TEST_REQ',
            0x0044: 'ZBHCI_CMD_AF_DATA_SEND_TEST_REQ',
            0x0045: 'ZBHCI_CMD_GET_OWN_NWK_INFO_REQ',

            0x8040: 'ZBHCI_CMD_NODES_JOINED_GET_RSP',
            0x8041: 'ZBHCI_CMD_NODES_TOGLE_TEST_RSP',
            0x8042: 'ZBHCI_CMD_TXRX_PERFORMANCE_TEST_RSP',
            0x8043: 'ZBHCI_CMD_NODES_DEV_ANNCE_IND',
            0x8044: 'ZBHCI_CMD_AF_DATA_SEND_TEST_RSP',
            0x8045: 'ZBHCI_CMD_GET_OWN_NWK_INFO_RSP',

            0x0100: 'ZBHCI_CMD_ZCL_ATTR_READ',
            0x0101: 'ZBHCI_CMD_ZCL_ATTR_WRITE',
            0x0102: 'ZBHCI_CMD_ZCL_CONFIG_REPORT',
            0x0103: 'ZBHCI_CMD_ZCL_READ_REPORT_CFG',
            0x8100: 'ZBHCI_CMD_ZCL_ATTR_READ_RSP',
            0x8101: 'ZBHCI_CMD_ZCL_ATTR_WRITE_RSP',
            0x8102: 'ZBHCI_CMD_ZCL_CONFIG_REPORT_RSP',
            0x8103: 'ZBHCI_CMD_ZCL_READ_REPORT_CFG_RSP',
            0x8104: 'ZBHCI_CMD_ZCL_REPORT_MSG_RCV',
            0x8105: 'ZBHCI_CMD_ZCL_DEFAULT_RSP',

            0x0110: 'ZBHCI_CMD_ZCL_BASIC_RESET',

            0x0120: 'ZBHCI_CMD_ZCL_GROUP_ADD',
            0x0121: 'ZBHCI_CMD_ZCL_GROUP_VIEW',
            0x0122: 'ZBHCI_CMD_ZCL_GROUP_GET_MEMBERSHIP',
            0x0123: 'ZBHCI_CMD_ZCL_GROUP_REMOVE',
            0x0124: 'ZBHCI_CMD_ZCL_GROUP_REMOVE_ALL',
            0x0125: 'ZBHCI_CMD_ZCL_GROUP_ADD_IF_IDENTIFY',
            0x8120: 'ZBHCI_CMD_ZCL_GROUP_ADD_RSP',
            0x8121: 'ZBHCI_CMD_ZCL_GROUP_VIEW_RSP',
            0x8122: 'ZBHCI_CMD_ZCL_GROUP_GET_MEMBERSHIP_RSP',
            0x8123: 'ZBHCI_CMD_ZCL_GROUP_REMOVE_RSP',

            0x0130: 'ZBHCI_CMD_ZCL_IDENTIFY',
            0x0131: 'ZBHCI_CMD_ZCL_IDENTIFY_QUERY',
            0x8131: 'ZBHCI_CMD_ZCL_IDENTIFY_QUERY_RSP',

            0x0140: 'ZBHCI_CMD_ZCL_ONOFF_ON',
            0x0141: 'ZBHCI_CMD_ZCL_ONOFF_OFF',
            0x0142: 'ZBHCI_CMD_ZCL_ONOFF_TOGGLE',

            0x0150: 'ZBHCI_CMD_ZCL_LEVEL_MOVE2LEVEL',
            0x0151: 'ZBHCI_CMD_ZCL_LEVEL_MOVE',
            0x0152: 'ZBHCI_CMD_ZCL_LEVEL_STEP',
            0x0153: 'ZBHCI_CMD_ZCL_LEVEL_STOP',
            0x0154: 'ZBHCI_CMD_ZCL_LEVEL_MOVE2LEVEL_WITHONOFF',
            0x0155: 'ZBHCI_CMD_ZCL_LEVEL_MOVE_WITHONOFF',
            0x0156: 'ZBHCI_CMD_ZCL_LEVEL_STEP_WITHONOFF',
            0x0157: 'ZBHCI_CMD_ZCL_LEVEL_STOP_WITHONOFF',

            0x0160: 'ZBHCI_CMD_ZCL_SCENE_ADD',
            0x0161: 'ZBHCI_CMD_ZCL_SCENE_VIEW',
            0x0162: 'ZBHCI_CMD_ZCL_SCENE_REMOVE',
            0x0163: 'ZBHCI_CMD_ZCL_SCENE_REMOVE_ALL',
            0x0164: 'ZBHCI_CMD_ZCL_SCENE_STORE',
            0x0165: 'ZBHCI_CMD_ZCL_SCENE_RECALL',
            0x0166: 'ZBHCI_CMD_ZCL_SCENE_GET_MEMBERSHIP',
            0x8160: 'ZBHCI_CMD_ZCL_SCENE_ADD_RSP',
            0x8161: 'ZBHCI_CMD_ZCL_SCENE_VIEW_RSP',
            0x8162: 'ZBHCI_CMD_ZCL_SCENE_REMOVE_RSP',
            0x8163: 'ZBHCI_CMD_ZCL_SCENE_REMOVE_ALL_RSP',
            0x8164: 'ZBHCI_CMD_ZCL_SCENE_STORE_RSP',
            0x8166: 'ZBHCI_CMD_ZCL_SCENE_GET_MEMBERSHIP_RSP',

            0x0170: 'ZBHCI_CMD_ZCL_COLOR_MOVE2HUE',
            0x0171: 'ZBHCI_CMD_ZCL_COLOR_MOVE2COLOR',
            0x0172: 'ZBHCI_CMD_ZCL_COLOR_MOVE2SAT',
            0x0173: 'ZBHCI_CMD_ZCL_COLOR_MOVE2TEMP',

            0x0180: 'ZBHCI_CMD_ZCL_IAS_ZONE',

            0x0190: 'ZBHCI_CMD_ZCL_OTA_IMAGE_NOTIFY',

            0x8200: 'ZBHCI_CMD_DATA_CONFIRM',
            0x8201: 'ZBHCI_CMD_MAC_ADDR_IND',
            0x8202: 'ZBHCI_CMD_NODE_LEAVE_IND',

            0x0210: 'ZBHCI_CMD_OTA_START_REQUEST',
            0x0211: 'ZBHCI_CMD_OTA_BLOCK_RESPONSE',

            0x8210: 'ZBHCI_CMD_OTA_START_RESPONSE',
            0x8211: 'ZBHCI_CMD_OTA_BLOCK_REQUEST',
            0x8212: 'ZBHCI_CMD_OTA_END_STATUS',
        }

        self.zcl_cluster_id = {
            0x0000: 'GEN_BASIC',
            0x0001: 'GEN_POWER_CFG',
            0x0002: 'GEN_TEMP_CFG',
            0x0003: 'GEN_IDENTIFY',
            0x0004: 'GEN_GROUPS',
            0x0005: 'GEN_SCENES',
            0x0006: 'GEN_ON_OFF',
            0x0007: 'GEN_ON_OFF_SWITCH_CFG',
            0x0008: 'GEN_LEVEL_CONTROL',
            0x0009: 'GEN_ALARMS',
            0x000A: 'GEN_TIME',
            0x000B: 'GEN_LOCATION',
            0x000C: 'GEN_ANALOG_INPUT_BASIC',
            0x000D: 'GEN_ANALOG_OUTPUT_BASIC',
            0x000E: 'GEN_ANALOG_VALUE_BASIC',
            0x000F: 'GEN_BINARY_INPUT_BASIC',
            0x0010: 'GEN_BINARY_OUTPUT_BASIC',
            0x0011: 'GEN_BINARY_VALUE_BASIC',
            0x0012: 'GEN_MULTISTATE_INPUT_BASIC',
            0x0013: 'GEN_MULTISTATE_OUTPUT_BASIC',
            0x0014: 'GEN_MULTISTATE_VALUE_BASIC',
            0x0015: 'GEN_COMMISSIONING',
            0x0016: 'GEN_PARTITION',

            0x0019: 'OTA',
            0x001A: 'GEN_POWER_PROFILE',
            0x001B: 'GEN_APPLIANCE_CONTROL',
            0x0020: 'GEN_POLL_CONTROL',
            0x0021: 'GEN_GREEN_POWER',
            0x0025: 'GEN_KEEP_ALIVE',
            0x0B05: 'GEN_DIAGNOSTICS',

            0x0100: 'CLOSURES_SHADE_CONFIG',
            0x0101: 'CLOSURES_DOOR_LOCK',
            0x0102: 'CLOSURES_WINDOW_COVERING',

            0x0200: 'HVAC_PUMP_CONFIG_CONTROL',
            0x0201: 'HAVC_THERMOSTAT',
            0x0202: 'HAVC_FAN_CONTROL',
            0x0203: 'HAVC_DIHUMIDIFICATION_CONTROL',
            0x0204: 'HAVC_USER_INTERFACE_CONFIG',

            0x0300: 'LIGHTING_COLOR_CONTROL',
            0x0301: 'LIGHTING_BALLAST_CONFIG',

            0x0400: 'MS_ILLUMINANCE_MEASUREMENT',
            0x0401: 'MS_ILLUMINANCE_LEVEL_SENSING_CONFIG',
            0x0402: 'MS_TEMPERATURE_MEASUREMENT',
            0x0403: 'MS_PRESSURE_MEASUREMENT',
            0x0404: 'MS_FLOW_MEASUREMENT',
            0x0405: 'MS_RELATIVE_HUMIDITY',
            0x0406: 'MS_OCCUPANCY_SENSING',
            0x0407: 'MS_LEAF_WETNESS',
            0x0408: 'MS_SOIL_MOISTURE',
            0x0B04: 'MS_ELECTRICAL_MEASUREMENT',

            0x0500: 'SS_IAS_ZONE',
            0x0501: 'SS_IAS_ACE',
            0x0502: 'SS_IAS_WD',

            0x0600: 'PI_GENERIC_TUNNEL',
            0x0601: 'PI_BACNET_PROTOCOL_TUNNEL',
            0x0602: 'PI_ANALOG_INPUT_BACNET_REG',
            0x0603: 'PI_ANALOG_INPUT_BACNET_EXT',
            0x0604: 'PI_ANALOG_OUTPUT_BACNET_REG',
            0x0605: 'PI_ANALOG_OUTPUT_BACNET_EXT',
            0x0606: 'PI_ANALOG_VALUE_BACNET_REG',
            0x0607: 'PI_ANALOG_VALUE_BACNET_EXT',
            0x0608: 'PI_BINARY_INPUT_BACNET_REG',
            0x0609: 'PI_BINARY_INPUT_BACNET_EXT',
            0x060A: 'PI_BINARY_OUTPUT_BACNET_REG',
            0x060B: 'PI_BINARY_OUTPUT_BACNET_EXT',
            0x060C: 'PI_BINARY_VALUE_BACNET_REG',
            0x060D: 'PI_BINARY_VALUE_BACNET_EXT',
            0x060E: 'PI_MULTISTATE_INPUT_BACNET_REG',
            0x060F: 'PI_MULTISTATE_INPUT_BACNET_EXT',
            0x0610: 'PI_MULTISTATE_OUTPUT_BACNET_REG',
            0x0611: 'PI_MULTISTATE_OUTPUT_BACNET_EXT',
            0x0612: 'PI_MULTISTATE_VALUE_BACNET_REG',
            0x0613: 'PI_MULTISTATE_VALUE_BACNET_EXT',
            0x0614: 'PI_11073_PROTOCOL_TUNNEL',

            0x0700: 'SE_PRICE',
            0x0701: 'SE_Demand_RSP_AND_LOAD_CONTROL',
            0x0702: 'SE_METERING',
            0x0703: 'SE_MESSAGING',
            0x0704: 'SE_SE_TUNNELING',
            0x0705: 'SE_PREPAYMENT',
            0x0800: 'GEN_KEY_ESTABLISHMENT',

            0x1000: 'TOUCHLINK_COMMISSIONING',

            0xFC57: 'CLUSTER_WWAH',

            0xFF00: 'TELINK_SDK_TEST',
            0xFF01: 'TELINK_SDK_TEST_REQ',
            0xFF02: 'TELINK_SDK_TEST_RSP',
            0xFF03: 'TELINK_SDK_TEST_CLEAR_REQ',
            0xFF04: 'TELINK_SDK_TEST_CLEAR_RSP',
        }
        self.zdp_cluster_id = {
            0x0000: 'NWK_ADDR',
            0x0001: 'IEEE_ADDR',
            0x0002: 'NODE_DESCRIPTOR',
            0x0003: 'POWER_DESCRIPTOR',
            0x0004: 'SIMPLE_DESCRIPTOR',
            0x0005: 'ACTIVE_ENDPOINTS',
            0x0006: 'MATCH_DESCRIPTOR',
            0x0010: 'COMPLEX_DESCRIPTOR',
            0x0011: 'USER_DESCRIPTOR',
            0x0012: 'DISCOVERY_CACHE',
            0x0013: 'DEVICE_ANNCE',
            0x0014: 'USER_DESC_CONF',
            0x0015: 'SYSTEM_SERVER_DISCOVERY',
            0x0016: 'DISCOVERY_STORE',
            0x0017: 'NODE_DESC_STORE',
            0x0018: 'POWER_DESC_STORE',
            0x0019: 'ACTIVE_EP_STORE',
            0x001A: 'SIMPLE_DESC_STORE',
            0x001B: 'REMOVE_NODE_CACHE',
            0x001C: 'FIND_NODE_CACHE',
            0x001D: 'EXTENDED_SIMPLE_DESC',
            0x001E: 'EXTENDED_ACTIVE_EP',
            0x001F: 'PARENT_ANNCE',
            0x0020: 'END_DEVICE_BIND',
            0x0021: 'BIND',
            0x0022: 'UNBIND',
            0x0023: 'BIND_REGISTER',
            0x0024: 'REPLACE_DEVICE',
            0x0025: 'STORE_BCKUP_BIND_ENTRY',
            0x0026: 'REMOVE_BCKUP_BIND_ENTRY',
            0x0027: 'BACKUP_BIND_TABLE',
            0x0028: 'RECOVER_BIND_TABLE',
            0x0029: 'BACKUP_SOURCE_BIND',
            0x002A: 'RECOVER_SOURCE_BIND',
            0x0030: 'MGMT_NWK_DISC',
            0x0031: 'MGMT_LQI',
            0x0032: 'MGMT_RTG',
            0x0033: 'MGMT_BIND',
            0x0034: 'MGMT_LEAVE',
            0x0035: 'MGMT_DIRECT_JOIN',
            0x0036: 'MGMT_PERMIT_JOINING',
            0x0037: 'MGMT_CACHE',
            0x0038: 'MGMT_NWK_UPDATE',

            0x8000: 'NWK_ADDR_RESP',
            0x8001: 'IEEE_ADDR_RESP',
            0x8002: 'NODE_DESCRIPTOR_RESP',
            0x8003: 'POWER_DESCRIPTOR_RESP',
            0x8004: 'SIMPLE_DESCRIPTOR_RESP',
            0x8005: 'ACTIVE_ENDPOINTS_RESP',
            0x8006: 'MATCH_DESCRIPTOR_RESP',
            0x8010: 'COMPLEX_DESCRIPTOR_RESP',
            0x8011: 'USER_DESCRIPTOR_RESP',
            0x8012: 'DISCOVERY_CACHE_RESP',
            0x8014: 'USER_DESC_CONF_RESP',
            0x8015: 'SYSTEM_SERVER_DISCOVERY_RESP',
            0x8016: 'DISCOVERY_STORE_RESP',
            0x8017: 'NODE_DESC_STORE_RESP',
            0x8018: 'POWER_DESC_STORE_RESP',
            0x8019: 'ACTIVE_EP_STORE_RESP',
            0x801A: 'SIMPLE_DESC_STORE_RESP',
            0x801B: 'REMOVE_NODE_CACHE_RESP',
            0x801C: 'FIND_NODE_CACHE_RESP',
            0x801D: 'EXTENDED_SIMPLE_DESC_RESP',
            0x801E: 'EXTENDED_ACTIVE_EP_RESP',
            0x801F: 'PARENT_ANNCE_RESP',
            0x8020: 'END_DEVICE_BIND_RESP',
            0x8021: 'BIND_RESP',
            0x8022: 'UNBIND_RESP',
            0x8023: 'BIND_REGISTER_RESP',
            0x8024: 'REPLACE_DEVICE_RESP',
            0x8025: 'STORE_BCKUP_BIND_ENTRY_RESP',
            0x8026: 'REMOVE_BCKUP_BIND_ENTRY_RESP',
            0x8027: 'BACKUP_BIND_TABLE_RESP',
            0x8028: 'RECOVER_BIND_TABLE_RESP',
            0x8029: 'BACKUP_SOURCE_BIND_RESP',
            0x802A: 'RECOVER_SOURCE_BIND_RESP',
            0x8030: 'MGMT_NWK_DISC_RESP',
            0x8031: 'MGMT_LQI_RESP',
            0x8032: 'MGMT_RTG_RESP',
            0x8033: 'MGMT_BIND_RESP',
            0x8034: 'MGMT_LEAVE_RESP',
            0x8035: 'MGMT_DIRECT_JOIN_RESP',
            0x8036: 'MGMT_PERMIT_JOINING_RESP',
            0x8037: 'MGMT_CACHE_RESP',
            0x8038: 'MGMT_NWK_UPDATE_RESP',
        }
        self.not_defined = 'NOT DEFINED'
        self.addr_mode = {
            'bound': 0,
            'group': 1,
            'short': 2,
            'ieee': 3,
            'broadcast': 4,
            'bound_no_ack': 5,
            'short_no_ack': 6,
            'ieee_no_ack': 7,
        }

        self.data_type_list = {
            0: 0x00,  # ZCL_DATA_TYPE_NO_DATA
            1: 0x08,  # ZCL_DATA_TYPE_DATA8
            2: 0x09,  # ZCL_DATA_TYPE_DATA16
            3: 0x0a,  # ZCL_DATA_TYPE_DATA24
            4: 0x0b,  # ZCL_DATA_TYPE_DATA32
            5: 0x0c,  # ZCL_DATA_TYPE_DATA40
            6: 0x0d,  # ZCL_DATA_TYPE_DATA48
            7: 0x0e,  # ZCL_DATA_TYPE_DATA56
            8: 0x0f,  # ZCL_DATA_TYPE_DATA64
            9: 0x10,  # ZCL_DATA_TYPE_BOOLEAN
            10: 0x18,  # ZCL_DATA_TYPE_BITMAP8
            11: 0x19,  # ZCL_DATA_TYPE_BITMAP16
            12: 0x1a,  # ZCL_DATA_TYPE_BITMAP24
            13: 0x1b,  # ZCL_DATA_TYPE_BITMAP32
            14: 0x1c,  # ZCL_DATA_TYPE_BITMAP40
            15: 0x1d,  # ZCL_DATA_TYPE_BITMAP48
            16: 0x1e,  # ZCL_DATA_TYPE_BITMAP56
            17: 0x1f,  # ZCL_DATA_TYPE_BITMAP64
            18: 0x20,  # ZCL_DATA_TYPE_UINT8
            19: 0x21,  # ZCL_DATA_TYPE_UINT16
            20: 0x22,  # ZCL_DATA_TYPE_UINT24
            21: 0x23,  # ZCL_DATA_TYPE_UINT32
            22: 0x24,  # ZCL_DATA_TYPE_UINT40
            23: 0x25,  # ZCL_DATA_TYPE_UINT48
            24: 0x26,  # ZCL_DATA_TYPE_UINT56
            25: 0x27,  # ZCL_DATA_TYPE_UINT64
            26: 0x28,  # ZCL_DATA_TYPE_INT8
            27: 0x29,  # ZCL_DATA_TYPE_INT16
            28: 0x2a,  # ZCL_DATA_TYPE_INT24
            29: 0x2b,  # ZCL_DATA_TYPE_INT32
            30: 0x2c,  # ZCL_DATA_TYPE_INT40
            31: 0x2d,  # ZCL_DATA_TYPE_INT48
            32: 0x2e,  # ZCL_DATA_TYPE_INT56
            33: 0x2f,  # ZCL_DATA_TYPE_INT64
            34: 0x30,  # ZCL_DATA_TYPE_ENUM8
            35: 0x31,  # ZCL_DATA_TYPE_ENUM16
            36: 0x38,  # ZCL_DATA_TYPE_SEMI_PREC
            37: 0x39,  # ZCL_DATA_TYPE_SINGLE_PREC
            38: 0x3a,  # ZCL_DATA_TYPE_DOUBLE_PREC
            39: 0x41,  # ZCL_DATA_TYPE_OCTET_STR
            40: 0x42,  # ZCL_DATA_TYPE_CHAR_STR
            41: 0x43,  # ZCL_DATA_TYPE_LONG_OCTET_STR
            42: 0x44,  # ZCL_DATA_TYPE_LONG_CHAR_STR
            43: 0x48,  # ZCL_DATA_TYPE_ARRAY
            44: 0x4c,  # ZCL_DATA_TYPE_STRUCT
            45: 0x50,  # ZCL_DATA_TYPE_SET
            46: 0x51,  # ZCL_DATA_TYPE_BAG
            47: 0xe0,  # ZCL_DATA_TYPE_TOD
            48: 0xe1,  # ZCL_DATA_TYPE_DATE
            49: 0xe2,  # ZCL_DATA_TYPE_UTC
            50: 0xe8,  # ZCL_DATA_TYPE_CLUSTER_ID
            51: 0xe9,  # ZCL_DATA_TYPE_ATTR_ID
            52: 0xea,  # ZCL_DATA_TYPE_BAC_OID
            53: 0xf0,  # ZCL_DATA_TYPE_IEEE_ADDR
            54: 0xf1,  # ZCL_DATA_TYPE_128_BIT_SEC_KEY
            55: 0xff,  # ZCL_DATA_TYPE_UNKNOWN
        }

        self.data_type_dict = {
            0x00: 'ZCL_DATA_TYPE_NO_DATA',
            0x08: 'ZCL_DATA_TYPE_DATA8',
            0x09: 'ZCL_DATA_TYPE_DATA16',
            0x0a: 'ZCL_DATA_TYPE_DATA24',
            0x0b: 'ZCL_DATA_TYPE_DATA32',
            0x0c: 'ZCL_DATA_TYPE_DATA40',
            0x0d: 'ZCL_DATA_TYPE_DATA48',
            0x0e: 'ZCL_DATA_TYPE_DATA56',
            0x0f: 'ZCL_DATA_TYPE_DATA64',
            0x10: 'ZCL_DATA_TYPE_BOOLEAN',
            0x18: 'ZCL_DATA_TYPE_BITMAP8',
            0x19: 'ZCL_DATA_TYPE_BITMAP16',
            0x1a: 'ZCL_DATA_TYPE_BITMAP24',
            0x1b: 'ZCL_DATA_TYPE_BITMAP32',
            0x1c: 'ZCL_DATA_TYPE_BITMAP40',
            0x1d: 'ZCL_DATA_TYPE_BITMAP48',
            0x1e: 'ZCL_DATA_TYPE_BITMAP56',
            0x1f: 'ZCL_DATA_TYPE_BITMAP64',
            0x20: 'ZCL_DATA_TYPE_UINT8',
            0x21: 'ZCL_DATA_TYPE_UINT16',
            0x22: 'ZCL_DATA_TYPE_UINT24',
            0x23: 'ZCL_DATA_TYPE_UINT32',
            0x24: 'ZCL_DATA_TYPE_UINT40',
            0x25: 'ZCL_DATA_TYPE_UINT48',
            0x26: 'ZCL_DATA_TYPE_UINT56',
            0x27: 'ZCL_DATA_TYPE_UINT64',
            0x28: 'ZCL_DATA_TYPE_INT8',
            0x29: 'ZCL_DATA_TYPE_INT16',
            0x2a: 'ZCL_DATA_TYPE_INT24',
            0x2b: 'ZCL_DATA_TYPE_INT32',
            0x2c: 'ZCL_DATA_TYPE_INT40',
            0x2d: 'ZCL_DATA_TYPE_INT48',
            0x2e: 'ZCL_DATA_TYPE_INT56',
            0x2f: 'ZCL_DATA_TYPE_INT64',
            0x30: 'ZCL_DATA_TYPE_ENUM8',
            0x31: 'ZCL_DATA_TYPE_ENUM16',
            0x38: 'ZCL_DATA_TYPE_SEMI_PREC',
            0x39: 'ZCL_DATA_TYPE_SINGLE_PREC',
            0x3a: 'ZCL_DATA_TYPE_DOUBLE_PREC',
            0x41: 'ZCL_DATA_TYPE_OCTET_STR',
            0x42: 'ZCL_DATA_TYPE_CHAR_STR',
            0x43: 'ZCL_DATA_TYPE_LONG_OCTET_STR',
            0x44: 'ZCL_DATA_TYPE_LONG_CHAR_STR',
            0x48: 'ZCL_DATA_TYPE_ARRAY',
            0x4c: 'ZCL_DATA_TYPE_STRUCT',
            0x50: 'ZCL_DATA_TYPE_SET',
            0x51: 'ZCL_DATA_TYPE_BAG',
            0xe0: 'ZCL_DATA_TYPE_TOD',
            0xe1: 'ZCL_DATA_TYPE_DATE',
            0xe2: 'ZCL_DATA_TYPE_UTC',
            0xe8: 'ZCL_DATA_TYPE_CLUSTER_ID',
            0xe9: 'ZCL_DATA_TYPE_ATTR_ID',
            0xea: 'ZCL_DATA_TYPE_BAC_OID',
            0xf0: 'ZCL_DATA_TYPE_IEEE_ADDR',
            0xf1: 'ZCL_DATA_TYPE_128_BIT_SEC_KEY',
            0xff: 'ZCL_DATA_TYPE_UNKNOWN',
        }

    def get_zcl_data_type(self, key_value):
        try:
            return self.data_type_dict[key_value]
        except KeyError:
            return self.not_defined

    def get_zdp_cluster_str(self, key_value):
        try:
            return self.zdp_cluster_id[key_value]
        except KeyError:
            return self.not_defined

    def get_zcl_cluster_str(self, key_value):
        try:
            return self.zcl_cluster_id[key_value]
        except KeyError:
            return self.not_defined

    def get_command_id_str(self, key_value):
        try:
            return self.hci_command_list[key_value]
        except KeyError:
            return self.not_defined

    def get_hci_command_id(self, command_str):
        for cell in self.hci_command_list:
            if command_str == self.hci_command_list[cell]:
                return cell
        else:
            return 0xffff

    def get_confirm_status_str(self, key_value):
        try:
            return self.send_confirm_status[key_value]
        except KeyError:
            return self.not_defined

    def get_zdp_msg_status_str(self, key_value):
        try:
            return self.zdo_msg_status[key_value]
        except KeyError:
            return self.not_defined

    def get_zcl_msg_status_str(self, key_value):
        try:
            return self.zcl_msg_status[key_value]
        except KeyError:
            return self.not_defined

    def get_hci_msg_status_str(self, key_value):
        try:
            return self.hci_msg_status[key_value]
        except KeyError:
            return self.not_defined

    def get_hci_ota_status_str(self, key_value):
        try:
            return self.hci_ota_status[key_value]
        except KeyError:
            return self.not_defined

    def get_dst_addr_mode(self, key_value):
        for item in self.addr_mode:
            if key_value == self.addr_mode[item]:
                return item
        else:
            return self.not_defined

    def is_analog_data_type(self, data_type):
        analog_list = [0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
                       0x38, 0x39, 0x3a, 0xe0, 0xe1, 0xe2]
        if data_type in analog_list:
            return True
        else:
            return False

    def get_data_type_len(self, data_type):
        list_sixe_1 = [0x10, 0x08, 0x18, 0x20, 0x28, 0x30]
        list_size_2 = [0x09, 0x19, 0x21, 0x29, 0x38, 0xe8, 0xe9]
        list_size_3 = [0x0a, 0x1a, 0x22, 0x2a]
        list_size_4 = [0x0b, 0x1b, 0x23, 0x2b, 0x39, 0xe2, 0xe1, 0xea]
        list_size_5 = [0x0b, 0x1b, 0x23, 0x2b, 0x39, 0xe2, 0xe1, 0xea]
        list_size_6 = [0x0d, 0x1d, 0x25, 0x2d]
        list_size_7 = [0x0e, 0x1e, 0x26, 0x2e]
        list_size_8 = [0x0f, 0x1f, 0x27, 0x2f, 0x3a, 0xf0]
        list_size_16 = [0xf1]
        if data_type in list_sixe_1:
            return 1
        elif data_type in list_size_2:
            return 2
        elif data_type in list_size_3:
            return 3
        elif data_type in list_size_4:
            return 4
        elif data_type in list_size_5:
            return 5
        elif data_type in list_size_6:
            return 6
        elif data_type in list_size_7:
            return 7
        elif data_type in list_size_8:
            return 8
        elif data_type in list_size_16:
            return 16
        else:
            return 0

    def get_attr_size(self, data_type, input_data):
        data_len = 0
        if data_type == 0x44 or data_type == 0x43:  # ZCL_DATA_TYPE_LONG_CHAR_STR  ZCL_DATA_TYPE_LONG_OCTET_STR
            data_len = input_data[1] << 8
            data_len += input_data[0]
            data_len += 2  # long string length + 2 for length field
        elif data_type == 0x42 or data_type == 0x41:  # ZCL_DATA_TYPE_CHAR_STR ZCL_DATA_TYPE_OCTET_STR
            data_len += input_data[0]
            data_len += 1
        elif data_type == 0x4c:
            item_num = input_data[0]
            data_len += 2
            for a in range(item_num):
                dt_union = input_data[data_len]
                attr_len = self.get_data_type_len(dt_union)
                data_len += (attr_len + 1)
        else:
            data_len = self.get_data_type_len(data_type)

        return data_len

