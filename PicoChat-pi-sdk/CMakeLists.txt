# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(PicoChat C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(PicoChat PicoChat.cpp)

pico_set_program_name(PicoChat "PicoChat")
pico_set_program_version(PicoChat "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(PicoChat 1)
pico_enable_stdio_usb(PicoChat 0)

target_compile_definitions(PicoChat PRIVATE
        PICO_DEFAULT_UART_BAUD_RATE=115200
        PICO_DEFAULT_UART_TX_PIN=0
        PICO_DEFAULT_UART_RX_PIN=1
)

# Add the standard library to the build
target_link_libraries(PicoChat
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(PicoChat PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(PicoChat 
        hardware_spi
        hardware_pwm
        pico_multicore
        pico_rand)

# External libs
include(FetchContent)

set(RADIOLIB_VERSION 7.1.2)
FetchContent_Declare(
        RadioLib
        URL https://github.com/jgromes/RadioLib/archive/refs/tags/${RADIOLIB_VERSION}.tar.gz
        FIND_PACKAGE_ARGS ${RADIOLIB_VERSION}
)

# TinyAES setup
set(TINYAES_VERSION 1.0.0)
FetchContent_Declare(
 TinyAES
 URL https://github.com/kokke/tiny-AES-c/archive/refs/tags/v${TINYAES_VERSION}.tar.gz
 FIND_PACKAGE_ARGS ${TINYAES_VERSION}
)
FetchContent_MakeAvailable(TinyAES)

# Create a proper library target for TinyAES
add_library(tiny_aes STATIC 
    ${tinyaes_SOURCE_DIR}/aes.c
)
target_include_directories(tiny_aes PUBLIC 
    ${tinyaes_SOURCE_DIR}
)

# Link everything
target_link_libraries(PicoChat
 RadioLib
 tiny_aes
)

FetchContent_MakeAvailable(RadioLib)
FetchContent_MakeAvailable(TinyAES)

pico_add_extra_outputs(PicoChat)

# Linker for picoboot3
if(${PICO_PLATFORM} STREQUAL "rp2040")
  pico_set_linker_script(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/memmap_default_rp2040.ld)
elseif(${PICO_PLATFORM} MATCHES "rp2350")
  pico_set_linker_script(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/memmap_default_rp2350.ld)
endif()

set_property(TARGET ${CMAKE_PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS "-Wl,--print-memory-usage")

