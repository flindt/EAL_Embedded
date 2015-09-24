
include_directories( ./ ) 

add_library(startup STATIC startup.c)
add_library(rit128x96x4 STATIC rit128x96x4.c)


add_executable(tryBuzzer main.c)
target_link_libraries(tryBuzzer libstartup librit128x96x4) 
