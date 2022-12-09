add_executable(2022-09-rope_movements main.cpp)

target_link_libraries(2022-09-rope_movements PRIVATE Parser)
target_link_libraries(2022-09-rope_movements PRIVATE Reader)
target_link_libraries(2022-09-rope_movements PRIVATE Grid)