//
// Created by bele on 07.04.17.
//

#ifndef CORE_MQTT_SN_GATEWAY_LOGGERIMPLEMENTATION_H
#define CORE_MQTT_SN_GATEWAY_LOGGERIMPLEMENTATION_H


#include <LoggerInterface.h>

#ifndef Arduino_h

#include <arduino-linux-abstraction/src/Arduino.h>

#if (RH_PLATFORM == RH_PLATFORM_RP)
#include <wiringPi.h>
#endif

#endif

class LinuxLogger : public LoggerInterface {
private:
    uint8_t current_log_lvl = 2;
    uint8_t last_started_log_lvl = UINT8_MAX;
#ifndef Arduino_h
    SerialLinux Serial;
#endif

public:
    bool begin();

    void set_log_lvl(uint8_t log_lvl);

    void log(char *msg, uint8_t log_lvl);

    void log(const char *msg, uint8_t log_lvl);

    void start_log(char *msg, uint8_t log_lvl);

    void start_log(const char *msg, uint8_t log_lvl);

    void set_current_log_lvl(uint8_t log_lvl);

    void append_log(char *msg);

    void append_log(const char *msg);
};


#endif //CORE_MQTT_SN_GATEWAY_LOGGERIMPLEMENTATION_H
