
include_directories( ./ ) 

add_library(startup OBJECT startup.c)
add_library(rit128x96x4 OBJECT rit128x96x4.c)


add_executable(tryBuzzer main.c )

arm_firmware( tryBuzzer )
