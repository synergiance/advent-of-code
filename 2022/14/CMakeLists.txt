add_executable(2022-14-regolith_reservoir main.cpp)

target_link_libraries(2022-14-regolith_reservoir PRIVATE Parser)
target_link_libraries(2022-14-regolith_reservoir PRIVATE Reader)
target_link_libraries(2022-14-regolith_reservoir PRIVATE Coordinates)
