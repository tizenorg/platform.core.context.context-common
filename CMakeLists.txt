CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(context-common)
INCLUDE(GNUInstallDirs)

INCLUDE(FindPkgConfig)
INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall -fPIC -Wl,--as-needed -fvisibility=hidden -flto")
ADD_DEFINITIONS("-DCTX_COMMON_EXPORT_API=__attribute__((visibility(\"default\")))")

# Install
INSTALL(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/context-service/internal
	FILES_MATCHING PATTERN "*.h"
)

ADD_SUBDIRECTORY(src/shared)
ADD_SUBDIRECTORY(src/server)
