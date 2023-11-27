# 1. 简介

一个基于WiFi技术的串口透传模块，**最高测试波特率921600** 。

几个典型的应用场景：

- RTK基站和移动端的通讯，也是这个我写这个项目的原因
- 智能车数传

对比淘宝其他解决方案的优势：

- 传输速率快！传输稳定！

对比淘宝其他解决方案的不足：

- 传输距离较近
- 目前由于项目需要，只实现的单向的传输

# 2. 软件使用

## 2.1 文件结构与简易用法

### 文件结构

- `master_s3`为AP端（提供WiFi信号，默认IP为192.168.4.1），在TCP通讯中是客户端
- `slave_s3`为移动端（连接AP端提供的WiFi，默认IP为192.168.4.2），在TCP通讯中是服务器

### 简易用法

**编译环境**：Platform IO，win11，Arduino框架

> 理论上用ArduinoIDE也可以编译通过，但我没有实验过

**注意事项**

1. ESP32双核处理器（ESP32，ESP32S3等）全系列应该都是兼容的。

2. 在编译为ESP32S3的可执行文件的时候，需要根据需要，调整UART0的位置。

   一个典型的platformio.ini如下

   ```ini
   [env:esp32-s3-devkitc-1]
   platform_packages = toolchain-riscv32-esp @ 8.4.0+2021r2-patch5
   platform = espressif32
   board = esp32-s3-devkitc-1
   framework = arduino
   monitor_speed = 115200
   build_flags = 
                 -DCORE_DEBUG_LEVEL=0
                 -DARDUINO_USB_CDC_ON_BOOT=1
   ```

   `-DARDUINO_USB_CDC_ON_BOOT=1` 表示利用ESP32S3内置的USB控制器进行虚拟串口，并作为UART0进行下载程序和通信。

## 2.2 配置文件的说明

mast和slve共享一个配置文件 `settings.h`

```c++
// WiFi配置
static const char*     WIFI_SSID     = "UM982";
static const char*     WIFI_PASSWORD = "UM982UM982";
static const uint32_t  WIFI_PORT     = 3031;
static const IPAddress SERVER_IP     (192, 168, 4, 2); 
static const IPAddress CLIENT_IP     (192, 168, 4, 1); 
static const IPAddress AP_GATEWAY    (192, 168, 4, 1); 
static const IPAddress AP_SUBNET     (255, 255, 255, 0);

// 透传串口配置 
static const uint32_t  BAUDRATE      = 961200;
static const int8_t    rxPin         = 17;
static const int8_t    txPin         = 18;
```

对上面配置进行解释如下：

- AP端建立WiFi信号，WiFi名称为UM982，密码为UM982UM982，本地IP为192.168.4.1，网关为192.168.4.1，子网掩码为255.255.255.0
- 移动端连接WiFi：UM982，本地IP 192.168.4.2

## 2.3 系统日志

（TODO）

# 3. 代码解读

（TODO）

