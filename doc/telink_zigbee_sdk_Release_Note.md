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