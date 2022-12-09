add_library(Grid Grid.h Grid.cpp Coordinate.h Coordinate.cpp)

target_include_directories(Grid PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_definitions(Grid PRIVATE GRID_VERSION=1.0)