add_executable(doorAlarm  doorAlarm.cpp)

find_package(Threads REQUIRED)
target_link_libraries(doorAlarm bricks pthread)
MESSAGE(STATUS "INFO: doorAlarm  done")

