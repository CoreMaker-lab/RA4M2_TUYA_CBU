# Overview
- **Name**: RA4M2_TUYA_CBU
- **MCU**: R7FA4M2AD3CFL#AA0
- **TUYA**:TUYA-CBU
- **IDE**: E2STUDIO

  


# Contact Information

- **Name**: Billy
- **交流群**: 925643491
- **Email**: a845656974@outlook.com
- **Phone**: +86 15622736378
- **CSDN Blog**: [Blog](https://blog.csdn.net/xinzuofang)
- **Video**: [Video](https://space.bilibili.com/3546563710290070)




# Buy Link
[https://shop192352884.taobao.com/](https://shop192352884.taobao.com/)

 
# Image

![image](https://github.com/user-attachments/assets/ad8dc359-bc53-4a19-8b89-794d13ac85b8)

![image](https://github.com/user-attachments/assets/a5e9f578-ec6e-4035-99e0-d99c2018f058)

![image](https://github.com/user-attachments/assets/75842a97-3cb3-4cf5-a9ea-eb7e3360a329)


# Project Introduction
- **RA4M2_TUYA_CBU_Project1**:RA4M2开发涂鸦模块CBU(1)----点亮LED灯
- **CSDN Blog**:[https://coremaker.blog.csdn.net/article/details/148778753](https://coremaker.blog.csdn.net/article/details/148778753)
- **Video**:[https://www.bilibili.com/video/BV1qnMHzoETM/](https://www.bilibili.com/video/BV1qnMHzoETM/)


点亮 LED 是 RA4M2 系列开发的“Hello World”。通过创建一个最小工程，让板载 LED 亮起来。

Turning on the LED is the "Hello World" of RA4M2 series development.
By creating a minimal project, the onboard LED is lit up.

- **RA4M2_TUYA_CBU_Project2**:RA4M2开发涂鸦模块CBU(2)----配置按键开启LED
- **CSDN Blog**:[https://coremaker.blog.csdn.net/article/details/148812579](https://coremaker.blog.csdn.net/article/details/148812579)
- **Video**:[https://www.bilibili.com/video/BV14kMHzkEwt/](https://www.bilibili.com/video/BV14kMHzkEwt/)


本实验演示如何在 Renesas RA4M2 单片机上使用 GPIO 输入（按键） 触发 GPIO 输出（LED），并使用e2studio快速完成外设配置的基本流程。

This experiment demonstrates how to use GPIO input (button) to trigger GPIO output (LED) on the Renesas RA4M2 microcontroller, and how to quickly complete peripheral configuration using e2studio.


- **RA4M2_TUYA_CBU_Project3**:RA4M2开发涂鸦模块CBU(3)----定时器GPT-PWM调光
- **CSDN Blog**:[https://coremaker.blog.csdn.net/article/details/148812644](https://coremaker.blog.csdn.net/article/details/148812644)
- **Video**:[https://www.bilibili.com/video/BV1mFMHz3EcM](https://www.bilibili.com/video/BV1mFMHz3EcM)


本实验基于 Renesas RA4M2 的 GPT（General PWM Timer） 外设，通过改变 PWM 占空比来调节 LED 亮度，实现软硬件结合的“无级调光”。

This experiment is based on the GPT (General PWM Timer) peripheral of the Renesas RA4M2. By adjusting the PWM duty cycle, it controls the LED brightness, achieving smooth dimming through a combination of hardware and software.



- **RA4M2_TUYA_CBU_Project4**:RA4M2开发涂鸦模块CBU(4)----配置串口
- **CSDN Blog**:[https://coremaker.blog.csdn.net/article/details/148812745](https://coremaker.blog.csdn.net/article/details/148812745)
- **Video**:[https://www.bilibili.com/video/BV17BMnz4Enc](https://www.bilibili.com/video/BV17BMnz4Enc)


printf 是 C 语言中常用的输出函数，广泛应用于各种嵌入式、桌面应用程序和调试过程中。这些函数可以将格式化的数据输出到标准输出（如控制台）或存储到字符串中。在系统开发中，了解它们的底层实现不仅能够帮助优化性能，还能提高代码的可移植性和灵活性。

printf is a commonly used output function in the C language, widely applied in various embedded systems, desktop applications, and debugging processes. These functions can output formatted data to standard output (such as a console) or store it in a string. In system development, understanding their underlying implementation not only helps optimize performance but also enhances code portability and flexibility.




- **RA4M2_TUYA_CBU_Project5**:RA4M2开发涂鸦模块CBU(5)----涂鸦模组快速上云
- **CSDN Blog**:[https://coremaker.blog.csdn.net/article/details/148812822](https://coremaker.blog.csdn.net/article/details/148812822)
- **Video**:[https://www.bilibili.com/video/BV1i4MWzhEA9](https://www.bilibili.com/video/BV1i4MWzhEA9)


本方案利用 Renesas RA4M2 MCU 与涂鸦智能预认证 Wi-Fi 模组的深度结合，实现多种传感器数据的云端采集与可视化管理。RA4M2 负责对传感器进行高速采样，并通过 UART接口将整合后的数据流推送给内置涂鸦协议栈的 Wi-Fi 模组。模组通过 TLS 加密通道与涂鸦云平台双向通信，既可将上传的数据点（DP）实时同步至云端，也可接收来自 App 或自建后台的控制指令。
借助涂鸚 IoT 平台，开发者只需在控制台一次性定义数据模型（DP）、下载 MCU 二次开发包并完成少量底层 HAL 实现，即可在几天内完成从硬件到移动端的端到端 Demo，在涂鸦智能 App 或基于 MQTT/HTTP 的自建后台中，可实时监控历史趋势并下发命令。
通过本方案，您可以在 1–2 天内完成硬件、固件与云端的一体化部署，实现任意传感器数据的可视化管理和智能联动。

This solution leverages the deep integration of the Renesas RA4M2 MCU with a Tuya pre-certified Wi-Fi module to enable cloud-based acquisition and visual management of various sensor data. The RA4M2 handles high-speed sampling of sensor data and transmits the aggregated data stream to the Wi-Fi module—equipped with a built-in Tuya protocol stack—via a UART interface. The module communicates bidirectionally with the Tuya Cloud Platform through a TLS-encrypted channel, allowing real-time synchronization of uploaded Data Points (DPs) to the cloud, as well as reception of control commands from the app or a custom backend.

With the Tuya IoT platform, developers only need to define the data model (DP) once in the console, download the MCU SDK, and implement a small portion of the underlying HAL. This enables completion of an end-to-end demo—from hardware to mobile app—within just a few days. In the Tuya Smart App or a custom backend based on MQTT/HTTP, developers can monitor historical trends in real time and issue commands as needed.

With this solution, you can achieve integrated deployment of hardware, firmware, and cloud services within 1–2 days, enabling visual management and intelligent linkage of any sensor data.




- **RA4M2_TUYA_CBU_Project6**:RA4M2开发涂鸦模块CBU(6)----RA4M2驱动涂鸦CBU模组
- **CSDN Blog**:[https://coremaker.blog.csdn.net/article/details/148813218](https://coremaker.blog.csdn.net/article/details/148813218)
- **Video**:[https://www.bilibili.com/video/BV1ETMBzrEVt](https://www.bilibili.com/video/BV1ETMBzrEVt)


本方案基于瑞萨 RA4M2 MCU 与涂鸦 CBU Wi-Fi & BLE 模组的无缝对接，旨在快速构建智能传感与控制终端。系统架构由 RA4M2 负责业务逻辑和外设驱动，CBU 模组提供网络接入与云端交互能力，两者通过标准串口通协议 (Tuya 通用协议) 完整实现产品功能上报与命令下发。


This solution is based on the seamless integration of the Renesas RA4M2 MCU with the Tuya CBU Wi-Fi & BLE module, aiming to quickly build intelligent sensing and control terminals. In the system architecture, the RA4M2 is responsible for business logic and peripheral control, while the CBU module provides network connectivity and cloud interaction capabilities. The two communicate via a standard UART protocol (Tuya Universal Protocol), fully enabling product function reporting and command delivery.




















