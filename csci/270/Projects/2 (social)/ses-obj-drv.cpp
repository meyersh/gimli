#include <iostream>
#include <string>
#include "session_objectified.hpp"

using namespace std;

int 
main()
{
  string username = "shaun";

  userProfile profile(username);
  cout << profile.username << endl;

  return 0;
}
