/* 
 * File:   mysqlconn_wrapper.cpp
 *
 * This is a wrapper class for the MySQL Connector/C++ Library
 * Developed from the wrapper by Eduardo Casas (www.eduardocasas.com & https://github.com/eduardocasas/MySQL-Connector-Cpp-Wrapper-Class)
 * Various code added and removed; Alan Childs.
 *
 */

#ifndef MYSQLCONN_WRAPPER_H
#define	MYSQLCONN_WRAPPER_H

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

class MySQLConnWrapper
{
    public:
        
        /* Example MySQL server settings */
    	string addr = "localhost:3306";
    	string user = "user";
    	string password = "pass";

	~MySQLConnWrapper();
        void manageException(sql::SQLException& e);
        void connect();
        void switchDb(const string& db_name);
        void execute(const string& query);
        void query(const string& query);
        void fin();
        bool fetch();
        string print(const string& field);
        string print(const int& index);

    private:
        sql::Driver *driver = get_driver_instance();
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
};


#endif	/* MYSQLCONN_WRAPPER_H */


