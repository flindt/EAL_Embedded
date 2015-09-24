cmake_minimum_required (VERSION 2.6)
project (ealEmbedded)

set( CMAKE_C_COMPILER /shared/toolchains/gcc-arm-none-eabi-4_9-2015q2/bin/arm-none-eabi-gcc )

set(CMAKE_C_FLAGS "-mthumb             \
       -mcpu=cortex-m3     \
       -Os                 \
       -ffunction-sections \
       -fdata-sections     \
       -MD                 \
       -std=c99            \
       -Wall               \
       -pedantic           \
       -DPART_LM3S6965      \
       -c" )

include_directories( /shared/toolchains/SW-EK-LM3S6965 )
include_directories( /shared/toolchains/SW-EK-LM3S6965/inc )
include_directories( /shared/toolchains/SW-EK-LM3S6965/driverlib )

add_subdirectory(ek-lm3s6965)


