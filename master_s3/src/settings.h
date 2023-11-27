#pragma once
#include <Arduino.h>
// 基站：  充当AP，       在TCP通讯中是客户端
// 移动端：连接基站的WiFi，在TCP通讯中是服务端

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