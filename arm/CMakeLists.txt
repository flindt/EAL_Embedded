cmake_minimum_required (VERSION 2.6)
project (ealEmbedded)

include(  ../arm/ek-lm3s6965/toolchain-ek-lm3s6965.cmake )

include_directories( /shared/toolchains/SW-EK-LM3S6965 )
include_directories( /shared/toolchains/SW-EK-LM3S6965/inc )
include_directories( /shared/toolchains/SW-EK-LM3S6965/driverlib )

add_subdirectory(ek-lm3s6965)


