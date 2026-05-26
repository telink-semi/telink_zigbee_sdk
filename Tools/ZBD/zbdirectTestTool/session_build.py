# from asyncio import exceptions
import struct
from ctypes import *
dll25519 = CDLL(".\\25519dll.dll")
dllP256 = cdll.LoadLibrary('.\\ecdhep256.dll')

negomethodE = {
    'Reserved': 0x00,
    'ECDHE_CURVE25519_HASH_AESMMO128': 0x01,
    'ECDHE_CURVE25519_HASH_SHA256': 0x02,
    'ECDHE_CURVEP256_HASH_SHA256': 0x03,
}

pre_secretId = {
    'Symmetric_Authentiation_Token': 0x00,
    'Preconfig_Linkkey_Derived_from_Installation_Code': 0x01,
    'Variable_Length_Pass_Code': 0x02,
    'Basic_Authorization_Key': 0x03,
    'Administrative_Authorization_Key': 0x04,
    'Anonymous_Well_Known_Secret': 0x05,
}

message_opcode = {
    'ZBD_SS_OPCODE_SEM1':0x01,
    'ZBD_SS_OPCODE_SEM2':0x02,
    'ZBD_SS_OPCODE_SEM3':0x03,
    'ZBD_SS_OPCODE_SEM4':0x04,
}

Zbd_tlvId = {
    'ZBD_SS_TLV_ID_KNR_SKNM': 0x00,  
    'ZBD_SS_TLV_ID_KNR_P256': 0x01,  
    'ZBD_SS_TLV_ID_KNR_X25519': 0x02,   
    'ZBD_SS_TLV_ID_NKSN': 0x03,   
    'ZBD_SS_TLV_ID_MACTAG': 0x04,   
}

def calc_crc16(input, len, output):
    # data = bytearray.fromhex(string)
    if len == 0:
        return 0

    crc = 0xFFFF
    for i in range(len):
        data = input[i]
        for i in range(8):
            if (crc & 1) ^ (data & 1)!= 0:
                crc >>= 1
                crc ^= 0x8408
            else:
                crc >>= 1
            data >>= 1
    crc = ~crc

    crc_0 = crc & 0xff
    crc_1 = (crc >> 8) & 0xff
    output.append(crc_0)
    output.append(crc_1)
    return 0 # 返回两部分十六进制字符

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

def UeiCompare(array1, array2, len):
    for m in reversed(range(len)):
        if array1[m] > array2[m]:
            return 1
        elif array1[m] == array2[m]:
            continue
        else:
            return -1
    return 0

def printHex(testshuzu):
    dhkTestB = struct.unpack("!%dB" % len(testshuzu), testshuzu)
    l = [hex(int(i)) for i in dhkTestB]
    print(" ".join(l))  

TEST_BASEPOINT_25519 = [0x90, 0x2b, 0x44, 0x85, 0xc8, 0x4e, 0xc4, 0xa0, 0x59, 0x44, 0xab, 0x34, 0x42, 0x92, 0x68, 0x78, 
                        0x90, 0x2b, 0x44, 0x85, 0xc8, 0x4e, 0xc4, 0xa0, 0x59, 0x44, 0xab, 0x34, 0x42, 0x92, 0x68, 0x78]
TEST_DI_25519 = [0xa0, 0xf1, 0xd9, 0x2a, 0x82, 0xc8, 0xd8, 0xfe, 0x43, 0x4d, 0x98, 0x55, 0x8c, 0xe2, 0xb3, 0x47,
                 0x17, 0x11, 0x98, 0x54, 0x2f, 0x11, 0x2d, 0x05, 0x58, 0xf5, 0x6b, 0xd6, 0x88, 0x07, 0x99, 0x52]

TEST_DI_P256 = [0x85, 0x5b, 0x95, 0xf0, 0xd4, 0x1f, 0xe3, 0x5d, 0x57, 0x64, 0x3e, 0x5c, 0x59, 0x7f, 0x17, 0x5a,
                0xac, 0x33, 0x2d, 0x75, 0x95, 0x71, 0x49, 0xce, 0x89, 0x26, 0x5a, 0x71, 0x90, 0xc3, 0x67, 0x7c]
