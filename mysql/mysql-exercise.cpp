#include <iostream>
#include <mysql.h>
#include <stdlib.h> // exit()

#include <string>
#include <vector>

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

  MYDB(char *server="localhost", char *user="meyersh", 
       char *password="forwhomtbt", char *database="dbmeyersh")
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

    res = mysql_use_result(conn);
  }

  void print_results()
  {
    /* ouput table name */

    cout << "Mysql tables in mysql db:\n";
    unsigned int num_fields = mysql_num_fields(res);
    unsigned int num_rows = mysql_num_rows(res);
    cout << "Query returned " << num_fields << " fields." 
	 << " and " << num_rows << " rows\n";

    while ((row = mysql_fetch_row(res)) != NULL)
      {
	cout << row[0] << endl;
	cout << "length: " << (unsigned int)mysql_fetch_lengths(res)[0] << endl;
      }

  }

  vector<string> results()
  {
    vector< vector<string> > result_set(mysql_num_rows(res));
    vector<string> row_set(mysql_num_fields(res));

    while ((row = mysql_fetch_row(res)) != NULL)
      {
	row_set.clear();
	for (int i = 0; i < mysql_num_fields(res); i++)
	  row_set[i] = row[i];
	result_set.push_back(row_set);
      }
  }
    
};


int main()
{

  MYDB db;
  db.query("select * from EMPLOYEE where Fname='shaun';");
  //  db.print_results();
  cout << "results: " <<  db.results().size() << endl;

  return 0;

}
