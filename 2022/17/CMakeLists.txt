add_executable(2022-17-pyroclastic_flow main.cpp)

target_link_libraries(2022-17-pyroclastic_flow PRIVATE Parser)
target_link_libraries(2022-17-pyroclastic_flow PRIVATE Reader)
target_link_libraries(2022-17-pyroclastic_flow PRIVATE Coordinates)