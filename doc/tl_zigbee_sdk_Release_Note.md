##V3.6.0-Beta

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