cmake_minimum_required(VERSION 3.10)
project(Connect_Pool)

set(CMAKE_CXX_STANDARD 17)

# 添加 MySQL 的头文件路径
include_directories(/usr/include/mysql)
# 添加 MySQL 的库文件路径
link_directories(/usr/lib/x86_64-linux-gnu)
add_library(Connect_Pool
        src/ConnectionPool.cpp
        include/ConnectionPool.h
        src/Connection.cpp
        include/Connection.h
        include/public.h)
# 链接 MySQL 客户端库
target_link_libraries(Connect_Pool mysqlclient)

add_subdirectory(tests)



