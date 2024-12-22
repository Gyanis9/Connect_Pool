//
// Created by guo on 24-12-8.
//

#include "../include/ConnectionPool.h"

ConnectionPool *ConnectionPool::getConnectionPool() {
    static ConnectionPool pool;
    return &pool;
}

void ConnectionPool::loadConfig(const string &filename) {
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(filename, pt);
    m_ip = pt.get<string>("mysql.ip");
    m_port = pt.get<unsigned short>("mysql.port");
    m_user = pt.get<string>("mysql.username");
    m_password = pt.get<string>("mysql.password");
    m_dbname = pt.get<string>("mysql.dbname");
    m_initSize = pt.get<int>("mysql.initSize");
    m_maxSize = pt.get<int>("mysql.maxSize");
    m_maxSpareTime = pt.get<int>("mysql.maxSpareTime");
    m_connectionTimeout = pt.get<int>("mysql.connectionTimeout");
}

ConnectionPool::ConnectionPool() : m_isRunning(true) {
    loadConfig("mysql.ini");
    for (size_t i = 0; i < m_initSize; ++i) {
        auto p = new Connection();
        p->connect(m_ip, m_port, m_user, m_password, m_dbname);
        p->refreshAliveTime();
        m_connectQueue.push(p);
        m_connectCount++;
    }
    thread produce(bind(&ConnectionPool::produceConnectionTask, this));
    produce.detach();

    thread scanner(bind(&ConnectionPool::scannereConnectionTask, this));
    scanner.detach();
}

void ConnectionPool::produceConnectionTask() {
    for (;;) {
        unique_lock<mutex> lock(m_mtx);
        m_cv.wait(lock, [&]() { return m_connectQueue.empty(); });
        if (!m_isRunning) {
            m_exit.notify_all();
            break;
        }
        if (m_connectCount < m_maxSize) {
            auto p = new Connection();
            p->connect(m_ip, m_port, m_user, m_password, m_dbname);
            p->refreshAliveTime();
            m_connectQueue.push(p);
            m_connectCount++;
        }
        m_cv.notify_all();
    }
}

shared_ptr<Connection> ConnectionPool::getConnection() {
    unique_lock<mutex> lock(m_mtx);
    if (!m_cv.wait_for(lock, std::chrono::microseconds(m_connectionTimeout),
                       [&]() { return !m_connectQueue.empty() || !m_isRunning; })) {
        LOG("获取空闲连接超时....");
        return nullptr;
    }
    shared_ptr<Connection> rp(m_connectQueue.front(), [&](Connection *pcn) {
        unique_lock<mutex> lock(m_mtx);
        pcn->refreshAliveTime();
        m_connectQueue.push(pcn);
    });
    m_connectQueue.pop();  // 弹出队列元素
    m_cv.notify_all();  // 通知其他线程
    return rp;
}

void ConnectionPool::scannereConnectionTask() {
    for (;;) {
        this_thread::sleep_for(chrono::seconds(m_maxSpareTime));
        unique_lock<mutex> lock(m_mtx);
        if (!m_isRunning) {
            m_exit.notify_all();
            break;
        }
        while (m_connectCount > m_initSize) {
            auto p = m_connectQueue.front();
            if (p->getAliveTime() >= (m_maxSpareTime * 1000)) {
                m_connectQueue.pop();
                delete p;
                m_connectCount--;
            } else {
                break;
            }
        }
    }
}

ConnectionPool::~ConnectionPool() {
    m_isRunning = false;
    unique_lock<mutex> lock(m_mtx);
    m_cv.notify_all();  // 唤醒所有正在等待的线程
    while (!m_connectQueue.empty()) {
        auto p = m_connectQueue.front();
        m_connectQueue.pop();
        delete p;
    }
}



