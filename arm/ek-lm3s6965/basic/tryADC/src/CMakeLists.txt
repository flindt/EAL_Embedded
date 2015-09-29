
include_directories( ./ ) 

add_executable(tryADC main.c startup.c rit128x96x4.c ../externalFunctions/itoa.c)
target_link_libraries(tryADC driver-cm3)
