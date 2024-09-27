cmake_minimum_required(VERSION 3.29)
project(cripto_scanner)

set(CMAKE_CXX_STANDARD 20)
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)


add_executable(cripto_scanner src/main.cpp
        include/interface/ICriptoStock.h
        include/structure/Symbol.h
        include/structure/Path.h
        src/class/Scanner.cpp
        include/class/Scanner.h
        src/class/Wallet.cpp
        include/class/Wallet.h
        include/structure/PathNode.h
        include/class/BinanceImpl.h
        src/class/BinanceImpl.cpp
        include/class/BaseCriptoStock.h
        include/class/OkxImpl.h
        src/class/OkxImpl.cpp
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
if(UNIX AND NOT APPLE)
    target_link_libraries(cripto_scanner PRIVATE rt)
endif()

