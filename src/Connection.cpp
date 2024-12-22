//
// Created by guo on 24-12-8.
//

#include "../include/Connection.h"

Connection::Connection() {
    m_con = mysql_init(nullptr);
}

Connection::~Connection() {
    if (m_con != nullptr)
        mysql_close(m_con);
}

bool Connection::connect(const string &ip, unsigned short port, const string &user, const string &password,
                         const string &dbname) {
    MYSQL *p = mysql_real_connect(m_con, ip.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0);
    return p != nullptr;
}

bool Connection::update(const string &sql) {
    if (mysql_query(m_con, sql.c_str())) {
        LOG("更新失败: " + sql);
        return false;
    }
    return true;
}

MYSQL_RES *Connection::query(const string &sql) {
    if (mysql_query(m_con, sql.c_str())) {
        LOG("查询失败: " + sql);
        return nullptr;
    }
    return mysql_use_result(m_con);
}

void Connection::refreshAliveTime() {
    m_aliveTime = clock();
}

clock_t Connection::getAliveTime() const {
    return clock() - m_aliveTime;
}
