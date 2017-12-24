cmake_minimum_required(VERSION 3.7)
project(RaspberryPiSimpleMqttSnClient)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DRH_PLATFORM=RH_PLATFORM_RP -D__RASPBERRY_PI_)
add_definitions(-DDRIVER_RH_NRF24)
add_definitions(-DPONG)

include_directories(lib/SimpleMqttSnClientCore)
add_subdirectory(lib/SimpleMqttSnClientCore)

include_directories(lib/mqtt-sn-sockets)
#include_directories(lib/mqtt-sn-sockets/lib)
#include_directories(lib/mqtt-sn-sockets/lib/arduino-linux-abstraction/src)
include_directories(lib/mqtt-sn-sockets/RadioHeadRpi)
add_subdirectory(lib/mqtt-sn-sockets)

include_directories(lib/SimpleMqttSnClientCore/arduino-linux-abstraction/lib)


set(SOURCE_FILES src/main.cpp
		lib/mqtt-sn-sockets/RadioHeadRpi/RHDatagram.h
        lib/mqtt-sn-sockets/RadioHeadRpi/RHDatagram.cpp
		lib/mqtt-sn-sockets/RadioHeadRpi/RHGenericDriver.h
		lib/mqtt-sn-sockets/RadioHeadRpi/RHGenericDriver.cpp
		lib/mqtt-sn-sockets/RadioHeadRpi/RHGenericSPI.h
		lib/mqtt-sn-sockets/RadioHeadRpi/RHGenericSPI.cpp
		lib/mqtt-sn-sockets/RadioHeadRpi/RHLinuxSPI.h
		lib/mqtt-sn-sockets/RadioHeadRpi/RHLinuxSPI.cpp
		lib/mqtt-sn-sockets/RadioHeadRpi/RH_RF95.h
		lib/mqtt-sn-sockets/RadioHeadRpi/RH_RF95.cpp
		src/LinuxLogger.h
		src/LinuxLogger.cpp
)



add_executable(RaspberryPiSimpleMqttSnClient ${SOURCE_FILES})
target_link_libraries(RaspberryPiSimpleMqttSnClient lib-rh96-socket arduino-linux-abstraction lib-SimpleMqttSnClient wiringPi pthread crypt rt)
