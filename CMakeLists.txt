cmake_minimum_required(VERSION 3.7)
project(RaspberryPiSockets)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DRH_PLATFORM=RH_PLATFORM_RP -D__RASPBERRY_PI_)
add_definitions(-DPING)

include_directories(lib/mqtt-sn-sockets)
include_directories(lib/mqtt-sn-sockets/lib)
include_directories(lib/mqtt-sn-sockets/lib/arduino-linux-abstraction)
include_directories(lib/mqtt-sn-sockets/RadioHead)
add_subdirectory(lib/mqtt-sn-sockets)

set(SOURCE_FILES src/main.cpp)


#[[set(SOURCE_FILES src/main.cpp
        lib/mqtt-sn-sockets/RadioHead/RH_RF95.h
        lib/mqtt-sn-sockets/RadioHead/RH_RF95.cpp
        #lib/mqtt-sn-sockets/RadioHead/RHLinuxSPI.h
        #lib/mqtt-sn-sockets/RadioHead/RHLinuxSPI.cpp
        lib/mqtt-sn-sockets/RadioHead/RHGenericDriver.h
        lib/mqtt-sn-sockets/RadioHead/RHGenericDriver.cpp
        lib/mqtt-sn-sockets/RadioHead/RHDatagram.h
        lib/mqtt-sn-sockets/RadioHead/RHDatagram.cpp
        lib/mqtt-sn-sockets/RadioHead/RHReliableDatagram.h
        lib/mqtt-sn-sockets/RadioHead/RHReliableDatagram.cpp
        #lib/mqtt-sn-sockets/RadioHead/RHGenericSPI.h
        #lib/mqtt-sn-sockets/RadioHead/RHGenericSPI.cpp

        lib/mqtt-sn-sockets/RF95Socket.h
        lib/mqtt-sn-sockets/RF95Socket.cpp
        lib/mqtt-sn-sockets/lib/SocketInterface.h
        lib/mqtt-sn-sockets/lib/MqttSnMessageHandler.h
        lib/mqtt-sn-sockets/lib/MqttSnMessageHandler.cpp
        lib/mqtt-sn-sockets/lib/LoggerInterface.h
        lib/mqtt-sn-sockets/lib/LoggerInterface.cpp
        lib/mqtt-sn-sockets/lib/LinuxLogger.h
        lib/mqtt-sn-sockets/lib/LinuxLogger.cpp
        lib/mqtt-sn-sockets/lib/global_defines.h
        lib/mqtt-sn-sockets/lib/mqttsn_messages.h)]]

#add_executable(RaspberryPiSockets ${SOURCE_FILES})
#target_link_libraries(RaspberryPiSockets wiringPi)

add_executable(RaspberryPiSockets ${SOURCE_FILES})
target_link_libraries(RaspberryPiSockets example-lib-rh96-socket wiringPi)

