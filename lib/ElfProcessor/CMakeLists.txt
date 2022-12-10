add_library(ElfProcessor ElfProcessor.h ElfProcessor.cpp)

target_link_libraries(ElfProcessor PRIVATE Parser)

target_include_directories(ElfProcessor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_definitions(ElfProcessor PRIVATE ELF_PROCESSOR_VERSION=1.0)