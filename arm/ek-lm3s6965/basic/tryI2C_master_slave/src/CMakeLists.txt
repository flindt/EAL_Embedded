
add_executable(tryI2C_master_slave main.c startup.c rit128x96x4.c ../externalFunctions)
target_link_libraries(tryI2C_master_slave driver-cm3 )

