add_executable(piezoSpeaker example_morse_code.cpp)

find_package(Threads REQUIRED)
target_link_libraries(piezoSpeaker bricks pthread)
MESSAGE(STATUS "INFO: piezoSpeaker  done")

