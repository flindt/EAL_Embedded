
add_executable(tryBuzzer main.c startup.c rit128x96x4.c)
target_link_libraries(tryBuzzer driver-cm3 )

