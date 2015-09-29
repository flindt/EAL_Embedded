
add_executable(trySysTickTimer main.c startup.c rit128x96x4.c ../externalFunctions)
target_link_libraries(trySysTickTimer driver-cm3 )

