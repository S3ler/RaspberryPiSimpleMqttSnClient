// nrf95_ping_pong.ino
// -*- mode: C++ -*-

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

//#include <Arduino.h>
#ifdef Arduino_h
#include <SPI.h>
#include <Ethernet.h>
#endif

#ifndef Arduino_h
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <LoggerInterface.h>
#include <arduino-linux-abstraction/src/Arduino.h>

SerialLinux Serial;
#endif

#include <MqttSnMessageHandler.h>
#include <RF95Socket.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <SimpleMqttSnClient.h>
#include "LinuxLogger.h"


RF95Socket socket;
LinuxLogger logger;
MqttSnMessageHandler mqttSnMessageHandler;
SimpleMqttSnClient client;

#ifdef ESP8266
RH_RF95 rf95(2, 15);
#else
RH_RF95 rf95;
#endif
RHReliableDatagram manager(rf95);

#ifdef PING
#define OWN_ADDRESS 0x05
#define PONG_ADDRESS 0x03
device_address target_address(PONG_ADDRESS, 0, 0, 0, 0, 0);
uint8_t msg[] = {5, 'P', 'i', 'n', 'g'};
#elif PONG
#define OWN_ADDRESS 0x03
#endif

// Start
void setup() {
    Serial.begin(9600);
    Serial.println("Starting");
    Serial.print("OWN_ADDRESS: ");
#ifndef Arduino_h
    std::string number_str = std::to_string(OWN_ADDRESS);
    Serial.println(number_str.c_str());
#else
    Serial.println(OWN_ADDRESS);
#endif
    Serial.print("ROLE: ");
#ifdef PING
    Serial.println("PING");
#elif PONG
    Serial.println("PONG");
#else
    Serial.println("UNDEFINED");
#endif

#ifndef Arduino_h
    wiringPiSetupGpio();
#endif
    // link socket dependencies
    manager.setThisAddress(OWN_ADDRESS);
    socket.setRf95(&rf95);
    socket.setManager(&manager);

    // link components to
    client.setSocketInterface(&socket);
    client.setLoggerInterface(&logger);
    client.setMqttSnMessageHandler(&mqttSnMessageHandler);

    if (!client.begin()) {
        Serial.println("Failure init SimpleMqttSnClient");
    } else {
        Serial.println("Started");
    }
}

void loop() {
    mqttSnMessageHandler.loop();
}

#ifndef Arduino_h
bool run;

/* Signal the end of the software */
void sigint_handler(int signal) {
    run = false;
}


int main(int argc, char **argv) {
    run = true;

    signal(SIGINT, sigint_handler);

    setup();

    while (run) {
        loop();
        usleep(1);
    }

    return EXIT_SUCCESS;
}
#endif
