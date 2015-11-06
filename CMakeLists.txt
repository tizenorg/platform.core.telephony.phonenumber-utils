CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(phonenumber-utils C CXX)

INCLUDE(FindPkgConfig)

SET(EXTRA_CFLAGS "-fdata-sections -ffunction-sections -Wl,--gc-sections")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -fvisibility=hidden ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x -fvisibility=hidden")

SET(CLIENT ${PROJECT_NAME})
SET(DAEMON "${PROJECT_NAME}-daemon")
SET(DBUS_INTERFACE "org.tizen.PhonenumberUtils.dbus")

ADD_SUBDIRECTORY(common)
ADD_SUBDIRECTORY(daemon)
ADD_SUBDIRECTORY(client)
ADD_SUBDIRECTORY(test)

