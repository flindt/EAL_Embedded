
INCLUDE(CMakeForceCompiler)

# this one is important
SET(CMAKE_SYSTEM_NAME generic)

# specify the cross compiler
SET(CMAKE_C_COMPILER   arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER arm-none-eabi-g++)

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

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
