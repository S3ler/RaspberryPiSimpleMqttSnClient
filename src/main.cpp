// nrf95_ping_pong.ino
// -*- mode: C++ -*-

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

#include <Arduino.h>
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
SerialLinux Serial;
#endif

#include <LinuxLogger.h>
#include <MqttSnMessageHandler.h>
#include <LinuxLogger.h>
#include <RF95Socket.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>


RF95Socket socket;
LoggerInterface logger;
MqttSnMessageHandler mqttSnMessageHandler;

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

    manager.setThisAddress(OWN_ADDRESS);
    /*
    socket.setRf95(&rf95);
    socket.setManager(&manager);
    socket.setLogger(&logger);
    socket.setMqttSnMessageHandler(&mqttSnMessageHandler);
    mqttSnMessageHandler.setLogger(&logger);
    mqttSnMessageHandler.setSocket(&socket);

    if (!mqttSnMessageHandler.begin()) {
        Serial.println("Failure init MqttSnMessageHandler");
    } else {
        Serial.println("Started");
    }
#ifdef PING
    mqttSnMessageHandler.send(&target_address, msg, (uint16_t) msg[0]);
#ifdef RH_RF95_h
    mqttSnMessageHandler.send(&target_address, msg, (uint16_t) msg[0]);
#endif
#endif
     */
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
