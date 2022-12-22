add_executable(2022-22-monkey_map main.cpp)

target_link_libraries(2022-22-monkey_map PRIVATE Parser)
target_link_libraries(2022-22-monkey_map PRIVATE Reader)
target_link_libraries(2022-22-monkey_map PRIVATE Coordinates)