add_executable(2022-04-cleanup_assignments main.cpp)

target_link_libraries(2022-04-cleanup_assignments PRIVATE Parser)
target_link_libraries(2022-04-cleanup_assignments PRIVATE Reader)