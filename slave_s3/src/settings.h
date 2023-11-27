#pragma once
#include <Arduino.h>
// 基站：  充当AP，       在TCP通讯中是客户端
// 移动端：连接基站的WiFi，在TCP通讯中是服务端
static const char*     WIFI_SSID     = "UM982";
static const char*     WIFI_PASSWORD = "zdh_1011";
static const uint32_t  WIFI_PORT     = 3031;
static const uint32_t  BAUDRATE      = 921600;
static const uint32_t  TCP_LEN       = 32;
static const IPAddress SERVER_IP     (192, 168, 4, 2); 
static const IPAddress CLIENT_IP     (192, 168, 4, 1); 
static const IPAddress AP_GATEWAY    (192, 168, 4, 1); 
static const IPAddress AP_SUBNET     (255, 255, 255, 0); 

uint32_t  NO_NEED_BEAT    = 0;
uint32_t  NEED_BEAT       = 1;