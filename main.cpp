#include <cstdio>
#include <iostream>
#include <mysql.h>

using namespace std;

int main(int argc, char *argv[]) {
    MYSQL conn;
    int res;
    mysql_init(&conn);
    if (mysql_real_connect(&conn, "localhost", "root", "781680696", "test", 0, nullptr, CLIENT_FOUND_ROWS)) {
        cout << "connect success" << endl;
        res = mysql_query(&conn, "insert into test values('user','123456')");
        if (res) {
            printf("error\n");
        } else {
            printf("OK\n");
        }
        mysql_close(&conn);
    } else {
        cout << "connect failed" << endl;
    }
    return 0;
}

