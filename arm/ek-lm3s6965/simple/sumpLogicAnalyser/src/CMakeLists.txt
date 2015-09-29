
add_executable(sumpLogicAnalyser main.c startup.c rit128x96x4.c)
target_link_libraries(sumpLogicAnalyser driver-cm3 )

