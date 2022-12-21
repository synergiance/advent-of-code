add_executable(2022-20-grove_positioning_system main.cpp MixerHelper.cpp MixerHelper.h)

target_link_libraries(2022-20-grove_positioning_system PRIVATE Parser)
target_link_libraries(2022-20-grove_positioning_system PRIVATE Reader)