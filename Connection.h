//
// Created by guo on 24-12-8.
//

#ifndef CONNECT_POOL_CONNECTION_H
#define CONNECT_POOL_CONNECTION_H

#include <mysql.h>
#include "public.h"
#include <iostream>
#include <ctime>

using namespace std;

class Connection {
public:

    Connection();

    ~Connection();

    bool
    connect(const string &ip, unsigned short port, const string &user, const string &password, const string &dbname);

    bool update(const string &sql);

    MYSQL_RES *query(const string &sql);

    void refreshAliveTime();

    clock_t getAliveTime()const;

private:
    MYSQL *m_con;
    clock_t m_aliveTime;
};


#endif //CONNECT_POOL_CONNECTION_H
