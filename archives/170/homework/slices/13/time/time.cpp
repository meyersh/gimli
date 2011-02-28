#include <iostream>
#include "tstamp.hpp"

using namespace std;

int 
main()
{
  tstamp now;
  now.setCurrentDateTimeStamp(1);
  string time;
  now.stampToString(time);
  cout << "Current time: " << time << endl;
  

  cout << "Offset? (sec) > ";
  int offset=0;
  cin >> offset;

  now += offset;
  now.stampToString(time);
  cout << "New Time: " << time << endl;

  return 0;
}
