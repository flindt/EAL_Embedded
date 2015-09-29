
add_executable(tryTimers main.c startup.c rit128x96x4.c ../externalFunctions)
target_link_libraries(tryTimers driver-cm3 )

