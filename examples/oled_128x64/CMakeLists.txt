add_executable(oled example_hello_world.c)

find_package(Threads REQUIRED)
target_link_libraries(oled bricks pthread)
MESSAGE(STATUS "INFO: oled  done")

