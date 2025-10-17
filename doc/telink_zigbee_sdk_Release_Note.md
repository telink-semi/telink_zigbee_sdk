## V3.7.2.0(PR)

### Version
* Chips
  - TC32: TLSR8269/TLSR8258/TLSR8278
  - RISCV: TLSR921x(B91)/TLSR952x(B92)/TL721x A3/TL321x A3
* Hardware Version
  - TLSR8258: C1T139A30_V1_2/C1T139A3_V2.0A
  - TLSR921x(B91): C1T213A20_V1_3/C1T213A3_V1_1
  - TL721x: C1T315A20_V1_2/C1T315A3_V1_1
  - TL321x: C1T335A20_V1_3/C1T335A3_V1_0
* Driver Version
  - tl_platform_sdk V3.8.0
* Toolchain Version
  - TLSR8269/TLSR8258/TLSR8278: TC32 ELF GCC4.3
  - TLSR921x(B91): TL32 ELF MCULIB V5F GCC7.4
  - TLSR952x(B92)/TL721x: TL32 ELF MCULIB V5F GCC12.2
  - TL721x: TL32 ELF MCULIB V5F GCC12.2
  - TL321x: TL32 ELF MCULIB V5 GCC12.2
* IDE
  - [Telink IoT Studio](https://www.telink-semi.com/development-tools)
### Features
* Update driver to V3.8.0.
* Provide registration callback for network status code indication.
* Update demo to enhance its robustness.
* A barbaric method to solve the decryption failure problem caused by Frame Counter overflow.
### Bug Fixes
* Fix the issue of APS encryption flag assignment in ZCL messages.
* Fix the issue of losing parent for a ZED with RX_On_When_Idle is 1.
### Performance
* Update the reporting processing mechanism to reduce power consumption.
### Code Refactoring
* ZCL attributes reporting.
### BREAKING CHANGES
* N/A
### CodeSize
* Units: KBytes

|   chip   | sampleGW (RAM/FLASH Size) | sampleLight (RAM/FLASH Size) | sampleSwitch (RAM/FLASH Size) |
| :------- | :-----------------------: | :--------------------------: | :---------------------------: |
| TLSR8258 | 35 / 178                  | 34 / 197                     | 24 / 141                      |
| TLSR8278 |  - / -                    |  - / -                       |  - / -                        |
| TLSR921x | 38 / 166                  | 36 / 185                     | 26 / 128                      |
| TLSR952x |  - / -                    |  - / -                       |  - / -                        |
| TL721x   |  - / -                    |  - / -                       |  - / -                        |
| TL321x   | 38 / 178                  | 36 / 199                     | 28 / 142                      |

### Version
* 芯片
  - TC32: TLSR8269/TLSR8258/TLSR8278
  - RISCV: TLSR921x(B91)/TLSR952x(B92)/TL721x A3/TL321x A3
* 硬件版本
  - TLSR8258: C1T139A30_V1_2/C1T139A3_V2.0A
  - TLSR921x(B91): C1T213A20_V1_3/C1T213A3_V1_1
  - TL721x: C1T315A20_V1_2/C1T315A3_V1_1
  - TL321x: C1T335A20_V1_3/C1T335A3_V1_0
* 驱动版本
  - tl_platform_sdk V3.8.0
* 工具链版本
  - TLSR8269/TLSR8258/TLSR8278: TC32 ELF GCC4.3
  - TLSR921x(B91): TL32 ELF MCULIB V5F GCC7.4
  - TLSR952x(B92)/TL721x: TL32 ELF MCULIB V5F GCC12.2
  - TL721x: TL32 ELF MCULIB V5F GCC12.2
  - TL321x: TL32 ELF MCULIB V5 GCC12.2
* IDE
  - [Telink IoT Studio](https://www.telink-semi.com/development-tools)
### Features
* 更新驱动到V3.8.0版本。
* 提供网络状态码指示的注册回调方法。
* 更新demo以增强其健壮性。
* 提供一种野蛮的方法来解决由帧计数器溢出引起的解密失败问题。
### Bug Fixes
* 修复ZCL消息中APS加密标志赋值的问题。
* 修复RX_On_When_ID为1的ZED丢失父节点的问题。
### Performance
* 更新报告处理机制以节省功耗。
### Code Refactoring
* ZCL属性上报.
### BREAKING CHANGES
* N/A
### CodeSize
* 单位: KBytes

|   chip   | sampleGW (RAM/FLASH Size) | sampleLight (RAM/FLASH Size) | sampleSwitch (RAM/FLASH Size) |
| :------- | :-----------------------: | :--------------------------: | :---------------------------: |
| TLSR8258 | 35 / 178                  | 34 / 197                     | 24 / 141                      |
| TLSR8278 |  - / -                    |  - / -                       |  - / -                        |
| TLSR921x | 38 / 166                  | 36 / 185                     | 26 / 128                      |
| TLSR952x |  - / -                    |  - / -                       |  - / -                        |
| TL721x   |  - / -                    |  - / -                       |  - / -                        |
| TL321x   | 38 / 178                  | 36 / 199                     | 28 / 142                      |


## V3.7.1.4(PR)

### Version
* Chips
  - TC32: TLSR8269/TLSR8258/TLSR8278
  - RISCV: TLSR921x(B91)/TLSR952x(B92)/TL721x A2/TL321x A2
* Hardware Version
  - TLSR8258: C1T139A30_V1_2/C1T139A3_V2.0A
  - TLSR921x(B91): C1T213A20_V1_3/C1T213A3_V1_1
  - TL321x: C1T335A20_V1_3/C1T335A3_V1_0
* Driver Version
  - tl_platform_sdk V3.5.0
* Toolchain Version
  - TLSR8269/TLSR8258/TLSR8278: TC32 ELF GCC4.3
  - TLSR921x(B91): TL32 ELF MCULIB V5F GCC7.4
  - TLSR952x(B92)/TL721x: TL32 ELF MCULIB V5F GCC12.2
  - TL321x: TL32 ELF MCULIB V5 GCC12.2
### Features
* N/A
### Bug Fixes
* Add interrupt protection to the sending queue at the MAC layer.
* Fix the Alarm cluster and IAS Zone cluster handlers.
* Update the handling of the default response of the ota upgrade end request command.
* Fix the issue of removing all groups.
### Performance
* N/A
### Code Refactoring
* Code style
### BREAKING CHANGES
* N/A
### Flash
|   chip   |                  flash                  |
| :------- | :-------------------------------------: |
| TLSR8258 | GD25LD40C/GD25LD40E/GD25LD80C/GD25LD80E |
| TLSR8278 |                    /                    |
| TLSR921x |        P25Q80U/P25Q80SU/P25Q16SU        |
| TLSR952x |                    /                    |
| TL721x   |                    /                    |
| TL321x   |        P25Q80SU/P25Q16SU/P25Q32SU       | 
### CodeSize
* Units: KBytes

|   chip   | sampleGW (RAM/FLASH Size) | sampleLight (RAM/FLASH Size) | sampleSwitch (RAM/FLASH Size) |
| :------- | :-----------------------: | :--------------------------: | :---------------------------: |
| TLSR8258 | 34 / 176                  | 32 / 196                     | 22 / 138                      |
| TLSR8278 |  - / -                    |  - / -                       |  - / -                        |
| TLSR921x | 36 / 161                  | 34 / 180                     | 24 / 122                      |
| TLSR952x |  - / -                    |  - / -                       |  - / -                        |
| TL721x   |  - / -                    |  - / -                       |  - / -                        |
| TL321x   | 37 / 176                  | 35 / 198                     | 28 / 140                      |

### Version
* 芯片
  - TC32: TLSR8269/TLSR8258/TLSR8278
  - RISCV: TLSR921x(B91)/TLSR952x(B92)/TL721x A2/TL321x A2
* 硬件版本
  - TLSR8258: C1T139A30_V1_2/C1T139A3_V2.0A
  - TLSR921x(B91): C1T213A20_V1_3/C1T213A3_V1_1
  - TL321x: C1T335A20_V1_3/C1T335A3_V1_0
* 驱动版本
  - tl_platform_sdk V3.5.0
* 工具链版本
  - TLSR8269/TLSR8258/TLSR8278: TC32 ELF GCC4.3
  - TLSR921x(B91): TL32 ELF MCULIB V5F GCC7.4
  - TLSR952x(B92)/TL721x: TL32 ELF MCULIB V5F GCC12.2
  - TL321x: TL32 ELF MCULIB V5 GCC12.2
### Features
* N/A
### Bug Fixes
* 在MAC层发送队列添加中断保护。
* 修复Alarm和IAS Zone集群的处理程序。
* 更新OTA结束请求命令的默认应答处理程序。
* 修复删除所有组的命令处理。
### Performance
* N/A
### Code Refactoring
* 代码风格
### BREAKING CHANGES
* N/A
### Flash
|   chip   |                  flash                  |
| :------- | :-------------------------------------: |
| TLSR8258 | GD25LD40C/GD25LD40E/GD25LD80C/GD25LD80E |
| TLSR8278 |                    /                    |
| TLSR921x |        P25Q80U/P25Q80SU/P25Q16SU        |
| TLSR952x |                    /                    |
| TL721x   |                    /                    |
| TL321x   |        P25Q80SU/P25Q16SU/P25Q32SU       | 
### CodeSize
* 单位: KBytes

|   chip   | sampleGW (RAM/FLASH Size) | sampleLight (RAM/FLASH Size) | sampleSwitch (RAM/FLASH Size) |
| :------- | :-----------------------: | :--------------------------: | :---------------------------: |
| TLSR8258 | 34 / 176                  | 32 / 196                     | 22 / 138                      |
| TLSR8278 |  - / -                    |  - / -                       |  - / -                        |
| TLSR921x | 36 / 161                  | 34 / 180                     | 24 / 122                      |
| TLSR952x |  - / -                    |  - / -                       |  - / -                        |
| TL721x   |  - / -                    |  - / -                       |  - / -                        |
| TL321x   | 37 / 176                  | 35 / 198                     | 28 / 140                      |


## V3.7.1.3(ER)

### Version
* Chips
  - TC32: TLSR8269/TLSR8258/TLSR8278
  - RISCV: TLSR921x(B91)/TLSR952x(B92)/TL721x A2/TL321x A2
* Hardware Version
  - TLSR8258: C1T139A30_V1_2/C1T139A3_V2.0A
  - TLSR921x(B91): C1T213A20_V1_3/C1T213A3_V1_1
  - TL321x: C1T335A20_V1_3/C1T335A3_V1_0
* Driver Version
  - SHA-1: fe2317eed5857c1afe6dc3f5006878be4ceca30a
* Toolchain Version
  - TLSR8269/TLSR8258/TLSR8278: TC32 ELF GCC4.3
  - TLSR921x(B91): TL32 ELF MCULIB V5F GCC7.4
  - TLSR952x(B92)/TL721x: TL32 ELF MCULIB V5F GCC12.2
  - TL321x: TL32 ELF MCULIB V5 GCC12.2
* IDE Version
  - Telink IoT Studio: [TelinkIoTStudio_V2024.8](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IoTStudio/TelinkIoTStudio_V2024.8.zip)
### Features
* Update the TL321x platform driver for version A2.
* Update MAC Address generation rules.
* Add Uart console to print logs.
### Bug Fixes
* Fix the Reset To Factory New command processing of Touch Link Commissioning Cluster.
* Fix the static declaration issue of zcl_electricalMeasure_cmdHandler().
### Performance
* N/A
### Code Refactoring
* Code style
### BREAKING CHANGES
* N/A
### Flash
|   chip   |                  flash                  |
| :------- | :-------------------------------------: |
| TLSR8258 | GD25LD40C/GD25LD40E/GD25LD80C/GD25LD80E |
| TLSR8278 |                    /                    |
| TLSR921x |        P25Q80U/P25Q80SU/P25Q16SU        |
| TLSR952x |                    /                    |
| TL721x   |                    /                    |
| TL321x   |        P25Q80SU/P25Q16SU/P25Q32SU       | 
### CodeSize
* Units: KBytes

|   chip   | sampleGW (RAM/FLASH Size) | sampleLight (RAM/FLASH Size) | sampleSwitch (RAM/FLASH Size) |
| :------- | :-----------------------: | :--------------------------: | :---------------------------: |
| TLSR8258 | 34 / 176                  | 32 / 196                     | 22 / 138                      |
| TLSR8278 | 34 / 171                  | 32 / 193                     | 22 / 133                      |
| TLSR921x | 36 / 161                  | 34 / 180                     | 24 / 122                      |
| TLSR952x | 36 / 170                  | 34 / 190                     | 27 / 134                      |
| TL721x   | 40 / 172                  | 37 / 193                     | 31 / 135                      |
| TL321x   | 37 / 170                  | 34 / 191                     | 28 / 133                      |

### Version
* 芯片
  - TC32: TLSR8269/TLSR8258/TLSR8278
  - RISCV: TLSR921x(B91)/TLSR952x(B92)/TL721x A2/TL321x A2
* 硬件版本
  - TLSR8258: C1T139A30_V1_2/C1T139A3_V2.0A
  - TLSR921x(B91): C1T213A20_V1_3/C1T213A3_V1_1
  - TL321x: C1T335A20_V1_3/C1T335A3_V1_0
* 驱动版本
  - SHA-1: fe2317eed5857c1afe6dc3f5006878be4ceca30a
* 工具链版本
  - TLSR8269/TLSR8258/TLSR8278: TC32 ELF GCC4.3
  - TLSR921x(B91): TL32 ELF MCULIB V5F GCC7.4
  - TLSR952x(B92)/TL721x: TL32 ELF MCULIB V5F GCC12.2
  - TL321x: TL32 ELF MCULIB V5 GCC12.2
* IDE版本
  - Telink IoT Studio: [TelinkIoTStudio_V2024.8](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IoTStudio/TelinkIoTStudio_V2024.8.zip)
### Features
* 更新A2版本的TL321x平台驱动。
* 更新MAC地址生成规则。
* 添加串口打印输出日志。
### Bug Fixes
* 修复Touch Link Cluster中Reset To Factory New命令的处理。
* 修复zcl_electricalMeasure_cmdHandler()函数静态声明的问题。
### Performance
* N/A
### Code Refactoring
* 代码风格
### BREAKING CHANGES
* N/A
### Flash
|   chip   |                  flash                  |
| :------- | :-------------------------------------: |
| TLSR8258 | GD25LD40C/GD25LD40E/GD25LD80C/GD25LD80E |
| TLSR8278 |                    /                    |
| TLSR921x |        P25Q80U/P25Q80SU/P25Q16SU        |
| TLSR952x |                    /                    |
| TL721x   |                    /                    |
| TL321x   |        P25Q80SU/P25Q16SU/P25Q32SU       | 
### CodeSize
* 单位: KBytes

|   chip   | sampleGW (RAM/FLASH Size) | sampleLight (RAM/FLASH Size) | sampleSwitch (RAM/FLASH Size) |
| :------- | :-----------------------: | :--------------------------: | :---------------------------: |
| TLSR8258 | 34 / 176                  | 32 / 196                     | 22 / 138                      |
| TLSR8278 | 34 / 171                  | 32 / 193                     | 22 / 133                      |
| TLSR921x | 36 / 161                  | 34 / 180                     | 24 / 122                      |
| TLSR952x | 36 / 170                  | 34 / 190                     | 27 / 134                      |
| TL721x   | 40 / 172                  | 37 / 193                     | 31 / 135                      |
| TL321x   | 37 / 170                  | 34 / 191                     | 28 / 133                      |


## V3.7.1.2(PR)

### Version
* Chips
  - TC32: TLSR8269/TLSR8258/TLSR8278
  - RISCV: TLSR921x(B91)/TLSR952x(B92)/TL721x A1/TL321x A1
* Hardware Version
  - TLSR8258: C1T139A30_V1_2/C1T139A3_V2.0A
  - TLSR921x(B91): C1T213A20_V1_3/C1T213A3_V1_1
  - TL321x: C1T335A20_V1_0/C1T335A3_V1_0
* Driver Version
  - tl_platform_sdk V3.2.0
* Toolchain Version
  - TLSR8269/TLSR8258/TLSR8278: TC32 ELF GCC4.3
  - TLSR921x(B91): TL32 ELF MCULIB V5F GCC7.4
  - TLSR952x(B92)/TL721x: TL32 ELF MCULIB V5F GCC12.2
  - TL321x: TL32 ELF MCULIB V5 GCC12.2
* IDE Version
  - Telink IoT Studio: [TelinkIoTStudio_V2024.8](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IoTStudio/TelinkIoTStudio_V2024.8.zip)
### Features
* Update sampleLight demo.
* Update some features of ZCL, such as Level and Color Control.
* Global ClusterRevision attribute added for Touch Link Cluster.
### Bug Fixes
* Fix the DeviceEnabled attribute handling of ZCL Basic Cluster.
* Fix the exception caused by report value checks.
* Fix the return pointer of memcpy() and memset().
* Update libraries to fix the issue of searching for end devices when the broadcast method is 0xFFFF.
* Update libraries to fix the length issue of APS encrypted data.
  - The affected SDK versions are from v3.6.8.4 to v3.7.1.1.
  - In addition, users also can fix this issue by using the tl_apsDataIndRegister() function as follows. 
  This patch is only available for SDK versions from v3.6.8.4 to v3.7.1.1.
	~~~
	void app_apsDataIndLengthFix(void *arg){
		aps_data_ind_t *ind = (aps_data_ind_t *)arg;
		if(ind->security_status & SECURITY_IN_APSLAYER){
			ind->asduLength += 4;
		}
	}

	tl_apsDataIndRegister(app_apsDataIndLengthFix);
	~~~
### Performance
* Update UART reception processing to improve fault tolerance.
### Code Refactoring
* Refactoring the ZCL report handler and supporting simultaneous reporting of multiple attributes.
### BREAKING CHANGES
* N/A
### Flash
|   chip   |                  flash                  |
| :------- | :-------------------------------------: |
| TLSR8258 | GD25LD40C/GD25LD40E/GD25LD80C/GD25LD80E |
| TLSR8278 |                    /                    |
| TLSR921x |        P25Q80U/P25Q80SU/P25Q16SU        |
| TLSR952x |                    /                    |
| TL721x   |                    /                    |
| TL321x   |        P25Q80SU/P25Q16SU/P25Q32SU       | 
### CodeSize
* Units: KBytes

|   chip   | sampleGW (RAM/FLASH Size) | sampleLight (RAM/FLASH Size) | sampleSwitch (RAM/FLASH Size) |
| :------- | :-----------------------: | :--------------------------: | :---------------------------: |
| TLSR8258 | 34 / 175                  | 32 / 196                     | 22 / 137                      |
| TLSR8278 | 34 / 171                  | 32 / 193                     | 22 / 133                      |
| TLSR921x | 36 / 161                  | 34 / 180                     | 25 / 122                      |
| TLSR952x | 36 / 170                  | 34 / 190                     | 27 / 133                      |
| TL721x   | 37 / 169                  | 35 / 190                     | 28 / 132                      |
| TL321x   | 36 / 169                  | 34 / 190                     | 27 / 131                      |

### Version
* 芯片
  - TC32: TLSR8269/TLSR8258/TLSR8278
  - RISCV: TLSR921x(B91)/TLSR952x(B92)/TL721x A1/TL321x A1
* 硬件版本
  - TLSR8258: C1T139A30_V1_2/C1T139A3_V2.0A
  - TLSR921x(B91): C1T213A20_V1_3/C1T213A3_V1_1
  - TL321x: C1T335A20_V1_0/C1T335A3_V1_0
* 驱动版本
  - tl_platform_sdk V3.2.0
* 工具链版本
  - TLSR8269/TLSR8258/TLSR8278: TC32 ELF GCC4.3
  - TLSR921x(B91): TL32 ELF MCULIB V5F GCC7.4
  - TLSR952x(B92)/TL721x: TL32 ELF MCULIB V5F GCC12.2
  - TL321x: TL32 ELF MCULIB V5 GCC12.2
* IDE版本
  - Telink IoT Studio: [TelinkIoTStudio_V2024.8](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IoTStudio/TelinkIoTStudio_V2024.8.zip)
### Features
* 更新sampleLight示例。
* 更新ZCL的一些功能，如亮度和颜色控制。
* Touch Link Cluster添加全局ClusterRevision属性。
### Bug Fixes
* 修复ZCL Basic Cluster中DeviceEnabled属性的处理。
* 修复上报值检查引起的异常。
* 修复memcpy()和memset()的返回指针。
* 更新库以修复在广播方法为0xFFFF时搜索终端设备的问题。
* 更新库以修复APS加密数据的长度问题。
  - 受影响的SDK版本为v3.6.8.4至v3.7.1.1.
  - 除此之外，用户还可以使用tl_apsDataIndRegister()来修复此问题，参考如下。
  该补丁仅限于版本为v3.6.8.4至v3.7.1.1的SDK。
  	~~~
	void app_apsDataIndLengthFix(void *arg){
		aps_data_ind_t *ind = (aps_data_ind_t *)arg;
		if(ind->security_status & SECURITY_IN_APSLAYER){
			ind->asduLength += 4;
		}
	}

	tl_apsDataIndRegister(app_apsDataIndLengthFix);
	~~~
### Performance
* 更新UART接收处理提高容错能力。
### Code Refactoring
* 重构ZCL报告处理程序，支持同时报告多个属性。
### BREAKING CHANGES
* N/A
### Flash
|   chip   |                  flash                  |
| :------- | :-------------------------------------: |
| TLSR8258 | GD25LD40C/GD25LD40E/GD25LD80C/GD25LD80E |
| TLSR8278 |                    /                    |
| TLSR921x |        P25Q80U/P25Q80SU/P25Q16SU        |
| TLSR952x |                    /                    |
| TL721x   |                    /                    |
| TL321x   |        P25Q80SU/P25Q16SU/P25Q32SU       | 
### CodeSize
* 单位: KBytes

|   chip   | sampleGW (RAM/FLASH Size) | sampleLight (RAM/FLASH Size) | sampleSwitch (RAM/FLASH Size) |
| :------- | :-----------------------: | :--------------------------: | :---------------------------: |
| TLSR8258 | 34 / 175                  | 32 / 196                     | 22 / 137                      |
| TLSR8278 | 34 / 171                  | 32 / 193                     | 22 / 133                      |
| TLSR921x | 36 / 161                  | 34 / 180                     | 25 / 122                      |
| TLSR952x | 36 / 170                  | 34 / 190                     | 27 / 133                      |
| TL721x   | 37 / 169                  | 35 / 190                     | 28 / 132                      |
| TL321x   | 36 / 169                  | 34 / 190                     | 27 / 131                      |


## V3.7.1.1(PR)

### Version
* Chips
  - TC32: TLSR8269/TLSR8258/TLSR8278
  - RISCV: TLSR921x(B91)/TLSR952x(B92)/TL721x A1/TL321x A1
* Hardware Version
  - TLSR8258: C1T139A30_V1_2/C1T139A3_V2.0A
  - TLSR921x(B91): C1T213A20_V1_3/C1T213A3_V1_1
  - TL321x: C1T335A20_V1_0/C1T335A3_V1_0
* Driver Version
  - tl_platform_sdk V3.2.0
* Toolchain Version
  - TLSR8269/TLSR8258/TLSR8278: TC32 ELF GCC4.3
  - TLSR921x(B91): TL32 ELF MCULIB V5F GCC7.4
  - TLSR952x(B92)/TL721x: TL32 ELF MCULIB V5F GCC12.2
  - TL321x: TL32 ELF MCULIB V5 GCC12.2
* IDE Version
  - Telink IoT Studio: [TelinkIoTStudio_V2024.8](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IoTStudio/TelinkIoTStudio_V2024.8.zip)
### Features
* Press the button to rejoin if ZED is offline.
* Add flash drivers for TLSR8278.
### Bug Fixes
* Fix the address range check of the ev_buffer.
* Fix the issue of setting RF TX Power through HCI.
* Fix the issue of the configure reporting command handler.
* Fix RF DMA channel setup issue on TL321x and TL721x.
### Performance
* Update libraries and provide workaround for RSSI Register abnormal.
### Code Refactoring
* N/A
### BREAKING CHANGES
* N/A
### Flash
|   chip   |                  flash                  |
| :------- | :-------------------------------------: |
| TLSR8258 | GD25LD40C/GD25LD40E/GD25LD80C/GD25LD80E |
| TLSR8278 |                    /                    |
| TLSR921x |        P25Q80U/P25Q80SU/P25Q16SU        |
| TLSR952x |                    /                    |
| TL721x   |                    /                    |
| TL321x   |        P25Q80SU/P25Q16SU/P25Q32SU       | 
### CodeSize
* Units: KBytes

|   chip   | sampleGW (RAM/FLASH Size) | sampleLight (RAM/FLASH Size) | sampleSwitch (RAM/FLASH Size) |
| :------- | :-----------------------: | :--------------------------: | :---------------------------: |
| TLSR8258 | 34 / 175                  | 31 / 194                     | 22 / 137                      |
| TLSR8278 | 34 / 170                  | 31 / 191                     | 22 / 133                      |
| TLSR921x | 36 / 160                  | 34 / 179                     | 25 / 122                      |
| TLSR952x | 36 / 169                  | 34 / 189                     | 27 / 133                      |
| TL721x   | 37 / 169                  | 34 / 189                     | 28 / 132                      |
| TL321x   | 36 / 168                  | 33 / 189                     | 27 / 131                      |

### Version
* 芯片
  - TC32: TLSR8269/TLSR8258/TLSR8278
  - RISCV: TLSR921x(B91)/TLSR952x(B92)/TL721x A1/TL321x A1
* 硬件版本
  - TLSR8258: C1T139A30_V1_2/C1T139A3_V2.0A
  - TLSR921x(B91): C1T213A20_V1_3/C1T213A3_V1_1
  - TL321x: C1T335A20_V1_0/C1T335A3_V1_0
* 驱动版本
  - tl_platform_sdk V3.2.0
* 工具链版本
  - TLSR8269/TLSR8258/TLSR8278: TC32 ELF GCC4.3
  - TLSR921x(B91): TL32 ELF MCULIB V5F GCC7.4
  - TLSR952x(B92)/TL721x: TL32 ELF MCULIB V5F GCC12.2
  - TL321x: TL32 ELF MCULIB V5 GCC12.2
* IDE版本
  - Telink IoT Studio: [TelinkIoTStudio_V2024.8](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IoTStudio/TelinkIoTStudio_V2024.8.zip)
### Features
* 当ZED处于离线状态时通过按键回连。
* TLSR8278添加Flash驱动代码。
### Bug Fixes
* 修复ev_buffer的地址范围检查。
* 修复通过HCI设置RF发射功率的问题。
* 修复配置上报命令的处理问题。
* 修复TL321x和TL721x的RF DMA通道设置问题。
### Performance
* 更新库并提供RSSI寄存器异常的解决方法。
### Code Refactoring
* N/A
### BREAKING CHANGES
* N/A
### Flash
|   chip   |                  flash                  |
| :------- | :-------------------------------------: |
| TLSR8258 | GD25LD40C/GD25LD40E/GD25LD80C/GD25LD80E |
| TLSR8278 |                    /                    |
| TLSR921x |        P25Q80U/P25Q80SU/P25Q16SU        |
| TLSR952x |                    /                    |
| TL721x   |                    /                    |
| TL321x   |        P25Q80SU/P25Q16SU/P25Q32SU       | 
### CodeSize
* 单位: KBytes

|   chip   | sampleGW (RAM/FLASH Size) | sampleLight (RAM/FLASH Size) | sampleSwitch (RAM/FLASH Size) |
| :------- | :-----------------------: | :--------------------------: | :---------------------------: |
| TLSR8258 | 34 / 175                  | 31 / 194                     | 22 / 137                      |
| TLSR8278 | 34 / 170                  | 31 / 191                     | 22 / 133                      |
| TLSR921x | 36 / 160                  | 34 / 179                     | 25 / 122                      |
| TLSR952x | 36 / 169                  | 34 / 189                     | 27 / 133                      |
| TL721x   | 37 / 169                  | 34 / 189                     | 28 / 132                      |
| TL321x   | 36 / 168                  | 33 / 189                     | 27 / 131                      |


## V3.7.1.0(PR)

### Version
* Chips
  - TC32: TLSR8269/TLSR8258/TLSR8278
  - RISCV: TLSR921x(B91)/TLSR952x(B92)/TL721x A1/TL321x A1
* Driver Version
  - tl_platform_sdk V3.2.0
* Toolchain Version
  - TLSR8269/TLSR8258/TLSR8278: TC32 ELF GCC4.3
  - TLSR921x(B91): TL32 ELF MCULIB V5F GCC7.4
  - TLSR952x(B92)/TL721x: TL32 ELF MCULIB V5F GCC12.2
  - TL321x: TL32 ELF MCULIB V5 GCC12.2
* IDE Version
  - Telink IoT Studio: [TelinkIoTStudio_V2024.8](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IoTStudio/TelinkIoTStudio_V2024.8.zip)
### Features
* Support TL321x platform.
* Update other platform drivers.
### Bug Fixes
* Fix the parsing issue of OTA Start Request command in ZGC tool.
* Fix the issue of CRC check failure during local upgrade through HCI.
* Fix the initial value in the reporting table.
### Performance
* Improve the execution efficiency of software timer tasks.
* Improve OTA safety checks.
* Optimize entering sleep mode.
### Code Refactoring
* N/A
### BREAKING CHANGES
* N/A
### Flash
|   chip   |                  flash                  |
| :------- | :-------------------------------------: |
| TLSR8258 | GD25LD40C/GD25LD40E/GD25LD80C/GD25LD80E |
| TLSR8278 |                    /                    |
| TLSR921x |             P25Q80U/P25Q16SU            |
| TLSR952x |                    /                    |
| TL721x   |                    /                    |
| TL321x   |        P25Q80SU/P25Q16SU/P25Q32SU       | 
### CodeSize
* Units: KBytes

|   chip   | sampleGW (RAM/FLASH Size) | sampleLight (RAM/FLASH Size) | sampleSwitch (RAM/FLASH Size) |
| :------- | :-----------------------: | :--------------------------: | :---------------------------: |
| TLSR8258 | 34 / 173                  | 31 / 193                     | 22 / 135                      |
| TLSR8278 | 34 / 169                  | 31 / 190                     | 22 / 131                      |
| TLSR921x | 36 / 160                  | 34 / 179                     | 24 / 121                      |
| TLSR952x | 36 / 169                  | 34 / 189                     | 27 / 132                      |
| TL721x   | 36 / 168                  | 34 / 188                     | 27 / 131                      |
| TL321x   | 36 / 168                  | 33 / 189                     | 27 / 131                      |

### Version
* 芯片
  - TC32: TLSR8269/TLSR8258/TLSR8278
  - RISCV: TLSR921x(B91)/TLSR952x(B92)/TL721x A1/TL321x A1
* 驱动版本
  - tl_platform_sdk V3.2.0
* 工具链版本
  - TLSR8269/TLSR8258/TLSR8278: TC32 ELF GCC4.3
  - TLSR921x(B91): TL32 ELF MCULIB V5F GCC7.4
  - TLSR952x(B92)/TL721x: TL32 ELF MCULIB V5F GCC12.2
  - TL321x: TL32 ELF MCULIB V5 GCC12.2
* IDE版本
  - Telink IoT Studio: [TelinkIoTStudio_V2024.8](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IoTStudio/TelinkIoTStudio_V2024.8.zip)
### Features
* 支持TL321x平台。
* 更新其他平台驱动。
### Bug Fixes
* 修复ZGC工具中OTA Start Request命令解析的问题。
* 修复通过HCI本地升级时CRC校验失败的问题。
* 修复reporting表中的初始值。
### Performance
* 提高软件定时器任务的执行效率。
* 改进OTA安全检查。
* 优化休眠模式。
### Code Refactoring
* N/A
### BREAKING CHANGES
* N/A
### Flash
|   chip   |                  flash                  |
| :------- | :-------------------------------------: |
| TLSR8258 | GD25LD40C/GD25LD40E/GD25LD80C/GD25LD80E |
| TLSR8278 |                    /                    |
| TLSR921x |             P25Q80U/P25Q16SU            |
| TLSR952x |                    /                    |
| TL721x   |                    /                    |
| TL321x   |        P25Q80SU/P25Q16SU/P25Q32SU       | 
### CodeSize
* 单位: KBytes

|   chip   | sampleGW (RAM/FLASH Size) | sampleLight (RAM/FLASH Size) | sampleSwitch (RAM/FLASH Size) |
| :------- | :-----------------------: | :--------------------------: | :---------------------------: |
| TLSR8258 | 34 / 173                  | 31 / 193                     | 22 / 135                      |
| TLSR8278 | 34 / 169                  | 31 / 190                     | 22 / 131                      |
| TLSR921x | 36 / 160                  | 34 / 179                     | 24 / 121                      |
| TLSR952x | 36 / 169                  | 34 / 189                     | 27 / 132                      |
| TL721x   | 36 / 168                  | 34 / 188                     | 27 / 131                      |
| TL321x   | 36 / 168                  | 33 / 189                     | 27 / 131                      |


## V3.7.0.0(ER)

### Version
* Chips
  - TC32: TLSR8269, TLSR8258, TLSR8278
  - RISCV: TLSR921x(B91), TLSR952x(B92), TL721x A1
* Driver Version
  - tl_platform_sdk V3.1.0
* Toolchain Version
  - TC32 ELF GCC4.3: TLSR8269, TLSR8258, TLSR8278
  - TL32 ELF MCULIB V5F GCC7.4: TLSR921x(B91)
  - TL32 ELF MCULIB V5F GCC12.2: TLSR952x(B92), TL721x
* IDE Version
  - Telink IDE: [Telink IDE](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IDE/Telink_IDE.zip)
  - Telink V323 RDS: [telink_v323_rds](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IDE/telink_v323_rds_official_windows.zip)
  - Telink IoT Studio: [TelinkIoTStudio_V2024.6](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IoTStudio/TelinkIoTStudio_V2024.6.zip)
### Features
* Support TL721x platform.
### Bug Fixes
* Fix ZCL recall scene command handler.
### Performance
* Improve the safety of software timers to ensure they are not illegally canceled.
### Code Refactoring
* Change ADC sampling mode to use non DMA mode.
### BREAKING CHANGES
* Remove "-fpack-struct" and "-fshort-enums" compilation options from the project configuration files, except for the TC32 project.
* Adjust the structure and enumeration definitions, decorate structures that require single byte alignment with "\_attributie_packed_".
### CodeSize
* Units: KBytes

|   chip   | sampleGW (RAM/FLASH Size) | sampleLight (RAM/FLASH Size) | sampleSwitch (RAM/FLASH Size) |
| :------- | :-----------------------: | :--------------------------: | :---------------------------: |
| TLSR8258 | 34 / 173                  | 31 / 193                     | 22 / 135                      |
| TLSR8278 | 34 / 169                  | 31 / 190                     | 22 / 131                      |
| TLSR921x | 36 / 160                  | 34 / 179                     | 24 / 121                      |
| TLSR952x | 36 / 169                  | 34 / 189                     | 27 / 132                      |
| TL721x   | 36 / 168                  | 34 / 188                     | 27 / 131                      |

### Version
* 芯片
  - TC32: TLSR8269, TLSR8258, TLSR8278
  - RISCV: TLSR921x(B91), TLSR952x(B92), TL721x A1
* 驱动版本
  - tl_platform_sdk V3.1.0
* 工具链版本
  - TC32 ELF GCC4.3: TLSR8269, TLSR8258, TLSR8278
  - TL32 ELF MCULIB V5F GCC7.4: TLSR921x(B91)
  - TL32 ELF MCULIB V5F GCC12.2: TLSR952x(B92), TL721x
* IDE版本
  - Telink IDE: [Telink IDE](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IDE/Telink_IDE.zip)
  - Telink V323 RDS: [telink_v323_rds](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IDE/telink_v323_rds_official_windows.zip)
  - Telink IoT Studio: [TelinkIoTStudio_V2024.6](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IoTStudio/TelinkIoTStudio_V2024.6.zip)
### Features
* 支持TL721x平台
### Bug Fixes
* 修复ZCL调用场景命令处理程序。
### Performance
* 提高软件计时器的安全性，确保它们不会被非法取消。
### Code Refactoring
* 将ADC采样模式更改为使用非DMA模式。
### BREAKING CHANGES
* 从项目工程配置文件中删除“-fpack struct”和“-fshort enums”编译选项，TC32项目工程除外。
* 调整结构体和枚举定义，用“_attributie_packed_”修饰需要单字节对齐的结构体。
### CodeSize
* 单位: KBytes

|   chip   | sampleGW (RAM/FLASH Size) | sampleLight (RAM/FLASH Size) | sampleSwitch (RAM/FLASH Size) |
| :------- | :-----------------------: | :--------------------------: | :---------------------------: |
| TLSR8258 | 34 / 173                  | 31 / 193                     | 22 / 135                      |
| TLSR8278 | 34 / 169                  | 31 / 190                     | 22 / 131                      |
| TLSR921x | 36 / 160                  | 34 / 179                     | 24 / 121                      |
| TLSR952x | 36 / 169                  | 34 / 189                     | 27 / 132                      |
| TL721x   | 36 / 168                  | 34 / 188                     | 27 / 131                      |


## V3.6.8.7(Public release)

### Features
* Support B92 platform.
* Update B91 platform drivers.
* Update HCI handler and ZGC Tool to support firmware updates.
* Add Flash protection processing.
### Bug Fixes
* Fix the ZCL write_no_rsp command handler.
### Code Refactoring
* N/A
### Performance
* N/A
### BREAKING CHANGES
* N/A

### Features
* 支持B92平台。
* 更新B91平台驱动程序。
* 更新HCI处理程序和ZGC工具，支持固件更新。
* 添加Flash保护处理。
### Bug Fixes
* 修复ZCL层write_no_rsp命令处理程序。
### Code Refactoring
* N/A
### Performance
* N/A
### BREAKING CHANGES
* N/A


## V3.6.8.6(Public release)

### Dependency Updates
* telink_b91m_driver_sdk V2.2.0
### Features
* Support for TELINK IoT Studio IDE.
* Provide CLOCK_32K_EXT_CRYSTAL macro to choose whether to use external 32K crystal oscillator.
### Bug Fixes
* Fix misspell and this may cause naming changes to certain APIs and Structures.
* Fix TC Rejoin issue.
* Fix buffer free issue during Find & Bind process.
* Fix configure reporting issues in ZCL.
* Fix the issue of reg_rf_rxtmaxlen data loss caused by enter sleep failure in B91.
### Code Refactoring
* N/A
### Performance
* Improve link algorithm.
### BREAKING CHANGES
* N/A
### CodeSize
* Typical reference values from 8258.
  + Flash:
    - ZC: 173kb
    - ZR: 192kb
    - ZED:135kb
  + RAM:
    - ZC: 34kb
    - ZR: 34kb
    - ZED:22kb

### Features
* 支持TELINK IoT Studio IDE。
* 提供CLOCK_32K_EXT_CRYSTAL宏选择是否使用外部32K晶振。
### Bug Fixes
* 修复拼写错误，这可能会导致某些API和数据结构的命名更改。
* 修复TC Rejoin。
* 修复查找和绑定过程中的buffer释放问题。
* 修复ZCL中配置属性上报的问题。
* 修复B91因休眠失败导致reg_rf_rxtmaxlen寄存器配置参数丢失的问题。
### Code Refactoring
* N/A
### Performance
* 提升链路算法。
### BREAKING CHANGES
* N/A


## V3.6.8.5(Public release)

### Features
* Provide a method to enable/disable whether the previous parent is preferred when rejoin.
### Bug Fixes
* Fix OTA Image Block Response sending failure.
### Code Refactoring
* N/A
### Performance
* N/A
### BREAKING CHANGES
* N/A

### Features
* 提供一个方法来使能/关闭在Rejoin时是否优先选择之前的父节点。
### Bug Fixes
* 修复OTA Image Block Response发送失败的问题。
### Code Refactoring
* N/A
### Performance
* N/A
### BREAKING CHANGES
* N/A


## V3.6.8.4(Public release)

### Features
* Update boot loader and application code.
* Update drivers(drv_nv.c, drv_radio.h and .link files).
### Bug Fixes
* Fix associate join failure for a few devices in a large network.
* Fix touch link issue.
* Fix aps fragment data issue.
* Fix the GP pairing issue.
* Fix the global link key issue.
* Fix misspell and this may cause naming changes to certain APIs and Structures.
### Code Refactoring
* N/A
### Performance
* N/A
### BREAKING CHANGES
* N/A

### Features
* 更新引导加载程序和应用程序代码。
* 更新驱动程序（drv_nv.c、drv_radio.h和.link文件）。
### Bug Fixes
* 修复大型网络中少数设备关联入网失败的问题。
* 修复touch link问题。
* 修复aps数据分包的问题。
* 修复GP配对的问题。
* 修复global link key问题。
* 修复拼写错误，这可能会导致某些API和数据结构的命名更改。
### Code Refactoring
* N/A
### Performance
* N/A
### BREAKING CHANGES
* N/A


## V3.6.8.3(Public release)

### Features
* Normalizes the naming of content related to private schemas.
* Update the NV module test in module_test.c.
* Change the macro RESV_FOR_APP_RAM_CODE_SIZE to 0x2000 to reduce the firmware size of the bootLoader.
* Provide a variable to change the allowRemoteTcPolicyChange of the Trust Center Policy.
* Provide app_userBindingTblRestore() to restore the binding table if the aps_binding_table_t is modified by user in the older version SDK.
### Bug Fixes
* Fix the abnormal restart issue of the gateway.
* Fix buffer free issue for the case incoming counter of the APS encrypted data doesn't match.
* The RREQ should only be retransmitted if the radius is greater than 0.
### Code Refactoring
* N/A
### Performance
* N/A
### BREAKING CHANGES
* N/A

### Features
* 规范化与私有模式相关内容的命名。
* 更新NV模块测试。
* 将RESV_FOR_APP_RAM_CODE_SIZE改成0x2000以减小bootLoader固件的大小。
* 提供一个变量来更改信任中心策略的allowRemoteTcPolicyChange值。
* 提供app_userBindingTblRestore()来恢复绑定表，如果用户在旧版本的SDK中修改了aps_binding_table_t的话。
### Bug Fixes
* 修复网关异常重启问题。
* 修复因APS加密数据的counter不匹配而导致的buffer未释放问题。
* 只有当RREQ的半径大于0时才需要转发。
### Code Refactoring
* N/A
### Performance
* N/A
### BREAKING CHANGES
* N/A


## V3.6.8.2(Public release)

### Features
* Add SDK version information in the bin firmware.
* Add CRC check to ensure the integrity of new firmware upgraded by OTA and bootstrap.
* Update ADC and flash driver for 8258 with embedded zbit flash.
* Add patches folder for other version to fix the flash writing issue(nv, ota...).
### Bug Fixes
* Fix NV storage issues and ensure the correctness of NV data.
* Fix buffer free issue under specific conditions in touch link process.
* Fix the issue that the paired link key may fail to recover after OTA.
### Code Refactoring
* N/A
### Performance
* Add RF busy check to avoid the error raw data.
### BREAKING CHANGES
* N/A

### Features
* 在bin固件中添加SDK版本信息。
* 添加CRC检查以确保OTA和引导升级的新固件的完整性。
* 为内嵌zbit Flash的8258芯片更新ADC和Flash驱动。
* 添加patches目录，针对其他版本修复flash相关问题。
### Bug Fixes
* 修复NV存储问题并确保NV数据的正确性。
* 修复Touch Link过程中在特定条件下buffer释放异常的问题。
* 修复OTA后配对链接密钥可能恢复失败的问题。
### Code Refactoring
* N/A
### Performance
* 添加RF忙标志检查避免错误的原始数据。
### BREAKING CHANGES
* N/A


## V3.6.8.1(Public release)

### Features
* N/A
### Bug Fixes
* Fix address mapping table update issue after address conflict.
* Fix APS group table management issue.
* Fix BRC passive ack issue.
* Fix MgmtBindResp handler issue.
* Fix APS counter issue when transferring data via bond mode.
### Code Refactoring
* Remove FLASH_W_PROTECT related code.
* Add new parameter "tcAddr" in bdb_join_direct().
### Performance
* Optimize OTA query.
* Update ADC driver to support two-point calibration.
### BREAKING CHANGES
* N/A

### Features
* N/A
### Bug Fixes
* 修复地址冲突后地址映射表更新的问题。
* 修复APS层分组表管理的问题。
* 修复广播被动应答的问题。
* 修复MgmtBindResp处理的问题。
* 修复绑定模式传输数据时的APS计数器问题。
### Code Refactoring
* 删除了FLASH_W_PROTECT相关代码。
* 在bdb_join_direct()中添加了新参数tcAddr。
### Performance
* 优化OTA查询。
* 更新ADC驱动程序以支持两点校准。
### BREAKING CHANGES
* N/A


## V3.6.8.0(Public release)

### Features
* Coordinator and router support green power combo basic feature.
* Bootloader supports firmware upgrade via UART.
* Provide PTA solutions.
* Provides internal flash write protection.
* Provides fast data polling setup for end devices.
* Update b91 driver to support 2M/4M internal flash.
* Update b91 to support protection code.
### Bug Fixes
* Fix the HCI_UART buffer overflow issue.
* Fix the seconds timer event lost issue for sleep end device.
* Fix the binding table, group table and scene table storage issue.
* Fix the decryption failure caused by frame counter.
### Code Refactoring
* Provides two network rejoin APIs for end devices,
  - zb_rejoinReq(), only do network rejoin once and call back the result to the upper layer, regardless of success.
  - zb_rejoinReqWithBackOff(), which will trigger the rejoin backoff according to the zdoCfgAttrDefault settings until rejoin success.
### Performance
* Improve network join performance.
### BREAKING CHANGES
* Change platform 9518 to b91.

### Features
* 协调器和路由设备支持GPCB。
* Bootloader支持通过UART固件升级。
* 提供PTA接口。
* 提供内部FLASH写保护接口。
* 为终端设备提供快速数据查询的配置接口。
* 更新b91驱动以支持2M/4M内部FLASH。
* 更新b91以支持保护码。
### Bug Fixes
* 修复HCI_UART内存溢出问题。
* 修复休眠设备的秒定时器事件丢失的问题。
* 修复绑定表、组表、场景表存储问题。
* 修复帧计数器导致的解密失败。
### Code Refactoring
* 为终端设备提供两种rejoin接口，
  - zb_rejoinReq()，只做一次rejoin并将结果回调给上层，不管成功与否。
  - zb_rejoinReqWithBackOff()，将依据zdoCfgAttrDefault的配置进行rejoin和回退，直到rejoin成功。
### Performance
* 提高网络加入性能。
### BREAKING CHANGES
* 将平台9518更改为b91。


## V3.6.7.4(Public release)

### Features
* N/A
### Bug Fixes
* Fix the OTA recovery issue.
* Fix the fragment message handling issue.
* Fix the network status command back roll issue.
### Code Refactoring
* Add board_8258_evk_v1p2 file for light demo.
### Performance
* N/A
### BREAKING CHANGES
* N/A
### CodeSize
* Flash:
  - ZC: 167kb
  - ZR: 187kb
  - ZED:127kb
* RAM:
  - ZC: 33kb
  - ZR: 30kb
  - ZED:25kb

### Features
* N/A
### Bug Fixes
* 修复OTA恢复问题。
* 修复分帧消息处理问题。
* 修复网络状态命令转圈的问题。
### Code Refactoring
* 灯示例中添加board_8258_evk_v1p2文件。
### Performance
* N/A
### BREAKING CHANGES
* N/A
### CodeSize
* Flash:
  - ZC: 167kb
  - ZR: 187kb
  - ZED:127kb
* RAM:
  - ZC: 33kb
  - ZR: 30kb
  - ZED:25kb


## V3.6.7.3(Public release)

### Features
* N/A
### Bug Fixes
* Fix the zb_coordinator library issue.
### Code Refactoring
* BRC jitter can be configured by user.
### Performance
* N/A
### BREAKING CHANGES
* N/A
### CodeSize
* Flash:
  - ZC: 167kb
  - ZR: 187kb
  - ZED:127kb
* RAM:
  - ZC: 33kb
  - ZR: 30kb
  - ZED:25kb

### Features
* N/A
### Bug Fixes
* 修复 zb_coordinator 库的问题。
### Code Refactoring
* 广播jitter参数用户可配。
### Performance
* N/A
### BREAKING CHANGES
* N/A
### CodeSize
* Flash:
  - ZC: 167kb
  - ZR: 187kb
  - ZED:127kb
* RAM:
  - ZC: 33kb
  - ZR: 30kb
  - ZED:25kb


## V3.6.7.2(Public release)

### Features
* Provide SRX mode for PHY layer.([35442f95](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/35442f95978fe10eb17a2f9d4c55c34b4345c2ef))
### Bug Fixes
* Fix the issue that the parent does not save the end device timeout value.
* Fix the poll control handler.
### Code Refactoring
* Clean HCI and APP layer code.
### Performance
* N/A
### BREAKING CHANGES
* N/A
### CodeSize
* Flash:
  - ZC: 167kb
  - ZR: 187kb
  - ZED:127kb
* RAM:
  - ZC: 33kb
  - ZR: 30kb
  - ZED:25kb

### Features
* PHY层提供SRX模式。([35442f95](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/35442f95978fe10eb17a2f9d4c55c34b4345c2ef))
### Bug Fixes
* 修复父设备没有保存end device timeout的问题。
* 修复poll control处理程序。
### Code Refactoring
* 清理HCI和APP层代码。
### Performance
* N/A
### BREAKING CHANGES
* N/A
### CodeSize
* Flash:
  - ZC: 167kb
  - ZR: 187kb
  - ZED:127kb
* RAM:
  - ZC: 33kb
  - ZR: 30kb
  - ZED:25kb


## V3.6.7.0(Public release)

### Features
* Replace Telink BSD license with Apache 2.0 License.([43aa6dc2](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/43aa6dc2fdf59f599a1eb1d2be2f4ce7dee07a3e))
* Update low voltage detection mechanism.([aff6c2d1](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/aff6c2d1fefdd60a31cd168f7656ca2c9c4a1440))
### Bug Fixes
* Fix OTA message processing issue.([26157426](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/26157426f2d649ff5f7d9ad5e12984b58606026b))
* Fix pre-config NWK key issue.([ffd26cba](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/ffd26cba8b9055925a4fc8b94e3c74656215acb2))
* Fix GPPB security issue.([2ea2a1ea](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/2ea2a1ea05b5a179593f0fb18328f88d93e20ea5))
* Fix Trust Center Rejoin mechanism.([b9fc3b00](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/b9fc3b002477fbc8a63c579a21e6298aee9baa34))
### Code Refactoring
* Clean up the ZCL code.([29816110](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/298161105a5cac68f0dace08f394fbc370c8d7ee))
* Clean up the HCI code.([861240ec](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/861240ec9ebee31c53cb3d14eef543f627021dc3))
### Performance
* Improve network performance and authentication speed.([ac626730](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/ac626730c2dc973ac4cbbda7c4feaffcdee64335))
### BREAKING CHANGES
* N/A

### CodeSize
* Flash:
  - ZC: 165kb
  - ZR: 187kb
  - ZED:127kb
* RAM:
  - ZC: 33kb
  - ZR: 30kb
  - ZED:25kb

### Features
* 将 Telink BSD license替换为 Apache 2.0 license。([43aa6dc2](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/43aa6dc2fdf59f599a1eb1d2be2f4ce7dee07a3e))
* 更新低压检测机制。([aff6c2d1](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/aff6c2d1fefdd60a31cd168f7656ca2c9c4a1440))
### Bug Fixes
* 修复OTA消息处理问题。([26157426](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/26157426f2d649ff5f7d9ad5e12984b58606026b))
* 修复预配网络密钥问题。([ffd26cba](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/ffd26cba8b9055925a4fc8b94e3c74656215acb2))
* 修复GPPB安全问题。([2ea2a1ea](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/2ea2a1ea05b5a179593f0fb18328f88d93e20ea5))
* 修复Truster Center rejoin机制。([b9fc3b00](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/b9fc3b002477fbc8a63c579a21e6298aee9baa34))
### Code Refactoring
* 清理ZCL代码。([29816110](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/298161105a5cac68f0dace08f394fbc370c8d7ee))
* 清理HCI代码。([861240ec](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/861240ec9ebee31c53cb3d14eef543f627021dc3))
### Performance
* 提升网络性能和鉴权速度。([ac626730](http://192.168.48.36/src/zigbee/telink_zigbee_src/commit/ac626730c2dc973ac4cbbda7c4feaffcdee64335))
### BREAKING CHANGES
* N/A

### CodeSize
* Flash:
  - ZC: 165kb
  - ZR: 187kb
  - ZED:127kb
* RAM:
  - ZC: 33kb
  - ZR: 30kb
  - ZED:25kb


## V3.6.6.0

### Features
* Add NWK key updating mechanism.
* Add WWAH feature for router device.
* Provide AUTO_QUICK_DATA_POLL_ENABLE to enable/disable automatic fast data polling.
### Bug Fixes
* The router device that has been connected to the network will no longer broadcast Device_annce command after being powered on.
* Fix the OTA server ID request failure issue.
### Code Refactoring
* Clean up the SS and ZDP layer code.
### Performance
* Improve routing performance and compatibility.
### BREAKING CHANGES
* None.

### Features
* 添加网络密钥更新机制。
* Router设备添加WWAH功能。
* 提供AUTO_QUICK_DATA_POLL_ENABLE以使能或关闭自动快速数据轮询。
### Bug Fixes
* 已经入网的路由设备上电后不再广播Device_annce命令。
* 修复OTA服务ID请求失败的问题。
### Code Refactoring
* 清理SS层和ZDP层代码。
### Performance
* 提升路由性能和兼容性。
### BREAKING CHANGES
* 无。


## V3.6.5.0

### Dependency Updates
* telink_b85m_driver_sdk V1.1.0
### Features
* Support zbit flash, and calibrate the power supply voltage of zbit according to the calibration value.
### Bug Fixes
* Change the voltage detect pin from PB3 to PC5, because the PB3 is a button on the 8258 EVK board.
* Fix the issue that the rejoin succeeds but the channel is not saved when the channel is changed.
* Fix RX DMA setting issue of RF on b91.
* Fix the DMA length setting issue of random.c on b85.
### Code Refactoring
* Change the routerAgeLimit default vaule to 3.
* Clean the nv.c/.h code, remove the FLASH_BASE_ADDR.
* Clean the zcl and touchLink code to reduce the size of the firmware.
### BREAKING CHANGES
* None.

### Features
* 支持zbit flash，并根据校准值校准zbit的电源电压。
### Bug Fixes
* 将电压检测引脚从PB3更改为PC5，因为PB3是8258 EVK板上的按键。
* 修复在channel改变的情况下，rejoin成功但channel未保存的问题。
* 修复b91中RF的RX DMA设置问题。
* 修复b85中random.c的DMA长度设置问题。
### Code Refactoring
* 将routerAgeLimit的默认值更改为3。
* 清理nv.c/.h代码, 移除FLASH_BASE_ADDR。
* 清理zcl和touchLink代码，减小固件的大小。
### BREAKING CHANGES
* 无。

## V3.6.4

### Dependency Updates
* telink_b91_driver_sdk V1.5.0-Beta
### Features
* Add voltage detection feature to avoid abnormal low voltage operation.
### Bug Fixes
* Fix the move command processing bug in the sampleLight demo.
### Code Refactoring
* Refactor the flash mapping in nv.c/.h to adapt to the bootloader mode.
### Performance
* Improve the configuration of bootloader working mode.
* Improve timing task management to reduce time deviation.
### BREAKING CHANGES
* None.

### Features
* 增加电压检测功能，避免异常低压运行。
### Bug Fixes
* 修复sampleLight实例中的move命令处理bug。
### Code Refactoring
* 重构nv.c/.h地址分配代码，兼容适配bootloader模式。
### Performance
* 改进bootloader工作模式的配置。
* 改善定时任务管理以减少时间偏差。
### BREAKING CHANGES
* 无。

## V3.6.3

### Features
* update flash driver(open source)
* add bootloader
* End Device accept the mgmt_leave from the node which isn't its parent node
### Bug Fixes
* Rf rx should be set off when the rx window is expired
### Performance
*  imporove mac layer performance.
### BREAKING CHANGES
* None.

### Features
* 更新flash驱动并开源。
* 添加bootloader。
* ZED设备处理来自非父节点设备的mgmt_leave命令
### Bug Fixes
* ZED设备超过rx window， 射频应设置为off
### Performance
* 提升mac的处理性能。
### BREAKING CHANGES
* 无。


## V3.6.2-Beta

### Features
* Add binary/multistate input/output cluster in the ZCL layer.
* Add manufacturer code processing in the ZCL layer.
### Bug Fixes
* Fix the stack buffer free issue.
* Fix the MAC TRX state machine issue.
* Fix the NWK broadcast passive ack buffer allocation issue.
### Code Refactoring
* Refactor the software timing task processing framework.
* Refactor the sleep management mechanism.
### Performance
* Modify the rejoin backoff processing mechanism.
### BREAKING CHANGES
* None.
### Note
* The current version SDK only verified on the 8258 platform.

### Features
* ZCL层添加binary/multistate input/output cluster。
* ZCL层manufacturer code处理。
### Bug Fixes
* 修复协议栈buffer释放问题。
* 修复MAC层TRX状态机处理问题。
* 修复NWK层广播passive ack申请buffer问题。
### Code Refactoring
* 重构软件计时任务处理框架。
* 重构睡眠管理机制。
### Performance
* 修改rejoin退避处理机制。
### BREAKING CHANGES
* 无。
### Note
* 当前版本仅在8258平台上得到验证。

## V3.6.1-Beta

### Dependency Updates
* telink_b91_driver_sdk V1.3.1-Beta
### Features
* Support 9518(B91) A2 chip.
* Improve MAC layer ack response speed.
* Open the source code related to APS layer group table.
* Add callback function for Trust Center(Coordinator) to notify the application layer when new devices join the network.
* Add ZGC tool, which can be used with HCI command.
### Bug Fixes
* Fix the poll rate setting issue related to poll control cluster in demo.
* Fix scene stored issue.
* Fix the loss interruption issue in 8269 uart driver.
* Fix the suspend issue in 8258 pm driver.
* Fix the issue of inaccurate conversion from rssi to lqi.
### Code Refactoring
* Refactor the abstract layer driver code.
* Refactor the relevant code that triggers "Reset To Factory New" through the power-on count.
### BREAKING CHANGES
* Move "zbhci" folder from the "zigbee" folder to "tl_zigbee_sdk/".

### Dependency Updates
* telink_b91_driver_sdk V1.3.1-Beta
### Features
* 支持9518（B91）A2芯片。
* 提高MAC ack的应答速度。
* 开源APS层group table相关的操作接口。
* 信任中心（协调器）添加回调函数接口，当有新设备加入网络时可以通知到应用层。
* 添加ZGC工具，可以配合HCI命令使用。
### Bug Fixes
* 修复Demo中与poll control cluster相关的poll速率设置问题。
* 修复场景保存问题。
* 修复8269串口驱动丢失中断问题。
* 修复8258 pm驱动suspend休眠问题。
* 修复rssi转换成lqi不准确的问题。
### Code Refactoring
* 重构抽象层驱动代码。
* 重构通过上电计数触发“恢复出厂”的相关代码。
### BREAKING CHANGES
* 将“zbhci”文件夹从“zigbee”文件夹挪到“tl_zigbee_sdk/”下。

## V3.6.0-Beta

### Features
* Support 9518(B91) A0 chip, it needs to install Telink RDS IDE to compile.
* Support zigbee APS layer fragment message processing.
* Add version_cfg.h to improve version control process, which is very important for OTA.
* Provide tl_zigbee_ota_tool_v2.0.exe, it will extract the information needed by OTA from the bin file and generate a file in .zigbee format automatically.
### Bug Fixes
* Fix the issue that the end device node orphan join fails to connect back.
### Code Refactoring
* Provide two different project configurations for Telink IDE and Telink RDS IDE, under the "tl_zigbee_sdk/build" directory. 
  When importing the project, please select tlsr_tc32 for Telink IDE and tlsr_riscv for Telink RDS IDE.
* Remove the user_config.h and the app_cfg.h is included via the compile configuration "-I../../../apps/sampleXXX" for every application.
* Move zb_af.c/zb_af.h from the "zigbee/zdo/" folder to the new folder "zigbee/af/".
* Clean up the driver abstraction layer.
### BREAKING CHANGES
* Change the directory structure to adapt to the multi-platform(826x/8258/8278/b91) SDK.

### Features
* 支持9518(B91) A0芯片,需要安装Telink RDS IDE编译环境。
* 支持zigbee APS层分片消息处理。
* 添加version_cfg.h以改进版本控制，这对于OTA非常重要。
* 提供tl_zigbee_ota_tool_v2.0.exe，它会自动从bin文件中提取OTA所需要的信息，并生成一个.zigbee格式的文件。
### Bug Fixes
* 修复终端节点orphan join回连失败的问题。
### Code Refactoring
* 为Telink IDE和Telink RDS IDE分别提供两个不同工程配置，在"tl_zigbee_sdk/build"目录下。
  在工程导入时，Telink IDE请选择tlsr_tc32，Telink RDS IDE选择tlsr_riscv。
* 移除user_config.h,每个应用的包含的app_cfg.h由编译"-I../../../apps/sampleXXX"来配置。
* zb_af.c/zb_af.h从"zigbee/zdo"文件夹移到新建的“zigbee/af”文件夹中。
* 清理驱动抽象层。
### BREAKING CHANGES
* 更改目录结构以适配多平台(826x/8258/8278/b91)SDK。

## telink_zigbee_sdk_v3.5.2

### Bug Fixes
* fix rejoin issue while the parent's short address is modified
### Features
* support 8278 A2 chip

## zigbee_sdk_v3.5.1

### Bug Fixes
* fix network key processing issue
* fix update device command handler
* avoid orphan scanning during network access
* fix missing storage issue of child neighbor nodes
* fix touchlink logical channel unexpected issue 
* make sure frame counter reset when network key is updated
* fix BDB find & bind issue
### Features
* provide a contact sensor demo running on chip 8258
* open routing settings of network layer to customer

## zigbee_sdk_v3.5.0

### Code Refactor
* refactoring usb code
* standardize code alignment and delete unnecessary debug information
### Features
* support 8278 A1 chip
* add APS ack encryption
* add pending data flag check 
* update demo for install code

## zigbee_sdk_v3.4.0.4

### Bug Fixes
* clean code
* update 8278 drivers

### Features
* add install code feature

## zigbee_sdk_v3.4.0.3

### Bug Fixes
* fix efuse load issue of 8278
* fix generate random data
* fix spi read issue
### Features
* add floating-point lib
* update zcl

## zigbee_sdk_v3.4.0.2

### Bug Fixes
* update driver for 8278
* update driver for 8258
* fix RF calibration for 8258
* fix duplicate irq registration of gpio

## zigbee_sdk_v3.4.0.0

### Bug Fixes
* fix ADC voltage reference value
* fix EMI driver for single tone frequency offset
* fix leave issue in distributed network
* fix touch link commissioning issue

### Features
* Provide a global variable for customer to enable/disable APS  
  layer encryption
* Improved routing efficiency
* Support flash mapping self-adaption according to the size,add  
  internal Flash SizeCheck func to get the correct MAC address and  
  pre configured parameters
* Added 8278 platform
* Clean code

## zigbee_sdk_v3.3.1.0

### Bug Fixes
* do noting when receiving transport key if the pre-nwkKey is set
* update 8258 adc driver
* add exception detection for uart Rx 
* update i2c driver
* aps/nwk sequence roll back issue
* end device rejoin issue when the parent is changed frequently
* mac state machine issue 
* add callback for panId conflict
* use random network key instead of the default nwk key 
* add switch function between the retention sleep mode and normal  
  sleep mode for 8258 ZED