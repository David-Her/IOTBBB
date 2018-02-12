add_executable(doorAlarm  example_callback.c)

find_package(Threads REQUIRED)
target_link_libraries(doorAlarm bricks pthread)
MESSAGE(STATUS "INFO: doorAlarm  done")

