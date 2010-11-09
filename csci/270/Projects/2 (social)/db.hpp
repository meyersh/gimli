/*****************************************************************************
 * Database access functions to drive our system.
 * designed for Project 2 (CSCI 270) 
 * Shaun Meyer, Oct 2010
 */

#include <mysql.h>

#include <vector>
#include <string>
#include <iostream>
#include <cstdlib> // for exit()

#define DB_PASSWORD "forwhomtbt"
#define DB_USER "meyersh"
#define DB_HOST "localhost"
#define DB_DATABASE "dbmeyersh"
#define TBL_PREFIX "270_social_"

using namespace std;

class MYDB {
private:
  char *server;
  char *user;
  char *password;
  char *database;

public:
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  MYDB(const char *server=DB_HOST, const char *user=DB_USER, 
       const char *password=DB_PASSWORD, const char *database=DB_DATABASE)
  {
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server,
			    user, password, database, 0, NULL, 0))
      {
	cout << mysql_error(conn) << endl;
	exit(1);
      }
  }

  ~MYDB()
  {
    /* close connection(s) */
    mysql_free_result(res);
    mysql_close(conn);
  }

  void query(string sql="show tables;")
  {
    /* Send SQL query */
    if (mysql_query(conn, sql.c_str())) 
      {
	cout << mysql_error(conn) << endl;
	exit(1);
      }

    res = mysql_store_result(conn);
  }

  void print_results()
  {
    /* ouput table name */

    cout << "Mysql tables in mysql db:\n";
    unsigned int num_fields = mysql_num_fields(res);
    unsigned int num_rows = mysql_num_rows(res);
    cout << "Query returned " << num_fields << " fields and "
	 << num_rows << " rows\n";

    while ((row = mysql_fetch_row(res)) != NULL)
      {
	cout << row[0] << endl;
	cout << "length: " << (unsigned int)mysql_fetch_lengths(res)[0] << endl;
      }

  }

  vector< vector<string> > results()
  {
    unsigned int num_fields = mysql_num_fields(res);
    vector< vector<string> > result_set;
    vector<string> row_set(num_fields);

    while ((row = mysql_fetch_row(res)) != NULL)
      {
	for (int i = 0; i < mysql_num_fields(res); i++)
	  {
	    if (row[i] == NULL) {
	      row_set[i] = "";
	    }
	    else {
	      row_set[i] = row[i];
	    }
	    cout << i << ": " <<  row_set[i] << endl;
	  }
	result_set.push_back(row_set);
      }

    return result_set;
  }

  void free()
  {
    mysql_free_result(res);
  }
  bool isFree()
  {
    if (res == NULL)
      return true;
    else
      return false;
  }
    
};

