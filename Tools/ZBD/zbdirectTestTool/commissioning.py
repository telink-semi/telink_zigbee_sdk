import struct

def tlv_process(input):
    output = {}
    if len(input) < 3:
        return 0, output
    output['tlvid'] = input[0]
    output['length'] = input[1] + 1
    if output['length'] + 2 > len(input):
        return 0, output
    else:
        output['data'] = []
        for i in range(output['length']):
            output['data'].append(input[i+2])
    return 1, output

def tlv_construct(inputId, inputData, inputLen):
    tlv = []
    tlv.append(inputId)
    if inputLen != 0:
        tlv.append(inputLen-1)
        for i in range(inputLen):
            tlv.append(inputData[i])
    else:
        tlv = []
    return tlv



class CommHandle:
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.commServiceStr = '{0000fff7-0000-1000-8000-00805f9b34fb}'
        self.formCharaStr = '{7072377d-0001-421c-b163-491c27333a61}'
        self.joinCharaStr = '{7072377d-0002-421c-b163-491c27333a61}'
        self.permitCharaStr = '{7072377d-0003-421c-b163-491c27333a61}'
        self.leaveCharaStr = '{7072377d-0004-421c-b163-491c27333a61}'
        self.commStaCharaStr = '{7072377d-0005-421c-b163-491c27333a61}'
        self.manaCharaStr = '{7072377d-0006-421c-b163-491c27333a61}'
        self.identifyCharaStr = '{7072377d-0007-421c-b163-491c27333a61}'
        self.fbCharaStr = '{7072377d-0008-421c-b163-491c27333a61}'

        self.extPanidTlvId = 0x00
        self.shortPanidTlvId = 0x01
        self.nwkChannelTlvId = 0x02
        self.nwkKeyTlvId = 0x03
        self.linkKeyTlvId = 0x04
        self.deviceTypeTlvId = 0x05
        self.nwkAddrTlvId = 0x06
        self.joinMethodTlvId = 0x07
        self.ieeeAddrTlvId = 0x08
        self.tcAddrTlvId = 0x09
        self.nwkStatusTlvId = 0x0a
        self.nwkUpdateIdTlvId = 0x0b
        self.nwkActiveKeyNumTlvId = 0x0c
        self.adminKeyTlvId = 0x0d
        self.statusCodeTlvId = 0x0e
        self.extStatusCodeTlvId = 0x0f

        #Manager Joiners TLV IDs
        self.manaProviLinkkeyTlvId = 0x00
        self.manaIeeeAddeTlvId = 0x01
        self.manaJoinersCommTlvId = 0x02

        self.extPanIdLen = 8
        self.panIdLen = 2
        self.nwkChannelLen = 5
        self.nwkKeyLen = 16
        self.linkKeyLen = 17
        self.devTypeLen = 1
        self.nwkAddrLen = 2
        self.joinMethodLen = 1
        self.ieeeAddrLen = 8
        self.tcAddrLen = 8
        self.nwkStatusLen = 1
        self.nwkUpIdLen = 1
        self.nwkActiveKeySeqLen = 1
        self.adminKeyLen = 16
        self.statusCodeLen = 2
        self.permitJoinLen = 1
        self.removeChLen = 1
        self.leaveRejoinLen = 1
        self.identifyTimeLen = 2
        self.endpointLen = 1

        self.commFromTlv = {
            'AdminKey':{'length': self.adminKeyLen, 'usage':'Optional'},
            'TCAddress':{'length': self.tcAddrLen, 'usage':'Optional'},
            'ExtPanId':{'length': self.extPanIdLen, 'usage':'Optional'},
            'shortPanId':{'length': self.panIdLen, 'usage':'Optional'},
            'nwkChannel':{'length': self.nwkChannelLen, 'usage':'Optional'},
            'nwkKey':{'length': self.nwkKeyLen, 'usage':'Optional'},
            'linkKey':{'length': self.linkKeyLen, 'usage':'Optional'},
            'nwkAddr':{'length': self.nwkAddrLen, 'usage':'Optional'},
            'nwkUpdataId':{'length': self.nwkUpIdLen, 'usage':'Optional'},
        }

        self.commJoinAssoTlv = {
            'ExtPanId':{'length': self.extPanIdLen, 'usage':'Optional'},
            'nwkChannel':{'length': self.nwkChannelLen, 'usage':'Optional'},
            'linkKey':{'length': self.linkKeyLen, 'usage':'Optional'},
            'nwkAddr':{'length': self.nwkAddrLen, 'usage':'Optional'},
            'TCAddress':{'length': self.tcAddrLen, 'usage':'Optional'},
            'AdminKey':{'length': self.adminKeyLen, 'usage':'Optional'},
        }

        self.commJoinRejoinTlv = {
            'ExtPanId':{'length': self.extPanIdLen, 'usage':'Required'},
            'nwkChannel':{'length': self.nwkChannelLen, 'usage':'Optional'},
            'TCAddress':{'length': self.tcAddrLen, 'usage':'Optional'},
            'AdminKey':{'length': self.adminKeyLen, 'usage':'Optional'},
            'nwkKey':{'length': self.nwkKeyLen, 'usage':'Optional'},
            'linkKey':{'length': self.linkKeyLen, 'usage':'Optional'},
        }

        self.commJoinOutBandTlv = {
            'ExtPanId':{'length': self.extPanIdLen, 'usage':'Required'},
            'shortPanId':{'length': self.panIdLen, 'usage':'Required'},
            'nwkChannel':{'length': self.nwkChannelLen, 'usage':'Required'},
            'nwkKey':{'length': self.nwkKeyLen, 'usage':'Required'},
            'TCAddress':{'length': self.tcAddrLen, 'usage':'Optional'},
            'linkKey':{'length': self.linkKeyLen, 'usage':'Optional'},
            'nwkAddr':{'length': self.nwkAddrLen, 'usage':'Optional'},
            'nwkUpdataId':{'length': self.nwkUpIdLen, 'usage':'Optional'},
            'nwkKeyNum':{'length': self.nwkActiveKeySeqLen, 'usage':'Optional'},
            'AdminKey':{'length': self.adminKeyLen, 'usage':'Optional'},
        }


        

    
    