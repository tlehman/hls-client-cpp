cmake_minimum_required(VERSION 3.10)
project(HLSClientCPP)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add executable targets
add_executable(hls_server src/hls_server.cpp)
add_executable(hls_client src/hls_client.cpp)

# civetweb
add_library(civetweb STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/civetweb.c)
target_include_directories(
    civetweb PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_options(civetweb PRIVATE -DNO_SSL)
target_link_libraries(hls_server PRIVATE civetweb)


# hls_client target: Find and link libavformat
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET libavformat)
target_include_directories(hls_client PRIVATE ./include)
target_link_libraries(hls_client PRIVATE PkgConfig::LIBAV)
target_link_libraries(hls_client PRIVATE civetweb)


# hls_server target
target_include_directories(hls_server PRIVATE include)
target_compile_options(hls_server PRIVATE -Wall -Wdeprecated-declarations -DNO_SSL)
