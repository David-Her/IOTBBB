add_executable(doorAlarm  doorAlarm.cpp)

find_package(Threads REQUIRED)
target_link_libraries(doorAlarm bricks ${CURL_LIBRARIES} ${JSON_LIBRARIES} jsoncpp pthread)
MESSAGE(STATUS "INFO: doorAlarm  done")

