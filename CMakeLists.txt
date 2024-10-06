cmake_minimum_required(VERSION 3.29)
project(cripto_scanner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
include(CheckCXXCompilerFlag)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")


find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)


add_executable(cripto_scanner src/main.cpp
        include/structure/Symbol.h
        include/structure/Path.h
        include/structure/PathNode.h
        include/class/Scanner.h
        include/class/BinanceImpl.h
        include/class/BaseCriptoStock.h
        include/class/BybitImpl.h
        include/utils/shared.h
        include/utils/request.h
        src/structure/Symbol.cpp
        src/class/Scanner.cpp
        src/class/BinanceImpl.cpp
        src/class/BaseCriptoStock.cpp
        src/class/BybitImpl.cpp
        src/utils/shared.cpp
        src/utils/request.cpp
)

target_include_directories(cripto_scanner PRIVATE ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
target_link_libraries(cripto_scanner
        PRIVATE
        ${Boost_LIBRARIES}
        OpenSSL::SSL
        OpenSSL::Crypto
        Threads::Threads
)

# If you're on a Unix-like system, you might also need to link against the realtime library
if (UNIX AND NOT APPLE)
    target_link_libraries(cripto_scanner PRIVATE rt)
endif ()

