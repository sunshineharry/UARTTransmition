#include <Arduino.h>
#include "settings.h"
#include <WiFi.h>
#include "log.h"
WiFiClient     client;
QueueHandle_t  buff;
QueueHandle_t  onBeat;
uint32_t  NOT_BEGIN    = 0;
uint32_t  ON_CONNECT   = 1;
uint32_t  LOSS_CONNECT = 2;

void serial1_callback()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    char data = ' ';
    while(Serial1.available())
    {
        data = Serial1.read();
        xQueueSendToBackFromISR(buff, &data, &xHigherPriorityTaskWoken);
    }
}


static void wifi_task(void* parameter)
{
    char       single_data;
    BaseType_t isReadSuccess;
    vTaskDelay(100);
    while(1)
    {
        // Step1：尝试连接移动端，只有连接上以后，才会进入Step2
        LOG_INFO("Linking mobile terminal ...");
        while (!client.connect(SERVER_IP, WIFI_PORT))
        {
            // connect 函数默认以3秒的频率执行
            LOG(".");
        }
        LOGLN(" ");
        LOGLN_INFO("Link mobile terminal success!");
        xQueueReset(buff);
        xQueueOverwrite(onBeat, &(ON_CONNECT));

        
        while (client.connected())
        {
            // Step2：从队列中获取数据
            isReadSuccess = xQueueReceive(buff, &single_data, 100);
            
            // Step3：发送数据
            if (isReadSuccess==pdPASS)
            {
                try{
                    client.write(&single_data, 1);
                }catch(...)
                {
                    LOGLN_ERROR("Lost TCP ! Restart linking !");
                } 
            }

            // Step4：检测心跳
            while (client.available())
            {
                client.read();
                xQueueOverwrite(onBeat, &(ON_CONNECT));
            }
        }
    }
}


static void loss_tcp(void* parameter)
{
    uint32_t state;
    while(1)
    {
        xQueuePeek(onBeat, &state, 0);
        while (state==NOT_BEGIN) {
            xQueueOverwrite(onBeat, &(NOT_BEGIN));
            vTaskDelay(2000);
            xQueuePeek(onBeat, &state, 0);
        }
        xQueueOverwrite(onBeat, &(LOSS_CONNECT));
        vTaskDelay(4000);
        xQueuePeek(onBeat, &state, 0);
        if (state==LOSS_CONNECT)
        {
            LOGLN_ERROR("Lost TCP ! Restart ESP32 !");
            WiFi.disconnect();
            vTaskDelay(1000);
            ESP.restart();
        }
    }
}


void setup() {

    // 打开调试串口
    Serial.begin(115200);
    LOGLN(" ");
    LOGLN(" ");
    LOGLN(" ");

    // 开启wifi
    LOGLN_INFO("Starting WiFi...");
    WiFi.mode(WIFI_AP); 
	WiFi.softAPConfig(CLIENT_IP, AP_GATEWAY, AP_SUBNET);
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    delay(500);
    LOGLN_INFO("Start Wifi success !");
    LOG_INFO("WiFi connected with IP: ");
    LOGLN(WiFi.softAPIP());

    // 建立心跳
    onBeat = xQueueCreate(1, sizeof(uint32_t) );
    xQueueReset(onBeat);
    xQueueOverwrite(onBeat, &(NOT_BEGIN));

    // 建立数据缓冲区
    LOGLN_INFO("Starting UART...");
    LOG_INFO("UART_FIFO_LEN = ");
    LOGLN(SOC_UART_FIFO_LEN);
    buff = xQueueCreate(1024, sizeof(char));
    xQueueReset(buff);

    // 打开通讯串口
    Serial1.begin(BAUDRATE, SERIAL_8N1, rxPin, txPin);
    Serial1.onReceive(serial1_callback);
    LOGLN_INFO("Start UART success !");


    // 开始Wifi任务
    xTaskCreatePinnedToCore(
        wifi_task,
        "wifi_task",
        10000,
        NULL,
        1,
        NULL,
        1
    );

    // 丢失tcp则重启
    xTaskCreatePinnedToCore(
        loss_tcp,
        "loss_tcp",
        1024,
        NULL,
        4,
        NULL,
        0
    );

    LOGLN_INFO("Start all tasks success !");
}

void loop()
{
    ;
}





