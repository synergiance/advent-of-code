add_library(Input Input.h Input.cpp)

target_include_directories(Input PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_definitions(Input PRIVATE INPUT_VERSION=1.0)