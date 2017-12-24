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
#include <RH_NRF24.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <SimpleMqttSnClient.h>
#include "LinuxLogger.h"


RF95Socket socket;
LinuxLogger logger;
MqttSnMessageHandler mqttSnMessageHandler;
SimpleMqttSnClient client;

#ifdef DRIVER_RH_RF95
#ifdef ESP8266
RH_RF95 rh_driver(2, 15);
#else
RH_RF95 rh_driver;
#endif
#endif

#ifdef DRIVER_RH_NRF24
#ifdef ESP8266
// TODO
RH_NRF24 rh_driver(2, 15);
#else
RH_NRF24 rh_driver;
#endif
#endif

RHReliableDatagram manager(rh_driver);

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

#ifdef DRIVER_RH_RF95
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
    if (!rh_driver.init()) {
        Serial.println("Failure init DRIVER_RH_RF95");
    }

#ifdef FREQUENCY
    if (!rh_driver.setFrequency(FREQUENCY)) {
        Serial.println("Failure set FREQUENCY");
    }
#endif

#ifdef TX_POWER_PIN
    if(!rh_driver.setTxPower(18, false)){
        Serial.println("Failure set TX_POWER_PIN");
    }
#endif

#ifdef MODEM_CONFIG_CHOICE
    if(!rh_driver.setModemConfig(RH_RF95::MODEM_CONFIG_CHOICE)){
        Serial.println("Failure set MODEM_CONFIG_CHOICE");
    }
#endif
#endif
#ifdef DRIVER_RH_NRF24
    if (!rh_driver.init()) {
        Serial.println("Failure init DRIVER_RH_NRF24");
    }
    if (!rh_driver.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPowerm18dBm)) {
        Serial.println("Failure set DataRate250kbps, TransmitPowerm18dBm");
    }
#endif

    // link socket dependencies
    manager.setThisAddress(OWN_ADDRESS);
    //socket.setRf95(&rf95);
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



    delay(2000);
    Serial.println("Waiting 40 Seconds for advertisment");
    device_address* gw_adv = client.await_advertise(40000);
    if(gw_adv == nullptr){
        Serial.println("No Gateway Advertisment found.");
    }else{
        Serial.println("Gateway found.");
    }

    delay(1000);
    Serial.println("SearchGW 10 Seconds.");
    device_address* gw_info = client.search_gateway();
    if(gw_info == nullptr){
        Serial.println("No Gateway Info found.");
    }else{
        Serial.println("Gateway Info received.");
    }


    device_address* ping_addr = nullptr;
    if(gw_info != nullptr){
        delay(1000);

        ping_addr = gw_info;
        Serial.println("Pinging Searched Gateway (Info) - timeout 10s");
        uint64_t ping_time = client.ping_gateway(ping_addr);
        if(ping_time == 0){
            Serial.println("No Pingresponse received.");
        }else{
            Serial.print("Received Pingresponse after: ");
            Serial.print(ping_time);
            Serial.println(" milliseconds.");
        }
    }
    if(gw_adv != nullptr){
        delay(1000);

        ping_addr = gw_adv;
        Serial.println("Pinging Gateway Advertisment - timeout 10s");
        uint64_t ping_time = client.ping_gateway(ping_addr);
        if(ping_time == 0){
            Serial.println("No Pingresponse received.");
        }else{
            Serial.print("Received Pingresponse after: ");
            Serial.print(ping_time);
            Serial.println(" milliseconds.");
        }
    }

    device_address* publish_addr = nullptr;
    uint16_t predefined_topicId = 2;
    bool retain = false;
    uint8_t data[6] = {'H', 'e', 'l', 'l', 'o', 0 };
    if(gw_info != nullptr){
        delay(1000);
        publish_addr = gw_info;
        Serial.print("Publish QoS -1 Searched Gateway - ");

        if(client.publish_m1(publish_addr,predefined_topicId, retain,
                             (uint8_t *) &data,
                             (uint16_t) sizeof(data))){
            Serial.println("success");
        }else{
            Serial.println("failure");
        }
    }
    if(gw_adv != nullptr){
        delay(1000);
        publish_addr = gw_adv;
        Serial.print("Publish QoS -1 Gateway Advertisment - ");

        if(client.publish_m1(publish_addr,predefined_topicId, retain,
                             (uint8_t *) &data,
                             (uint16_t) 6)){
            Serial.println("success");
        }else{
            Serial.println("failure");
        }
    }

}

void loop() {
    device_address* gw_info = nullptr;
    Serial.println("SearchGW 10 Seconds.");
    gw_info = client.search_gateway();
    Serial.println(".");

    if(gw_info == nullptr){
        Serial.println("No Gateway Info found.");
    }else{
        Serial.println("Gateway Info received.");
    }
    client.loop();
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
