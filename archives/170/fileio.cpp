#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <sstream> // for linestream()

using namespace std;

int
main(int argc, char* argv[])
{
    ifstream f;
    string line;
    //istringstream linestream(line);
    stringstream l; // stupid name, the Oo is getting weird now.
    int foo, bar;

    f.open("passwords.txt");

    while (getline(f, line))  // store each line from f in string `line'
    {
      l.clear(); // <-- look this up
      l.str(line); // stream-ify `line'.
      l >> foo >> bar;

      cout << "LINE: " << line << endl;
      cout << foo << " | " << bar << endl;

    }

    f.close();

    return 0;
}
