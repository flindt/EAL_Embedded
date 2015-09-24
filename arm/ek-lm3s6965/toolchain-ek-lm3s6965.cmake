INCLUDE(CMakeForceCompiler)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

SET(COMMON_FLAGS "-mcpu=cortex-m3 -mthumb -mthumb-interwork \
	-ffunction-sections -fdata-sections \
	-Wall -pedantic -c -Os\
	-DPART_LM3S6965")
SET(CMAKE_CXX_FLAGS "${COMMON_FLAGS}  -std=gnu++0x")
SET(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu99")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections ")
