
cmake_minimum_required(VERSION 2.6)

project(dart_pdfium)

set(CMAKE_MACOSX_RPATH 1)

add_library(dart_pdfium SHARED src/pdfium.cc)

target_include_directories(dart_pdfium PRIVATE dart-sdk/include/ pdfium/public/)
target_link_directories(dart_pdfium PRIVATE pdfium/out/obj/)

target_compile_definitions(dart_pdfium PRIVATE DART_SHARED_LIB=1)

target_link_libraries(dart_pdfium pdfium)

if (APPLE)
    target_link_options(dart_pdfium PRIVATE "-undefineddynamic_lookup")
endif (APPLE)

install(TARGETS dart_pdfium DESTINATION lib)