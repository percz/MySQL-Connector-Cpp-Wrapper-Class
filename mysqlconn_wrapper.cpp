/* 
 * File:   mysqlconn_wrapper.h
 *
 * This is a wrapper class for the MariaDB Connector/C++ Library
 * Developed from the wrapper by Eduardo Casas (www.eduardocasas.com & https://github.com/eduardocasas/MySQL-Connector-Cpp-Wrapper-Class)
 * Various code added and removed (notably the memory leaking prep_stmt stuff); Alan Childs.
 * To build probably do something like:    apt-get install libmariadb-client-lgpl-dev libmysql++-dev
 *
 */

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include "mysqlconn_wrapper.h"

MySQLConnWrapper::~MySQLConnWrapper()
{
	fin();
}

void MySQLConnWrapper::manageException(sql::SQLException& e)
{
	if (e.getErrorCode() != 0){
		//You could also try and restart MySQL, or Throw and let the caller handle an error.
		cout << "\n\033[1;31mFATAL ERROR: Connection to database failed. \n " << e.what() << "\033[0m \n";
		exit(0); 
	}
}

void MySQLConnWrapper::connect()
{
	try{
		con = driver->connect(addr, user, password);
	} catch (sql::SQLException &e){
		manageException(e);
	}
}

void MySQLConnWrapper::switchDb(const string& db_name)
{
	try{
		con->setSchema(db_name);
		stmt = con->createStatement();
	} catch (sql::SQLException &e) {
		manageException(e);
	}
}

void MySQLConnWrapper::execute(const string& query)
{
	try {
		stmt->executeQuery(query);
	} catch (sql::SQLException &e) {
		manageException(e);
	}
}

void MySQLConnWrapper::query(const string& query)
{
	if (!res == 0) { delete res; res = 0;} //In case we didn't close the connect before making a new query, stops memory leak of res.
	try {
		res = stmt->executeQuery(query);
	} catch (sql::SQLException &e) {
		manageException(e);
	}
}

bool MySQLConnWrapper::fetch()
{
	return res->next();
}

string MySQLConnWrapper::print(const string& field)
{
	try {
		return res->getString(field);
	} catch (sql::SQLException &e) {
        	manageException(e);
        	return "ERROR";
    	}
}

string MySQLConnWrapper::print(const int& index)
{
	try {
		return res->getString(index);
	} catch (sql::SQLException &e) {
		manageException(e);
		return "ERROR";
	}
}

void MySQLConnWrapper::fin()
{
	if (!res == 0) { delete res; res = 0;} //Possibly we only ran an execute and no res generating query.
	delete stmt;
	con -> close();
	delete con;
	con = NULL; //TODO: Check, but this really doesn't seem to do anything at this point. Probs needs removing.
}
