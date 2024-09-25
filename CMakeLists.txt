cmake_minimum_required(VERSION 3.29)
project(cripto_scanner)

set(CMAKE_CXX_STANDARD 20)
find_package(ixwebsocket CONFIG REQUIRED)
find_package(Boost REQUIRED)


add_executable(cripto_scanner src/main.cpp
#        old_src/json.cpp
#        old_src/graph.cpp
#        old_src/websocket.cpp
#        old_src/websocket.h
#        old_src/json.h
#        old_src/graph.h
        include/interface/ICriptoStock.h
        include/structure/Symbol.h
        include/structure/Path.h
        src/class/Scanner.cpp
        src/intefrace/ICriptoStock.cpp
        include/class/Scanner.h
        src/class/Wallet.cpp
        include/class/Wallet.h
        include/structure/PathNode.h
        src/intefrace/ICriptoStock.cpp
        include/class/BinanceImpl.h
        src/class/BinanceImpl.cpp
)
target_link_libraries(cripto_scanner PRIVATE ixwebsocket)

