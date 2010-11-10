#include "db.hpp"
#include "database.hpp"

#include <iostream>

using namespace std;

int main()
{
  MYDB db;
  db.query("SELECT * FROM 270_social_message");
  // db.print_results();
  cout << db.results().size() << " rows returned.\n";
  cout << "Is free? " << db.isFree() << endl;
  db.free();
  cout << "Is free? " << db.isFree() << endl;
  db.query();
  cout << db.results().size() << " rows returned.\n";

  cout << mysql_num_rows(result("SHOW TABLES;", conn())) << endl;

  return 0;
}
