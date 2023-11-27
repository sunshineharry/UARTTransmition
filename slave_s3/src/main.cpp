#include <Arduino.h>
#include "settings.h"
#include <WiFi.h>
#include "log.h"


WiFiServer    server;
QueueHandle_t buff;
QueueHandle_t onBeat;

struct s_uart_data{
    uint8_t length;
    uint8_t data[TCP_LEN];
};

typedef union u_tcp_data{
    struct s_uart_data uart_data;
    uint8_t tcp_data;
}union_data;


static void wifi_task(void* parameter)
{
    char single_data;
    uint32_t is_need_beat;
    vTaskDelay(100);
    LOG_INFO("Connecting AP ...");
    while(1)
    {
        vTaskDelay(200);
        LOG(".");
        WiFiClient client = server.available();                 // 尝试建立客户对象
        if (client)                                             // 如果当前客户可用
        {
            LOG_NEWLINE();
            LOGLN_INFO("Connect AP success !");
            while (client.connected())                      // 如果客户端处于连接状态
            {
                if (client.available())                     // 如果有可读数据
                {
                    single_data = client.read();            // 读取一个字节
                    xQueueSendToBack(buff, &single_data, 100);
                }
                // 判断是否需要给一个心跳
                xQueuePeek(onBeat, &is_need_beat, 0);
                if (is_need_beat==NEED_BEAT)
                {
                    client.write('A');
                }
                xQueueOverwrite(onBeat, &(NO_NEED_BEAT));

            }
            client.stop();                                  // 结束当前连接:
            LOGLN_ERROR("Lost AP");
        }
    }
}


static void tcp_beating_timeing(void* parameter)
{
    while(1)
    {
        xQueueOverwrite(onBeat, &(NEED_BEAT));
        vTaskDelay(700);
    }
}

static void uart_task(void* parameter)
{
    char single_data;
    while(1)
    {
        xQueueReceive(buff, &single_data, portMAX_DELAY);
        Serial1.write(&single_data, 1);
        Serial.print(single_data);
    }
}


static void no_wifi_restart(void* parameter)
{
    while (WiFi.status() == WL_CONNECTED)
    {
        vTaskDelay(1000);
    }
    LOGLN_ERROR("Lost WiFi ! Restart ESP32 !");
    ESP.restart();
}


void setup() {

    // 打开调试串口
    Serial.begin(115200);
    LOGLN(" ");
    LOGLN(" ");
    LOGLN(" ");

    // 开启wifi 及 TCP 服务器
    WiFi.mode(WIFI_STA);
    WiFi.config(SERVER_IP, AP_GATEWAY, AP_SUBNET);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
    LOG_INFO("Connecting WiFi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
    }
    LOGLN(" ");
    LOG_INFO("Connect WiFi success !, IP address: ");
    LOGLN(WiFi.localIP());
    server.begin(WIFI_PORT);


    // 建立数据缓冲区
    buff = xQueueCreate(4096, sizeof(char));
    xQueueReset(buff);

    // 建立心跳
    onBeat = xQueueCreate(1, sizeof(uint32_t) );
    xQueueReset(onBeat);
    xQueueOverwrite(onBeat, &(NEED_BEAT));

    // 打开通讯串口
    Serial1.begin(BAUDRATE, SERIAL_8N1, 17, 18);
    // Serial1.write("helloesp32",10);

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

    // 开始串口任务
    xTaskCreatePinnedToCore(
        uart_task,
        "uart_task",
        10000,
        NULL,
        1,
        NULL,
        0
    );

    // 如果掉Wifi，则重启
    xTaskCreatePinnedToCore(
        no_wifi_restart,
        "no_wifi_restart",
        1024,
        NULL,
        4,
        NULL,
        0
    );    


    // tcp 心跳
    xTaskCreatePinnedToCore(
        tcp_beating_timeing,
        "tcp_beating_timeing",
        1024,
        NULL,
        2,
        NULL,
        0
    );    

    Serial.println("Start success!"); 
}


void loop()
{
    ;
}





