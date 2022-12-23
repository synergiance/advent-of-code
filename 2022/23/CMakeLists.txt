add_executable(2022-23-unstable_diffusion main.cpp)

target_link_libraries(2022-23-unstable_diffusion PRIVATE Parser)
target_link_libraries(2022-23-unstable_diffusion PRIVATE Reader)
target_link_libraries(2022-23-unstable_diffusion PRIVATE Coordinates)