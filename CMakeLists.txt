
cmake_minimum_required(VERSION 2.6)

project(dart_pdfium)

set(CMAKE_MACOSX_RPATH 1)

set(DART_SDK_PATH dart-sdk CACHE STRING "The dart sdk path")
set(PDFIUM_PATH pdfium CACHE STRING "The pdfium path")

message("Using DART_SDK_PATH:" ${DART_SDK_PATH})
message("Using PDFIUM_PATH:" ${PDFIUM_PATH})

add_library(dart_pdfium SHARED lib/pdfium.cc)

target_include_directories(dart_pdfium PRIVATE ${DART_SDK_PATH}/include/ ${PDFIUM_PATH}/public/)
target_link_directories(dart_pdfium PRIVATE ${PDFIUM_PATH}/out/obj/)

target_compile_definitions(dart_pdfium PRIVATE DART_SHARED_LIB=1)

target_link_libraries(dart_pdfium pdfium)

if (APPLE)
    target_link_options(dart_pdfium PRIVATE "-undefineddynamic_lookup")
    target_link_libraries(dart_pdfium pdfium "-framework CoreGraphics")
endif (APPLE)

install(TARGETS dart_pdfium DESTINATION lib)

set(LLVM_COV_PATH /usr/bin/llvm-cov)

if(CMAKE_BUILD_TYPE STREQUAL "coverage" OR CODE_COVERAGE)
    if("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
        message("Building with llvm Code Coverage Tools")

        # Warning/Error messages
        if(NOT LLVM_COV_PATH)
            message(FATAL_ERROR "llvm-cov not found! Aborting.")
        endif()

        # set Flags
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")

    elseif(CMAKE_COMPILER_IS_GNUCXX)
        message("Building with lcov Code Coverage Tools")

        # Warning/Error messages
        if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
            message(WARNING "Code coverage results with an optimized (non-Debug) build may be misleading")
        endif()
        if(NOT LCOV_PATH)
            message(FATAL_ERROR "lcov not found! Aborting...")
        endif()
        if(NOT GENHTML_PATH)
            message(FATAL_ERROR "genhtml not found! Aborting...")
        endif()

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    else()
        message(FATAL_ERROR "Code coverage requires Clang or GCC. Aborting.")
    endif()
endif()

