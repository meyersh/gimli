#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <cstddef> // for NULL
#include <mysql.h>

#define DB_PASSWORD "forwhomtbt"
#define DB_USER "meyersh"
#define DB_HOST "localhost"
#define DB_DATABASE "dbmeyersh"
#define TBL_PREFIX "270_social_"

/* Function definitions */

MYSQL* conn(const char *username=DB_USER, const char *password=DB_PASSWORD,
	    const char *database=DB_DATABASE, const char *host=DB_HOST);

MYSQL_RES* result(const char *query, MYSQL *connection=conn(), 
		  bool closeConnection=true);

#endif
