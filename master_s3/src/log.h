#pragma once

#define LOG_NEWLINE            Serial.println("\n\n")
#define LOG(fmt, ...)          Serial.print(fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)     Serial.print("INFO: ");Serial.print(fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)    Serial.print("ERROR:");Serial.print(fmt, ##__VA_ARGS__)

#define LOGLN(fmt, ...)        Serial.println(fmt, ##__VA_ARGS__)
#define LOGLN_INFO(fmt, ...)   Serial.print("INFO: ");Serial.println(fmt, ##__VA_ARGS__)
#define LOGLN_ERROR(fmt, ...)  Serial.print("ERROR:");Serial.println(fmt, ##__VA_ARGS__)