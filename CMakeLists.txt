CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(context-common)
INCLUDE(GNUInstallDirs)

INCLUDE(FindPkgConfig)
INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
ADD_DEFINITIONS(-O2 -Wall -fPIC -fdata-sections -ffunction-sections -fvisibility=hidden)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIC -Wl,--as-needed -Wl,--gc-section -Wl,--print-gc-section")

# Install
INSTALL(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/context-service/internal
	FILES_MATCHING PATTERN "*.h"
)

ADD_SUBDIRECTORY(src/shared)
ADD_SUBDIRECTORY(src/server)
