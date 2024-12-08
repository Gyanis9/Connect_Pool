cmake_minimum_required(VERSION 3.10)
project(Connect_Pool)

set(CMAKE_CXX_STANDARD 17)

# 添加 MySQL 的头文件路径
include_directories(/usr/include/mysql)

# 添加 MySQL 的库文件路径
link_directories(/usr/lib/x86_64-linux-gnu)

# 定义可执行文件
add_executable(Connect_Pool main.cpp)

# 链接 MySQL 客户端库
target_link_libraries(Connect_Pool mysqlclient)



