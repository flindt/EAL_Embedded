
add_executable(tryUART main.c startup.c rit128x96x4.c)
target_link_libraries(tryUART driver-cm3)
