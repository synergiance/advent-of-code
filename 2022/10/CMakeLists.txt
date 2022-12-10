add_executable(2022-10-cathode_ray_tube main.cpp)

target_link_libraries(2022-10-cathode_ray_tube PRIVATE Reader)
target_link_libraries(2022-10-cathode_ray_tube PRIVATE ElfProcessor)