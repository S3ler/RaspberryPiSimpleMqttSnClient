cmake_minimum_required(VERSION 3.7)
project(RaspberryPiSockets)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DRH_PLATFORM=RH_PLATFORM_RP -D__RASPBERRY_PI_)
add_definitions(-DPING)

include_directories(lib/mqtt-sn-sockets)
include_directories(lib/mqtt-sn-sockets/lib)
include_directories(lib/mqtt-sn-sockets/lib/arduino-linux-abstraction)
include_directories(lib/mqtt-sn-sockets/RadioHeadRpi)
add_subdirectory(lib/mqtt-sn-sockets)

set(SOURCE_FILES src/main.cpp)


#[[set(SOURCE_FILES src/main.cpp
        lib/mqtt-sn-sockets/RadioHeadRpi/RH_RF95.h
        lib/mqtt-sn-sockets/RadioHeadRpi/RH_RF95.cpp
        #lib/mqtt-sn-sockets/RadioHeadRpi/RHLinuxSPI.h
        #lib/mqtt-sn-sockets/RadioHeadRpi/RHLinuxSPI.cpp
        lib/mqtt-sn-sockets/RadioHeadRpi/RHGenericDriver.h
        lib/mqtt-sn-sockets/RadioHeadRpi/RHGenericDriver.cpp
        lib/mqtt-sn-sockets/RadioHeadRpi/RHDatagram.h
        lib/mqtt-sn-sockets/RadioHeadRpi/RHDatagram.cpp
        lib/mqtt-sn-sockets/RadioHeadRpi/RHReliableDatagram.h
        lib/mqtt-sn-sockets/RadioHeadRpi/RHReliableDatagram.cpp
        #lib/mqtt-sn-sockets/RadioHeadRpi/RHGenericSPI.h
        #lib/mqtt-sn-sockets/RadioHeadRpi/RHGenericSPI.cpp

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

