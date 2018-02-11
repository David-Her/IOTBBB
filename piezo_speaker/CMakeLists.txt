cmake_minimum_required(VERSION 2.8)
project( relayDistance )

INCLUDE_DIRECTORIES(
    ${CMAKE_HOME_DIRECTORY}/include
    ${CMAKE_HOME_DIRECTORY}/jsonrpccpp
)

FIND_PACKAGE(CURL)
IF (CURL_FOUND)
	MESSAGE(STATUS "SUCCESSFUL: CURL found")
ELSE (CURL_FOUND) 
	MESSAGE(STATUS "ERROR: CURL not found")
ENDIF (CURL_FOUND)

#add_executable( relayDistance example_simple.cpp ip_connection.cpp brick_master.cpp bricklet_distance_us.cpp bricklet_industrial_quad_relay.cpp)
add_executable( relayDistance distance.cpp ip_connection.cpp brick_master.cpp bricklet_distance_us.cpp bricklet_industrial_quad_relay.cpp)

find_package(Threads REQUIRED)
MESSAGE(STATUS "INFO: Threads: ${CMAKE_THREAD_LIBS_INIT}")

if(CMAKE_THREAD_LIBS_INIT)
 set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
 #MESSAGE(STATUS "SUCCESSFUL: threads found!!! new4")
 set_property(TARGET relayDistance PROPERTY COMPILE_OPTIONS "-pthread")
 set_property(TARGET relayDistance PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
 #set_target_properties(sensorTest PROPERTIES COMPILE_FLAGS -pthread LINK_FLAGS -pthread)
 #target_link_libraries(sensorTest ${CMAKE_THREAD_LIBS_INIT} ${RUFUSCOMMON_LIBRARIES})
endif(CMAKE_THREAD_LIBS_INIT)

target_link_libraries(relayDistance ${CMAKE_THREAD_LIBS_INIT} ${LIBC_LIBRARIES} ${CURL_LIBRARIES} ${JSON_LIBRARIES} jsoncpp)

MESSAGE(STATUS "INFO: relayDistance done")
