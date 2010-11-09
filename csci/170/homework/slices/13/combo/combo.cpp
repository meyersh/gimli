#include <iostream>
#include <string>
#include <cstdlib>
#include "tstamp.hpp"
#include "randomStr.hpp"

using namespace std;

string getInput(string prompt="")
{
  cout << prompt;

  string inpt;
  std::getline(cin, inpt);
  return inpt;
}

int getIntInput(string prompt="")
{
  return atoi( getInput(prompt).c_str() );
}

void menu()
{
  cout << endl
       << "Choose One:\n"
       << "[R]andom String\n"
       << "[T]ime Offset\n"
       << "[Q]uit\n\n";

}

void randomMenu()
{
  cout << "\n\n -= Random String Generator =-\n\n";

  int len = getIntInput("How long should our string be?\n> ");
  if (len < 1)
    len = 10;

  string CHARS = getInput("What characters should we draw from? [default: All alphanum]\n> ");

  string random_string;

  if (CHARS == "")
    randomString(random_string, len);
  else 
    randomString(random_string, len, CHARS);

  cout << "Your string is:\n"
       << random_string 
       << endl;

}

void timeMenu()
{
  int year, month, day, hour, min, sec;

  cout << "\n\n -= Time Offset Generator =-\n\n";
  tstamp now;
  now.setCurrentDateTimeStamp(1);
  now.getStampParts(year, month, day, hour, min, sec);
  string time;
  now.stampToString(time);
  cout << "The current time is: " << time << endl;
  cout << "(" << year << "/" << month << "/" << day << " " << hour << ":" << min << ":" << sec << ")\n";
  int offset = getIntInput("Offset? \n> ");
  now += offset;
  now.stampToString(time);
  now.getStampParts(year, month, day, hour, min, sec);
  cout << "\nAdjusted Time: " << time << endl;
  cout << "(" << year << "/" << month << "/" << day << " " << hour << ":" << min << ":" << sec << ")\n";
}

int
main()
{
  menu();
  char input;

  
  /* Wait for valid input... */
  
  while (input != 'Q')
    {
      input = toupper(getInput(">")[0]);
      
      
      if (input == 'R')
	randomMenu();
      else if (input == 'T')
	timeMenu();

      if (input == 'R' || input == 'T')
	menu();
        
    }
  
  
  return 0;
}
