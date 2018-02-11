add_executable(touch example_callback.c)

find_package(Threads REQUIRED)
target_link_libraries(touch bricks pthread)
MESSAGE(STATUS "INFO: touch  done")

