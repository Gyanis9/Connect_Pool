
#include "../include/Connection.h"
#include "../include/ConnectionPool.h"

int main() {
    clock_t begin = clock();
    for (int i = 0; i < 10000; ++i) {
/*        Connection conn;
        char sql[1024] = {0};
        sprintf(sql, "insert into users(name, age, gender) values('%s', '%d', '%s')", "Li si", 21, "male");
        conn.connect("127.0.0.1", 3306, "root", "781680696", "chat");
        conn.update(sql);*/
        ConnectionPool *cp = ConnectionPool::getConnectionPool();
        auto sp = cp->getConnection();
        char sql[1024] = {0};
        sprintf(sql, "insert into users(nickname, age, sex) values('%s', '%d', '%s')", "Li si", 21, "male");
        sp->update(sql);
    }
/*    thread t1([]() {
        ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 1250; ++i) {
            char sql[1024] = {0};
            sprintf(sql, "insert into users(nickname, age, sex) values('%s', '%d', '%s')", "Li si", 21, "male");
            auto sp = cp->getConnection();
            sp->update(sql);
        }
    });
    thread t2([]() {
        ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 1250; ++i) {
            char sql[1024] = {0};
            sprintf(sql, "insert into users(nickname, age, sex) values('%s', '%d', '%s')", "Li si", 21, "male");
            auto sp = cp->getConnection();
            sp->update(sql);
        }
    });
    thread t3([]() {
        ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 1250; ++i) {
            char sql[1024] = {0};
            sprintf(sql, "insert into users(nickname, age, sex) values('%s', '%d', '%s')", "Li si", 21, "male");
            auto sp = cp->getConnection();
            sp->update(sql);
        }
    });
    thread t4([]() {
        ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 1250; ++i) {
            char sql[1024] = {0};
            sprintf(sql, "insert into users(nickname, age, sex) values('%s', '%d', '%s')", "Li si", 21, "male");
            auto sp = cp->getConnection();
            sp->update(sql);
        }
    });

    t1.join();
    t2.join();
    t3.join();
    t4.join();*/
    clock_t end = clock();
    cout << "花费时间 : " << (end - begin) << "ms" << endl;
    return 0;
}

