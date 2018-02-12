SET(bricks_include_dirs
	${CMAKE_HOME_DIRECTORY}/include
)

SET (bricks_files
	bricklet_piezo_speaker.c
	brick_master.cpp
	ip_connection.cpp
	bricklet_oled_128x64.c
	bricklet_multi_touch.c
	bricklet_distance_us.cpp  
	bricklet_industrial_quad_relay.cpp
)

INCLUDE_DIRECTORIES(${bricks_include_dirs}) 

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

ADD_LIBRARY(bricks ${bricks_files})

TARGET_LINK_LIBRARIES(bricks ${bricks_libs})

