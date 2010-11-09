#include "database.hpp"

MYSQL* conn(const char *username, const char *password,
	    const char *database, const char *host)
{
  MYSQL *connection = mysql_init(NULL);
  if (!mysql_real_connect(connection, host, username, 
			  password, database, 0, NULL, 0))
    return NULL;
  
  return connection;
}

MYSQL_RES *result(const char *query, MYSQL *connection, bool closeConnection)
{
  if (mysql_query(connection, query) || connection == NULL)
    {
      // something went wrong with our query or there is no connection.
      return NULL;
    }
  else
    {
      MYSQL_RES *res = mysql_store_result(connection);
      if (closeConnection) 
	mysql_close(connection);
      return res;
    }
}
