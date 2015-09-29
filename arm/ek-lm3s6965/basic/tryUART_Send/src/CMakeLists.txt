
add_executable(tryUART_Send main.c startup.c rit128x96x4.c ../externalFunctions/itoa.c)
target_link_libraries(tryUART_Send driver-cm3)
