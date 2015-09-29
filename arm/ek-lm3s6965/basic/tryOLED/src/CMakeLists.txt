
add_executable(tryOLED main.c startup.c rit128x96x4.c)
target_link_libraries(tryOLED driver-cm3)
