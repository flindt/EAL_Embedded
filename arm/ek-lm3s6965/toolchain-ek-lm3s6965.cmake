INCLUDE(CMakeForceCompiler)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)
SET(CMAKE_C_LINKER arm-none-eabi-ld)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

SET(COMMON_FLAGS "-mcpu=cortex-m3 -mthumb -mthumb-interwork \
	-ffunction-sections -fdata-sections \
	-Wall -pedantic -g -Os\
	-DPART_LM3S6965")
SET(CMAKE_CXX_FLAGS "${COMMON_FLAGS}  -std=gnu++0x")
SET(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu99")
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections \
	-L/shared/toolchains/SW-EK-LM3S6965/driverlib/gcc-cm3/ \
	-T /home/flindt/git/EAL_Embedded/arm/ek-lm3s6965/standalone.ld")
	
function(arm_firmware INPUT)
              add_custom_command(TARGET ${INPUT}
                      COMMAND arm-none-eabi-objcopy ${INPUT} ${INPUT}.bin
                      COMMENT "objcopying to make arm compatible firmware")
              set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${INPUT}.bin)
endfunction(make_mbed_firmware)
