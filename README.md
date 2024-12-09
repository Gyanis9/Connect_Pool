# 项目概述

## 关键技术点
- **MySQL数据库编程**
- **单例模式**
- **Queue队列容器**
- **C++11多线程编程**
- **线程互斥**
- **线程同步通信和unique_lock**
- **基于CAS的原子整形**
- **智能指针shared_ptr**
- **Lambda表达式**
- **生产者-消费者线程模型**

## 项目背景
为了提高MySQL数据库（基于C/S设计）的访问瓶颈，除了在服务器端增加缓存服务器缓存常用的数据（例如Redis），还可以增加连接池来提高MySQL Server的访问效率。在高并发情况下，大量的TCP三次握手、MySQL Server连接认证、MySQL Server关闭连接回收资源和TCP四次挥手所耗费的性能时间也是很明显的，增加连接池就是为了减少这一部分的性能损耗。

在市场上比较流行的连接池包括阿里的Druid，C3P0以及Apache DBCP连接池，它们对于短时间内大量的数据库增删改查操作性能的提升是很明显的，但是它们有一个共同点，就是全部由Java实现的。

本项目就是为了在C/C++项目中，提供MySQL Server的访问效率，实现基于C++代码的数据库连接池模块。

## 连接池功能点介绍
连接池一般包含了数据库连接所用的IP地址、端口号、用户名和密码以及其他的性能参数，例如初始连接量、最大连接量、最大空闲时间、连接超时时间等。该项目是基于C++语言实现的连接池，主要实现以下几个连接池的通用基础功能：

### 初始连接量（initSize）
表示连接池事先会和MySQL Server创建`initSize`个数的连接。当应用发起MySQL访问时，不用再创建新的连接，直接从连接池中获取一个可用的连接就可以。使用完成后，并不去释放连接，而是把当前连接归还到连接池当中。

### 最大连接量（maxSize）
当并发访问MySQL Server的请求增多时，初始连接量已经不够使用，此时会根据新的请求数量去创建更多的连接给应用去使用。但是新创建的连接数量上限是`maxSize`，不能无限制的创建连接，因为每一个连接都会占用一个socket资源。如果连接池占用过多的socket资源，服务器就不能接收太多的客户端请求了。

当这些连接使用完成后，再次归还到连接池当中来维护。

### 最大空闲时间（maxIdleTime）
当访问MySQL的并发请求多了以后，连接池里面的连接数量会动态增加，最大连接量为`maxSize`个。当这些连接用完并且再次归还到连接池时，如果在指定的`maxIdleTime`时间内，这些新增加的连接没有被使用过，那么新增加的这些连接资源就要被回收掉。此时，只需要保持初始连接量`initSize`个连接就可以了。

### 连接超时时间（connectionTimeout）
当MySQL的并发请求量过大，连接池中的连接数量已经到达`maxSize`，而此时没有空闲的连接可供使用，应用从连接池获取连接的时间如果超过`connectionTimeout`，则会获取失败，无法访问数据库。


# 连接池功能点和性能对比

## 连接池参数功能

| 功能点               | 描述                                                                                         |
|----------------------|----------------------------------------------------------------------------------------------|
| 初始连接量（initSize）   | 连接池事先创建的连接数，应用访问时直接从连接池获取，使用后归还。                                           |
| 最大连接量（maxSize）   | 连接池最大允许的连接数，超过此数目不再创建新连接。连接会被复用。                                        |
| 最大空闲时间（maxIdleTime） | 如果连接池中有空闲连接超过指定的最大空闲时间，则会被回收，以保证连接池中的连接不会过多。                             |
| 连接超时时间（connectionTimeout） | 获取连接的最大等待时间，超过此时间如果没有可用连接，则会获取失败，无法访问数据库。                             |

## MySQL Server 配置

| 配置项              | 描述                                                       |
|---------------------|------------------------------------------------------------|
| `max_connections`   | MySQL Server支持的最大连接数。可以通过 `SHOW VARIABLES LIKE 'max_connections';` 命令查看。 |

## 性能对比（未使用连接池 vs 使用连接池）

| 数据量 | 未使用连接池（单线程） | 使用连接池（单线程） | 使用连接池（四线程） |
|--------|-------------|----------|----------|
| 1000   | 14267063ms  | 263191ms | 270785ms |
| 5000   | 71284930ms  | 479455ms | 471865ms |
| 10000  | 144521330ms | 788177ms | 720472ms |
