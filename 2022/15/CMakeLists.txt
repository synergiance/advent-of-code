add_executable(2022-15-beacon_exclusion_zone main.cpp)

target_link_libraries(2022-15-beacon_exclusion_zone PRIVATE Parser)
target_link_libraries(2022-15-beacon_exclusion_zone PRIVATE Reader)
target_link_libraries(2022-15-beacon_exclusion_zone PRIVATE Coordinates)