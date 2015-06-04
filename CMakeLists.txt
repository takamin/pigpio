cmake_minimum_required(VERSION 2.8)
project(pigpio)
find_package (Threads)
set(CMAKE_C_FLAGS -pthread)
set(CMAKE_EXE_LINKER_FLAGS -pthread)
set(LIBSRC
	src/gpio.c
	src/gpio_port.c
	src/Gpio.cpp)

include_directories( . src )
add_library(pigpio ${LIBSRC})

if(CMAKE_GENERATOR MATCHES "MinGW")
set(LIBS_PIGPIO_DEPENDS ${CMAKE_THREAD_LIBS_INIT} mman)
else()
set(LIBS_PIGPIO_DEPENDS ${CMAKE_THREAD_LIBS_INIT})
endif()

add_executable(write_gpio sample/write_gpio.cpp)
target_link_libraries(write_gpio pigpio ${LIBS_PIGPIO_DEPENDS})

add_executable(read_gpio sample/read_gpio.cpp)
target_link_libraries(read_gpio pigpio ${LIBS_PIGPIO_DEPENDS})

add_executable(read_gpio_pull sample/read_gpio_pull.cpp)
target_link_libraries(read_gpio_pull pigpio ${LIBS_PIGPIO_DEPENDS})

add_executable(numdisp sample/numdisp.cpp)
target_link_libraries(numdisp pigpio ${LIBS_PIGPIO_DEPENDS})

add_executable(gpioOut8Bit sample/gpioOut8Bit.cpp)
target_link_libraries(gpioOut8Bit pigpio ${LIBS_PIGPIO_DEPENDS})

add_executable(gpioReadButton sample/gpioReadButton.cpp)
target_link_libraries(gpioReadButton pigpio ${LIBS_PIGPIO_DEPENDS})

add_executable(pedsignal
	pedsignal/main.c
	pedsignal/pedsignal.c
	pedsignal/daemonize.c)

target_link_libraries(pedsignal pigpio ${LIBS_PIGPIO_DEPENDS})
