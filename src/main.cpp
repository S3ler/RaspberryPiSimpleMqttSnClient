/*
Original code by: https://github.com/Snootlab/lora_chisterapi
Edited by: Ramin Sangesari
*/

#define OWN_ADDRESS 0x02

/*-----------------------------------------*/
#include <dirent.h>
#include <fcntl.h>
/*-----------------------------------------*/

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <chrono>
#include <thread>
#include <LoggerInterface.h>
#include <MqttSnMessageHandler.h>
#include <RF95Socket.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <iostream>

#define PING

RF95Socket socket;
LoggerInterface logger;
MqttSnMessageHandler mqttSnMessageHandler;

RH_RF95 rf95;
RHReliableDatagram manager(rf95);

#ifdef PING
#define OWN_ADDRESS 0x02
#define PONG_ADDRESS 0x03
device_address target_address(PONG_ADDRESS, 0, 0, 0, 0, 0);
uint8_t msg[] = {5, 'P', 'i', 'n', 'g'};
#elif PONG
#define OWN_ADDRESS 0x03
#endif

bool run;

/* Signal the end of the software */
void sigint_handler(int signal) {
    run = false;
}

void setup() {
    std::cout << "Starting" << std::endl;
    wiringPiSetupGpio();

    manager.setThisAddress(OWN_ADDRESS);
    socket.setRf95(&rf95);
    socket.setManager(&manager);
    socket.setLogger(&logger);
    socket.setMqttSnMessageHandler(&mqttSnMessageHandler);
    mqttSnMessageHandler.setLogger(&logger);
    mqttSnMessageHandler.setSocket(&socket);

    if (!mqttSnMessageHandler.begin()) {
        std::cout << "Failure init MqttSnMessageHandler" << std::endl;
    } else {
        std::cout << "Started" << std::endl;
#ifdef PING
        mqttSnMessageHandler.send(&target_address, msg, (uint16_t) msg[0]);
#endif
    }
}

void loop() {
    mqttSnMessageHandler.loop();
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

