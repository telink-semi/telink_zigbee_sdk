class TunnelHandle:
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.tunnelServiceStr = '{8bd178fd-0000-45f4-8120-b2378bd5313f}'
        self.tunnelCharaStr = '{8bd178fd-0001-45f4-8120-b2378bd5313f}'

        self.zvdIeeeAddr = [0x01, 0x00, 0x00, 0x00, 0x00, 0xee, 0x1f, 0x00]
        # self.zvdIeeeAddr = [0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11]
        self.zvdNwkAddr = [0x34, 0x12]
        # self.zvdNwkAddr = [0xD2, 0x78]
        self.parentIeeeAddr = []
        self.parentNwkAddr = []
        self.sendShortAddr = []
        self.onoffCmdId = 0
        self.nwkSeqNum = 0
        self.apsCounter = 0
        self.zclSeqNum = 0
        self.zvdJoinState = False
        self.zvdCommRespStatus = False
        self.sendDevAnnce = False

        #zvd establish trust link
        # self.commissionReqData = [0x00, 0x34, 0x01, 0x33, 0x09, 0x18, 0x3C, 0x6F, 0x34, 0x12, 0x01, 0x29, 0x60, 0xdF,\
        #     0xF7, 0x74, 0xD2, 0x38, 0xc1, 0xa4, 0x01, 0x00, 0x00, 0x00, 0x00, 0xee, 0x1f, 0x00, 0x0E, 0x02, 0x8E, 0x48,\
        #     0x15, 0x41, 0x09, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xee, 0x1f, 0x00, 0x47, 0x03, 0x34, 0x12, 0x01, \
        #     0xCF, 0x4C, 0x01, 0x01, 0x00]

        # zvd init join
        self.commissionReqData = [0x00, 0x35, 0x00, 0x34, 0x09, 0x18, 0x3C, 0x6F, 0x34, 0x12, 0x01, 0x29, 0x60, 0xdF,\
            0xF7, 0x74, 0xD2, 0x38, 0xc1, 0xa4, 0x01, 0x00, 0x00, 0x00, 0x00, 0xee, 0x1f, 0x00, 0x0E, 0x00, 0x8E, 0x48,\
            0x16, 0x41, 0x09, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xee, 0x1f, 0x00, 0x47, 0x04, 0x34, 0x12, 0x01, \
            0xCF,0x00, 0x4C, 0x01, 0x01, 0x00]
        self.commReqZvdIeeeIdx = [20,37]
        self.commReqZvdshortIdx = [8,47]
        self.commReqParentIeeeIdx = [12]
        self.commReqParentshortIdx = [6]
        self.nwkSeqIdx = 11
        self.onoffApsCounterIdx = 19

    def zvd_tunnel_send_cb(self, sendDataCb):
        self.tunnelSendDataCb = sendDataCb

    def zvd_start_join_network(self):
        for idx in self.commReqZvdIeeeIdx:
            for i in range(8):
                self.commissionReqData[idx+i] = self.zvdIeeeAddr[i]

        for idx in self.commReqZvdshortIdx:
            for i in range(2):
                self.commissionReqData[idx+i] = self.zvdNwkAddr[i]

        for idx in self.commReqParentIeeeIdx:
            for i in range(8):
                self.commissionReqData[idx+i] = self.parentIeeeAddr[i]

        for idx in self.commReqParentshortIdx:
            for i in range(2):
                self.commissionReqData[idx+i] = self.parentNwkAddr[i]

        self.commissionReqData[self.nwkSeqIdx] = self.nwkSeqNum
        self.nwkSeqNum += 1

        if self.tunnelSendDataCb:
            self.tunnelSendDataCb(self.commissionReqData)
        return

    def recv_data_handle(self, recvData):
        # print("recvData,tunnel....")
        # print(recvData)
        if recvData[1] == 0x1d and recvData[4] == 0x09 and recvData[28] == 0x0f: #commissioning response
            if recvData[31] == 0:
                self.zvdCommRespStatus = True
                self.zvdNwkAddr[0] = recvData[29]
                self.zvdNwkAddr[1] = recvData[30]

                self.zvdJoinState = True
                if recvData[2] == 1: #rejoin
                    self.zvd_send_device_announce()
                return 1
            else:
                self.zvdCommRespStatus = False
                return 2

        if self.zvdCommRespStatus == True:
            if recvData[12] == 0x21 and recvData[14] == 0x38:  #key-transport-key
                self.zvdJoinState = True
                self.zvd_send_device_announce()
                return 3
        else:
            return 4

        return 0
            

    def send_onoff_cmd(self):
        onoffCmdData = [0x00, 0x14, 0x01, 0x13, 0x08, 0x00, 0xFC, 0xFF, 0x34, 0x12, 0x1E, 0x7E, 0x08, 0x01,\
            0x06, 0x00, 0x04, 0x01, 0x01, 0x73, 0x11, 0x35, 0x02]

        onoffCmdData[6] = self.sendShortAddr[0]
        onoffCmdData[7] = self.sendShortAddr[1]

        onoffCmdData[8] = self.zvdNwkAddr[0]
        onoffCmdData[9] = self.zvdNwkAddr[1]

        onoffCmdData[-1] = self.onoffCmdId

        onoffCmdData[self.nwkSeqIdx] = self.nwkSeqNum
        self.nwkSeqNum += 1
        
        onoffCmdData[self.onoffApsCounterIdx] = self.apsCounter
        self.apsCounter += 1

        if self.tunnelSendDataCb:
            self.tunnelSendDataCb(onoffCmdData)


    def zvd_send_link_status(self):
        linkStatusData = [0x00, 0x16, 0x01, 0x15, 0x09, 0x10, 0xFC, 0xFF, 0x34, 0x12, 0x01, 0x02, 0xFE, 0x48, 0x18, 0x49, \
            0xC5, 0x38, 0xC1, 0xA4, 0x08, 0x61, 0x00, 0x00, 0x11]
        lsLoNwkAddrIdx = 8
        lsLoIeeeAddrIdx = 12
        lsPaNwkAddrIdx = 22

        linkStatusData[lsLoNwkAddrIdx] = self.zvdNwkAddr[0]
        linkStatusData[lsLoNwkAddrIdx + 1] = self.zvdNwkAddr[1]

        for idx in range(lsLoIeeeAddrIdx, lsLoIeeeAddrIdx + 8):
            linkStatusData[idx] = self.zvdIeeeAddr[idx - lsLoIeeeAddrIdx]

        linkStatusData[lsPaNwkAddrIdx] = self.parentNwkAddr[0]
        linkStatusData[lsPaNwkAddrIdx + 1] = self.parentNwkAddr[1]

        linkStatusData[self.nwkSeqIdx] = self.nwkSeqNum
        self.nwkSeqNum += 1

        if self.tunnelSendDataCb:
            self.tunnelSendDataCb(linkStatusData)

    def zvd_send_manytoone_route_request(self):
        routerRequest = [0x00, 0x17, 0x01, 0x16, 0x09, 0x10, 0xFC, 0xFF, 0x00, 0x00, 0x1E, 0x02, 0xFE, 0x48, 0x18, 0x49, \
            0xC5, 0x38, 0xC1, 0xA4, 0x01, 0x08, 0x01, 0xFC, 0xFF, 0x00]
        
        lsLoIeeeAddrIdx = 12

        for idx in range(lsLoIeeeAddrIdx, lsLoIeeeAddrIdx + 8):
            routerRequest[idx] = self.zvdIeeeAddr[idx - lsLoIeeeAddrIdx]

        routerRequest[self.nwkSeqIdx] = self.nwkSeqNum
        self.nwkSeqNum += 1

        if self.tunnelSendDataCb:
            self.tunnelSendDataCb(routerRequest)


    # def zvd_leave_network(self):
    #     leaveCmdData = [0x00, 0x13, 0x01, 0x12, 0x09, 0x10, 0xFD, 0xFF, 0x34, 0x12, 0x01, 0x45, 0x01, 0x00, 0x00, 0x00, \
    #         0x00, 0xEE, 0x1F, 0x00, 0x04, 0x00]

    #     lnSrcNwkAddrIdx = 8
    #     lnSrcIeeeIdx = 12

    #     leaveCmdData[lnSrcNwkAddrIdx] = self.zvdNwkAddr[0]
    #     leaveCmdData[lnSrcNwkAddrIdx + 1] = self.zvdNwkAddr[1]

    #     for idx in range(lnSrcIeeeIdx, lnSrcIeeeIdx + 8):
    #         leaveCmdData[idx] = self.zvdIeeeAddr[idx - lnSrcIeeeIdx]

    #     leaveCmdData[self.nwkSeqIdx] = self.nwkSeqNum
    #     self.nwkSeqNum += 1

    #     if self.tunnelSendDataCb:
    #         self.tunnelSendDataCb(leaveCmdData)

    def zvd_leave_network(self):
        leaveCmdData = [0x00, 0x1B, 0x01, 0x1A, 0x48, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x1E, 0xF2, 0x08\
             , 0x00, 0x34, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x09, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x00]

        leaveIeeeIdx = 21

        # for idx in range(leaveIeeeIdx, leaveIeeeIdx + 8):
        #     leaveCmdData[idx] = self.zvdIeeeAddr[idx - leaveIeeeIdx]

        leaveCmdData[self.nwkSeqIdx] = self.nwkSeqNum
        self.nwkSeqNum += 1

        if self.tunnelSendDataCb:
            self.tunnelSendDataCb(leaveCmdData)

    def zvd_send_device_announce(self):
        devannceData = [0x00, 0x1d, 0x01, 0x1c, 0x48, 0x00, 0xfd, 0xff, 0x34, 0x12, 0x1e, 0x76, 0x08, 0x00, 0x13, \
            0x00, 0x00, 0x00, 0x00, 0x73, 0x00, 0x34, 0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0xee, 0x1f, 0x00, 0x8e]
        devZVDshortIdx = [8, 21]
        devZVDIeeeIdx = 23
        annceApsCounterIdx = 19

        for idx in devZVDshortIdx:
            for mn in range(2):
                devannceData[idx + mn] = self.zvdNwkAddr[mn]

        for mn in range(8):
            devannceData[devZVDIeeeIdx + mn] = self.zvdIeeeAddr[mn]

        devannceData[self.nwkSeqIdx] = self.nwkSeqNum
        self.nwkSeqNum += 1
        
        devannceData[annceApsCounterIdx] = self.apsCounter
        self.apsCounter += 1

        if self.tunnelSendDataCb:
            self.tunnelSendDataCb(devannceData)
                

    
    