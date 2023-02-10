cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK directly
#set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/pico.cmake)
include(./lib/pico-sdk/pico_sdk_init.cmake)
project(pico-tests)


pico_sdk_init()
# rest of your project
add_executable(blink 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/blink.cpp
)
add_executable(fade 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/fade.cpp
)
add_executable(buzzer 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/buzzer.cpp
)
#target_compile_options(blink)
# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(blink pico_stdlib)
target_link_libraries(fade pico_stdlib hardware_pwm)
target_link_libraries(buzzer pico_stdlib hardware_pwm)
# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(blink)
pico_add_extra_outputs(fade)
pico_add_extra_outputs(buzzer)

include_directories(
    /usr/arm-none-eabi/include
    /usr/arm-none-eabi/include/c++/12.2.0
    /usr/arm-none-eabi/include/c++/12.2.0/arm-none-eabi
    ./lib/pico-sdk/lib/pico-sdk/src/rp2_common/hardware_pwm/include
)
