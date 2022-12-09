add_library(Coordinates Coordinate.h Coordinate.cpp)

target_include_directories(Coordinates PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_definitions(Coordinates PRIVATE GRID_VERSION=1.0)