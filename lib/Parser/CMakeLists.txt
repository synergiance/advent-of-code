add_library(Parser Parser.h Parser.cpp)

target_include_directories(Parser PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_definitions(Parser PRIVATE PARSER_VERSION=1.0)