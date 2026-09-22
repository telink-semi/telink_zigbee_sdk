# telink\_zigbee\_sdk README

* [English](./README.md)

# SDK 介绍

telink\_zigbee\_sdk 是一款基于泰凌微电子 TL321x、TL323x、TLSR921x 等系列 SoC 的 Zigbee 4.0 软件开发平台，专为低功耗、长距离、自组网以及网络自愈应用场景设计，高效构建适用于智能家居、工业控制等领域的 Zigbee 产品。

本 SDK 提供了完备的软件体系，包括芯片底层驱动、适配层驱动、Zigbee/BLE 双模协议栈、丰富的示例工程，以及 ZGC （Zigbee Gateway Controller）功能演示工具和 OTA （Over-The-Air）升级工具，全面支持从产品原型设计到量产部署的整个研发周期。

![](./SDK_Architecture.jpg)

**核心能力**

| 类别 | 能力 |
| --- | --- |
| 无线连接 | 支持 Zigbee 4.0 协议栈，兼容 Zigbee 与 BLE 双协议并发运行 |
| 软件框架 | 提供标准化外设驱动、安全组件（含 AES、Hash、SHA-256、C25519 等算法），采用模块化分层设计，便于功能扩展与维护 |
| 系统服务 | 集成时钟管理、事件调度、功耗管理、非易失性存储（NV）管理以及 OTA 固件升级管理，保障系统高效稳定运行 |

您可借助该SDK快速实现 Zigbee 智能网关、照明灯具、传感器等各类终端产品的开发。

**典型应用**

| 无线技术 | 应用领域 | 典型产品 |
| --- | --- | --- |
| Zigbee 4.0 | 智能家居 | 智能照明（灯具、开关）、智能家电、温湿度传感器、门窗传感器、智能门锁等 |
|  | 工业控制 | 设备状态监测、环境数据采集、无线仪表等 |
|  | 网关与桥接 | Zigbee 智能网关、多协议边缘网关等 |

**支持信息**

关于完整、准确的芯片型号、对应的开发板、开发平台、工具链以及 SDK 版本的详细信息，请参考 [Release Notes](./doc/telink_zigbee_sdk_Release_Note.md) 。

# 文档与资源

**文档导航**

| **文档** | **说明** |
| --- | --- |
| [快速入门](https://doc.telink-semi.cn/doc/zh/software/res/sdk/zigbee/get_started/telink_zigbee_sdk_get_started_cn/) | 开发环境配置、SDK获取及快速上手方法 |
| [用户手册](https://doc.telink-semi.cn/doc/zh/software/res/sdk/zigbee/zigbee_sdk_developer_manual_cn/) | 详细的软件架构、仓库结构及功能模块说明 |
| [示例工程](./tl_zigbee_sdk/apps/) | 示例程序及使用说明 |
| [Release Notes](./doc/telink_zigbee_sdk_Release_Note.md) | 支持平台、版本说明与详细变化 |

**社区与资源**

| **资源** | **说明** |
| --- | --- |
| [Telink 官方论坛](https://forum.telink-semi.cn/) | 技术支持与讨论 |
| [Telink 官方网站](https://www.telink-semi.com/) | 产品与文档中心 |
| [GitHub](https://github.com/telink-semi/telink_zigbee_sdk) / [Gitee](https://gitee.com/telink-semi/telink_zigbee_sdk) | SDK 源码仓库 |

# 许可证

本项目采用以下许可证：

**Apache License, Version 2.0**

Licensed under the Apache License, Version 2.0 (the "License");

You may not use this file except in compliance with the License.

You may obtain a copy of the License at:

[http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and limitations under the License. 
