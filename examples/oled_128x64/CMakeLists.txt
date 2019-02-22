add_executable(oled example_hello_world.cpp)

find_package(Threads REQUIRED)
target_link_libraries(oled bricks pthread)
MESSAGE(STATUS "INFO: oled  done")

