add_executable(2022-13-distress_signal main.cpp)

target_link_libraries(2022-13-distress_signal PRIVATE Parser)
target_link_libraries(2022-13-distress_signal PRIVATE Reader)