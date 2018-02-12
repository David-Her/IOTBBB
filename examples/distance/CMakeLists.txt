add_executable(distance distance.cpp)

find_package(Threads REQUIRED)
target_link_libraries(distance bricks pthread ${CURL_LIBRARIES} ${JSON_LIBRARIES} jsoncpp)
MESSAGE(STATUS "INFO: distance  done")