TEST_QI_P256 = [0x37, 0x7f, 0x24, 0x60, 0x3f, 0x7f, 0x49, 0x34, 0xe3, 0xbc, 0x11, 0x3f, 0x58, 0xad, 0x45, 0x81,
                0x50, 0x78, 0xb3, 0x39, 0xa7, 0x87, 0x49, 0x12, 0xdd, 0xd9, 0xbf, 0x3d, 0x94, 0x35, 0x8a, 0x1d,
                0x2e, 0x03, 0x06, 0x08, 0x42, 0x81, 0xed, 0xbb, 0x98, 0x44, 0x69, 0xa2, 0x83, 0x64, 0xed, 0x0c,
                0xba, 0x6f, 0x47, 0x83, 0x4d, 0x18, 0xcb, 0x40, 0x80, 0x35, 0x28, 0xd9, 0x75, 0x4b, 0x1c, 0x42]
class SessBuild:
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.defaultPreKey = [0x5a, 0x69, 0x67, 0x42, 0x65, 0x65, 0x41, 0x6c, 0x6c, 0x69, 0x61, 0x6e, 0x63, 0x65, 0x31, 0x38] #zigbeealliance18
        self.keyNegoMethod = negomethodE['ECDHE_CURVE25519_HASH_AESMMO128']
        self.securityCharaStr1 = '{29144af4-0001-4481-bfe9-6d0299b429e3}'
        self.securityCharaStr2 = '{29144af4-0003-4481-bfe9-6d0299b429e3}'
        self.securityServiceStr = '{29144af4-0000-4481-bfe9-6d0299b429e3}'
        self.prePadU = [0x4b, 0x43, 0x5f, 0x32 ,0x5f, 0x55] #"KC_2_U"
        self.prePadV = [0x4b, 0x43, 0x5f, 0x32 ,0x5f, 0x56] #"KC_2_V"
        self.presharedSecretType = 0xff
        self.presharedSecret = []
        self.deriveSharedKey = []
        self.euiLocal = [0x01, 0x00, 0x00, 0x00, 0x00, 0xee, 0x1f, 0x00]
        self.euiRemote = [0] * 8
        self.preEuiRemote = []
        self.sessionKey = [0] * 16
        self.nwkSeqNum = 0
        self.nwkSeqNumRemote = 0
        self.nwkSeqNumSetFlag = False
        self.data_trans_cb = None
        self.sendData = []
        self.Qr = (c_char * 64)()
        self.sendDataLen = 0
        self.di = (c_char * 32)()
        self.Qi = (c_char * 64)()
        self.basepoint = (c_char * 32)()
        self.preSessionKey = (c_char * 16)()
        self.sessionFlag = False
        self.incomingFramecounter = 0
        self.outgoingFramecounter = 1
        self.securityLevel = 5
        self.sessionType = ''
        self.preSessionType = '(Provisioning session)'
        self.adminKeyPre = []
        self.wetherDerived = False
        self.testVector = False

    def ss_mmoHash_local(self, data, len, result):
        data_input = (c_char * len)()
        result_output = (c_char * 16)()
        for i in range(len):
            data_input[i] = data[i]
        dll25519.ss_mmoHash.argtypes = [POINTER(c_char),c_char,POINTER(c_char)]
        dll25519.ss_mmoHash(data_input, len, result_output)
        for i in range(16):
            result[i] = result_output[i]
        # print(result)

    def ss_hmac_aes_mmo_local(self, len, padV, input, hashOut):
        padV_input = (c_char * len)()
        input_input = (c_char * 16)()
        hashOut_out = (c_char * 16)()
        for i in range(len):
            padV_input[i] = padV[i]
        for i in range(16):
            input_input[i] = input[i]
        dll25519.ss_hmac_aes_mmo.argtypes = [c_char, POINTER(c_char),POINTER(c_char),POINTER(c_char)]
        dll25519.ss_hmac_aes_mmo(len, padV_input, input_input, hashOut_out)
        for i in range(16):
            hashOut[i] = hashOut_out[i]
        # print(hashOut)

    def memsetCchar(self, inputData, length):
        for i in range(length):
            inputData[i] = 0


    def start_session_init(self):
        self.memsetCchar(self.Qr, 64)
        self.memsetCchar(self.di, 32)
        self.memsetCchar(self.Qi, 64)
        self.memsetCchar(self.basepoint, 32)
        self.memsetCchar(self.preSessionKey, 16)
        self.sendDataLen = 0

        self.sendData.clear()
        self.preEuiRemote.clear()
        self.deriveSharedKey.clear()

    def session_start(self, data_cb):
        self.data_trans_cb = data_cb
        self.start_session_init()
        hashout_assign = False
        self.preSessionType = '(Provisioning session)'
        
        hashOut_out = (c_char * 16)()
        if self.presharedSecretType == pre_secretId['Basic_Authorization_Key'] or self.presharedSecretType == pre_secretId['Administrative_Authorization_Key']:
            if self.presharedSecretType == pre_secretId['Administrative_Authorization_Key']:
                self.preSessionType = '(Admin level authorization)'
                if len(self.adminKeyPre) != 0:
                    # print("len(self.adminKeyPre) != 0")
                    hashout_assign = True
                    for i in range(16):
                        hashOut_out[i] = self.adminKeyPre[i]
                else:
                    # print("len(self.presharedSecret) != 0")
                    # print(len(self.presharedSecret))
                    # print(self.wetherDerived)
                    # print(self.presharedSecret)
                    if len(self.presharedSecret) != 0 and self.wetherDerived == False:
                        hashout_assign = True
                        for i in range(16):
                            hashOut_out[i] = self.presharedSecret[i]
           
            if hashout_assign == False:
                if len(self.presharedSecret) == 0:
                    self.presharedSecret = self.defaultPreKey
                hash_in = (c_char * 24)()
                keyMmoHash = (c_char * 16)()
                for i in range(8):
                    hash_in[i] = self.euiLocal[i]
                for i in range(8, 24):
                    hash_in[i] = self.presharedSecret[i-8]

                dll25519.ss_mmoHash.argtypes = [POINTER(c_char),c_char,POINTER(c_char)]
                dll25519.ss_mmoHash(hash_in, 24, keyMmoHash)

                padV_input = (c_char * 1)()
                if self.presharedSecretType == pre_secretId['Basic_Authorization_Key']:    #Basic_Authorization_Key
                    padV_input[0] = pre_secretId['Basic_Authorization_Key'] 
                    self.preSessionType = '(Basic level authorization)'
                elif self.presharedSecretType == pre_secretId['Administrative_Authorization_Key']: #Administrative_Authorization_Key
                    padV_input[0] = pre_secretId['Administrative_Authorization_Key']
                    self.preSessionType = '(Admin level authorization)'                   

                dll25519.ss_hmac_aes_mmo.argtypes = [c_char, POINTER(c_char),POINTER(c_char),POINTER(c_char)]
                dll25519.ss_hmac_aes_mmo(1, padV_input, keyMmoHash, hashOut_out)
        elif self.presharedSecretType == pre_secretId['Preconfig_Linkkey_Derived_from_Installation_Code']: #Preconfig_Linkkey_Derived_from_Installation_Code
            if len(self.presharedSecret) == 0:
                self.presharedSecret = self.defaultPreKey
            hashIn = [0]*18
            crc16 = []
            for i in range(16):
                hashIn[i] = self.presharedSecret[i]
            calc_crc16(hashIn, 16, crc16)
            hashIn[16] = crc16[0]
            hashIn[17] = crc16[1]

            hash_in_2 = (c_char * 18)()
            for i in range(18):
                hash_in_2[i] = hashIn[i]
            dll25519.ss_mmoHash.argtypes = [POINTER(c_char),c_char,POINTER(c_char)]
            dll25519.ss_mmoHash(hash_in_2, 18, hashOut_out)
        else:
            if len(self.presharedSecret) == 0:
                self.presharedSecret = self.defaultPreKey
            for i in range(16):
                hashOut_out[i] = self.presharedSecret[i]

        for i in range(16):
            self.deriveSharedKey.append(hashOut_out[i])
        if self.keyNegoMethod == negomethodE['ECDHE_CURVEP256_HASH_SHA256']:
            dllP256.eccMakeKey.argtypes = [POINTER(c_char), POINTER(c_char)]
            dllP256.eccMakeKey(self.Qi, self.di)

            if self.testVector:
                self.presharedSecretType = 0xff
                for i in range(32):
                    self.di[i] = TEST_DI_P256[i]
                for i in range(64):
                    self.Qi[i] = TEST_QI_P256[i]
                for i in range(16):
                    self.deriveSharedKey[i] = self.defaultPreKey[i]
            self.sendDataLen = 79
        else:
            dll25519.ss_x25519_basePointGet.argtypes = [POINTER(c_char),POINTER(c_char)]
            dll25519.ss_x25519_basePointGet(hashOut_out, self.basepoint)

            dll25519.rand_get.argtypes = [POINTER(c_char), c_char]
            dll25519.rand_get(self.di, 32)

            if self.testVector:
                self.presharedSecretType = 0xff
                for i in range(32):
                    self.di[i] = TEST_DI_25519[i]
                for i in range(32):
                    self.basepoint[i] = TEST_BASEPOINT_25519[i]

            dll25519.curve25519_dh_CalculatePublicKey.argtypes = [POINTER(c_char), POINTER(c_char), POINTER(c_char)]
            dll25519.curve25519_dh_CalculatePublicKey(self.Qi, self.di, self.basepoint)

            self.sendDataLen = 47

        if self.nwkSeqNumSetFlag:
            self.sendDataLen += 3

        self.sendData.append(0x01)
        self.sendData.append(0x00)
        self.sendData.append(0x01)
        self.sendData.append(self.keyNegoMethod)
        self.sendData.append(self.presharedSecretType)

        diLen = 32
        if self.keyNegoMethod == negomethodE['ECDHE_CURVEP256_HASH_SHA256']:
            self.sendData.append(0x01)
            self.sendData.append(0x47)
            diLen = 64
        else:
            self.sendData.append(0x02)
            self.sendData.append(0x27)
        for i in range(8):
            self.sendData.append(self.euiLocal[i])

        QiData = struct.unpack("!%dB" % 64, self.Qi)
        for i in range(diLen):
            self.sendData.append(QiData[i])

        if self.nwkSeqNumSetFlag:
            self.sendData.append(0x03)
            self.sendData.append(0x00)
            self.sendData.append(self.nwkSeqNum)
            self.nwkSeqNumSetFlag = False

        self.data_trans_cb()

    def recv_data_handle(self, dataRecv):
        try:
            opCode = dataRecv[0]
            # print("opCode:%d" % opCode)
            if opCode == message_opcode['ZBD_SS_OPCODE_SEM2']: #ZBD_SS_OPCODE_SEM2
                dataIdx = 1
                Qr = []
                while dataIdx < len(dataRecv):
                    parseData = dataRecv[dataIdx: ]
                    ret, tlvStruct = tlv_process(parseData)
                    if ret:
                        dataIdx += tlvStruct['length'] + 2
                        if tlvStruct['tlvid'] == Zbd_tlvId['ZBD_SS_TLV_ID_KNR_SKNM']:
                            self.nwkSeqNumRemote = tlvStruct['data'][0]
                        elif tlvStruct['tlvid'] == Zbd_tlvId['ZBD_SS_TLV_ID_KNR_X25519']:
                            for i in range(8):
                                self.preEuiRemote.append(tlvStruct['data'][i])
                            for i in range(32):
                                Qr.append(tlvStruct['data'][i+8])
                        elif tlvStruct['tlvid'] == Zbd_tlvId['ZBD_SS_TLV_ID_KNR_P256']:
                            for i in range(8):
                                self.preEuiRemote.append(tlvStruct['data'][i])
                            for i in range(64):
                                Qr.append(tlvStruct['data'][i+8])
                    else:
                        break
                if Qr:
                    for i in range(len(Qr)):
                        self.Qr[i] = Qr[i]
                    xki = (c_char * 32)()
                    if self.keyNegoMethod == negomethodE['ECDHE_CURVEP256_HASH_SHA256']:#'ECDHE_CURVEP256_HASH_SHA256':
                        dllP256.p256_dh_eccSharedSecret.argtypes = [POINTER(c_char), POINTER(c_char), POINTER(c_char)]
                        dllP256.p256_dh_eccSharedSecret(self.Qr, self.di, xki)
                    else:
                        dll25519.curve25519_dh_CalculatePublicKey.argtypes = [POINTER(c_char), POINTER(c_char), POINTER(c_char)]
                        dll25519.curve25519_dh_CalculatePublicKey(xki, self.di, self.Qr)
                    localEui = (c_char * 8)()
                    for i in range(8):
                        localEui[i] = self.euiLocal[i]
                    remoteEui = (c_char * 8)()
                    for i in range(8):
                        remoteEui[i] = self.preEuiRemote[i]
                    prePadU = (c_char * 6)()
                    for i in range(6):
                        prePadU[i] = self.prePadU[i]
                    if self.keyNegoMethod == negomethodE['ECDHE_CURVEP256_HASH_SHA256']:#'ECDHE_CURVEP256_HASH_SHA256':
                        # hashPre = (c_char * 192)()
                        psk = (c_char * 16)()

                        for i in range(16):
                            psk[i] = self.deriveSharedKey[i]
                        # for i in range(len(xki)):
                        #     hashPre[i] = xki[i]

                        dllP256.p256_sessionKeyGet.argtypes = [POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char)]
                        if UeiCompare(self.euiLocal, self.preEuiRemote, 8) < 0:
                            dllP256.p256_sessionKeyGet(localEui, self.Qi, remoteEui, self.Qr, xki, psk, self.preSessionKey)
                        else:
                            dllP256.p256_sessionKeyGet(remoteEui, self.Qr, localEui, self.Qi, xki, psk, self.preSessionKey)

                        MacTagR = (c_char * 32)()
                        dllP256.p256_mactagGenerate.argtypes = [POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char)]
                        dllP256.p256_mactagGenerate(prePadU, localEui, remoteEui, self.Qi, self.Qr, self.preSessionKey, MacTagR)
                    else:
                        dll25519.ss_x25519_sessionKeyGet.argtypes = [POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char)]
                        if UeiCompare(self.euiLocal, self.preEuiRemote, 8) < 0:
                            # print("euilocal small...")
                            dll25519.ss_x25519_sessionKeyGet(localEui, self.Qi, remoteEui, self.Qr, xki, self.basepoint, self.preSessionKey)
                        else:
                            # print("preEuiRemote small...")
                            dll25519.ss_x25519_sessionKeyGet(remoteEui, self.Qr, localEui, self.Qi, xki, self.basepoint, self.preSessionKey)
                    
                        MacTagR = (c_char * 16)()
                        dll25519.zbd_ss_macTagGenerate.argtypes = [POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char)]
                        dll25519.zbd_ss_macTagGenerate(prePadU, localEui, remoteEui, self.Qi, self.Qr, self.preSessionKey, MacTagR)

                    self.sendData = []
                    self.sendData.append(message_opcode['ZBD_SS_OPCODE_SEM3'])
                    self.sendData.append(Zbd_tlvId['ZBD_SS_TLV_ID_MACTAG'])
                    if self.keyNegoMethod == negomethodE['ECDHE_CURVEP256_HASH_SHA256']:#'ECDHE_CURVEP256_HASH_SHA256':
                        self.sendData.append(0x1f) #length
                    else:
                        self.sendData.append(0x0f) #length
                    peData = struct.unpack("!%dB" % len(MacTagR), MacTagR)
                    for i in range(len(MacTagR)):
                        self.sendData.append(peData[i])

                    self.data_trans_cb()
                else:
                    return
            elif opCode == message_opcode['ZBD_SS_OPCODE_SEM4']:
                dataIdx = 1
                while dataIdx < len(dataRecv):
                    parseData = dataRecv[dataIdx: ]
                    ret, tlvStruct = tlv_process(parseData)
                    dataIdx += tlvStruct['length'] + 2

                if ret:
                    if tlvStruct['tlvid'] == Zbd_tlvId['ZBD_SS_TLV_ID_MACTAG']:
                        rmtMacTag = []
                        for i in range(tlvStruct['length']):
                            rmtMacTag.append(tlvStruct['data'][i])
                        prePadV = (c_char * 6)()
                        for i in range(6):
                            prePadV[i] = self.prePadV[i]
                        localEui = (c_char * 8)()
                        for i in range(8):
                            localEui[i] = self.euiLocal[i]

                        remoteEui = (c_char * 8)()
                        for i in range(8):
                            remoteEui[i] = self.preEuiRemote[i]

                        if self.keyNegoMethod == negomethodE['ECDHE_CURVEP256_HASH_SHA256']:#'ECDHE_CURVEP256_HASH_SHA256':
                            MacTagIC = (c_char * 32)()
                            dllP256.p256_mactagGenerate.argtypes = [POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char)]
                            dllP256.p256_mactagGenerate(prePadV, localEui, remoteEui, self.Qi, self.Qr, self.preSessionKey, MacTagIC)
                        else:
                            MacTagIC = (c_char * 16)()
                            dll25519.zbd_ss_macTagGenerate.argtypes = [POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char), POINTER(c_char)]
                            dll25519.zbd_ss_macTagGenerate(prePadV, localEui, remoteEui, self.Qi, self.Qr, self.preSessionKey, MacTagIC)
                    
                        MacTagI = struct.unpack("!%dB" % len(rmtMacTag), MacTagIC)
                        # print(MacTagI)
                        # print(rmtMacTag)

                        if UeiCompare(MacTagI, rmtMacTag, len(MacTagIC)) == 0:
                            self.sessionFlag = True
                            self.incomingFramecounter = 0
                            self.outgoingFramecounter = 1
                            self.sessionType = self.preSessionType
                            peData = struct.unpack("!%dB" % 16, self.preSessionKey)
                            for i in range(16):
                                self.sessionKey[i] = peData[i]

                            for i in range(8):
                                self.euiRemote[i] = self.preEuiRemote[i]
                        else:
                            self.sessionFlag = False    
                    else:
                        self.sessionFlag = False   
                        return
                else:
                    self.sessionFlag = False   
        except exceptions as e:
            self.sessionFlag = False
            # print(e)

    def encrypt_data(self, servUuid, charaUuid, inputData):
        enData = []
        nonce = (c_char * 13)()

        for i in range(8):
            nonce[i] = self.euiLocal[i]
        nonce[8] = self.outgoingFramecounter & 0xff
        nonce[9] = (self.outgoingFramecounter >> 8) & 0xff
        nonce[10] = (self.outgoingFramecounter >> 16) & 0xff
        nonce[11] = (self.outgoingFramecounter >> 24) & 0xff
        nonce[12] = self.securityLevel

        authData = (c_char * 34)()
        authData[16] = 0    #service Instance
        authData[33] = 0    #characteristic Instance
        for i in range(16):
            authData[i] = servUuid[i]
            authData[17 + i] = charaUuid[i]

        sessionKey = (c_char * 16)()
        for i in range(16):
            sessionKey[i] = self.sessionKey[i]

        # print('sessionKey:')
        # print(self.sessionKey)

        claMic = (c_char * 4)()
        plainData = (c_char * len(inputData))()
        for i in range(len(inputData)):
            plainData[i] = inputData[i]
        dll25519.ss_ccmEncryption.argtypes = [POINTER(c_char), POINTER(c_char), c_char, POINTER(c_char), c_char, POINTER(c_char), POINTER(c_char)]
        ret = dll25519.ss_ccmEncryption(sessionKey, nonce, 34, authData, len(plainData), plainData, claMic)
        if ret == 0:
            return 1, enData #encrypt fail
        
        enData.append(self.outgoingFramecounter & 0xff)
        enData.append((self.outgoingFramecounter >> 8) & 0xff)
        enData.append((self.outgoingFramecounter >> 16) & 0xff)
        enData.append((self.outgoingFramecounter >> 24) & 0xff)

        self.outgoingFramecounter += 1
        converData = struct.unpack("!%dB" % len(plainData), plainData)
        for i in range(len(plainData)):
            enData.append(converData[i])
        
        converData = struct.unpack("!%dB" % len(claMic), claMic)
        for i in range(4):
            enData.append(converData[i])
        
        return 0, enData

    def decrypt_data(self, servUuid, charaUuid, inputData):
        plainData = []

        if len(inputData) < 8:
            return 3, plainData
        nonce = (c_char * 13)()
        for i in range(8):
            nonce[i] = self.euiRemote[i]
        incomCounter = inputData[0] + inputData[1] * 256 + inputData[2] * 256 * 256 + inputData[3] * 256 * 256 * 256
        # print("incomCounter:0x%x"%incomCounter)
        # print("incomingFramecounter:0x%x"%self.incomingFramecounter)
        if incomCounter <= self.incomingFramecounter:
            return 2, plainData        #framecounter verify error
        else:
            nonce[8] = inputData[0]
            nonce[9] = inputData[1]
            nonce[10] = inputData[2]
            nonce[11] = inputData[3]
        
        nonce[12] = self.securityLevel

        authData = (c_char * 34)()
        authData[16] = 0    #service Instance
        authData[33] = 0    #characteristic Instance
        for i in range(16):
            authData[i] = servUuid[i]
            authData[17 + i] = charaUuid[i]

        sessionKey = (c_char * 16)()
        for i in range(16):
            sessionKey[i] = self.sessionKey[i]

        claMic = (c_char * 4)()
        for i in range(4, 0, -1):
            claMic[4-i] = inputData[-i]
        encryData = (c_char * (len(inputData)-8))()
        for i in range(len(inputData)-8):
            encryData[i] = inputData[4 + i]

        dll25519.ss_ccmDecryption.argtypes = [POINTER(c_char), POINTER(c_char), c_char, POINTER(c_char), c_char, POINTER(c_char), POINTER(c_char)]
        ret = dll25519.ss_ccmDecryption(sessionKey, nonce, 34, authData, len(encryData), encryData, claMic)
        if ret != 0:
            return 1, plainData

        self.incomingFramecounter = incomCounter
        
        converData = struct.unpack("!%dB" % len(encryData), encryData)
        for i in range(len(encryData)):
            plainData.append(converData[i])
        
        return 0, plainData              

        

     



            
            




        

