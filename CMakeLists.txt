cmake_minimum_required(VERSION 2.8)
project(pigpio)
set(CMAKE_C_FLAGS -pthread)
set(CMAKE_EXE_LINKER_FLAGS -pthread)
set(LIBSRC
	src/gpio.c
	src/gpio_port.c
	src/Gpio.cpp)

include_directories( . src )
add_library(pigpio ${LIBSRC})

add_executable(write_gpio sample/write_gpio.cpp)
target_link_libraries(write_gpio pigpio)

add_executable(read_gpio sample/read_gpio.cpp)
target_link_libraries(read_gpio pigpio)

add_executable(numdisp sample/numdisp.cpp)
target_link_libraries(numdisp pigpio)
