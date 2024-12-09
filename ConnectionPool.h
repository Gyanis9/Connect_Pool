//
// Created by guo on 24-12-8.
//


#ifndef CONNECT_POOL_CONNECTIONPOOL_H
#define CONNECT_POOL_CONNECTIONPOOL_H

#include <mysql.h>
#include <iostream>
#include <queue>
#include "Connection.h"
#include <mutex>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <atomic>
#include <memory>
#include <thread>
#include <functional>
#include <condition_variable>

using namespace std;

class ConnectionPool {
public:
    static ConnectionPool *getConnectionPool();

    shared_ptr<Connection> getConnection();

    ~ConnectionPool();

protected:
    void loadConfig(const string &filename = "mysql.ini");

    void produceConnectionTask();

    void scannereConnectionTask();

private:

    ConnectionPool();

    string m_ip;
    unsigned short m_port;
    string m_user;
    string m_password;
    string m_dbname;
    int m_initSize;
    int m_maxSize;
    int m_maxSpareTime;
    int m_connectionTimeout;
    queue<Connection *> m_connectQueue;
    mutex m_mtx;
    condition_variable m_cv;
    atomic<int> m_connectCount{};
    bool m_isRunning;
    condition_variable m_exit;
};


#endif //CONNECT_POOL_CONNECTIONPOOL_H
