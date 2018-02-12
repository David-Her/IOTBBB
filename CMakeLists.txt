cmake_minimum_required(VERSION 2.8)
project( bricks )

SET(BRICKS_INCLUDE_DIR 
	${CMAKE_HOME_DIRECTORY}/include
	${CMAKE_HOME_DIRECTORY}/jsonrpccpp)

SET (CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH} )
SET(LIBRARY_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

INCLUDE_DIRECTORIES(
    ${CMAKE_HOME_DIRECTORY}/include
)


FIND_PACKAGE(CURL)
IF (CURL_FOUND)
	MESSAGE(STATUS "SUCCESSFUL: CURL found")
ELSE (CURL_FOUND) 
	MESSAGE(STATUS "ERROR: CURL not found")
ENDIF (CURL_FOUND)

#add_executable( relayDistance example_simple.cpp ip_connection.cpp brick_master.cpp bricklet_distance_us.cpp bricklet_industrial_quad_relay.cpp)
#add_executable( piezoSpeaker example_morse_code.c ip_connection.cpp brick_master.cpp bricklet_piezo_speaker.c)

find_package(Threads REQUIRED)
MESSAGE(STATUS "INFO: Threads: ${CMAKE_THREAD_LIBS_INIT}")

if(CMAKE_THREAD_LIBS_INIT)
# set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
# #MESSAGE(STATUS "SUCCESSFUL: threads found!!! new4")
# set_property(TARGET piezoSpeaker PROPERTY COMPILE_OPTIONS "-pthread")
# set_property(TARGET piezoSpeaker PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
# #set_target_properties(sensorTest PROPERTIES COMPILE_FLAGS -pthread LINK_FLAGS -pthread)
# #target_link_libraries(sensorTest ${CMAKE_THREAD_LIBS_INIT} ${RUFUSCOMMON_LIBRARIES})
endif(CMAKE_THREAD_LIBS_INIT)

#target_link_libraries(piezoSpeaker  ${CMAKE_THREAD_LIBS_INIT} ${LIBC_LIBRARIES} ${CURL_LIBRARIES} ${JSON_LIBRARIES} jsoncpp)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(examples)

#MESSAGE(STATUS "INFO: piezoSpeaker done")
