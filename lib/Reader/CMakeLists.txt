add_library(Reader Reader.h Reader.cpp)

target_include_directories(Reader PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_definitions(Reader PRIVATE READER_VERSION=1.0)